#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

#include "fixtures/RefCountedObject.hpp"

TEST(intrusive_ptr, dereference_operator) {
  auto* object = new RefCountedObject{};
  const crimson_cell::intrusive_ptr ptr{object};

  EXPECT_EQ(&*ptr, object);
}

TEST(intrusive_ptr, arrow_operator) {
  auto* object = new RefCountedObject{};
  const crimson_cell::intrusive_ptr ptr{object};

  EXPECT_EQ(ptr->RefCount(), object->RefCount());
}

TEST(intrusive_ptr, get_method) {
  auto* object = new RefCountedObject{};
  const crimson_cell::intrusive_ptr ptr{object};

  EXPECT_EQ(ptr.get(), object);
}

TEST(intrusive_ptr, detach_method) {
  auto* object = new RefCountedObject{};
  crimson_cell::intrusive_ptr ptr{object};

  auto* detached = ptr.release();

  EXPECT_EQ(object, detached);
  EXPECT_FALSE(ptr);
  EXPECT_EQ(detached->RefCount(), 1);

  detached->Release();
}