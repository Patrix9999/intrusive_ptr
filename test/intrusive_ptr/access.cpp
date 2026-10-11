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
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> ptr;
    ptr.reset(raw);
    if (ptr.operator->()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_copy_assignment) {
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> copy;
    copy = src;
    if (copy.operator->()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_assignment) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};
  RefCountedObject* initial_base = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> base;
    base.reset(initial_base);
    base = src;
    if (base.operator->()) {
      raw = nullptr;
      initial_base = nullptr;
    } else {
      EXPECT_FALSE(base);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_reset) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};
  RefCountedObjectExtension* initial = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> ptr;
    ptr.reset(initial);
    ptr.reset(raw);
    if (ptr.operator->()) {
      raw = nullptr;
      initial = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, arrow_operator_from_derived_copy_assignment) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> copy;
    copy = src;
    if (copy.operator->()) {
      raw = nullptr;
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
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> ptr;
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_from_copy_assignment) {
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> copy;
    copy = src;
    if (copy.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_assignment) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> base;
    base = src;
    if (base.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(base);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_reset) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> ptr;
    ptr.reset(raw);
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_from_derived_copy_assignment) {
  RefCountedObjectExtension* raw = new RefCountedObjectExtension{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObjectExtension> copy;
    copy = src;
    if (copy.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(copy);
    }
  }
}

TEST(intrusive_ptr, get_method_from_move) {
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> src;
    src.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> ptr{std::move(src)};
    if (ptr.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(ptr);
    }
  }
}

TEST(intrusive_ptr, get_method_from_copy_constructor) {
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> src;
    src.reset(raw);
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    crimson_cell::intrusive_ptr<RefCountedObject> copy{src};
    src.reset(copy.get());
    if (src.get()) {
      raw = nullptr;
    } else {
      EXPECT_FALSE(src);
    }
  }
}

TEST(intrusive_ptr, get_method_after_swap) {
  RefCountedObject* raw = new RefCountedObject{};

  for (int i = 0; i < 2; ++i) {
    crimson_cell::intrusive_ptr<RefCountedObject> other;
    other.reset(raw);
    crimson_cell::intrusive_ptr<RefCountedObject> ptr;
    ptr.swap(other);
    ptr.reset(ptr.get());
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