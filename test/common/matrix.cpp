#include <gtest/gtest.h>
#include <cstring>

#include <imp/common/matrix>


using namespace imp;
using testing::Types;


template <typename T> class MatrixTests: public ::testing::Test { };


using MathTypes = Types
<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t,
    float, double, long double
>;

TYPED_TEST_CASE(MatrixTests, MathTypes);


TYPED_TEST(MatrixTests, default_ctor)
{
    Matrix<TypeParam> m(2, 3);
    ASSERT_TRUE(m.is_own_memory());

    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 2);
    ASSERT_EQ(m.size(), 3*2);
    ASSERT_EQ(m.mem_size(), 3*2*sizeof(TypeParam));
}


TYPED_TEST(MatrixTests, raw_ptr_copy_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m.is_own_memory());

    std::memset(ptr, 0, sizeof(TypeParam)*3*2);

    ASSERT_EQ(m(0, 0), TypeParam(1));
    ASSERT_EQ(m(1, 0), TypeParam(2));
    ASSERT_EQ(m(2, 0), TypeParam(3));
    ASSERT_EQ(m(0, 1), TypeParam(4));
    ASSERT_EQ(m(1, 1), TypeParam(5));
    ASSERT_EQ(m(2, 1), TypeParam(6));
}


TYPED_TEST(MatrixTests, raw_ptr_reuse_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_FALSE(m.is_own_memory());

    std::memset(ptr, 0, sizeof(TypeParam)*3*2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ptr[i] = TypeParam(i);
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, const_raw_ptr_copy_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    const TypeParam* const_ptr = ptr;

    Matrix<TypeParam> m(3, 2, const_ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m.is_own_memory());

    std::memset(ptr, 0, sizeof(TypeParam)*3*2);

    ASSERT_EQ(m(0, 0), TypeParam(1));
    ASSERT_EQ(m(1, 0), TypeParam(2));
    ASSERT_EQ(m(2, 0), TypeParam(3));
    ASSERT_EQ(m(0, 1), TypeParam(4));
    ASSERT_EQ(m(1, 1), TypeParam(5));
    ASSERT_EQ(m(2, 1), TypeParam(6));
}


TYPED_TEST(MatrixTests, const_raw_ptr_reuse_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    const TypeParam* const_ptr = ptr;

    const Matrix<TypeParam> m(3, 2, const_ptr, MatrixMem::kReuse);
    ASSERT_FALSE(m.is_own_memory());

    std::memset(ptr, 0, sizeof(TypeParam)*3*2);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ptr[i] = TypeParam(i);
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, unique_ptr_copy_ctor)
{
    std::unique_ptr<TypeParam[]> ptr(new TypeParam[3]);

    ptr[0] = TypeParam(0);
    ptr[1] = TypeParam(1);
    ptr[2] = TypeParam(2);

    Matrix<TypeParam> m(3, 1, ptr);
    ASSERT_TRUE(m.is_own_memory());

    std::memset(ptr.get(), 0, sizeof(TypeParam)*3);

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, unique_ptr_move_ctor)
{
    std::unique_ptr<TypeParam[]> ptr(new TypeParam[3]);

    TypeParam* ptr_addr = ptr.get();

    ptr[0] = TypeParam(0);
    ptr[1] = TypeParam(1);
    ptr[2] = TypeParam(2);

    Matrix<TypeParam> m(3, 1, std::move(ptr));

    ASSERT_EQ(ptr.get(), nullptr);

    ASSERT_EQ(m.ptr(), ptr_addr);
    ASSERT_TRUE(m.is_own_memory());

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, copy_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_EQ(m.ptr(), ptr);

    Matrix<TypeParam> m2(m);
    ASSERT_TRUE(m2.ptr() != m.ptr());
    ASSERT_TRUE(m2.is_own_memory());
    ASSERT_EQ(m2.size(), m.size());
    ASSERT_EQ(m2.rows(), m.rows());
    ASSERT_EQ(m2.cols(), m.cols());
    ASSERT_EQ(m2.mem_size(), m.mem_size());

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m2[i], ptr[i]);
    }


    Matrix<TypeParam> m1(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m1.ptr() != ptr);

    Matrix<TypeParam> m4(m1);
    ASSERT_TRUE(m4.ptr() != m1.ptr());
    ASSERT_TRUE(m4.is_own_memory());
    ASSERT_EQ(m4.size(), m1.size());
    ASSERT_EQ(m4.rows(), m1.rows());
    ASSERT_EQ(m4.cols(), m1.cols());
    ASSERT_EQ(m4.mem_size(), m1.mem_size());

    for (index_t i = 0; i < m1.size(); ++i)
    {
        ASSERT_EQ(m4[i], ptr[i]);
    }
}


