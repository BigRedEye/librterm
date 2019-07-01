#pragma once

#include <unicode/unistr.h>

#include <string>
#include <string_view>

namespace rterm {

using String = icu::UnicodeString;
using StringView = std::u16string_view;

} // namespace rterm
