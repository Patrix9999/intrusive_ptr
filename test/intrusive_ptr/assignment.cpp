#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"

TEST(intrusive_ptr, copy_assignment) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  // ReSharper disable once CppJoinDeclarationAndAssignment
  crimson_cell::intrusive_ptr<RefCountedObject> second;
  second = first;

  ASSERT_TRUE(second);
  EXPECT_EQ(second->RefCount(), 2);
  EXPECT_EQ(first, second);
}

TEST(intrusive_ptr, move_assignment) {
  crimson_cell::intrusive_ptr first{new RefCountedObject};

  ASSERT_TRUE(first);
  EXPECT_EQ(first->RefCount(), 1);

  auto* object = first.get();

  // ReSharper disable once CppJoinDeclarationAndAssignment
  crimson_cell::intrusive_ptr<RefCountedObject> second;
  second = std::move(first);

  ASSERT_FALSE(first);
  ASSERT_TRUE(second);
  EXPECT_EQ(second.get(), object);
  EXPECT_EQ(second->RefCount(), 1);
}