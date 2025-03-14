
#
# Eigen
#

kt_download_eigen()
# include_directories("${AUTOTOOLS3D_DEP}/eigen")

# set EIGEN var
set(AUTOTOOLS3D_EIGEN "${AUTOTOOLS3D_DEP}/eigen")

set(EIGEN3_INCLUDE_DIR "${AUTOTOOLS3D_EIGEN}")
find_package(Eigen3 3.3.7 REQUIRED)

# TODO: check eigen is in at3d dir
# TODO: check libigl (and others) uses it

# # check eigen
# if(NOT DEFINED AUTOTOOLS3D_EIGEN)
#     message(FATAL_ERROR "Eigen position not defined")
# else()
#     if(NOT EXISTS ${AUTOTOOLS3D_EIGEN})
#         message(FATAL_ERROR "Eigen not found")
#     endif()
#     else()
#         message(STATUS "found Eigen in:")
#     endif()
# endif()

list(APPEND LIBRARY_TARGET_INCLUDES "${EIGEN3_INCLUDE_DIR}")

# add compile definitions
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    print("Eigen definition: EIGEN_INITIALIZE_MATRICES_BY_NAN")
    add_compile_definitions(EIGEN_INITIALIZE_MATRICES_BY_NAN=1)
# endif()
