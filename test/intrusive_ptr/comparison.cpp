#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"

TEST(intrusive_ptr, null_comparison) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;

  EXPECT_EQ(ptr, nullptr);
  EXPECT_FALSE(ptr != nullptr);

  ptr.reset(new RefCountedObject);

  EXPECT_NE(ptr, nullptr);
  EXPECT_FALSE(ptr == nullptr);
}

TEST(intrusive_ptr, equality_comparison) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr second{first};
  const crimson_cell::intrusive_ptr third{new RefCountedObject};

  EXPECT_TRUE(first == second);
  EXPECT_FALSE(first == third);
}

TEST(intrusive_ptr, inequality_comparison) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr second{first};
  const crimson_cell::intrusive_ptr third{new RefCountedObject};

  EXPECT_FALSE(first != second);
  EXPECT_TRUE(first != third);
}

TEST(intrusive_ptr, three_way_comparison_pointers) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr second{first};

  EXPECT_EQ(first <=> second, std::strong_ordering::equal);
}

TEST(intrusive_ptr, three_way_comparison_nullptr) {
  const crimson_cell::intrusive_ptr<RefCountedObject> first;

  EXPECT_EQ(first <=> nullptr, std::strong_ordering::equal);
}