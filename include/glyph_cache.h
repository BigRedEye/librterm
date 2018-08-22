#pragma once

#include "color.h"
#include "font.h"
#include "renderer.h"
#include "texture_view.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace rterm {

class GlyphCache {
public:
    template<typename T>
    GlyphCache(Renderer<Api::api>& renderer, T&& font)
        : font_(std::forward<T>(font))
        , renderer_(renderer) {
    }

    void setFont(const std::string& path, size_t sz);
    void setFont(const std::string& path, size_t w, size_t h);

    int w();
    int h();

    TextureView<Api::api> getGlyph(char_t ch);

private:
    template<typename T>
    void reset(T&& t) {
        cache_.clear();
        font_ = std::forward<T>(t);
    }

private:
    template<typename T>
    class GlyphMapping {
    public:
        GlyphMapping()
            : small_(threshold, T{}) {
        }

        void set(char_t ch, const T& hit) {
            if (ch < threshold) {
                small_[ch] = hit;
            } else {
                huge_.emplace(ch, hit);
            }
        }

        bool has(char_t ch) {
            if (ch < threshold) {
                return small_[ch].texture != T::invalid;
            } else {
                return huge_.find(ch) != huge_.end();
            }
        }

        const T& get(char_t ch) {
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

        void clear() {
            textures.clear();
            mapping.clear();
        }
    };

    Cache cache_;
    std::unique_ptr<Font> font_;
    Renderer<Api::api>& renderer_;
};

} // namespace rterm
