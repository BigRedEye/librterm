#include <rterm/util/defs.h>
#include <rterm/util/murmur.h>

#include <utility>

namespace rterm::named_args {

template<typename T, size_t TagId, size_t Hash>
struct Tag {
    using value_type = T;
    static constexpr size_t id = TagId;

    operator const T&() const {
        return data;
    }

    T data;
};

template<typename TagT>
struct Setter {
    using Tag = TagT;
    using Value = typename Tag::value_type;

    Tag operator=(Value&& t) const {
        return {std::forward<Value>(t)};
    }
};

} // namespace rterm::named_args

#define RTERM_CAT(a, b) a##b
#define RTERM_DECLARE_NAMED_ARGUMENT(TYPE, NAME)                                   \
    inline constexpr ::rterm::named_args::Setter<                                  \
        ::rterm::named_args::Tag<TYPE, __LINE__, ::rterm::murmur::hash(__FILE__)>> \
        NAME;                                                                      \
    using RTERM_CAT(NAME, Tag) = decltype(NAME)::Tag;
