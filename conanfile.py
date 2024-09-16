from conan import ConanFile
from conan.tools.cmake import CMakeToolchain

class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("asio/1.30.2")
        self.requires("gsl-lite/0.41.0")
        self.requires("cli11/2.4.2")
        self.requires("spdlog/1.14.1")
        self.requires("zpp_bits/4.4.20")
        self.requires("fmt/11.0.1", override=True)
        self.requires("onetbb/2021.12.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()
