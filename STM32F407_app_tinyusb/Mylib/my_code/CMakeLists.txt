cmake_minimum_required(VERSION 3.22)


set(mode_name my_code)

project(${mode_name})
add_library(${mode_name} INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(${mode_name} INTERFACE 
	USE_HAL_DRIVER 
	STM32F407xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(${mode_name} INTERFACE
    ./include
)

target_sources(${mode_name} INTERFACE
    ./code.c
    ./usb_descriptors.c
    ./msc_disk.c
    ./dcd_dwc2.c
)

target_link_directories(${mode_name} INTERFACE
)

target_link_libraries(${mode_name} INTERFACE
)

target_link_libraries(${mode_name} INTERFACE tinyusb
)
# Validate that ${mode_name} code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


