#pragma once

namespace rterm {

/**
 * @brief RAII-styled loader for SDL libraries
 */

class SdlLoader {
public:
    SdlLoader();
    ~SdlLoader();

private:
    static int countOfLoads_;
};

} // namespace rterm
