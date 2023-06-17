#include <gmock/gmock.h>

#include "kingw/mock/ser/mock_serializer.hpp"
#include "kingw/ser/templates/stdmap.hpp"
#include "kingw/ser/templates/stdvector.hpp"

using namespace kingw;
using namespace testing;


namespace {

/// ser::serialize<std::map<K, V>>(Serializer, std::map<K, V>) will correctly
/// insert contents into Serializer.
TEST(KingwSerde, StdMapSerialize) {
    ser::MockSerializer serializer;
    EXPECT_CALL(serializer, map_begin(3))
        .Times(1);
    EXPECT_CALL(serializer, map_serialize_entry(_, _))
        .Times(3)
        .WillRepeatedly([&](const ser::Serialize & key, const ser::Serialize & value) {
            key.serialize(serializer);
            value.serialize(serializer);
        });
    EXPECT_CALL(serializer, map_end())
        .Times(1);

    // Each key must come before the value, but the order in which pairs
    // are serialized is not specified.
    Sequence kvp1;
    EXPECT_CALL(serializer, serialize_i32(1)).Times(1).InSequence(kvp1);
    EXPECT_CALL(serializer, serialize_i32(2)).Times(1).InSequence(kvp1);
    Sequence kvp2;
    EXPECT_CALL(serializer, serialize_i32(3)).Times(1).InSequence(kvp2);
    EXPECT_CALL(serializer, serialize_i32(4)).Times(1).InSequence(kvp2);
    Sequence kvp3;
    EXPECT_CALL(serializer, serialize_i32(5)).Times(1).InSequence(kvp3);
    EXPECT_CALL(serializer, serialize_i32(6)).Times(1).InSequence(kvp3);

    std::map<int, int> map;
    map[1] = 2;
    map[3] = 4;
    map[5] = 6;
    ser::serialize(serializer, map);
}

/// ser::accessor(std::map<K, V>)::serialize() invokes ser::serialize(std::map<K, V>)
///
TEST(KingwSerde, StdMapSerAccessor) {
    ser::MockSerializer serializer;
    EXPECT_CALL(serializer, map_begin(3))
        .Times(1);
    EXPECT_CALL(serializer, map_serialize_entry(_, _))
        .Times(3)
        .WillRepeatedly([&](const ser::Serialize & key, const ser::Serialize & value) {
            key.serialize(serializer);
            value.serialize(serializer);
        });
    EXPECT_CALL(serializer, map_end())
        .Times(1);

    // Each key must come before the value, but the order in which pairs
    // are serialized is not specified.
    Sequence kvp1;
    EXPECT_CALL(serializer, serialize_i32(1)).Times(1).InSequence(kvp1);
    EXPECT_CALL(serializer, serialize_i32(2)).Times(1).InSequence(kvp1);
    Sequence kvp2;
    EXPECT_CALL(serializer, serialize_i32(3)).Times(1).InSequence(kvp2);
    EXPECT_CALL(serializer, serialize_i32(4)).Times(1).InSequence(kvp2);
    Sequence kvp3;
    EXPECT_CALL(serializer, serialize_i32(5)).Times(1).InSequence(kvp3);
    EXPECT_CALL(serializer, serialize_i32(6)).Times(1).InSequence(kvp3);

    std::map<int, int> map;
    map[1] = 2;
    map[3] = 4;
    map[5] = 6;
    ser::Accessor<std::map<int, int>> accessor = ser::accessor(map);
    accessor.serialize(serializer);
}

/// ser::Accessor<std::map<K, V>>::traits() invokes TypeTraits::of<std::map<K, V>>()
///
TEST(KingwSerde, StdMapSerAccessorTraits) {
    std::map<int, int> map;
    ser::Accessor<std::map<int, int>> accessor = ser::accessor(map);
    auto traits = serde::TypeTraits::of<std::map<int, int>>();
    EXPECT_EQ(accessor.traits(), traits);
}


/// ser::serialize<std::vector<T>>(Serializer, std::vector<T>) will correctly
/// insert contents into Serializer.
TEST(KingwSerde, StdVectorSerialize) {
    ser::MockSerializer serializer;
    EXPECT_CALL(serializer, seq_begin(3))
        .Times(1);
    EXPECT_CALL(serializer, seq_serialize_element(_))
        .Times(3)
        .WillRepeatedly([&](const ser::Serialize & element) {
            element.serialize(serializer);
        });
    EXPECT_CALL(serializer, seq_end())
        .Times(1);

    InSequence order;  // Elements must be serialized in this order.
    EXPECT_CALL(serializer, serialize_i32(1)).Times(1);
    EXPECT_CALL(serializer, serialize_i32(2)).Times(1);
    EXPECT_CALL(serializer, serialize_i32(3)).Times(1);

    std::vector<int> vec{ 1, 2, 3 };
    ser::serialize(serializer, vec);
}

/// ser::accessor(std::vector<T>)::serialize() invokes ser::serialize(std::vector<T>)
///
TEST(KingwSerde, StdVectorSerAccessor) {
    ser::MockSerializer serializer;
    EXPECT_CALL(serializer, seq_begin(3))
        .Times(1);
    EXPECT_CALL(serializer, seq_serialize_element(_))
        .Times(3)
        .WillRepeatedly([&](const ser::Serialize & element) {
            element.serialize(serializer);
        });
    EXPECT_CALL(serializer, seq_end())
        .Times(1);

    InSequence order;  // Elements must be serialized in this order.
    EXPECT_CALL(serializer, serialize_i32(1)).Times(1);
    EXPECT_CALL(serializer, serialize_i32(2)).Times(1);
    EXPECT_CALL(serializer, serialize_i32(3)).Times(1);

    std::vector<int> vec{ 1, 2, 3 };
    ser::Accessor<std::vector<int>> accessor = ser::accessor(vec);
    accessor.serialize(serializer);
}

/// ser::Accessor<std::vector<T>>::traits() invokes TypeTraits::of<std::vector<T>>()
///
TEST(KingwSerde, StdVectorSerAccessorTraits) {
    std::vector<int> vec;
    ser::Accessor<std::vector<int>> accessor = ser::accessor(vec);
    auto traits = serde::TypeTraits::of<std::vector<int>>();
    EXPECT_EQ(accessor.traits(), traits);
}

}  // namespace
