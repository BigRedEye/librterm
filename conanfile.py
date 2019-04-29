from conans import ConanFile, CMake, tools

class LibRtermConan(ConanFile):
    name = "librterm"
    settings = "os", "compiler", "build_type", "arch"
    requires = "icu/63.1@bincrafters/stable"
    generators = "cmake"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
