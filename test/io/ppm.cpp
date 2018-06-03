#include <gtest/gtest.h>

#include <sstream>
#include <imp/io/ppm>


template <index_t N>
bool check_stream(std::ostringstream& stream, const int (&against)[N])
{
    const std::string buffer = stream.str();

    if (buffer.length() != N)
        return false;

    const char* buf = buffer.c_str();

    for (index_t i = 0; i < N; ++i)
    {
        if (buf[i] != char(against[i]))
            return false;
    }

    return true;
}


TEST(ppm_test, color_order)
{
    imp::RgbImage<int> img
    ({
        { { 16, 16 },
          { 16, 16 }, },

        { { 16, 16 },
          { 16, 16 }, },

        { { 32, 32 },
          { 32, 32 }, }
    });

    std::ostringstream stream(std::ios_base::out | std::ios_base::binary);

    imp::ppm::save(stream, img, 32);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32, 0x35, 0x35, 0x0a, 0x80,
                                       0x80, 0xff, 0x80, 0x80, 0xff, 0x80, 0x80, 0xff, 0x80, 0x80, 0xff } ));

    img.r_plane() = { { 64, 64 },
                      { 64, 64 } };


    stream.seekp(0);
    imp::ppm::save(stream, img, 64);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32, 0x35, 0x35, 0x0a, 0xff,
                                       0x40, 0x80, 0xff, 0x40, 0x80, 0xff, 0x40, 0x80, 0xff, 0x40, 0x80 }));

    img.g_plane() = { { 128, 128 },
                      { 128, 128 } };

    stream.seekp(0);
    imp::ppm::save(stream, img, 128);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32, 0x35, 0x35, 0x0a, 0x80,
                                       0xff, 0x40, 0x80, 0xff, 0x40, 0x80, 0xff, 0x40, 0x80, 0xff, 0x40 }));
}


TEST(ppm_test, float_ppm)
{
    imp::RgbImage<double> img
    ({
        { { 16.0, 16.0 },
          { 16.0, 16.0 }, },

        { { 16.0, 16.0 },
          { 16.0, 16.0 }, },

        { { 32.0, 32.0 },
          { 32.0, 32.0 }, }
    });

    std::ostringstream stream(std::ios_base::out | std::ios_base::binary);

    imp::ppm::save(stream, img, 32.0);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x36, 0x35, 0x35, 0x33, 0x35, 0x0a, 0x80, 0x00,
                                       0x80, 0x00, 0xff, 0xff, 0x80, 0x00, 0x80, 0x00, 0xff, 0xff, 0x80, 0x00, 0x80, 0x00, 0xff,
                                       0xff, 0x80, 0x00, 0x80, 0x00, 0xff, 0xff } ));
}


TEST(ppm_test, ppm16_bit)
{
    imp::RgbImage<int> img
    ({
        { { 16, 16, 16 },
          { 16, 16, 16 }, },

        { { 1600, 1600, 1600 },
          { 1600, 1600, 1600 }, },

        { { 32, 32, 32 },
          { 32, 32, 32 }, }
    });

    std::ostringstream stream(std::ios_base::out | std::ios_base::binary);

    imp::ppm::save(stream, img, 1600);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x33, 0x20, 0x32, 0x0a, 0x36, 0x35, 0x35, 0x33,
                                       0x35, 0x0a, 0x02, 0x8f, 0xff, 0xff, 0x05, 0x1f, 0x02, 0x8f, 0xff,
                                       0xff, 0x05, 0x1f, 0x02, 0x8f, 0xff, 0xff, 0x05, 0x1f, 0x02, 0x8f,
                                       0xff, 0xff, 0x05, 0x1f, 0x02, 0x8f, 0xff, 0xff, 0x05, 0x1f, 0x02,
                                       0x8f, 0xff, 0xff, 0x05, 0x1f }));
}


TEST(ppm_test, ppm8_bit)
{
    imp::RgbImage<int> img
    ({
        { { 16, 16 },
          { 16, 16 }, },

        { { 255, 255 },
          { 255, 255 }, },

        { { 32, 32 },
          { 32, 32 }, }
    });

    std::ostringstream stream(std::ios_base::out | std::ios_base::binary);



    imp::ppm::save(stream, img, 255);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32, 0x35, 0x35, 0x0a,
                                       0x10, 0xff, 0x20, 0x10, 0xff, 0x20, 0x10, 0xff, 0x20, 0x10, 0xff, 0x20}));
}


TEST(ppm_test, black_level)
{
    imp::RgbImage<int> img
    ({
        { { 16, 16 },
          { 16, 16 }, },

        { { 255, 255 },
          { 255, 255 }, },

        { { 32, 32 },
          { 32, 32 }, }
    });

    std::ostringstream stream(std::ios_base::out | std::ios_base::binary);

    imp::ppm::save(stream, img, 255, 16);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x36, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32, 0x35, 0x35, 0x0a,
                                       0x00, 0xff, 0x11, 0x00, 0xff, 0x11, 0x00, 0xff, 0x11, 0x00, 0xff, 0x11 }));
}


TEST(ppm_test, load_8bit_ppm)
{
    imp::RgbImage<int> img1
    ({
        { { 16, 16 },
          { 16, 16 }, },

        { { 16, 16 },
          { 16, 16 }, },

        { { 32, 32 },
          { 32, 32 }, }
    });

    std::stringstream stream(std::ios_base::out | std::ios_base::in | std::ios_base::binary);

    imp::ppm::save(stream, img1, 32);

    stream.seekp(0);

    auto img2 = imp::ppm::load<int>(stream, 32);

    ASSERT_TRUE(img2 == img1);
}


TEST(ppm_test, load_float)
{
    imp::RgbImage<double> img1
    ({
        { { 16.0, 16.0 },
          { 16.0, 16.0 }, },

        { { 16.0, 16.0 },
          { 16.0, 16.0 }, },

        { { 32.0, 32.0 },
          { 32.0, 32.0 }, }
    });

    std::stringstream stream(std::ios_base::out | std::ios_base::in | std::ios_base::binary);

    imp::ppm::save(stream, img1, 32.0);

    stream.seekp(0);

    auto img2 = imp::ppm::load<double>(stream, 32.0);

    ASSERT_TRUE(img2 == img1);
}


TEST(ppm_test, load_16bit_ppm)
{
    imp::RgbImage<int> img1
    ({
        { { 16, 16, 16 },
          { 16, 16, 16 }, },

        { { 1600, 1600, 1600 },
          { 1600, 1600, 1600 }, },

        { { 32, 32, 32 },
          { 32, 32, 32 }, }
    });


    std::stringstream stream(std::ios_base::out | std::ios_base::in | std::ios_base::binary);

    imp::ppm::save(stream, img1, 1600);

    stream.seekp(0);

    auto img2 = imp::ppm::load<int>(stream, 1600);

    ASSERT_TRUE(img2 == img1);
}


TEST(ppm_test, load_with_black_level)
{
    imp::RgbImage<int> img1
    ({
        { { 16, 16 },
          { 16, 16 }, },

        { { 255, 255 },
          { 255, 255 }, },

        { { 32, 32 },
          { 32, 32 }, }
    });


    std::stringstream stream(std::ios_base::out | std::ios_base::in | std::ios_base::binary);

    imp::ppm::save(stream, img1, 255, 16);

    stream.seekp(0);

    auto img2 = imp::ppm::load<int>(stream, 255, 16);

    ASSERT_TRUE(img2 == img1);
}