TYPED_TEST(MatrixTests, copy_operator)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    // test reuse mode
    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_EQ(m.ptr(), ptr);

    Matrix<TypeParam> m1(1, 1);

    m1 = m;

    ASSERT_TRUE(m1.ptr() != m.ptr());
    ASSERT_TRUE(m1.is_own_memory());
    ASSERT_EQ(m1.size(), m.size());
    ASSERT_EQ(m1.rows(), m.rows());
    ASSERT_EQ(m1.cols(), m.cols());
    ASSERT_EQ(m1.mem_size(), m.mem_size());

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m1[i], ptr[i]);
    }

    // test copy mode
    Matrix<TypeParam> m2(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m2.ptr() != ptr);

    Matrix<TypeParam> m3(1, 1);

    m3 = m2;

    ASSERT_TRUE(m3.ptr() != m2.ptr());
    ASSERT_TRUE(m3.is_own_memory());
    ASSERT_EQ(m3.size(), m2.size());
    ASSERT_EQ(m3.rows(), m2.rows());
    ASSERT_EQ(m3.cols(), m2.cols());
    ASSERT_EQ(m3.mem_size(), m2.mem_size());

    for (index_t i = 0; i < m2.size(); ++i)
    {
        ASSERT_EQ(m3[i], ptr[i]);
    }
}


TYPED_TEST(MatrixTests, mem_shrinkage_test)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);

    // test no memory shrinkage
    index_t size = Matrix<TypeParam>::Config::kMemShrinkageScale*m.size() - 1;

    Matrix<TypeParam> m1(size, 1);

    m1 = m;
    ASSERT_EQ(m1.mem_size(), size * index_t(sizeof(TypeParam)));

    // test memory shrinkage
    size = Matrix<TypeParam>::Config::kMemShrinkageScale*m.size() + 1;

    Matrix<TypeParam> m2(size, 1);

    m2 = m;
    ASSERT_EQ(m2.mem_size(), m.mem_size());
}


TYPED_TEST(MatrixTests, move_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    // test reuse mode
    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_EQ(m.ptr(), ptr);

    Matrix<TypeParam> m2(std::move(m));
    ASSERT_EQ(m2.ptr(), ptr);
    ASSERT_FALSE(m2.is_own_memory());
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 3);
    ASSERT_EQ(m2.mem_size(), 6 * sizeof(TypeParam));

    ASSERT_EQ(m.ptr(), nullptr);
    ASSERT_FALSE(m.is_own_memory());
    ASSERT_EQ(m.size(), 0);
    ASSERT_EQ(m.rows(), 0);
    ASSERT_EQ(m.cols(), 0);
    ASSERT_EQ(m.mem_size(), 0);

    // test copy mode
    Matrix<TypeParam> m1(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m1.ptr() != ptr);

    TypeParam* p = m1.ptr();

    Matrix<TypeParam> m3(std::move(m1));
    ASSERT_EQ(m3.ptr(), p);
    ASSERT_TRUE(m3.is_own_memory());
    ASSERT_EQ(m3.size(), 6);
    ASSERT_EQ(m3.rows(), 2);
    ASSERT_EQ(m3.cols(), 3);
    ASSERT_EQ(m3.mem_size(), 6 * sizeof(TypeParam));

    ASSERT_EQ(m1.ptr(), nullptr);
    ASSERT_FALSE(m1.is_own_memory());
    ASSERT_EQ(m1.size(), 0);
    ASSERT_EQ(m1.rows(), 0);
    ASSERT_EQ(m1.cols(), 0);
    ASSERT_EQ(m1.mem_size(), 0);
}


TYPED_TEST(MatrixTests, move_rvalue_ctor)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m2(Matrix<TypeParam>(3, 2, ptr, MatrixMem::kReuse));
    ASSERT_EQ(m2.ptr(), ptr);
    ASSERT_FALSE(m2.is_own_memory());
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 3);
    ASSERT_EQ(m2.mem_size(), 6 * sizeof(TypeParam));

    Matrix<TypeParam> m3(Matrix<TypeParam>(3, 2, ptr, MatrixMem::kCopy));
    ASSERT_TRUE(m3.ptr() != ptr);
    ASSERT_TRUE(m3.is_own_memory());
    ASSERT_EQ(m3.size(), 6);
    ASSERT_EQ(m3.rows(), 2);
    ASSERT_EQ(m3.cols(), 3);
    ASSERT_EQ(m3.mem_size(), 6 * sizeof(TypeParam));
}


