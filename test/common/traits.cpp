#include <gtest/gtest.h>

#include <imp/common/traits>


using namespace imp;




TEST(traits, is_eigen_xpr)
{
    static_assert(!is_eigen_xpr<Matrix<int>>::value, "matrix isn't expression");
    static_assert(!is_eigen_xpr<Array<int>>::value, "array isn't expression");

    Matrix<int> m(2,2);

    static_assert(is_eigen_xpr< decltype(m.row(0)) >::value, "row/col is expression");
    static_assert(is_eigen_xpr< decltype(m.transpose()) >::value, "row/col is expression");
    static_assert(is_eigen_xpr< decltype(m.block(0, 0, 1, 1)) >::value, "block is expression");
}


TEST(traits, eigen_decay)
{
    using T1 = typename eigen_decay<Matrix<int, 3, 4>>::type;
    static_assert(std::is_same<T1, Matrix<int, 3, 4>>::value, "Matrix<T, 3, 4> decay");

    using T2 = typename eigen_decay<Eigen::Transpose<Matrix<int, 3, 4>>>::type;
    static_assert(std::is_same<T2, Matrix<int, 4, 3>>::value, "Transpose<Matrix<int, 3, 4> decay");

    using T3 = typename eigen_decay<Eigen::Block<Array<int, 3, 4>>>::type;
    static_assert(std::is_same<T3, Array<int, Eigen::Dynamic, Eigen::Dynamic, 3, 4>>::value, "Block<Array<int, 3, 4> decay");

    using T4 = typename eigen_decay<Eigen::Block<Matrix<int, 3, 4>>>::type;
    static_assert(std::is_same<T4, Matrix<int, Eigen::Dynamic, Eigen::Dynamic, 3, 4>>::value, "Block<Matrix<int, 3, 4> decay");

    using T5 = typename eigen_decay<Eigen::Block<Array<int>>>::type;
    static_assert(std::is_same<T5, Array<int>>::value, "Block<Array<int> decay");

    using T6 = typename eigen_decay<Eigen::Block<Matrix<int>>>::type;
    static_assert(std::is_same<T6, Matrix<int>>::value, "Block<Matrix<int> decay");
}


TEST(traits, eigen_transpose)
{
    using T1 = typename eigen_transpose<Matrix<int, 3, 4>>::type;
    static_assert(std::is_same<T1, Matrix<int, 4, 3>>::value, "Matrix<T, 3, 4> decay");

    using T2 = typename eigen_transpose<Eigen::Transpose<Matrix<int, 3, 4>>>::type;
    static_assert(std::is_same<T2, Matrix<int, 3, 4>>::value, "Transpose<Matrix<int, 3, 4> decay");

    using T3 = typename eigen_transpose<Eigen::Block<Array<int, 3, 4>>>::type;
    static_assert(std::is_same<T3, Array<int, Eigen::Dynamic, Eigen::Dynamic, 4, 3>>::value, "Block<Array<int, 3, 4> decay");

    using T4 = typename eigen_transpose<Eigen::Block<Matrix<int, 3, 4>>>::type;
    static_assert(std::is_same<T4, Matrix<int, Eigen::Dynamic, Eigen::Dynamic, 4, 3>>::value, "Block<Matrix<int, 3, 4> decay");

    using T5 = typename eigen_transpose<Eigen::Block<Array<int>>>::type;
    static_assert(std::is_same<T5, Array<int>>::value, "Block<Array<int> decay");

    using T6 = typename eigen_transpose<Eigen::Block<Matrix<int>>>::type;
    static_assert(std::is_same<T6, Matrix<int>>::value, "Block<Matrix<int> decay");
}
