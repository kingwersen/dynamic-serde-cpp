#pragma once

#include <vector>

#include "kingw/ser/serialize.hpp"
#include "kingw/ser/serializer.hpp"


namespace kingw {
namespace ser {

template <class T>
void serialize(ser::Serializer & serializer, const std::vector<T> & data) {
    auto seq = serializer.serialize_seq(data.size());
    for (const auto & element : data) {
        seq.serialize_element(ser::accessor(element));
    }
    seq.end();
}

template <class T>
class Accessor<std::vector<T>> : public ser::Serialize {
public:
    explicit Accessor(const std::vector<T> & item) : item(item) { }
    void serialize(ser::Serializer & serializer) const override {
        ser::serialize(serializer, item);
    }
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<std::vector<T>>();
    };
private:
    const std::vector<T> & item;
};

template <class T>
Accessor<std::vector<T>> accessor(const std::vector<T> & item) {
    return Accessor<std::vector<T>>(item);
}

}  // namespace ser
}  // namespace kingw
