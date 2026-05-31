from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps


class ImtSentraConan(ConanFile):
    name = "imtsentra"
    version = "0.1.0"
    license = "Proprietary"
    author = "ImagingTools"
    url = "https://github.com/ImagingTools/Sentra"
    description = "Sentra — AI Visual Testing Platform"
    topics = ("visual-testing", "ai", "automation", "playwright")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "build_tests": [True, False],
        "build_cli": [True, False],
        "build_gui": [True, False],
    }
    default_options = {
        "shared": False,
        "build_tests": True,
        "build_cli": True,
        "build_gui": True,
    }

    exports_sources = (
        "CMakeLists.txt",
        "Include/*",
        "Impl/*",
        "Sdl/*",
        "Qml/*",
        "Tools/*",
        "Tests/*",
        "Config/*",
    )

    def requirements(self):
        # ImtCore dependencies
        self.requires("imtcore/[>=1.0]")
        # Acf image definition (iimg) and image processing (iipr)
        self.requires("acf/[>=1.0]")
        self.requires("acfsln/[>=1.0]")
        self.requires("qt/[>=6.5]")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20]")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["SENTRA_BUILD_TESTS"] = self.options.build_tests
        tc.variables["SENTRA_BUILD_CLI"] = self.options.build_cli
        tc.variables["SENTRA_BUILD_GUI"] = self.options.build_gui
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["imtsentra", "imtsentragql"]
        self.cpp_info.includedirs = ["Include"]
