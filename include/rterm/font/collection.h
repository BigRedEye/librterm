#pragma once

#include <rterm/font/font.h>
#include <rterm/font/filter.h>

#include <vector>

namespace rterm::font {

class Collection {
public:
    struct Entry {
        Font font;
        i32 priority;
    };

    using Storage = std::vector<Entry>;
    using Iterator = typename Storage::iterator;
    using ConstIterator = typename Storage::const_iterator;

    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator begin();
    Iterator end();

    Collection& push(const Font& font, i32 priority = 0);
    Collection& push(Font&& font, i32 priority = 0);

    Collection& filter(const detail::Filter& filter);
    Collection& merge(const Collection& other);
    Collection& merge(Collection&& other);

    /// Sets priority for each entry
    Collection& priority(i32 priority);

private:
    Storage fonts_;
};

Collection& operator|=(Collection& lhs, const detail::Filter& rhs);
Collection& operator+=(Collection& lhs, const Collection& rhs);

Collection operator|(const Collection& lhs, const detail::Filter& rhs);
Collection operator+(const Collection& lhs, const Collection& rhs);


RTERM_DECLARE_NAMED_ARGUMENT(fs::path, path);
RTERM_DECLARE_NAMED_ARGUMENT(font::Blob, blob);
RTERM_DECLARE_NAMED_ARGUMENT(i32, priority);

namespace impl {

Collection system(i32 priority);

} // namespace impl

template<typename ...Args>
Collection system(Args&& ...args) {
    struct Visitor {
        i32& priority;

        int visit(priorityTag tag) {
            priority = tag.data;
            return 0;
        }
    };

    i32 priority{0};
    Visitor vis{priority};

    (void)(vis.visit(args) + ... + 0);

    return impl::system(priority);
}

} // namespace rterm::font
