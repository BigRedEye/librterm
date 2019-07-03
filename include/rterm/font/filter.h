#pragma once

#include <rterm/font/properties.h>
#include <rterm/util/named_args.h>
#include <rterm/util/optional.h>

namespace rterm::font {

namespace detail {

using Filter = std::function<bool(const Properties&)>;

struct MatchingFilter {
    Optional<Family> family;
    Optional<std::string> postscriptName;
    Optional<Weight> weight;
    Optional<Width> width;
    Optional<std::string> style;
    Optional<bool> monospace;
};

}

RTERM_DECLARE_NAMED_ARGUMENT(Weight, weight);
RTERM_DECLARE_NAMED_ARGUMENT(Width, width);
RTERM_DECLARE_NAMED_ARGUMENT(std::string, style);
RTERM_DECLARE_NAMED_ARGUMENT(bool, monospace);
RTERM_DECLARE_NAMED_ARGUMENT(Family, familyName);
RTERM_DECLARE_NAMED_ARGUMENT(std::string, postscriptName);

template<typename ...Args>
detail::Filter filter(Args&& ...args) {
    struct Visitor {
        detail::MatchingFilter& filter;

        int visit(weightTag tag) {
            filter.weight = tag.data;
            return 0;
        }

        int visit(widthTag tag) {
            filter.width = tag.data;
            return 0;
        }

        int visit(styleTag tag) {
            filter.style = tag.data;
            return 0;
        }

        int visit(monospaceTag tag) {
            filter.monospace = tag.data;
            return 0;
        }
    };

    detail::MatchingFilter filter{};
    Visitor vis{filter};

    (void)(vis.visit(args) + ... + 0);

    return [filter](const Properties& props) -> bool {
        bool ok{true};
        ok = ok && filter.family.map_or([&](auto& s) { return s == props.family; }, true);
        ok = ok && filter.postscriptName.map_or([&](auto& s) { return s == props.postscriptName; }, true);
        ok = ok && filter.weight.map_or([&](auto& s) { return s == props.weight; }, true);
        ok = ok && filter.width.map_or([&](auto& s) { return s == props.width; }, true);
        ok = ok && filter.style.map_or([&](auto& s) { return s == props.style; }, true);
        ok = ok && filter.monospace.map_or([&](auto& s) { return s == props.monospace; }, true);
        return ok;
    };
}

} // namespace rterm::font
