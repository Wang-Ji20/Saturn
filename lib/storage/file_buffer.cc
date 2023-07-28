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
                       MemoryByte requestSize)
    : allocator{allocator}, type{type} {
  if (requestSize != 0U) {
    Resize(requestSize);
  }
  Clear();
}

FileBuffer::FileBuffer(FileBuffer &source, FileBufferType type)
    : allocator{source.allocator},
      type{type},
      buffer(source.buffer),
      limitSize{source.limitSize},
      internalBuffer(source.internalBuffer),
      internalSize(source.internalSize) {
  Clear();
}

FileBuffer::~FileBuffer() {
  if (internalBuffer == nullptr) {
    return;
  }
  allocator.FreeData(internalBuffer, internalSize);
}

void FileBuffer::Read(FileHandle &handle, MemoryByte location) const {
  DCHECK(type != FileBufferType::TINY_BUFFER);
  handle.ReadAt(buffer, limitSize, location);
}

void FileBuffer::Write(FileHandle &handle, MemoryByte location) const {
  DCHECK(type != FileBufferType::TINY_BUFFER);
  handle.WriteAt(buffer, limitSize, location);
}

void FileBuffer::Clear() { memset(internalBuffer, 0, internalSize); }

//===------------------------------------------------===
// find a correct size to allocate
//===------------------------------------------------===

struct BufferMemoryRequirement {
  MemoryByte header;
  MemoryByte content;
};

static auto CalculateMemory(MemoryByte reqSize, FileBufferType type)
    -> BufferMemoryRequirement {
  BufferMemoryRequirement result{};
  if (type == FileBufferType::TINY_BUFFER) {
    result.header = 0ULL;
    result.content = reqSize;
  } else {
    result.header = Storage::BLOCK_HEADER_SIZE;
    result.content =
        AlignDown<MemoryByte>(result.header + reqSize, Storage::SECTOR_SIZE);
  }
  return result;
}

void FileBuffer::ReallocBuffer(MemoryByte newSize) {
  DatumPtr newBuffer = nullptr;
  if (internalBuffer != nullptr) {
    newBuffer = allocator.ReallocateData(internalBuffer, newSize);
  } else {
    newBuffer = allocator.AllocateData(newSize);
  }
  if (newBuffer == nullptr) {
    LOG(FATAL) << "Failed to allocate buffer of size " << newSize;
  }
  internalBuffer = newBuffer;
  internalSize = newSize;
  // caller's business
  buffer = nullptr;
  limitSize = 0ULL;
}

void FileBuffer::Resize(MemoryByte newSize) {
  auto [headerSize, contentSize] = CalculateMemory(newSize, type);
  ReallocBuffer(contentSize);
  if (newSize > 0) {
    buffer = internalBuffer + headerSize;
    limitSize = internalSize - headerSize;
  }
}

} // namespace saturn
