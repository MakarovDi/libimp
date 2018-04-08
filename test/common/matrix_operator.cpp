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
    ASSERT_TRUE( Matrix<TypeParam>({ TypeParam(1), TypeParam(2) }) == Matrix<TypeParam>({ TypeParam(1), TypeParam(2) }) );
    ASSERT_TRUE( Matrix<TypeParam>({ TypeParam(1) }) != Matrix<TypeParam>({ TypeParam(1), TypeParam(2) }) );

    Matrix<TypeParam> m1(2, 1, { TypeParam(1), TypeParam(2) });
    Matrix<TypeParam> m2(1, 2, { TypeParam(1), TypeParam(2) });

    ASSERT_TRUE(m1 != m2);
    ASSERT_TRUE(m1 == m1);

    Matrix<TypeParam> m3({
         { TypeParam(1), TypeParam(2) },
         { TypeParam(3), TypeParam(4) },
         { TypeParam(5), TypeParam(6) },
    });

    Matrix<TypeParam> m4(2, 3, { TypeParam(1), TypeParam(2),
                                 TypeParam(3), TypeParam(4),
                                 TypeParam(5), TypeParam(6) });

    ASSERT_TRUE(m3 == m4);
}


TYPED_TEST(MatrixOperatorTypeMixture, sum_operator_ctor)
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


TYPED_TEST(MatrixOperatorTypeMixture, sub_operator_ctor)
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