#include <gtest/gtest.h>

#include <ex/stream/memory>
#include <imp/io/pgm>


template <index_t N>
bool check_stream(ex::MemoryStream& stream, const int (&against)[N])
{
    if (stream.position() != N) return false;

    for (index_t i = 0; i < N; ++i)
    {
        if (stream.data()[i] != against[i])
            return false;
    }

    return true;
}


TEST(pgm_test, pgm_8bit)
{
    imp::Matrix<int> img
    ({
       { 4,   8 },
       { 16, 24 },
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);


    imp::pgm::save(stream, img, 24);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x35, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32,
                                       0x35, 0x35, 0x0a, 0x2b, 0x55, 0xaa, 0xff } ));
}


TEST(pgm_test, float_ppm)
{
    imp::Matrix<double> img
    ({
        { 4.0,  8.0 },
        { 16.0, 24.0 },
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img, 24.0);

    ASSERT_TRUE(check_stream(stream, { 0x50, 0x35, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x36, 0x35, 0x35, 0x33, 0x35, 0x0a,
                                       0x2a, 0xab, 0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff } ));
}


TEST(pgm_test, pgm16_bit)
{
    imp::Matrix<int> img
    ({
        { 16, 1600 },
        { 16, 800 }
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img, 1600);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x35, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x36, 0x35, 0x35, 0x33, 0x35,
                                       0x0a, 0x02, 0x8f, 0xff, 0xff, 0x02, 0x8f, 0x80, 0x00 }));
}



TEST(pgm_test, black_level)
{
    imp::Matrix<int> img
    ({
        { 16, 255 },
        { 16, 32  },
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img, 255, 16);
    ASSERT_TRUE(check_stream(stream, { 0x50, 0x35, 0x0a, 0x32, 0x20, 0x32, 0x0a, 0x32,
                                       0x35, 0x35, 0x0a, 0x00, 0xff, 0x00, 0x11 }));
}


TEST(pgm_test, load_8bit_ppm)
{
    imp::Matrix<int> img1
    ({
        { 16, 32, 3 },
        { 16, 10, 4 }
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img1, 32);

    stream.seek(0);

    auto img2 = imp::pgm::load<int>(stream, 32);

    ASSERT_TRUE(img2 == img1);
}


TEST(pgm_test, load_float)
{
    imp::Matrix<double> img1
    ({
        { 32.0, 16.0, 4.0 },
        { 16.0, 32.0, 4.0 },
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img1, 32.0);

    stream.seek(0);

    auto img2 = imp::pgm::load<double>(stream, 32.0);

    ASSERT_TRUE(img2 == img1);
}


TEST(pgm_test, load_16bit_ppm)
{
    imp::Matrix<int> img1
    ({
        { 16, 1600, 0 },
        { 16, 32,   0 }
    });

    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img1, 1600);

    stream.seek(0);

    auto img2 = imp::pgm::load<int>(stream, 1600);

    ASSERT_TRUE(img2 == img1);
}


TEST(pgm_test, load_with_black_level)
{
    imp::Matrix<int> img1
    ({
        { 16, 32,  18 },
        { 16, 255, 18 },
    });


    std::unique_ptr<uint8_t[]> data(new uint8_t[256]);
    ex::MemoryStream stream(data.get(), 256);

    imp::pgm::save(stream, img1, 255, 16);

    stream.seek(0);

    auto img2 = imp::pgm::load<int>(stream, 255, 16);

    ASSERT_TRUE(img2 == img1);
}
