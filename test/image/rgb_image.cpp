#include <gtest/gtest.h>
#include <cstring>

#  define eigen_assert(x) if (!(x)) { throw (std::runtime_error("error")); }
#include <imp/image/rgb_image>




using namespace imp;
using testing::Types;


template <typename T> class rgb_image: public ::testing::Test { };


using MathTypes = Types
<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t,
    float, double, long double
>;

TYPED_TEST_CASE(rgb_image, MathTypes);


TYPED_TEST(rgb_image, default_ctor)
{
    RgbImage<TypeParam> img(3, 2);

    ASSERT_EQ(img.height(), 3);
    ASSERT_EQ(img.width(), 2);
    ASSERT_EQ(img.plane_size(), 3*2);
    ASSERT_EQ(img.size(), 3*3*2);

    memset(img.data(), 0, sizeof(TypeParam)*size_t(img.size()));

    ASSERT_TRUE(img.r_plane() == img.g_plane());
    ASSERT_TRUE(img.r_plane() == img.b_plane());
}


TYPED_TEST(rgb_image, raw_ptr_copy_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kCopy);
    ASSERT_TRUE(img.data() != ptr);

    ASSERT_TRUE(img.r_plane() == r);
    ASSERT_TRUE(img.g_plane() == g);
    ASSERT_TRUE(img.b_plane() == b);
}


TYPED_TEST(rgb_image, raw_ptr_move_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);
    ASSERT_TRUE(img.data() == ptr);

    ASSERT_TRUE(img.r_plane() == r);
    ASSERT_TRUE(img.g_plane() == g);
    ASSERT_TRUE(img.b_plane() == b);
}


TYPED_TEST(rgb_image, from_array_ctor)
{
    using T = TypeParam;

    RgbImage<T> img({
        {
            { T(1), T(2), T(3) },
            { T(4), T(5), T(6) }
        },

        {
            { T(2*1), T(2*2), T(2*3) },
            { T(2*4), T(2*5), T(2*6) }
        },

        {
            { T(3*1), T(3*2), T(3*3) },
            { T(3*4), T(3*5), T(3*6) }
        }
    });

    ASSERT_TRUE(img.width() == 3);
    ASSERT_TRUE(img.height() == 2);

    index_t i = 1;

    for (index_t h = 0; h < img.height(); ++h)
        for (index_t w = 0; w < img.width(); ++w)
        {
            ASSERT_TRUE(img.r(w, h) == T(i));
            ASSERT_TRUE(img.g(w, h) == T(2*i));
            ASSERT_TRUE(img.b(w, h) == T(3*i));

            ++i;
        }
}


TYPED_TEST(rgb_image, copy_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kCopy);
    ASSERT_FALSE(img.data() == ptr);

    RgbImage<T> img2(img);
    ASSERT_TRUE(img2.data() != img.data());

    ASSERT_TRUE(img2.r_plane() == r);
    ASSERT_TRUE(img2.g_plane() == g);
    ASSERT_TRUE(img2.b_plane() == b);
}


TYPED_TEST(rgb_image, copy_operator)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);

    RgbImage<T> img2(3, 3);

    img2 = img;
    ASSERT_TRUE(img2.data() != img.data());

    ASSERT_TRUE(img2.r_plane() == r);
    ASSERT_TRUE(img2.g_plane() == g);
    ASSERT_TRUE(img2.b_plane() == b);
}


TYPED_TEST(rgb_image, move_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);
    ASSERT_TRUE(img.data() == ptr);

    RgbImage<T> img2(std::move(img));

    ASSERT_TRUE(img2.data() == ptr);
    ASSERT_EQ(img2.width(), 3);
    ASSERT_EQ(img2.height(), 2);

    ASSERT_EQ(img.data(), nullptr);
    ASSERT_EQ(img.size(), 0);
}


TYPED_TEST(rgb_image, move_operator)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    Matrix<T> r({ {T(1), T(2), T(3) },
                  {T(4), T(5), T(6) } });

    Matrix<T> g({ {T(2*1), T(2*2), T(2*3) },
                  {T(2*4), T(2*5), T(2*6) } });

    Matrix<T> b({ {T(3*1), T(3*2), T(3*3) },
                  {T(3*4), T(3*5), T(3*6) } });

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);
    ASSERT_TRUE(img.data() == ptr);

    RgbImage<T> img2(1, 1);

    img2 = std::move(img);

    ASSERT_TRUE(img2.data() == ptr);
    ASSERT_EQ(img2.width(), 3);
    ASSERT_EQ(img2.height(), 2);

    ASSERT_EQ(img.data(), nullptr);
    ASSERT_EQ(img.size(), 0);
}


