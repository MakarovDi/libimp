#include <gtest/gtest.h>
#include <cstring>

#include <imp/common/rgb_image>


using namespace imp;
using testing::Types;


template <typename T> class RgbImageTypeMixture: public ::testing::Test { };


using MathTypes = Types
<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t,
    float, double, long double
>;

TYPED_TEST_CASE(RgbImageTypeMixture, MathTypes);


TYPED_TEST(RgbImageTypeMixture, default_ctor)
{
    RgbImage<TypeParam> img(2, 3);
    ASSERT_TRUE(img.is_own_memory());

    ASSERT_EQ(img.height(), 3);
    ASSERT_EQ(img.width(), 2);
    ASSERT_EQ(img.plane_size(), 3*2);
    ASSERT_EQ(img.mem_size(), 3*3*2*sizeof(TypeParam));

    memset(img.ptr(), 0, size_t(img.mem_size()));

    ASSERT_TRUE(img.r_plane() == img.g_plane());
    ASSERT_TRUE(img.r_plane() == img.b_plane());

    ASSERT_FALSE(img.r_plane().is_own_memory());
    ASSERT_FALSE(img.g_plane().is_own_memory());
    ASSERT_FALSE(img.b_plane().is_own_memory());
}


TYPED_TEST(RgbImageTypeMixture, raw_ptr_copy_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(img.is_own_memory());
    ASSERT_TRUE(img.ptr() != ptr);

    ASSERT_TRUE(img.r_plane() == r);
    ASSERT_TRUE(img.g_plane() == g);
    ASSERT_TRUE(img.b_plane() == b);

    ASSERT_FALSE(img.r_plane().is_own_memory());
    ASSERT_FALSE(img.g_plane().is_own_memory());
    ASSERT_FALSE(img.b_plane().is_own_memory());
}


TYPED_TEST(RgbImageTypeMixture, raw_ptr_move_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(img.is_own_memory());
    ASSERT_TRUE(img.ptr() == ptr);

    ASSERT_TRUE(img.r_plane() == r);
    ASSERT_TRUE(img.g_plane() == g);
    ASSERT_TRUE(img.b_plane() == b);

    ASSERT_FALSE(img.r_plane().is_own_memory());
    ASSERT_FALSE(img.g_plane().is_own_memory());
    ASSERT_FALSE(img.b_plane().is_own_memory());
}


TYPED_TEST(RgbImageTypeMixture, copy_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(img.is_own_memory());

    RgbImage<TypeParam> img2(img);
    ASSERT_TRUE(img2.is_own_memory());
    ASSERT_TRUE(img2.ptr() != img.ptr());

    ASSERT_TRUE(img2.r_plane() == r);
    ASSERT_TRUE(img2.g_plane() == g);
    ASSERT_TRUE(img2.b_plane() == b);

    ASSERT_FALSE(img.r_plane().is_own_memory());
    ASSERT_FALSE(img.g_plane().is_own_memory());
    ASSERT_FALSE(img.b_plane().is_own_memory());
}


TYPED_TEST(RgbImageTypeMixture, copy_operator)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(img.is_own_memory());

    RgbImage<TypeParam> img2(3, 3);

    img2 = img;
    ASSERT_TRUE(img2.is_own_memory());
    ASSERT_TRUE(img2.ptr() != img.ptr());

    ASSERT_TRUE(img2.r_plane() == r);
    ASSERT_TRUE(img2.g_plane() == g);
    ASSERT_TRUE(img2.b_plane() == b);

    ASSERT_FALSE(img.r_plane().is_own_memory());
    ASSERT_FALSE(img.g_plane().is_own_memory());
    ASSERT_FALSE(img.b_plane().is_own_memory());
}


TYPED_TEST(RgbImageTypeMixture, move_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(img.is_own_memory());
    ASSERT_TRUE(img.ptr() == ptr);

    RgbImage<TypeParam> img2(std::move(img));

    ASSERT_TRUE(img2.ptr() == ptr);
    ASSERT_FALSE(img2.is_own_memory());
    ASSERT_EQ(img2.width(), 3);
    ASSERT_EQ(img2.height(), 2);

    ASSERT_FALSE(img.is_own_memory());
    ASSERT_EQ(img.ptr(), nullptr);
    ASSERT_EQ(img.width(), 0);
    ASSERT_EQ(img.height(), 0);
    ASSERT_EQ(img.mem_size(), 0);
}


