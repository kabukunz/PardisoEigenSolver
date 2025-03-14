cmake_minimum_required(VERSION 3.13)

enable_testing()

project(oneMKL_Example LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MKL CONFIG REQUIRED)

#message(STATUS "${MKL_IMPORTED_TARGETS}") #Provides available list of targets based on input

# 
# 
# 

# # set EIGEN var
# set(AUTOTOOLS3D_EIGEN "ext/eigen")

# # set(EIGEN3_INCLUDE_DIR "${AUTOTOOLS3D_EIGEN}/Eigen")

# add_subdirectory("${AUTOTOOLS3D_EIGEN}")

# find_package(Eigen3 3.3.7 REQUIRED)

message(CHECK_START "Fetching Eigen3")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

include(FetchContent)
FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
# set(BUILD_TESTING OFF)
# set(EIGEN_BUILD_TESTING OFF)
# set(EIGEN_MPL2_ONLY ON)
# set(EIGEN_BUILD_PKGCONFIG OFF)
# set(EIGEN_BUILD_DOC OFF)
FetchContent_MakeAvailable(Eigen)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "fetched")

# target_link_libraries(YourTarget PRIVATE Eigen3::Eigen)

# 
# 
# 

# add_executable(myapp pardiso_sym.c)

add_executable(myapp pardiso_eigen_sym.cpp)

target_compile_options(myapp PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)

target_include_directories(myapp PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)

# target_include_directories(myapp PUBLIC "${EIGEN3_INCLUDE_DIR}")

target_link_libraries(myapp PUBLIC $<LINK_ONLY:MKL::MKL>)

target_link_libraries(myapp PUBLIC Eigen3::Eigen)

add_test(NAME mytest COMMAND myapp)

if(MKL_ENV)
  set_tests_properties(mytest PROPERTIES ENVIRONMENT "${MKL_ENV}")
endif()