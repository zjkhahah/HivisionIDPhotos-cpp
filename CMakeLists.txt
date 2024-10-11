cmake_minimum_required(VERSION 3.11)
project(HivisionIDPhotos)

set(CMAKE_CXX_STANDARD  11)
set(CMAKE_C_STANDARD 99)

option(COMPILE_LIBRARY "API" OFF)
option(COMPILE_ANDROID "android_compile" OFF)

set (CMAKE_CXX_FLAGS  " -std=c++11 -O3 -Wall -fno-stack-protector")

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
  endif()

  set(MYLIB_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  message("Checking MYLIB_LIBRARY_PATH: ${MYLIB_LIBRARY_PATH}")
  message("Checking EXTERN_LIBRARY_PATH: ${EXTERN_LIBRARY_PATH}")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

 
 if(COMPILE_LIBRARY)
    set(SOURCE_FILES Hivision_SDK.cpp)
    file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    list(APPEND SOURCE_FILES ${SOURCES})
    add_library(HivisionIDphotos SHARED ${SOURCE_FILES})
else()
    add_compile_definitions(TERMINAL)
    set(SOURCE_FILES main.cpp)
    file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    list(APPEND SOURCE_FILES ${SOURCES})
    add_executable(HivisionIDphotos  ${SOURCE_FILES})
endif()


  file(GLOB curr_lib_files "${MYLIB_LIBRARY_PATH}/*.a")
  list(APPEND LIB_FILES ${curr_lib_files})
  file(GLOB curr_lib_files "${MYLIB_LIBRARY_PATH}/*.so")
  list(APPEND LIB_FILES ${curr_lib_files})
  file(GLOB curr_lib_files "${MYLIB_LIBRARY_PATH}/*.lib")
  list(APPEND LIB_FILES ${curr_lib_files})

# foreach(file_path ${EXTERN_LIBRARY_PATH})
#   file(GLOB curr_lib_files "${file_path}/*.a")
#   list(APPEND EXTERN_LIB_FILES ${curr_lib_files})
#   file(GLOB curr_lib_files "${file_path}/*.so")
#   list(APPEND EXTERN_LIB_FILES ${curr_lib_files})
#   file(GLOB curr_lib_files "${file_path}/*.lib")
#   list(APPEND EXTERN_LIB_FILES ${curr_lib_files})
# endforeach()

  foreach(LIB_FILE ${LIB_FILES})
      target_link_libraries(HivisionIDphotos ${LIB_FILE})
  endforeach()

#   foreach(EXTERN_LIB_FILE ${EXTERN_LIB_FILES})
#     target_link_libraries(HivisionIDphotos ${EXTERN_LIB_FILE})
#   endforeach()

  if(COMPILE_ANDROID)
  target_link_libraries(HivisionIDphotos  log)
  endif()
