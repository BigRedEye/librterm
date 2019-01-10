#pragma once

#include "rterm/util/color.h"
#include "rterm/util/util.h"
#include "rterm/rasterizer/font.h"
#include "rterm/renderer/renderer.h"
#include "rterm/renderer/texture_view.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace rterm {

class GlyphCache {
public:
    template<typename T>
    GlyphCache(Renderer<Api::api>& renderer, T&& font)
        : renderer_(renderer) {
        reset(std::forward<T>(font));
    }

    GlyphCache(Renderer<Api::api>& renderer);

    void setFont(const std::string& path, ui32 sz);
    void setFont(const std::string& path, ui32 w, ui32 h);

    i32 w();
    i32 h();

    TextureView<Api::api> getGlyph(ch32 ch);

private:
    template<typename T>
    void reset(T&& t) {
        cache_.clear();
        font_ = std::forward<T>(t);
        addAtlas();
    }

private:
    template<typename T>
    class GlyphMapping {
    public:
        GlyphMapping()
            : small_(threshold, T{}) {
        }

        void set(ch32 ch, const T& hit) {
            if (ch < threshold) {
                small_[ch] = hit;
            } else {
                huge_.emplace(ch, hit);
            }
        }

        bool has(ch32 ch) {
            if (ch < threshold) {
                return small_[ch].texture != T::invalid;
            } else {
                return huge_.find(ch) != huge_.end();
            }
        }

        const T& get(ch32 ch) {
            if (ch < threshold) {
                return small_[ch];
            } else {
                return huge_[ch];
            }
        }

        void clear() {
            std::fill(small_.begin(), small_.end(), T{});
            huge_.clear();
        }

        static constexpr std::size_t threshold = 0xffff;

    private:
        std::vector<T> small_;
        std::unordered_map<size_t, T> huge_;
    };

    struct CacheHit {
        static constexpr size_t invalid = static_cast<size_t>(-1);

        size_t texture = invalid;
        iRect rect = iRect{};
    };

    struct Cache {
        std::vector<HardwareTexture<Api::api>> textures;
        GlyphMapping<CacheHit> mapping;
        Vector<i32, 2> nextEmpty_;

        void clear() {
            textures.clear();
            mapping.clear();
            nextEmpty_ = {0, 0};
        }
    };

private:
    void addAtlas();
    void writeGlyph(const SoftwareTexture& texture);

    Cache cache_;
    std::unique_ptr<Font> font_;
    Renderer<Api::api>& renderer_;
};

} // namespace rterm
