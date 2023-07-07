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
  std::vector<std::thread> threads;
  for (int i = 0; i < 50000; ++i) {
    threads.emplace_back([&queue, i]() {
      for (int j = 0; j < 100; j++) {
        queue.Enqueue(i);
      }
    });
  }
  // wait all
  for (auto &t : threads) {
    t.join();
  }
  int iii = 0;
  std::thread t2([&queue, &iii]() {
    for (int i = 0; i < 5000000; ++i) {
      if (queue.TryDequeue(i)) {
        iii++;
      }
    }
  });
  t2.join();
  EXPECT_EQ(iii, 5000000);
}
