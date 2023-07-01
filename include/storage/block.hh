//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\block.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===--------------------------------------===
// block is the basic unit of storage, aka. page.
//===--------------------------------------===

#pragma once

#include "storage/file_buffer.hh"

namespace saturn {

class Block : FileBuffer {};

} // namespace saturn
