//
// Created by jeremiah tesfaye on 10/25/25.
//

#include <gtest/gtest.h>
#include <util/LRUCache.h>

TEST(LRUCacheTest, InsertAndRetrieveSingleItem) {
    LRUCache<int> cache(2);

    cache.add(1, 100);
    auto value = cache.get(1);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 100);
}

TEST(LRUCacheTest, GetNonexistentItemReturnsNullopt) {
    LRUCache<int> cache(2);

    cache.add(1, 100);
    auto value = cache.get(2);

    EXPECT_FALSE(value.has_value());
}

TEST(LRUCacheTest, EvictsLeastRecentlyUsedItem) {
    LRUCache<int> cache(2);

    cache.add(1, 100);
    cache.add(2, 200);
    cache.add(3, 300);

    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_TRUE(cache.get(2).has_value());
    EXPECT_TRUE(cache.get(3).has_value());
}

TEST(LRUCacheTest, AccessUpdatesRecency) {
    LRUCache<int> cache(2);

    cache.add(1, 100);
    cache.add(2, 200);

    auto val = cache.get(1);
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 100);

    cache.add(3, 300);

    EXPECT_TRUE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_TRUE(cache.get(3).has_value());
}

TEST(LRUCacheTest, ReplaceExistingKeyUpdatesValue) {
    LRUCache<int> cache(2);

    cache.add(1, 100);
    cache.add(1, 200);

    auto value = cache.get(1);
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 200);
}

TEST(LRUCacheTest, MaintainsMaxLimit) {
    constexpr size_t MAX_SIZE = 3;
    LRUCache<int> cache(MAX_SIZE);

    cache.add(1, 10);
    cache.add(2, 20);
    cache.add(3, 30);
    cache.add(4, 40);

    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_TRUE(cache.get(2).has_value());
    EXPECT_TRUE(cache.get(3).has_value());
    EXPECT_TRUE(cache.get(4).has_value());
}