#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>
#include <utility>

#include "fixtures/RefCountedObject.hpp"
#include "fixtures/RefCountedObjectExtension.hpp"

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

TEST(intrusive_ptr, arrow_operator_from_reset) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.operator->()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_copy_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObject> src{new RefCountedObject{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> copy;
    copy = src;
    if (copy.operator->()) {
      src.reset();
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> src{
      new RefCountedObjectExtension{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> base;
    base = src;
    if (base.operator->()) {
      src.reset();
    } else {
      EXPECT_FALSE(base);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_reset) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> ptr;
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.operator->()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_copy_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> src{
      new RefCountedObjectExtension{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> copy;
    copy = src;
    if (copy.operator->()) {
      src.reset();
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, get_method) {
  auto* object = new RefCountedObject{};
  const crimson_cell::intrusive_ptr ptr{object};

  EXPECT_EQ(ptr.get(), object);
}

TEST(intrusive_ptr, get_method_from_reset) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr;
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_from_copy_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObject> src{new RefCountedObject{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> copy;
    copy = src;
    if (copy.get()) {
      src.reset();
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> src{
      new RefCountedObjectExtension{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> base;
    base = src;
    if (base.get()) {
      src.reset();
    } else {
      EXPECT_FALSE(base);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_reset) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> ptr;
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_copy_assignment) {
  crimson_cell::intrusive_ptr<RefCountedObjectExtension> src{
      new RefCountedObjectExtension{}};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> copy;
    copy = src;
    if (copy.get()) {
      src.reset();
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, get_method_after_move) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr{new RefCountedObject{}};
  const crimson_cell::intrusive_ptr<RefCountedObject> moved{std::move(ptr)};
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_after_swap) {
  crimson_cell::intrusive_ptr<RefCountedObject> ptr{new RefCountedObject{}};
  crimson_cell::intrusive_ptr<RefCountedObject> other;
  ptr.swap(other);
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
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