#include <rterm/util/fs.h>
#include <rterm/util/variant.h>

#include <utility>
#include <vector>

namespace rterm::font {

using Blob = std::vector<std::byte>;
using Handle = Variant<fs::path, Blob>;

} // namespace rterm::font