TYPED_TEST(MatrixTests, move_operator)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    // test reuse mode
    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);
    ASSERT_EQ(m.ptr(), ptr);

    Matrix<TypeParam> m2(1, 1);

    m2 = std::move(m);

    ASSERT_EQ(m.ptr(), nullptr);
    ASSERT_FALSE(m.is_own_memory());
    ASSERT_EQ(m.size(), 0);
    ASSERT_EQ(m.rows(), 0);
    ASSERT_EQ(m.cols(), 0);
    ASSERT_EQ(m.mem_size(), 0);

    ASSERT_EQ(m2.ptr(), ptr);
    ASSERT_FALSE(m2.is_own_memory());
    ASSERT_EQ(m2.size(), 6);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 3);
    ASSERT_EQ(m2.mem_size(), 6 * sizeof(TypeParam));

    // test copy mode
    Matrix<TypeParam> m1(3, 2, ptr, MatrixMem::kCopy);
    ASSERT_TRUE(m1.ptr() != ptr);

    TypeParam* p = m1.ptr();

    Matrix<TypeParam> m3(1, 1);

    m3 = std::move(m1);

    ASSERT_EQ(m1.ptr(), nullptr);
    ASSERT_FALSE(m1.is_own_memory());
    ASSERT_EQ(m1.size(), 0);
    ASSERT_EQ(m1.rows(), 0);
    ASSERT_EQ(m1.cols(), 0);
    ASSERT_EQ(m1.mem_size(), 0);

    ASSERT_EQ(m3.ptr(), p);
    ASSERT_TRUE(m3.is_own_memory());
    ASSERT_EQ(m3.size(), 6);
    ASSERT_EQ(m3.rows(), 2);
    ASSERT_EQ(m3.cols(), 3);
    ASSERT_EQ(m3.mem_size(), 6 * sizeof(TypeParam));
}


TYPED_TEST(MatrixTests, from_array_ctor)
{
    Matrix<TypeParam> m(2, 3, { TypeParam(0), TypeParam(1), TypeParam(2),
                                TypeParam(3), TypeParam(4), TypeParam(5) });

    ASSERT_TRUE(m.is_own_memory());

    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 2);
    ASSERT_EQ(m.size(), 3*2);
    ASSERT_EQ(m.mem_size(), 3*2*sizeof(TypeParam));

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, vector_ctor)
{
    Matrix<TypeParam> m({ TypeParam(0), TypeParam(1), TypeParam(2) });

    ASSERT_TRUE(m.is_own_memory());

    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 1);
    ASSERT_EQ(m.size(), 3*1);
    ASSERT_EQ(m.mem_size(), 3*1*sizeof(TypeParam));

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, matrix_array_ctor)
{
    Matrix<TypeParam> m({ { TypeParam(0), TypeParam(1), TypeParam(2) },
                          { TypeParam(3), TypeParam(4), TypeParam(5) } });

    ASSERT_TRUE(m.is_own_memory());

    ASSERT_EQ(m.rows(), 2);
    ASSERT_EQ(m.cols(), 3);
    ASSERT_EQ(m.size(), 3*2);
    ASSERT_EQ(m.mem_size(), 3*2*sizeof(TypeParam));

    for (index_t i = 0; i < m.size(); ++i)
    {
        ASSERT_EQ(m[i], TypeParam(i));
    }
}


TYPED_TEST(MatrixTests, indexing)
{
    TypeParam ptr[] = { TypeParam(1), TypeParam(2), TypeParam(3),
                        TypeParam(4), TypeParam(5), TypeParam(6)  };

    Matrix<TypeParam> m(3, 2, ptr, MatrixMem::kReuse);

    index_t i = 0;

    for (index_t r = 0; r < m.rows(); ++r)
        for (index_t c = 0; c < m.cols(); ++c)
        {
            ASSERT_EQ(m[i], m(i));
            ASSERT_EQ(m[i], m(c, r));
            ASSERT_EQ(m[i], m.item(i));
            ASSERT_EQ(m[i], m.item(c, r));

            ++i;
        }

    ASSERT_EQ(i, m.size());

    i = 0;

    for (index_t r = 0; r < m.rows(); ++r)
        for (index_t c = 0; c < m.cols(); ++c)
        {
            m[i] = TypeParam(i);

            ASSERT_EQ(m[i], TypeParam(i));
            ASSERT_EQ(m(i), TypeParam(i));
            ASSERT_EQ(m(c, r), TypeParam(i));
            ASSERT_EQ(m.item(i), TypeParam(i));
            ASSERT_EQ(m.item(c, r), TypeParam(i));

            ++i;
        }
}