#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"
#include "fixtures/RefCountedObjectExtension.hpp"

TEST(intrusive_ptr, bool_conversion_from_reset) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;
  RefCountedObject* raw = new RefCountedObject;

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, bool_conversion_from_copy) {
  crimson_cell::intrusive_ptr<RefCountedObject> src{new RefCountedObject};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> copy;
    copy = src;
    if (copy) {
      src.reset();
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, bool_conversion_from_derived) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> src{
      new RefCountedObjectExtension};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> base;
    base = src;
    if (base) {
      src.reset();
    } else {
      EXPECT_FALSE(base);
    }
  }
}

TEST(intrusive_ptr, null_comparison) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;

  EXPECT_EQ(ptr, nullptr);

  ptr.reset(new RefCountedObject);

  EXPECT_NE(ptr, nullptr);
}

TEST(intrusive_ptr, equality_comparison) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  const crimson_cell::intrusive_ptr second{first};

  EXPECT_EQ(first, second);
}

TEST(intrusive_ptr, inequality_comparison) {
  const crimson_cell::intrusive_ptr first{new RefCountedObject};
  const crimson_cell::intrusive_ptr second{new RefCountedObject};

  EXPECT_NE(first, second);
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