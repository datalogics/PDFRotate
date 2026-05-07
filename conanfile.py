import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class CLU_PDF_RotateConan(ConanFile):
    # Use the DLConanBase class in the dl_conanfile project to extend this ConanFile.
    # dl_conanfile.DLConanBase contains common code applicable to multiple projects,
    # reducing duplication.
    # See: https://github.com/pages/datalogics/dl-conanfile
    # SemVer compatible with any 1.x version
    python_requires = 'dl_conanfile/[~1.0.4]@datalogics/stable'
    python_requires_extend = 'dl_conanfile.DLConanBase'

    name = 'CLU_PDF_Rotate'
    license = 'Proprietary'
    url = ''
    description = 'CLU PDF Rotate Tool'
    generators = 'CMakeDeps', 'VirtualRunEnv'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {
        'verbose': [True, False],
        'with_apdfl': ['ANY'],
    }
    default_options = {
        'verbose': True,
        'with_apdfl': None,
    }
    keep_imports = True

    @property
    def _adobe_pdf_library(self):
        return self.conan_data['adobe_pdf_library']

    def set_version(self):
        self.version = self.conan_data['CLU_PDF_Rotate']['version']

    def requirements(self):
        if self.options.with_apdfl.value is None:
            self.options.with_apdfl = self._adobe_pdf_library
        self.requires(self.options.with_apdfl.value, visible=False)
        self.requires('common-utils/[>=2.0.11]@datalogics/nightly', visible=False)
        self.test_requires('catch2/[>=3.0.0]')
        self.requires('installer-resources/[>=0.8]@datalogics/stable', visible=False)
        self.requires('boost/1.88.0', visible=False)

    def build_requirements(self):
        self.tool_requires('ninja/[>=1.10]')

    def generate(self):
        super().generate()
        self.import_ext_pkgs()
        tc = CMakeToolchain(self)
        self.set_version()
        major, minor, rel = (str(v) for v in self.version.split('.'))
        tc.cache_variables['ROTATE_MAJOR_VER'] = major
        tc.cache_variables['ROTATE_MINOR_VER'] = minor
        tc.cache_variables['ROTATE_RELEASE_VER'] = rel
        tc.cache_variables['CMAKE_BUILD_TYPE'] = str(self.settings.build_type)
        tc.cache_variables['CLU_PDFROTATE_VERSION'] = self.version
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        build_type = self.settings.get_safe('build_type')
        if not build_type:
            raise ConanInvalidConfiguration('Multi-config generators are not supported; specify a single build_type.')
        cmake_layout(self)
        self.folders.build = os.path.join('build', build_type)
        self.folders.generators = os.path.join(self.folders.build, 'generators')

    def import_ext_pkgs(self):
        apdfl_pkg = self.dependencies['adobe_pdf_library']
        installer_resources_pkg = self.dependencies['installer-resources']
        # Since we use visual studio to build for windows,
        # The locations of where these files go are different
        bin_dir = os.path.join(self.build_folder, 'bin')
        lib_dir = os.path.join(self.build_folder, 'lib')
        dst_pdflinit = os.path.join(self.build_folder, 'src', 'apdfl_init')
        copy(self, '*', src=os.path.join(apdfl_pkg.package_folder, 'src'), dst=dst_pdflinit)
        resources = os.path.join(bin_dir, 'Resources')
        copy(self, '*.ppi', src=os.path.join(apdfl_pkg.package_folder, 'bin'), dst=bin_dir)
        copy(self, '*', src=os.path.join(apdfl_pkg.package_folder, 'Frameworks'), dst=bin_dir)
        copy(self, '*', src=os.path.join(apdfl_pkg.package_folder, 'Resources'), dst=resources)
        copy(self, '*.dll', src=os.path.join(apdfl_pkg.package_folder, 'bin'),
             dst=bin_dir, keep_path=False, excludes=['*clang*', '*DLI_PDFL*'])
        copy(self, '*.dylib*', src=os.path.join(apdfl_pkg.package_folder, 'lib'),
             dst=lib_dir, keep_path=False, excludes='*clang*')
        copy(self, '*.so*', src=os.path.join(apdfl_pkg.package_folder, 'lib'),
             dst=lib_dir, keep_path=False, excludes=['*clang*', '*DLI_PDFL*'])
        copy(self, '*.lib', src=os.path.join(apdfl_pkg.package_folder, 'lib'),
             dst=lib_dir, keep_path=False, excludes=['*clang*', '*DLI_PDFL*'])

        if self.settings.build_type == 'Debug':
            copy(self, '*.pdb', dst=bin_dir, src='bin')
            copy(self, '*.pdb', dst=lib_dir, src='lib')

        # Copy Datalogics bmp and ico
        installer_path = os.path.join(installer_resources_pkg.package_folder, '.')
        copy(self, 'Datalogics.bmp', src=installer_path, dst='../../..')
        copy(self, 'Datalogics.ico', src=installer_path, dst='../../..')

    def package_id(self):
        del self.info.settings.compiler
        del self.info.options.verbose
        del self.info.options.with_apdfl

    def package(self):
        copy(self, '*', os.path.join(self.build_folder, '..', '..', 'Installers', 'Release'), self.package_folder)
