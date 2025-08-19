cmake_minimum_required(VERSION 3.20)
project(GuessAbove LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source
add_executable(GuessAbove main.cpp)

# -------------------------------
# Include project headers (json.hpp is here)
# -------------------------------
target_include_directories(GuessAbove PRIVATE "${CMAKE_SOURCE_DIR}")

# -------------------------------
# Include Windows SDK headers for C++/WinRT
# -------------------------------
target_include_directories(GuessAbove PRIVATE
    "$ENV{WindowsSdkDir}/Include/$ENV{WindowsSdkVersion}/cppwinrt"
    "$ENV{WindowsSdkDir}/Include/$ENV{WindowsSdkVersion}/winrt"
)

# Link Windows Runtime library
target_link_libraries(GuessAbove PRIVATE windowsapp)

# -------------------------------
# OpenSSL / cpp-httplib setup
# -------------------------------
# Set OpenSSL root folder 
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

# Tell CMake to find OpenSSL
find_package(OpenSSL REQUIRED)

# Include OpenSSL headers
target_include_directories(GuessAbove PRIVATE "${OPENSSL_ROOT_DIR}/include")

# Link OpenSSL libraries
target_link_libraries(GuessAbove PRIVATE OpenSSL::SSL OpenSSL::Crypto)
