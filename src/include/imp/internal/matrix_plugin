#ifndef    IMP_EIGEN_PLUGIN_HEADER
#   define IMP_EIGEN_PLUGIN_HEADER


// Eigen::Matrix plugin

public:
    // matrix initialization
    template <Index M, Index N>
    explicit Matrix(const _Scalar (&data)[M][N]) : Matrix(M, N)
    {
        init_matrix(data);
    }


    template <Index M, Index N>
    Matrix& operator=(const _Scalar (&ptr)[M][N])
    {
        if (this->size() == M*N)
        {
            this->resize(M, N);
            init_matrix(ptr);
        }
        else
        {
            Matrix<_Scalar, Dynamic, Dynamic, _Options, Dynamic, Dynamic> data(ptr);
            this->operator=(std::move(data));
        }

        return *this;
    }

private:

    template <Index M, Index N>
    void init_matrix(const _Scalar (& data)[M][N])
    {
        for (Index y = 0; y < M; ++y)
            for (Index x = 0; x < N; ++x)
            {
                this->operator()(y, x) = data[y][x];
            }
    }

public:



#endif