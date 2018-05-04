#ifndef RTERM_TEXTURE_H
#define RTERM_TEXTURE_H

namespace rterm {
class Texture {
public:
    virtual ~Texture() = default;

    void render(const iRect &dst) = 0;
};

} // namespace rterm

#endif // RTERM_TEXTURE_H
