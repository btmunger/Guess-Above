cmake_minimum_required(VERSION 3.20)
project(GuessAbove LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GuessAbove main.cpp)

# Include the Windows SDK headers for C++/WinRT
target_include_directories(GuessAbove PRIVATE
    "$ENV{WindowsSdkDir}/Include/$ENV{WindowsSdkVersion}/cppwinrt"
    "$ENV{WindowsSdkDir}/Include/$ENV{WindowsSdkVersion}/winrt"
)

# Link Windows Runtime library
target_link_libraries(GuessAbove PRIVATE windowsapp)
