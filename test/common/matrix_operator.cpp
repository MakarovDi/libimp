#include <gtest/gtest.h>
#include <cstring>

#include <imp/common/matrix>


using namespace imp;
using testing::Types;


template <typename T> class MatrixOperatorTypeMixture: public ::testing::Test { };


using MathTypes = Types
<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t,
    float, double, long double
>;

TYPED_TEST_CASE(MatrixOperatorTypeMixture, MathTypes);


TYPED_TEST(MatrixOperatorTypeMixture, equality_operator)
{
    using T = TypeParam;

    T eps = std::numeric_limits<T>::epsilon();

    ASSERT_TRUE( Matrix<T>({ T(T(1) + eps), T(2) }) == Matrix<T>({ T(1), T(2) }) );

    if (std::is_floating_point<T>::value)
    {
        ASSERT_TRUE( Matrix<T>({ T(T(1) + 2*eps), T(2) }) != Matrix<T>({ T(1), T(2) }) );
    }

    ASSERT_TRUE( Matrix<T>({ T(1) }) != Matrix<T>({ T(1), T(2) }) );

    Matrix<T> m1(2, 1, { T(1), T(2) });
    Matrix<T> m2(1, 2, { T(1), T(2) });

    ASSERT_TRUE(m1 != m2);
    ASSERT_TRUE(m1 == m1);

    Matrix<T> m3({
         { T(1), T(2) },
         { T(3), T(4) },
         { T(5), T(6) },
    });

    Matrix<T> m4(2, 3, { T(1), T(2),
                         T(3), T(4),
                         T(5), T(6) });

    ASSERT_TRUE(m3 == m4);
}


TYPED_TEST(MatrixOperatorTypeMixture, sum_operator)
{
    Matrix<TypeParam> m({ { TypeParam(1), TypeParam(2), TypeParam(3) },
                          { TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> result({ { TypeParam(1+1), TypeParam(2+2), TypeParam(3+3) },
                               { TypeParam(4+4), TypeParam(5+5), TypeParam(6+6) } });

    Matrix<TypeParam> m2 = m;

    m += m;
    ASSERT_TRUE(m == result);
    ASSERT_TRUE(m2 + m2 == result);

    Matrix<TypeParam> m4(4, 4);
    ASSERT_THROW(m4 += m, std::logic_error);
}


TYPED_TEST(MatrixOperatorTypeMixture, sub_operator)
{
    Matrix<TypeParam> m({ { TypeParam(1), TypeParam(2), TypeParam(3) },
                          { TypeParam(4), TypeParam(5), TypeParam(6) } });

    Matrix<TypeParam> result({ { TypeParam(1-1), TypeParam(2-2), TypeParam(3-3) },
                               { TypeParam(4-4), TypeParam(5-5), TypeParam(6-6) } });

    Matrix<TypeParam> m2 = m;

    m -= m;
    ASSERT_TRUE(m == result);
    ASSERT_TRUE(m2 - m2 == result);

    Matrix<TypeParam> m4(4, 4);
    ASSERT_THROW(m4 -= m, std::logic_error);
}


TYPED_TEST(MatrixOperatorTypeMixture, add_overflow_test)
{
    TypeParam max = std::numeric_limits<TypeParam>::max();

    Matrix<TypeParam> m1({ max, max });
    Matrix<TypeParam> m2({ TypeParam(2), TypeParam(2) });

    Matrix<TypeParam> result({ TypeParam(TypeParam(2) + max), TypeParam(TypeParam(2) + max) });

    ASSERT_TRUE(m1 + m2 == result);
}


TYPED_TEST(MatrixOperatorTypeMixture, sub_overflow_test)
{
    TypeParam min = std::numeric_limits<TypeParam>::min();

    Matrix<TypeParam> m1({ min, min });
    Matrix<TypeParam> m2({ TypeParam(2), TypeParam(2) });

    Matrix<TypeParam> result({ TypeParam(min - TypeParam(2)), TypeParam(min - TypeParam(2)) });

    ASSERT_TRUE(m1 - m2 == result);
}