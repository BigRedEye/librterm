from conans import ConanFile, CMake, tools

class LibRtermConan(ConanFile):
    name = "librterm"
    settings = "os", "compiler", "build_type", "arch"
    requires = "sdl2/2.0.9@bincrafters/stable", "sdl2_ttf/2.0.15@bincrafters/stable"
    generators = "cmake"
    default_options = {"sdl2:shared": False}

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def configure(self):
        if self.settings.os == "Linux":
            self.options["sdl2"].nas = False
        if self.settings.compiler != "Visual Studio":
            self.options["sdl2_ttf"].shared = False
