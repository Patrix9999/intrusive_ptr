#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>
#include <functional>
#include <unordered_set>

#include "fixtures/RefCountedObject.hpp"

TEST(intrusive_ptr, hash) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr second{first};
  const crimson_cell::intrusive_ptr third{new RefCountedObject};

  constexpr std::hash<crimson_cell::intrusive_ptr<RefCountedObject>> hasher;

  EXPECT_EQ(hasher(first), hasher(second));
  EXPECT_NE(hasher(first), hasher(third));
}

TEST(intrusive_ptr, unordered_set) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  const crimson_cell::intrusive_ptr second{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr same_as_first{first};

  std::unordered_set<crimson_cell::intrusive_ptr<RefCountedObject>> set;

  set.insert(first);
  set.insert(second);
  set.insert(same_as_first);

  EXPECT_EQ(set.size(), 2);
  EXPECT_TRUE(set.contains(first));
  EXPECT_TRUE(set.contains(second));
  EXPECT_TRUE(set.contains(same_as_first));
}