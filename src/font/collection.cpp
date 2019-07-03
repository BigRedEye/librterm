#include <rterm/font/collection.h>
#if RTERM_CONFIG_SOURCE_FONTCONFIG
#include <rterm/font/source/fontconfig.h>
#elif RTERM_CONFIG_SOURCE_DIRECTWRITE
#include <rterm/font/source/directwrite.h>
#elif RTERM_CONFIG_SOURCE_CORETEXT
#include <rterm/font/source/coretext.h>
#endif

#include <algorithm>

namespace rterm::font {

Collection::ConstIterator Collection::begin() const {
    return fonts_.begin();
}

Collection::ConstIterator Collection::end() const {
    return fonts_.end();
}

Collection::Iterator Collection::begin() {
    return fonts_.begin();
}

Collection::Iterator Collection::end() {
    return fonts_.end();
}

Collection& Collection::push(Font&& font, i32 priority) {
    fonts_.emplace_back(std::move(font), priority);
    return *this;
}

Collection& Collection::push(const Font& font, i32 priority) {
    fonts_.emplace_back(font, priority);
    return *this;
}

Collection& Collection::filter(const detail::Filter& f) {
    auto it = std::partition(begin(), end(), [&] (const Entry& e) {
        return f(e.font.properties);
    });
    fonts_.erase(it, fonts_.end());
    return *this;
}

Collection& Collection::merge(const Collection& other) {
    std::copy(other.begin(), other.end(), std::back_inserter(fonts_));
    return *this;
}

Collection& Collection::merge(Collection&& other) {
    std::move(other.begin(), other.end(), std::back_inserter(fonts_));
    return *this;
}

Collection& Collection::priority(i32 priority) {
    std::for_each(begin(), end(), [priority](Entry& e) {
        e.priority = priority;
    });
    return *this;
}

} // namespace rterm::font
