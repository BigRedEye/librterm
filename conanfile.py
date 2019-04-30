from conans import ConanFile, CMake, tools

class LibRtermConan(ConanFile):
    name = "librterm"
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "icu/63.1@bincrafters/stable",
        "freetype/2.9.0@bincrafters/stable",      
        "harfbuzz/2.3.0@bincrafters/stable",
    )
    generators = "cmake"
    default_options = {
        "icu:shared": False,

        "freetype:shared": False,
        "freetype:with_png": False,
        "freetype:with_zlib": False,
        "freetype:with_bzip2": False,

        "harfbuzz:shared": False,
        "harfbuzz:with_freetype": True,
    }

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
