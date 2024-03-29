cmake_minimum_required(VERSION 3.16)

project(EmotivBluetoothDevice VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Bluetooth)

qt_standard_project_setup(REQUIRES 6.5)

file(GLOB SOURCES
    src/*.cpp
    src/*.h)

qt_add_resources(QRCS Resources/EmotivBluetoothDevice.qrc)

qt_add_executable(appEmotivBluetoothDevice
    ${SOURCES}
    ${QRCS}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appEmotivBluetoothDevice PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appEmotivBluetoothDevice
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appEmotivBluetoothDevice
    PRIVATE Qt6::Quick Qt6::Bluetooth
)
target_link_directories(appEmotivBluetoothDevice
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (APPLE)
    # Using absolute path for shared plist files is a Ninja bug workaround
    get_filename_component(CONFIG_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/config ABSOLUTE)
    if (IOS)
        set_target_properties(appEmotivBluetoothDevice PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CONFIG_PLIST}/Info.cmake.ios.plist"
        )
    else()
        set_target_properties(appEmotivBluetoothDevice PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CONFIG_PLIST}/Info.cmake.macos.plist"
        )
    endif()
endif()

include(GNUInstallDirs)
install(TARGETS appEmotivBluetoothDevice
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
