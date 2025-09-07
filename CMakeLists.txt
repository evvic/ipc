cmake_minimum_required(VERSION 3.10)
project(DBusCalculator CXX)

# Find the standard D-Bus library using PkgConfig, which Homebrew installs
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

# Add the service executable
add_executable(calculator_service calculator_service.cpp)
target_include_directories(calculator_service PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(calculator_service PRIVATE ${DBUS_LIBRARIES})

# Add the client executable
add_executable(calculator_client calculator_client.cpp)
target_include_directories(calculator_client PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(calculator_client PRIVATE ${DBUS_LIBRARIES})

