#include <gtest/gtest.h>
#include <cstring>

#  define eigen_assert(x) if (!(x)) { throw (std::runtime_error("error")); }
#include <imp/common/matrix>


using namespace imp;
using testing::Types;


template <typename T> class matrix: public ::testing::Test { };


using MathTypes = Types
<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t,
    float, double, long double
>;

TYPED_TEST_CASE(matrix, MathTypes);


TYPED_TEST(matrix, default_ctor)
{
    Matrix<TypeParam> m(2, 3);

    ASSERT_EQ(m.rows(), 2);
    ASSERT_EQ(m.cols(), 3);
    ASSERT_EQ(m.size(), 3*2);
}


TYPED_TEST(matrix, raw_ptr_copy_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6) };

    Matrix<T, 2, 3> m(ptr);

    std::memset(ptr, 0, sizeof(T)*3*2);

    ASSERT_EQ(m(0, 0), T(1));
    ASSERT_EQ(m(0, 1), T(2));
    ASSERT_EQ(m(0, 2), T(3));
    ASSERT_EQ(m(1, 0), T(4));
    ASSERT_EQ(m(1, 1), T(5));
    ASSERT_EQ(m(1, 2), T(6));

    Matrix<T, 2, 3> m1({ { T(1), T(2), T(3) },
                         { T(4), T(5), T(6) } });

    ASSERT_EQ(m1(0, 0), T(1));
    ASSERT_EQ(m1(0, 1), T(2));
    ASSERT_EQ(m1(0, 2), T(3));
    ASSERT_EQ(m1(1, 0), T(4));
    ASSERT_EQ(m1(1, 1), T(5));
    ASSERT_EQ(m1(1, 2), T(6));
}


TYPED_TEST(matrix, raw_ptr_reuse_ctor)
{
    using T = TypeParam;
    
    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6)  };

    Map<Matrix<T>> m(ptr, 2, 3);

    ASSERT_TRUE(m.data() == ptr);
    std::memset(ptr, 0, sizeof(T)*3*2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ptr[i] = T(i);
        ASSERT_EQ(m(i), T(i));
    }
}


TYPED_TEST(matrix, const_raw_ptr_copy_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6)  };

    const T* const_ptr = ptr;

    Matrix<T, 2, 3> m(const_ptr);
    ASSERT_TRUE(m.data() != ptr);

    std::memset(ptr, 0, sizeof(T)*3*2);

    ASSERT_EQ(m(0, 0), T(1));
    ASSERT_EQ(m(0, 1), T(2));
    ASSERT_EQ(m(0, 2), T(3));
    ASSERT_EQ(m(1, 0), T(4));
    ASSERT_EQ(m(1, 1), T(5));
    ASSERT_EQ(m(1, 2), T(6));
}


TYPED_TEST(matrix, const_raw_ptr_reuse_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6)  };

    const T* const_ptr = ptr;

    Map<const Matrix<T, 2, 3>> m(const_ptr);
    ASSERT_TRUE(m.data() == ptr);

    std::memset(ptr, 0, sizeof(T)*3*2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ptr[i] = T(i);
        ASSERT_EQ(m(i), T(i));
    }
}


TYPED_TEST(matrix, copy_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6)  };

    Map<Matrix<T, 2, 3>> m(ptr);
    ASSERT_EQ(m.data(), ptr);

    Matrix<T> m2(m);
    ASSERT_TRUE(m2.data() != m.data());
    ASSERT_EQ(m2.size(), m.size());
    ASSERT_EQ(m2.rows(), m.rows());
    ASSERT_EQ(m2.cols(), m.cols());

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m2(i), ptr[i]);
    }


    Matrix<T, 2, 3> m1(ptr);
    ASSERT_TRUE(m1.data() != ptr);

    Matrix<T> m4(m1);
    ASSERT_TRUE(m4.data() != m1.data());
    ASSERT_EQ(m4.size(), m1.size());
    ASSERT_EQ(m4.rows(), m1.rows());
    ASSERT_EQ(m4.cols(), m1.cols());

    for (index_t i = 0; i < m1.size(); ++i)
    {
        ASSERT_EQ(m4(i), ptr[i]);
    }
}


TYPED_TEST(matrix, copy_operator)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6) };

    // test reuse mode
    Map<Matrix<T, 2, 3>> m(ptr);
    ASSERT_EQ(m.data(), ptr);

    Matrix<T> m1(1, 1);

    m1 = m;

    ASSERT_TRUE(m1.data() != m.data());
    ASSERT_EQ(m1.size(), m.size());
    ASSERT_EQ(m1.rows(), m.rows());
    ASSERT_EQ(m1.cols(), m.cols());

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m1(i), ptr[i]);
    }

    // test copy mode
    Matrix<T, 2, 3> m2(ptr);
    ASSERT_TRUE(m2.data() != ptr);

    Matrix<T> m3(1, 1);

    m3 = m2;

    ASSERT_TRUE(m3.data() != m2.data());
    ASSERT_EQ(m3.size(), m2.size());
    ASSERT_EQ(m3.rows(), m2.rows());
    ASSERT_EQ(m3.cols(), m2.cols());

    for (index_t i = 0; i < m2.size(); ++i)
    {
        ASSERT_EQ(m3(i), ptr[i]);
    }
}


