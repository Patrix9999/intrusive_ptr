#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>
#include <utility>

#include "fixtures/RefCountedObject.hpp"
#include "fixtures/RefCountedObjectExtension.hpp"

TEST(intrusive_ptr, default_constructor) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;

  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_FALSE(ptr);
}

TEST(intrusive_ptr, add_ref_constructor) {
  crimson_cell::intrusive_ptr ptr{new RefCountedObject};

  ASSERT_TRUE(ptr);
  EXPECT_EQ(ptr->RefCount(), 1);
}

TEST(intrusive_ptr, add_ref_constructor_from_null_then_reset) {
  RefCountedObject* raw = new RefCountedObject;

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> ptr{nullptr};
    if (i == 0) {
      ptr.reset(raw);
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, add_ref_constructor_from_null_then_copy_assignment) {
  RefCountedObject* raw = new RefCountedObject;

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> ptr{nullptr};
    if (i == 0) {
      ptr = src;
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, add_ref_constructor_from_null_then_derived_assignment) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension;

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> ptr{nullptr};
    if (i == 0) {
      ptr = src;
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, add_ref_constructor_derived_from_null_then_reset) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension;

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> ptr{nullptr};
    if (i == 0) {
      ptr.reset(raw);
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, no_add_ref_constructor) {
  auto* raw = new RefCountedObject;
  raw->AddRef();

  crimson_cell::intrusive_ptr ptr{raw, crimson_cell::adopt_ref};

  ASSERT_TRUE(ptr);
  EXPECT_EQ(ptr->RefCount(), 1);
}

TEST(intrusive_ptr, copy_constructor_from_reset) {
  RefCountedObject* raw = new RefCountedObject;

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> first;
    first.reset(raw);
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    crimson_cell::intrusive_ptr<RefCountedObject> second{first};
    if (second) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(second);
    }
  }
}

TEST(intrusive_ptr, copy_constructor_from_empty_then_reset) {
  const crimson_cell::intrusive_ptr<RefCountedObject> empty;
  RefCountedObject* raw = new RefCountedObject;

  for (int i = 0; i < 2; ++i) {
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    crimson_cell::intrusive_ptr<RefCountedObject> copy{empty};
    if (i == 0) {
      copy.reset(raw);
      raw = nullptr;
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, copy_constructor_derived_from_empty_then_reset) {
  const crimson_cell::intrusive_ptr<RefCountedObjectExtension> empty;
  RefCountedObjectExtension* raw = new RefCountedObjectExtension;

  for (int i = 0; i < 2; ++i) {
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> copy{empty};
    if (i == 0) {
      copy.reset(raw);
      raw = nullptr;
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, copy_constructor_add_ref) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  crimson_cell::intrusive_ptr second{first};

  ASSERT_TRUE(second);
  EXPECT_EQ(second, first);
  EXPECT_EQ(first->RefCount(), 2);
  EXPECT_EQ(second->RefCount(), 2);
}

TEST(intrusive_ptr, copy_constructor_release_ref) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  {
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    crimson_cell::intrusive_ptr second{first};

    EXPECT_EQ(first->RefCount(), 2);
  }

  EXPECT_EQ(first->RefCount(), 1);
}

TEST(intrusive_ptr, move_constructor_transfers_ownership) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  crimson_cell::intrusive_ptr second{std::move(first)};

  EXPECT_FALSE(first);
  ASSERT_TRUE(second);
  EXPECT_EQ(second->RefCount(), 1);
}