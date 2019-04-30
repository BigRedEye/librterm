#pragma once

#include <string_view>

namespace rterm::named_args {

template<typename T, size_t TagId, const char* TagFile>
struct Tag {
    using value_type = T;
    static constexpr size_t id = TagId;

    T data;
};

template<typename Tag>
struct Setter {
    using T = typename Tag::value_type;

    Tag operator=(T&& t) const {
        return {std::forward<T>(t)};
    }
};

} // namespace rterm::named_args
