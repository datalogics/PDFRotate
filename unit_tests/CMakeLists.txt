project(CLU_PDF_Rotate_Project CXX)

set(sources
    "UnitTests.cpp"
)

add_executable(UnitTest ${sources})

target_include_directories(UnitTest PUBLIC "." "../src/RotatorLib")

if(WIN32)
    target_compile_definitions(UnitTest PUBLIC
        $<$<CONFIG:Debug>:WIN32;_DEBUG;_CONSOLE>
        $<$<NOT:$<CONFIG:Debug>>:WIN32;NDEBUG;_CONSOLE>)
    target_compile_options(UnitTest PUBLIC $<$<CONFIG:Debug>:/Zi>)
else()
    target_compile_definitions(UnitTest PUBLIC
        $<$<CONFIG:Debug>:DEBUG;_DEBUG;UNIX_ENV=1;UNIX_PLATFORM=1>
        $<$<NOT:$<CONFIG:Debug>>:NDEBUG;UNIX_ENV=1;UNIX_PLATFORM=1>)
endif()

target_compile_definitions(UnitTest PUBLIC PRODUCT="HFTLibrary.h" PI_ACROCOLOR_VERSION=AcroColorHFT_VERSION_6)

add_custom_command(TARGET UnitTest POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:UnitTest>/test_input/"
      )

add_custom_command(TARGET UnitTest POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/test_input/"
      "$<TARGET_FILE_DIR:UnitTest>/test_input"
      )
