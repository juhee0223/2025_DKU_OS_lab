#include "skiplist.h"
#include "skiplist_impl.h"
#include "test_util.h"
#include "gtest/gtest.h"

/// @brief Test SkipList without lock in single-thread
TEST_P(SkipListSingleThreadTest, Single) {
  skiplist = new SkipList();
}

/// @brief Test SkipList with coarse-grained lock in single-thread
TEST_P(SkipListSingleThreadTest, Coarse) {
  skiplist = new CoarseSkipList();
}

/// @brief Test SkipList with fine-grained lock in single-thread
TEST_P(SkipListSingleThreadTest, Fine) {  
  skiplist = new FineSkipList();
}

/// @brief SkipListSingleThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, SkipListSingleThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(INSERT_ONLY, 1000000, 1),
    std::make_tuple(INSERT_LOOKUP, 1000000, 1),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 1)
  )
);

/// @brief Test SkipList with coarse-grained lock in multi-thread
TEST_P(SkipListMultiThreadTest, Coarse) {
  // Test SkipList with coarse-grained lock
  skiplist = new CoarseSkipList();
}

/// @brief Test SkipList with fine-grained lock in multi-thread
TEST_P(SkipListMultiThreadTest, Fine) {
  skiplist = new FineSkipList();
}

/// @brief SkipListMultiThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, SkipListMultiThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(INSERT_ONLY, 1000000, 2),
    std::make_tuple(INSERT_ONLY, 1000000, 4),
    std::make_tuple(INSERT_ONLY, 1000000, 8),
    std::make_tuple(INSERT_LOOKUP, 1000000, 2),
    std::make_tuple(INSERT_LOOKUP, 1000000, 4),
    std::make_tuple(INSERT_LOOKUP, 1000000, 8),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 2),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 4),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 8)
  )
);

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
} 