//===------------------------------------------===
// Saturn 2023
//
// Identification: test/container/concurrent_queue_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "container/concurrent_queue.hh"

#include <thread>

#include "gtest/gtest.h"

TEST(ContainerTest, QueueTest) {
  auto &allocator = saturn::Allocator::GetDefaultAllocator();
  saturn::ConcurrentQueue<int> queue(allocator);
  // spawn 100 threads to enqueue
  for (int i = 0; i < 500000; ++i) {
    std::thread t([&queue, i]() {
      queue.Enqueue(i);
    });
    t.detach();
  }
  int iii = 0;
  std::thread t2([&queue, &iii]() {
    for (int i = 0; i < 500000; ++i) {
      if (queue.TryDequeue(i)) {
        iii++;
      }
    }
  });
  t2.join();
  EXPECT_EQ(iii, 500000);
}
