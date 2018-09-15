#include "error.h"

#include <SDL_error.h>
#include <SDL_ttf.h>

namespace rterm {

Exception::Exception()
    : error_(SDL_GetError()) {
    SDL_ClearError();
}

Exception::Exception(const std::string& error)
    : error_(error) {
}

const char* Exception::what() const noexcept {
    return error_.data();
}

BadTexture::BadTexture()
    : Exception() {
}

BadRenderer::BadRenderer()
    : Exception() {
}

BadFont::BadFont()
    : Exception(TTF_GetError()) {
}

} // namespace rterm
