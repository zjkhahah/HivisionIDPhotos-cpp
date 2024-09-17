cmake_minimum_required(VERSION 3.13)
project(HivisionIDPhotos)


if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
  SET(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")

  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267 /wd4018 /wd4251 /wd4996 /wd4244 /wd4146 /wd4129 /wd4305 /wd4275 /wd4101")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267 /wd4018 /wd4251 /wd4996 /wd4244 /wd4146 /wd4129 /wd4305 /wd4275 /wd4101")


else()
  set(CMAKE_C_COMPILER  /usr/local/gcc-linaro-12.2.1-2023.04-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
  set(CMAKE_CXX_COMPILER  /usr/local/gcc-linaro-12.2.1-2023.04-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

endif()
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD  11)
set(CMAKE_C_STANDARD 99)
set(MYLIB_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(EXTERN_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv4/3rdparty")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("Checking MYLIB_LIBRARY_PATH: ${MYLIB_LIBRARY_PATH}")
message("Checking EXTERN_LIBRARY_PATH: ${EXTERN_LIBRARY_PATH}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_FILES main.cpp)
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(APPEND SOURCE_FILES ${SOURCES})
add_executable(HivisionIDPhotos_cpp  ${SOURCE_FILES})


file(GLOB LIB_FILES "${MYLIB_LIBRARY_PATH}/*.a" "${MYLIB_LIBRARY_PATH}/*.so" "${MYLIB_LIBRARY_PATH}/*.dll" "${MYLIB_LIBRARY_PATH}/*.lib")
file(GLOB EXTERN_LIB_FILES "${EXTERN_LIBRARY_PATH}/*.a" "${EXTERN_LIBRARY_PATH}/*.so" "${EXTERN_LIBRARY_PATH}/*.dll" "${EXTERN_LIBRARY_PATH}/*.lib")


foreach(LIB_FILE ${LIB_FILES})
    target_link_libraries(HivisionIDPhotos_cpp ${LIB_FILE})
    if("${LIB_FILE}" MATCHES ".a$")
        # 对于静态库
        add_link_options("-Wl,--whole-archive" "${LIB_FILE}" "-Wl,--no-whole-archive")
    endif()
endforeach()

foreach(EXTERN_LIB_FILE ${EXTERN_LIB_FILES})
  target_link_libraries(HivisionIDPhotos_cpp ${EXTERN_LIB_FILE})
endforeach()
