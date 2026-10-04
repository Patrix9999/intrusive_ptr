#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"

// ReSharper disable once CppUseInternalLinkage
void intrusive_ptr_add_ref(RefCountedObject* object) noexcept {
  object->AddRef();
}

// ReSharper disable once CppUseInternalLinkage
void intrusive_ptr_release(RefCountedObject* object) noexcept {
  object->Release();
}

TEST(intrusive_ptr, default_constructor) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;

  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_FALSE(ptr);
}

TEST(intrusive_ptr, add_ref_constructor) {
  crimson_cell::intrusive_ptr ptr{new RefCountedObject};

  ASSERT_TRUE(ptr);
  EXPECT_EQ(ptr.get()->RefCount(), 1);
}

TEST(intrusive_ptr, no_add_ref_constructor) {
  auto* raw = new RefCountedObject;
  raw->AddRef();

  crimson_cell::intrusive_ptr ptr{raw, false};

  ASSERT_TRUE(ptr);
  EXPECT_EQ(ptr->RefCount(), 1);
}

TEST(intrusive_ptr, copy_constructor_add_ref) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  crimson_cell::intrusive_ptr second{first};

  ASSERT_TRUE(second);
  EXPECT_EQ(second.get(), first.get());
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