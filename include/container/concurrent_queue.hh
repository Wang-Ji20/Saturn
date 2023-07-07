//===------------------------------------------===
// Saturn 2023
//
// Identification: include\container\concurrent_queue.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// provides multi-producer, multi-consumer lock-free queue.
//===----------------------------------------------------===

#pragma once

#include "common/atomic.hh"
#include "common/macro.hh"
#include "common/mutex.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

#include "storage/allocator.hh"

namespace saturn {

template <typename T> class ConcurrentQueue {
public:
  ConcurrentQueue(Allocator &allocator) : allocator_(allocator) {}

  DISALLOW_COPY_AND_MOVE(ConcurrentQueue);

  ~ConcurrentQueue() {
    scoped_lock<mutex> lock{lock_};
    auto *freehead = head_.load();
    while (head_.load() != nullptr) {
      head_ = head_.load()->next;
      allocator_.FreeData(freehead, NODE_SIZE);
    }
  }

  void Enqueue(T &&value) {
    auto *node = allocator_.AllocateData<Node>(NODE_SIZE);
    node->value = std::move(value);
    node->next = nullptr;
    auto *tail = tail_.load();
    while (!tail_.compare_exchange_weak(tail, node)) {
    }
    tail->next = node;
  }

  auto TryDequeue(T &item) -> bool {
    auto *head = head_.load();
    while (head != nullptr && head_.compare_exchange_weak(head, head->next)) {
      item = head->value;
      allocator_.FreeData(head, NODE_SIZE);
      return true;
    }
    return false;
  }

private:
  struct Node {
    T value;
    Node *next;
  };
  constexpr static Size NODE_SIZE = Size(sizeof(Node));
  atomic<Node *> head_{nullptr};
  atomic<Node *> tail_{nullptr};
  mutable mutex lock_;
  Allocator &allocator_;
};

} // namespace saturn
