include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/src/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/eigen)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)


add_executable(ImpTests
    common/matrix.cpp
#    image/rgb_image.cpp
#    io/ppm.cpp
    io/pgm.cpp
#    filter/minmax.cpp
)


target_link_libraries(ImpTests PRIVATE ex gtest gtest_main)