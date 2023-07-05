//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/file_buffer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/file_buffer.hh"
#include "common/align.hh"
#include "common/constant.hh"
#include "common/logger.hh"

namespace saturn {

FileBuffer::FileBuffer(Allocator &allocator,
                       FileBufferType type,
                       Size requestSize)
    : allocator{allocator}, type{type}, limitSize{requestSize} {
  Init();
  if (requestSize) {
    Resize(requestSize);
  }
}

void FileBuffer::Init() {
  buffer = internalBuffer = nullptr;
  limitSize = internalSize = 0_Size;
}

FileBuffer::FileBuffer(FileBuffer &source, FileBufferType type)
    : allocator{source.allocator}, type{type}, limitSize{source.limitSize} {
  buffer = source.buffer;
  internalBuffer = source.internalBuffer;
  internalSize = source.internalSize;
  limitSize = source.limitSize;
  source.Init();
}

FileBuffer::~FileBuffer() {
  if (!internalBuffer) {
    return;
  }
  allocator.FreeData(internalBuffer, internalSize);
}

void FileBuffer::Read(FileHandle &handle, Offset location) {
  DCHECK(type != FileBufferType::TINY_BUFFER);
  handle.ReadAt(buffer, limitSize, location);
}

void FileBuffer::Write(FileHandle &handle, Offset location) {
  DCHECK(type != FileBufferType::TINY_BUFFER);
  handle.WriteAt(buffer, limitSize, location);
}

void FileBuffer::Clear() { memset(buffer, 0, limitSize); }

//===------------------------------------------------===
// find a correct size to allocate
//===------------------------------------------------===

struct BufferMemoryRequirement {
  Size header;
  Size content;
};

static auto CalculateMemory(Size reqSize, FileBufferType type)
    -> BufferMemoryRequirement {
  BufferMemoryRequirement result;
  if (type == FileBufferType::TINY_BUFFER) {
    result.header = 0_Size;
    result.content = reqSize;
  } else {
    result.header = Storage::BLOCK_HEADER_SIZE;
    result.content =
        AlignDown<Size>(result.header + reqSize, Storage::SECTOR_SIZE);
  }
  return result;
}

void FileBuffer::ReallocBuffer(Size newSize) {
  DatumPtr newBuffer = nullptr;
  if (internalBuffer) {
    newBuffer = allocator.ReallocateData(internalBuffer, newSize);
  } else {
    newBuffer = allocator.AllocateData(newSize);
  }
  if (!newBuffer) {
    LOG(FATAL) << "Failed to allocate buffer of size " << newSize;
  }
  internalBuffer = newBuffer;
  internalSize = newSize;
  // caller's business
  buffer = nullptr;
  limitSize = 0_Size;
}

void FileBuffer::Resize(Size newSize) {
  auto [headerSize, contentSize] = CalculateMemory(newSize, type);
  ReallocBuffer(contentSize);
  if (newSize > 0) {
    buffer = internalBuffer + headerSize;
    limitSize = internalSize - headerSize;
  }
}

} // namespace saturn
