from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class ExamplesCppRecipe(ConanFile):
    name = "examples_cpp"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}    
    #, "cmake_find_package"

    def requirements(self):
        self.requires("boost/1.82.0")
        self.requires("benchmark/1.8.3")
    
#    def layout(self):
#        # We make the assumption that if the compiler is msvc the
#        # CMake generator is multi-config
#        if self.settings.get_safe("compiler") == "msvc":
#            multi = True
#        else:
#            multi = False          

#        self.folders.build = "build" if multi else f"build/{str(self.settings.build_type)}"
#        self.folders.generators = "build"
#        cmake_layout(self)
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()     

