//===-- VMRange.cpp ---------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "lldb/Utility/VMRange.h"

#include "lldb/Utility/Stream.h"
#include "lldb/lldb-types.h"

#include <algorithm>
#include <iterator>
#include <vector>

#include <stddef.h>
#include <stdint.h>

using namespace lldb;
using namespace lldb_private;

bool VMRange::ContainsValue(const VMRange::collection &coll,
                            lldb::addr_t value) {
  return llvm::find_if(coll, [&](const VMRange &r) {
           return r.Contains(value);
         }) != coll.end();
}

bool VMRange::ContainsRange(const VMRange::collection &coll,
                            const VMRange &range) {
  return llvm::find_if(coll, [&](const VMRange &r) {
           return r.Contains(range);
         }) != coll.end();
}

void VMRange::Dump(Stream *s, lldb::addr_t offset, uint32_t addr_width) const {
  DumpAddressRange(s->AsRawOstream(), offset + GetBaseAddress(),
                   offset + GetEndAddress(), addr_width);
}

bool lldb_private::operator==(const VMRange &lhs, const VMRange &rhs) {
  return lhs.GetBaseAddress() == rhs.GetBaseAddress() &&
         lhs.GetEndAddress() == rhs.GetEndAddress();
}

bool lldb_private::operator!=(const VMRange &lhs, const VMRange &rhs) {
  return !(lhs == rhs);
}

bool lldb_private::operator<(const VMRange &lhs, const VMRange &rhs) {
  if (lhs.GetBaseAddress() < rhs.GetBaseAddress())
    return true;
  else if (lhs.GetBaseAddress() > rhs.GetBaseAddress())
    return false;
  return lhs.GetEndAddress() < rhs.GetEndAddress();
}

bool lldb_private::operator<=(const VMRange &lhs, const VMRange &rhs) {
  return !(lhs > rhs);
}

bool lldb_private::operator>(const VMRange &lhs, const VMRange &rhs) {
  return rhs < lhs;
}

bool lldb_private::operator>=(const VMRange &lhs, const VMRange &rhs) {
  return !(lhs < rhs);
}
