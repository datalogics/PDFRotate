cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
# Use CMAKE_MSVC_RUNTIME_LIBRARY abstractions; this gets the compiler.runtime
# from Conan and sets up CMAKE_MSVC_RUNTIME_LIBRARY
# Has to be done before first project()
cmake_policy(SET CMP0091 NEW)

project(CLU_PDF_Rotate_Project)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if(CMAKE_CONFIGURATION_TYPES)
  message(FATAL_ERROR "Multi-config generators are not supported; use a single-config generator (e.g., Ninja) via Conan presets.")
endif()

enable_testing()

# Executables are built with relative RPATH support, so the dynamic libs are
# found either together with the executable, or in the adjacent lib directory.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib;$ORIGIN/Binaries")

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# We include some of the static libraries in dynamic libraries, so always make
# position independent code. -fPic
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,-exclude-libs,ALL -Wl,--no-undefined")
endif()

message(STATUS "Runtime from Profile is:" "${PROFILE_RUNTIME}" )

if(NOT DEFINED HAS_LICENSE_MANAGER)
  set(HAS_LICENSE_MANAGER FALSE)
endif()

get_property(MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

find_package(common-utils CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options filesystem headers)
find_package(Catch2 CONFIG REQUIRED)

# Assemble the executable
set(exe_sources "src/clu_pdf_rotate/main_func.cpp")

set(lib_sources "src/RotatorLib/clu_pdf_rotate.cpp"
                "src/RotatorLib/clu_pdf_rotate_utility.cpp")

set(resource_file "src/clu_pdf_rotate/version.rc")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/clu_pdf_rotate/pdfrotate.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/clu_pdf_rotate/pdfrotate.h
    @ONLY)

add_executable(pdfrotate ${exe_sources} ${resource_file})
add_library(rotatorlib STATIC ${lib_sources})
set_target_properties(rotatorlib PROPERTIES OUTPUT_NAME "rotatorlib")
target_include_directories(rotatorlib PUBLIC "src/clu_pdf_rotate" "src/RotatorLib" ${CMAKE_CURRENT_BINARY_DIR}/src/clu_pdf_rotate)
add_subdirectory("unit_tests")

target_compile_definitions(rotatorlib PUBLIC ROTATE_VERSION="${CLU_PDFROTATE_VERSION}")

if(WIN32)
    target_link_options(pdfrotate PRIVATE
        $<$<CONFIG:Debug>:/DEBUG /INCREMENTAL /DELAYLOAD:DL180pdfl.dll>
        $<$<NOT:$<CONFIG:Debug>>:/DELAYLOAD:DL180pdfl.dll>)
    target_link_libraries(pdfrotate PRIVATE delayimp)
    target_compile_definitions(pdfrotate PUBLIC
        UNICODE
        _UNICODE
        WIN_PLATFORM
        WIN_ENV
        _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_definitions(pdfrotate PUBLIC
        UNIX_ENV
        UNIX_PLATFORM
        HAS_LICENSE_MANAGER=$<BOOL:${HAS_LICENSE_MANAGER}>)
  target_link_libraries(rotatorlib PUBLIC pthread)
endif()

if(NOT DEFINED EXTRA_TESTING_LIBS)
  set(EXTRA_TESTING_LIBS "")
endif()
target_compile_definitions(rotatorlib PUBLIC PRODUCT="HFTLibrary.h" PI_ACROCOLOR_VERSION=AcroColorHFT_VERSION_6)
target_link_libraries(rotatorlib PUBLIC apdfl_init common-utils::common-utils Boost::program_options Boost::filesystem)
target_link_libraries(pdfrotate PUBLIC rotatorlib)
target_link_libraries(rotatorlib PUBLIC Boost::headers)
target_link_libraries(pdfrotate PUBLIC Boost::headers)
target_link_libraries(UnitTest PUBLIC Catch2::Catch2WithMain Boost::program_options Boost::headers rotatorlib ${EXTRA_TESTING_LIBS})

add_test(NAME pdfrotate_unit_tests
    COMMAND $<TARGET_FILE:UnitTest> -r junit -o unit_test_out.xml
    WORKING_DIRECTORY $<TARGET_FILE_DIR:UnitTest>)

# Set the start up project to the exe
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pdfrotate)