TYPED_TEST(matrix, copy_array_operator)
{
    using T = TypeParam;

    // test reuse mode
    Matrix<T> m({ { T(1), T(2), T(3) },
                  { T(4), T(5), T(6) } });
    const T* ptr = m.data();


    m = { { T(0), T(1), T(2) },
          { T(3), T(4), T(5) } };

    ASSERT_EQ(m.data(), ptr);
    ASSERT_EQ(m.rows(), 2);
    ASSERT_EQ(m.cols(), 3);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(i));
    }

    m = { { T(0), T(0) },
          { T(0), T(0) },
          { T(0), T(0) } };

    ASSERT_EQ(m.data(), ptr);
    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(0));
    }

    m = { { T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1) },
          { T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1) },
          { T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1) } };

    ASSERT_TRUE(m.data() != ptr);
    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 8);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(1));
    }
}


TYPED_TEST(matrix, move_ctor)
{
    using T = TypeParam;

    Matrix<T> m({ { T(1), T(2), T(3) },
                  { T(4), T(5), T(6) } });
    const T* ptr = m.data();

    Matrix<T> m2(std::move(m));
    ASSERT_EQ(m2.data(), ptr);
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 3);

    ASSERT_EQ(m.data(), nullptr);
    ASSERT_EQ(m.size(), 0);
    ASSERT_EQ(m.rows(), 0);
    ASSERT_EQ(m.cols(), 0);
}


TYPED_TEST(matrix, move_rvalue_ctor)
{
    using T = TypeParam;

    T ptr[] = { T(1), T(2), T(3),
                T(4), T(5), T(6)  };

    Matrix<T> m3(Map<Matrix<T, 2, 3>>{ptr});

    ASSERT_TRUE(m3.data() != ptr);
    ASSERT_EQ(m3.size(), 6);
    ASSERT_EQ(m3.rows(), 2);
    ASSERT_EQ(m3.cols(), 3);
}


TYPED_TEST(matrix, move_operator)
{
    using T = TypeParam;

    Matrix<T> m({ { T(1), T(2), T(3) },
                  { T(4), T(5), T(6) } });

    const T* ptr = m.data();

    Matrix<T> m2(1, 2);

    m2 = std::move(m);

    ASSERT_EQ(m2.data(), ptr);
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 3);

    // test copy mode
    Matrix<T> m1(m2);
    ASSERT_TRUE(m1.data() != ptr);

    T* p = m1.data();

    Matrix<T> m3(1, 1);

    m3 = std::move(m1);

    ASSERT_EQ(m3.data(), p);
    ASSERT_EQ(m3.size(), 6);
    ASSERT_EQ(m3.rows(), 2);
    ASSERT_EQ(m3.cols(), 3);
}


TYPED_TEST(matrix, vector_ctor)
{
    using T = TypeParam;

    Eigen::VectorXi v(Eigen::Index(4));

    Vector<T> m({ {T(0)},
                  {T(1)},
                  {T(2)} });

    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 1);
    ASSERT_EQ(m.size(), 3);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(i));
    }
}


TYPED_TEST(matrix, row_vector_ctor)
{
    using T = TypeParam;

    Eigen::VectorXi v(Eigen::Index(4));

    RowVector<T> m({{ T(0), T(1), T(2) }});

    ASSERT_EQ(m.cols(), 3);
    ASSERT_EQ(m.rows(), 1);
    ASSERT_EQ(m.size(), 3);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(i));
    }
}


TYPED_TEST(matrix, matrix_array_ctor)
{
    using T = TypeParam;

    Matrix<T> m({ { T(0), T(1), T(2) },
                  { T(3), T(4), T(5) } });

    ASSERT_EQ(m.rows(), 2);
    ASSERT_EQ(m.cols(), 3);
    ASSERT_EQ(m.size(), 3*2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m(i), T(i));
    }
}


TYPED_TEST(matrix, matrix_array_assignment)
{
    using T = TypeParam;

    Matrix<T> m1(2, 3);
    T* ptr = m1.data();

    m1 = { {T(0), T(1)},
           {T(1), T(0)},
           {T(1), T(1)}};
    ASSERT_TRUE(m1.data() == ptr);
    ASSERT_TRUE(m1.cols() == 2);
    ASSERT_TRUE(m1.rows() == 3);
    ASSERT_TRUE(m1 == Matrix<T>({ {T(0), T(1)}, {T(1), T(0)}, {T(1), T(1)}}));


    m1 = { {T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10), T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10)},
           {T(1), T(0), T(3), T(5), T(0), T(3), T(4), T(3),  T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10)} };
    ASSERT_TRUE(m1.data() != ptr);
    ASSERT_TRUE(m1.cols() == 16);
    ASSERT_TRUE(m1.rows() == 2);
    ASSERT_TRUE(m1 == Matrix<T>( { {T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10), T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10)},
                                   {T(1), T(0), T(3), T(5), T(0), T(3), T(4), T(3),  T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10)} }));
}


TYPED_TEST(matrix, map_array_assignment)
{
    using T = TypeParam;

    T ptr[] = { 1, 2, 3, 4, 5, 6 };
    Map<Matrix<T>> m1(ptr, 2, 3);

    ASSERT_THROW((m1 = { {T(0), T(1)},
                         {T(1), T(0)},
                         {T(1), T(1)} }), std::runtime_error);

    ASSERT_THROW((m1 = { {T(0), T(1), T(2), T(4), T(6), T(8), T(9), T(10)},
                         {T(1), T(0), T(3), T(5), T(0), T(3), T(4), T(3) } }), std::runtime_error);


    ASSERT_NO_THROW((m1 = { {T(0), T(1), T(0)}, {T(1), T(0), T(1)}} ));
    ASSERT_TRUE(m1.data() == ptr);
    ASSERT_TRUE(m1.cols() == 3);
    ASSERT_TRUE(m1.rows() == 2);

    ASSERT_TRUE(m1 == Matrix<T>({ {T(0), T(1), T(0)}, {T(1), T(0), T(1)}} ));
}