#include <gmock/gmock.h>

#include "kingw/mock/de/mock_deserializer.hpp"
#include "kingw/de/templates/stdmap.hpp"
#include "kingw/de/templates/stdvector.hpp"

using namespace kingw;
using namespace testing;


namespace {

/// StdMapVisitor<K, V>::expecting() returns "a map of items"
///
TEST(KingwSerde, StdMapVisitorExpecting) {
    std::map<int, int> map;
    de::StdMapVisitor<int, int> visitor(map);
    EXPECT_STREQ(visitor.expecting(), "a map of items");
}

/// StdMapVisitor<K, V>::visit_map() will correctly extract contents
/// from a Deserializer using its MapAccess object.
TEST(KingwSerde, StdMapVisitorVisit) {
    // Define a deserializer that has a map of 3 items:
    //  1: 2
    //  3: 4
    //  5: 6
    //
    // The StdMapVisitor should call has_next() 4 times.
    // The first 3 calls will return true and invoke next_entry().
    // Each call to next_entry() will set a key:value pair in the map.
    // The final call will return false and exit.
    de::MockDeserializer deserializer;
    de::MockMapAccess map_access;
    EXPECT_CALL(map_access, has_next())
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(map_access, next_entry(_, _))
        .Times(3)
        .WillRepeatedly([&](de::Deserialize & key, de::Deserialize & value) {
            key.deserialize(deserializer);
            value.deserialize(deserializer);
        });

    // Verify that values applied during next_entry() will actually go into the map
    // by returing real values during these calls.
    EXPECT_CALL(deserializer, deserialize_i32(_))
        .Times(6)
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(1); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(2); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(3); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(4); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(5); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(6); });

    std::map<int, int> map;
    de::StdMapVisitor<int, int> visitor(map);
    visitor.visit_map(map_access);  // Extract the contents from Deserializer
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map[1], 2);
    EXPECT_EQ(map[3], 4);
    EXPECT_EQ(map[5], 6);
}

/// deserialize<std::map<K, V>>(deserializer, std::map<K, V>) will invoke
/// deserializer.deserialize_map(StdMapVisitor<K, V>)
TEST(KingwSerde, StdMapDeserialize) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_map(WhenDynamicCastTo<de::StdMapVisitor<int, int> &>(_)))
        .Times(1);

    std::map<int, int> map;
    de::deserialize(deserializer, map);
}

/// de::accessor(std::map<K, V>)::deserialize() invokes de::deserialize(std::map<K, V>)
///
TEST(KingwSerde, StdMapDeAccessor) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_map(WhenDynamicCastTo<de::StdMapVisitor<int, int> &>(_)))
        .Times(1);

    std::map<int, int> map;
    de::Accessor<std::map<int, int>> accessor = de::accessor(map);
    accessor.deserialize(deserializer);

    auto traits = serde::TypeTraits::of<std::map<int, int>>();
    EXPECT_EQ(accessor.traits(), traits);
}

/// de::Accessor<std::map<K, V>>::traits() invokes TypeTraits::of<std::map<K, V>>()
///
TEST(KingwSerde, StdMapDeAccessorTraits) {
    std::map<int, int> map;
    de::Accessor<std::map<int, int>> accessor = de::accessor(map);
    auto traits = serde::TypeTraits::of<std::map<int, int>>();
    EXPECT_EQ(accessor.traits(), traits);
}


/// StdVectorVisitor<T>::expecting() returns "a sequence of items"
///
TEST(KingwSerde, StdVectorVisitorExpecting) {
    std::vector<int> vec;
    de::StdVectorVisitor<int> visitor(vec);
    EXPECT_STREQ(visitor.expecting(), "a sequence of items");
}

/// StdVectorVisitor<K, V>::visit_map() will correctly extract contents
/// from a Deserializer using its MapAccess object.
TEST(KingwSerde, StdVectorVisitorVisit) {
    // Define a deserializer that has a sequence of 3 items:
    //  1, 2, 3
    //
    // The StdVectorVisitor should call has_next() 4 times.
    // The first 3 calls will return true and invoke next_element().
    // Each call to next_element() will enter a new value in the vector.
    // The final call will return false and exit.
    de::MockDeserializer deserializer;
    de::MockSeqAccess seq_access;
    EXPECT_CALL(seq_access, has_next())
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(seq_access, next_element(_))
        .Times(3)
        .WillRepeatedly([&](de::Deserialize & element) {
            element.deserialize(deserializer);
        });

    // Verify that values applied during next_element() will actually go into the vector
    // by returing real values during these calls.
    EXPECT_CALL(deserializer, deserialize_i32(_))
        .Times(3)
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(1); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(2); })
        .WillOnce([](de::Visitor & visitor){ visitor.visit_i32(3); });

    std::vector<int> vec;
    de::StdVectorVisitor<int> visitor(vec);
    visitor.visit_seq(seq_access);  // Extract the contents from Deserializer
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

/// deserialize<std::vector<T>>(deserializer, std::vector<T>) will invoke
/// deserializer.deserialize_vector(StdVectorVisitor<T>)
TEST(KingwSerde, StdVectorDeserialize) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_seq(WhenDynamicCastTo<de::StdVectorVisitor<int> &>(_)))
        .Times(1);

    std::vector<int> vec;
    de::deserialize(deserializer, vec);
}

/// de::accessor(std::vector<T>)::deserialize() invokes de::deserialize(std::vector<T>)
///
TEST(KingwSerde, StdVectorDeAccessor) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_seq(WhenDynamicCastTo<de::StdVectorVisitor<int> &>(_)))
        .Times(1);

    std::vector<int> vec;
    de::Accessor<std::vector<int>> accessor = de::accessor(vec);
    accessor.deserialize(deserializer);
}

/// de::Accessor<std::vector<T>>::traits() invokes TypeTraits::of<std::vector<int>>()
///
TEST(KingwSerde, StdVectorDeAccessorTraits) {
    std::vector<int> vec;
    de::Accessor<std::vector<int>> accessor = de::accessor(vec);
    auto traits = serde::TypeTraits::of<std::vector<int>>();
    EXPECT_EQ(accessor.traits(), traits);
}

}  // namespace
