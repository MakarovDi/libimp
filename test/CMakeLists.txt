include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)


add_executable(ImpTests
    common/matrix.cpp
)


target_link_libraries(ImpTests PRIVATE ex gtest gtest_main)