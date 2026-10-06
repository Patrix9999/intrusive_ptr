#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"

TEST(intrusive_ptr, reset_method_no_args) {
  crimson_cell::intrusive_ptr ptr{new RefCountedObject};
  ptr.reset();

  EXPECT_EQ(ptr.get(), nullptr);
}

TEST(intrusive_ptr, reset_method_acquires_reference) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;
  ptr.reset(new RefCountedObject);

  ASSERT_TRUE(ptr);
  EXPECT_EQ(ptr->RefCount(), 1);
}

TEST(intrusive_ptr, swap_method) {
  auto* first_object = new RefCountedObject{};
  auto* second_object = new RefCountedObject{};

  crimson_cell::intrusive_ptr first_ptr{first_object};
  crimson_cell::intrusive_ptr second_ptr{second_object};

  first_ptr.swap(second_ptr);

  ASSERT_TRUE(first_ptr);
  ASSERT_TRUE(second_ptr);
  EXPECT_EQ(first_ptr.get(), second_object);
  EXPECT_EQ(second_ptr.get(), first_object);
  EXPECT_EQ(first_ptr->RefCount(), 1);
  EXPECT_EQ(second_ptr->RefCount(), 1);
}

TEST(intrusive_ptr, swap_function) {
  auto* first_object = new RefCountedObject{};
  auto* second_object = new RefCountedObject{};

  crimson_cell::intrusive_ptr first_ptr{first_object};
  crimson_cell::intrusive_ptr second_ptr{second_object};

  crimson_cell::swap(first_ptr, second_ptr);

  ASSERT_TRUE(first_ptr);
  ASSERT_TRUE(second_ptr);
  EXPECT_EQ(first_ptr.get(), second_object);
  EXPECT_EQ(second_ptr.get(), first_object);
  EXPECT_EQ(first_ptr->RefCount(), 1);
  EXPECT_EQ(second_ptr->RefCount(), 1);
}