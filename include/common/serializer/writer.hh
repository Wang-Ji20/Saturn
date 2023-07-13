//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/writer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/serializer/serializer.hh"

namespace saturn {

class Writer {
  Writer(Serializer &serializer) : serializer(serializer) {}

protected:
  Serializer &serializer;
};

} // namespace saturn
