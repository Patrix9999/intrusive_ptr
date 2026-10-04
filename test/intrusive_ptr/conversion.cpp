#include <gtest/gtest.h>

#include <crimson_cell/intrusive_ptr.hpp>

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

  auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(base);

  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), derived.get());
  EXPECT_EQ(extension->Value(), 1337);
}

TEST(intrusive_ptr, derived_to_base_move_construction) {
  crimson_cell::intrusive_ptr derived{new RefCountedObjectExtension};

  auto* object = derived.get();

  crimson_cell::intrusive_ptr<RefCountedObject> base{std::move(derived)};

  EXPECT_FALSE(derived);
  ASSERT_TRUE(base);
  EXPECT_EQ(base.get(), object);
  EXPECT_EQ(base->RefCount(), 1);

  auto extension =
      crimson_cell::dynamic_pointer_cast<RefCountedObjectExtension>(base);

  ASSERT_TRUE(extension);
  EXPECT_EQ(extension.get(), base.get());
  EXPECT_EQ(extension->Value(), 1337);
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