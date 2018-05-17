#pragma once

namespace rterm {
class Texture {
public:
    virtual ~Texture() = default;

    void render(const iRect &dst) = 0;
};

} // namespace rterm
