/**
 * Copyright (c) 2018-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include "gloo/context.h"
#include "gloo/transport/unbound_buffer.h"

namespace gloo {

class GatherOptions {
public:
  explicit GatherOptions(const std::shared_ptr<Context>& context)
      : context(context) {
  }

  template <typename T>
  void setInput(std::unique_ptr<transport::UnboundBuffer> buf) {
    elementSize = sizeof(T);
    in = std::move(buf);
  }

  template <typename T>
  void setInput(T* ptr, size_t elements) {
    elementSize = sizeof(T);
    in = context->createUnboundBuffer(ptr, elements * sizeof(T));
  }

  template <typename T>
  void setOutput(std::unique_ptr<transport::UnboundBuffer> buf) {
    elementSize = sizeof(T);
    out = std::move(buf);
  }

  template <typename T>
  void setOutput(T* ptr, size_t elements) {
    elementSize = sizeof(T);
    out = context->createUnboundBuffer(ptr, elements * sizeof(T));
  }

  void setRoot(int root) {
    this->root = root;
  }

  void setTag(uint32_t tag) {
    this->tag = tag;
  }

protected:
  std::shared_ptr<Context> context;
  std::unique_ptr<transport::UnboundBuffer> in;
  std::unique_ptr<transport::UnboundBuffer> out;

  // Number of bytes per element.
  size_t elementSize = 0;

  // Rank of receiving process.
  int root = -1;

  // Tag for this operation.
  // Must be unique across operations executing in parallel.
  uint32_t tag = 0;

  friend void gather(GatherOptions&);
};

void gather(GatherOptions& opts);

} // namespace gloo
