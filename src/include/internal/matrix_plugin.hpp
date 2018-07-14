#ifndef    IMP_EIGEN_PLUGIN_HEADER
#   define IMP_EIGEN_PLUGIN_HEADER


// Eigen::Matrix plugin

public:
    // matrix initialization
    template <Index M, Index N>
    explicit Matrix(const _Scalar (&data)[M][N]) : Matrix(M, N)
    {
        static_assert( (_Rows == Dynamic  && _Cols == Dynamic) ||
                      ((_Rows != Dynamic  && _Cols != Dynamic) &&
                       (_Rows == M        && _Cols == N)),
                       "initialization array and matrix size mismatch");

        for (Index y = 0; y < M; ++y)
            for (Index x = 0; x < N; ++x)
            {
                this->coeffRef(y, x) = data[y][x];
            }
    }


    // vector initialization
    template <Index N>
    Matrix(const _Scalar (&data)[N]) : Matrix(N)
    {
       static_assert( (_Rows == 1 && (_Cols == N || _Cols == Dynamic)) ||
                      (_Cols == 1 && (_Rows == N || _Rows == Dynamic)),
                       "initialization array and vector size mismatch");

        for (Index i = 0; i < N; ++i)
        {
            this->coeffRef(i) = data[i];
        }
    }


#endif