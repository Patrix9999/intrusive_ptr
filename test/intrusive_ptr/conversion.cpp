#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>
#include <utility>

#include "fixtures/RefCountedObject.hpp"
#include "fixtures/RefCountedObjectExtension.hpp"

TEST(intrusive_ptr, derived_to_base_copy_construction) {
  crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  ASSERT_TRUE(derived);
  EXPECT_EQ(derived->RefCount(), 1);
  EXPECT_EQ(derived->Value(), 1337);

  crimson_cell::intrusive_ptr<RefCountedObject> base{derived};

  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), derived.get());
  EXPECT_EQ(base->RefCount(), 2);
  EXPECT_EQ(derived->RefCount(), 2);
}

TEST(intrusive_ptr, derived_to_base_move_construction) {
  crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  auto* object = derived.get();

  crimson_cell::intrusive_ptr<RefCountedObject> base{std::move(derived)};

  EXPECT_FALSE(derived);
  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), object);
  EXPECT_EQ(base->RefCount(), 1);
}

TEST(intrusive_ptr, derived_to_base_copy_assignment) {
  const crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  crimson_cell::intrusive_ptr<RefCountedObject> base;
  base = derived;

  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), derived.get());
  EXPECT_EQ(base->RefCount(), 2);
  EXPECT_EQ(derived->RefCount(), 2);
}

TEST(intrusive_ptr, derived_to_base_move_assignment) {
  crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  auto* object = derived.get();

  crimson_cell::intrusive_ptr<RefCountedObject> base;

  base = std::move(derived);

  EXPECT_FALSE(derived);
  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), object);
  EXPECT_EQ(base->RefCount(), 1);
}

TEST(intrusive_ptr, conversion_size) {
  EXPECT_EQ(sizeof(crimson_cell::intrusive_ptr<RefCountedObject>),
            sizeof(RefCountedObject*));

  EXPECT_EQ(sizeof(crimson_cell::intrusive_ptr<RefCountedObjectExtension>),
            sizeof(RefCountedObjectExtension*));

  EXPECT_EQ(sizeof(crimson_cell::intrusive_ptr<RefCountedObject>),
            sizeof(crimson_cell::intrusive_ptr<RefCountedObjectExtension>));
}

TEST(intrusive_ptr, static_pointer_cast_lvalue) {
  const crimson_cell::intrusive_ptr<RefCountedObject> base{
      new RefCountedObjectExtension};

  const auto extension =
      crimson_cell::static_pointer_cast<RefCountedObjectExtension>(base);

  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), base.get());
  EXPECT_EQ(extension->RefCount(), 2);
  EXPECT_EQ(extension->Value(), 1337);
}

TEST(intrusive_ptr, static_pointer_cast_rvalue) {
  crimson_cell::intrusive_ptr<RefCountedObject> base{
      new RefCountedObjectExtension};
  auto* raw = base.get();

  const auto extension =
      crimson_cell::static_pointer_cast<RefCountedObjectExtension>(
          std::move(base));

  EXPECT_FALSE(base);
  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), raw);
  EXPECT_EQ(extension->RefCount(), 1);
  EXPECT_EQ(extension->Value(), 1337);
}

TEST(intrusive_ptr, const_pointer_cast_lvalue) {
  const crimson_cell::intrusive_ptr<const RefCountedObject> const_ptr{
      new RefCountedObject};

  const auto mutable_ptr =
      crimson_cell::const_pointer_cast<RefCountedObject>(const_ptr);

  ASSERT_TRUE(mutable_ptr);
  EXPECT_EQ(mutable_ptr.get(), const_ptr.get());
  EXPECT_EQ(mutable_ptr->RefCount(), 2);
}

TEST(intrusive_ptr, const_pointer_cast_rvalue) {
  crimson_cell::intrusive_ptr<const RefCountedObject> const_ptr{
      new RefCountedObject};
  const auto* raw = const_ptr.get();

  const auto mutable_ptr =
      crimson_cell::const_pointer_cast<RefCountedObject>(std::move(const_ptr));

  EXPECT_FALSE(const_ptr);
  ASSERT_TRUE(mutable_ptr);
  EXPECT_EQ(mutable_ptr.get(), raw);
  EXPECT_EQ(mutable_ptr->RefCount(), 1);
}

TEST(intrusive_ptr, dynamic_pointer_cast_lvalue) {
  const crimson_cell::intrusive_ptr<RefCountedObject> base{
      new RefCountedObjectExtension};

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(base);

  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), base.get());
  EXPECT_EQ(extension->RefCount(), 2);
  EXPECT_EQ(extension->Value(), 1337);
}

TEST(intrusive_ptr, dynamic_pointer_cast_lvalue_null) {
  const crimson_cell::intrusive_ptr<RefCountedObject> base;

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(base);

  EXPECT_EQ(extension, nullptr);
}

TEST(intrusive_ptr, dynamic_pointer_cast_lvalue_failed) {
  const crimson_cell::intrusive_ptr base{new RefCountedObject};

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(base);

  EXPECT_EQ(extension, nullptr);
  EXPECT_EQ(base->RefCount(), 1);
}

TEST(intrusive_ptr, dynamic_pointer_cast_rvalue) {
  crimson_cell::intrusive_ptr<RefCountedObject> base{
      new RefCountedObjectExtension};
  auto* raw = base.get();

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(
          std::move(base));

  EXPECT_FALSE(base);
  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), raw);
  EXPECT_EQ(extension->RefCount(), 1);
  EXPECT_EQ(extension->Value(), 1337);
}

TEST(intrusive_ptr, dynamic_pointer_cast_rvalue_null) {
  crimson_cell::intrusive_ptr<RefCountedObject> base;

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(
          std::move(base));

  EXPECT_FALSE(base);
  EXPECT_EQ(extension, nullptr);
}

TEST(intrusive_ptr, dynamic_pointer_cast_rvalue_failed) {
  crimson_cell::intrusive_ptr base{new RefCountedObject};
  auto* raw = base.get();

  const auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(
          std::move(base));

  EXPECT_EQ(extension, nullptr);
  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), raw);
  EXPECT_EQ(base->RefCount(), 1);
}

TEST(intrusive_ptr, reinterpret_pointer_cast_lvalue) {
  const crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  const auto base =
      crimson_cell::reinterpret_pointer_cast<RefCountedObject>(derived);

  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), reinterpret_cast<RefCountedObject*>(derived.get()));
  EXPECT_EQ(base->RefCount(), 2);
}

TEST(intrusive_ptr, reinterpret_pointer_cast_rvalue) {
  crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};
  auto* raw = derived.get();

  const auto base = crimson_cell::reinterpret_pointer_cast<RefCountedObject>(
      std::move(derived));

  EXPECT_FALSE(derived);
  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), reinterpret_cast<RefCountedObject*>(raw));
  EXPECT_EQ(base->RefCount(), 1);
}