TYPED_TEST(rgb_image, indexing)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);



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
            ASSERT_EQ(img.r(i), img(i, RgbImage<T>::ColorPlane::kR));
            ASSERT_EQ(img.r(i), img(c, r, RgbImage<T>::ColorPlane::kR));
            ASSERT_EQ(img.r(i), img.color(c, r, RgbImage<T>::ColorPlane::kR));
            ASSERT_EQ(img.r(i), img.color(i, RgbImage<T>::ColorPlane::kR));

            img.g(c, r) = T(2*i);

            ASSERT_EQ(img.g(i), T(2*i));
            ASSERT_EQ(img.g(i), ptr[i + img.plane_size()]);
            ASSERT_EQ(img.g(i), img(i + img.plane_size()));
            ASSERT_EQ(img.g(i), img.g(c, r));
            ASSERT_EQ(img.g(i), img(i, RgbImage<T>::ColorPlane::kG));
            ASSERT_EQ(img.g(i), img(c, r, RgbImage<T>::ColorPlane::kG));
            ASSERT_EQ(img.g(i), img.color(c, r, RgbImage<T>::ColorPlane::kG));
            ASSERT_EQ(img.g(i), img.color(i, RgbImage<T>::ColorPlane::kG));

            img.b(c, r) = T(3*i);

            ASSERT_EQ(img.b(i), T(3*i));
            ASSERT_EQ(img.b(i), ptr[i + 2*img.plane_size()]);
            ASSERT_EQ(img.b(i), img(i + 2*img.plane_size()));
            ASSERT_EQ(img.b(i), img.b(c, r));
            ASSERT_EQ(img.b(i), img(i, RgbImage<T>::ColorPlane::kB));
            ASSERT_EQ(img.b(i), img(c, r, RgbImage<T>::ColorPlane::kB));
            ASSERT_EQ(img.b(i), img.color(c, r, RgbImage<T>::ColorPlane::kB));
            ASSERT_EQ(img.b(i), img.color(i, RgbImage<T>::ColorPlane::kB));

            ++i;
        }
}


TYPED_TEST(rgb_image, planes_access)
{
    using T = TypeParam;

    const Matrix<T, 2, 3> r_data
    ({
        { T(1), T(2), T(3) },
        { T(4), T(5), T(6) }
    });

    const Matrix<T, 2, 3> g_data
    ({
         { T(2*1), T(2*2), T(2*3) },
         { T(2*4), T(2*5), T(2*6) }
    });

    const Matrix<T, 2, 3> b_data
    ({
         { T(3*1), T(3*2), T(3*3) },
         { T(3*4), T(3*5), T(3*6) }
    });

    T ptr[] = {
        T(1), T(2), T(3),
        T(4), T(5), T(6),

        T(2*1), T(2*2), T(2*3),
        T(2*4), T(2*5), T(2*6),

        T(3*1), T(3*2), T(3*3),
        T(3*4), T(3*5), T(3*6)
    };

    RgbImage<T> img(2, 3, ptr, RawMemory::kMap);

    const ImagePlane<T>& r_mx = img.r_plane();
          ImagePlane<T>& r1   = img.r_plane();

    ASSERT_TRUE(r_mx.data() == r1.data());
    ASSERT_TRUE(r1.data() == ptr);

    r1 = std::move(g_data);

    ASSERT_TRUE(r1.data() == ptr);
    ASSERT_TRUE(img.r_plane() == g_data);

    Matrix<T, 3, 3> m;
    ASSERT_THROW(r1 = m, std::runtime_error);
}


TYPED_TEST(rgb_image, equality_operator)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6),

                T(2*1), T(2*2), T(2*3),
                T(2*4), T(2*5), T(2*6),

                T(3*1), T(3*2), T(3*3),
                T(3*4), T(3*5), T(3*6)  };

    RgbImage<T> img1(2, 3, ptr, RawMemory::kMap);
    RgbImage<T> img2(3, 2, ptr, RawMemory::kMap);
    RgbImage<T> img3(2, 3, ptr, RawMemory::kCopy);

    ASSERT_TRUE(img1 != img2);
    ASSERT_TRUE(img3 == img1);
    ASSERT_TRUE(img1 == img1);
}