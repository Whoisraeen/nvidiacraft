cmake_minimum_required(VERSION 3.15)
project(nvidiacraft_native)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JNI
find_package(JNI REQUIRED)

# Streamline SDK inclusion
set(STREAMLINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Streamline)
include_directories(${STREAMLINE_DIR}/include)

# We will build a shared library (.dll / .so)
add_library(${PROJECT_NAME} SHARED NvidiacraftJNI.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${JNI_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${JNI_LIBRARIES}
)

# For Windows, Streamline libraries would be linked here
if(WIN32)
    # target_link_libraries(${PROJECT_NAME} PRIVATE ${STREAMLINE_DIR}/lib/x64/sl.interposer.lib)
endif()