TYPED_TEST(RgbImageTypeMixture, move_operator)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6),

                        TypeParam(2*1), TypeParam(2*2), TypeParam(2*3),
                        TypeParam(2*4), TypeParam(2*5), TypeParam(2*6),

                        TypeParam(3*1), TypeParam(3*2), TypeParam(3*3),
                        TypeParam(3*4), TypeParam(3*5), TypeParam(3*6)  };

    Matrix<TypeParam> r({ {TypeParam(1), TypeParam(2), TypeParam(3) },
                          {TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> g({ {TypeParam(2*1), TypeParam(2*2), TypeParam(2*3) },
                          {TypeParam(2*4), TypeParam(2*5), TypeParam(2*6) } });

    Matrix<TypeParam> b({ {TypeParam(3*1), TypeParam(3*2), TypeParam(3*3) },
                          {TypeParam(3*4), TypeParam(3*5), TypeParam(3*6) } });

    RgbImage<TypeParam> img(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(img.is_own_memory());
    ASSERT_TRUE(img.ptr() == ptr);

    RgbImage<TypeParam> img2(1, 1);

    img2 = std::move(img);

    ASSERT_TRUE(img2.ptr() == ptr);
    ASSERT_FALSE(img2.is_own_memory());
    ASSERT_EQ(img2.width(), 3);
    ASSERT_EQ(img2.height(), 2);

    ASSERT_FALSE(img.is_own_memory());
    ASSERT_EQ(img.ptr(), nullptr);
    ASSERT_EQ(img.width(), 0);
    ASSERT_EQ(img.height(), 0);
    ASSERT_EQ(img.mem_size(), 0);
}


TYPED_TEST(RgbImageTypeMixture, indexing_test)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    RgbImage<T> img(3, 2, ptr, MatrixMem::kReuse);



    index_t i = 0;

    for (index_t r = 0; r < img.height(); ++r)
        for (index_t c = 0; c < img.width(); ++c)
        {
            ASSERT_EQ(img(i), ptr[i]);
            ASSERT_EQ(img[i], ptr[i]);

            img.r(c, r) = T(i);


            ASSERT_EQ(img.r(i), T(i));
            ASSERT_EQ(img.r(i), ptr[i]);
            ASSERT_EQ(img.r(i), img(i));
            ASSERT_EQ(img.r(i), img.r(c, r));
            ASSERT_EQ(img.r(i), img(i, ColorPlane::kR));
            ASSERT_EQ(img.r(i), img(c, r, ColorPlane::kR));
            ASSERT_EQ(img.r(i), img.color(c, r, ColorPlane::kR));
            ASSERT_EQ(img.r(i), img.color(i, ColorPlane::kR));

            img.g(c, r) = T(2*i);

            ASSERT_EQ(img.g(i), T(2*i));
            ASSERT_EQ(img.g(i), ptr[i + img.plane_size()]);
            ASSERT_EQ(img.g(i), img(i + img.plane_size()));
            ASSERT_EQ(img.g(i), img.g(c, r));
            ASSERT_EQ(img.g(i), img(i, ColorPlane::kG));
            ASSERT_EQ(img.g(i), img(c, r, ColorPlane::kG));
            ASSERT_EQ(img.g(i), img.color(c, r, ColorPlane::kG));
            ASSERT_EQ(img.g(i), img.color(i, ColorPlane::kG));

            img.b(c, r) = T(3*i);

            ASSERT_EQ(img.b(i), T(3*i));
            ASSERT_EQ(img.b(i), ptr[i + 2*img.plane_size()]);
            ASSERT_EQ(img.b(i), img(i + 2*img.plane_size()));
            ASSERT_EQ(img.b(i), img.b(c, r));
            ASSERT_EQ(img.b(i), img(i, ColorPlane::kB));
            ASSERT_EQ(img.b(i), img(c, r, ColorPlane::kB));
            ASSERT_EQ(img.b(i), img.color(c, r, ColorPlane::kB));
            ASSERT_EQ(img.b(i), img.color(i, ColorPlane::kB));

            ++i;
        }
}


TYPED_TEST(RgbImageTypeMixture, planes_test)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    RgbImage<T> img(3, 2, ptr, MatrixMem::kReuse);


    const Matrix<T> r_mx = img.r_plane();
    const Matrix<T> g_mx = img.g_plane();
    const Matrix<T> b_mx = img.b_plane();


    const Matrix<T>& r1 = img.r_plane();
          Matrix<T>  r2 = img.r_plane();

    ASSERT_TRUE(r1.ptr() == r2.ptr());

    Matrix<T> m(3, 3, ptr+4, MatrixMem::kCopy);

    r2 = std::move(m);
    ASSERT_TRUE(r2.ptr() != r1.ptr());
    ASSERT_TRUE(r1 == r_mx);


    const Matrix<T>& g1 = img.g_plane();
          Matrix<T>  g2 = img.g_plane();

    ASSERT_TRUE(g1.ptr() == g2.ptr());

    m = Matrix<T>(3, 3, ptr+4, MatrixMem::kCopy);

    g2 = std::move(m);
    ASSERT_TRUE(g2.ptr() != g1.ptr());
    ASSERT_TRUE(g1 == g_mx);

    const Matrix<T>& b1 = img.b_plane();
          Matrix<T>  b2 = img.b_plane();

    ASSERT_TRUE(b1.ptr() == b2.ptr());

    m = Matrix<T>(3, 3, ptr+4, MatrixMem::kCopy);

    b2 = std::move(m);
    ASSERT_TRUE(b2.ptr() != b1.ptr());
    ASSERT_TRUE(b1 == b_mx);
}