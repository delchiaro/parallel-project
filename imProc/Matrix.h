//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_MATRIX_H
#define DISPLAYIMAGE_MATRIX_H


namespace imProc {


    template<class T>
    class Matrix {
    private:
        const int cols;
        const int rows;
        T *mat;

    protected:
        inline int _index(int row, int col) const {
            return row + col * this->rows;
        }


    public:
        Matrix(int rows, int cols) : cols(rows), rows(cols) {
            mat = new T[rows * cols];
        }

        Matrix(Matrix &m const) : cols(m.rows), rows(m.cols) {
            mat = new T[rows * cols];
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < rows; j++) {
                    this->mat[_index(i, j)] = m.get(i, j); // verificare che sia performante, dato che è inline
                }
            }
        }


        ~Matrix() {
            delete mat;
        }

        inline T get(int row, int col) const {
            return mat[_index(row, col)];
        }

        inline const T &getR(int row, int col) const {
            return mat[_index(row, col)];
        }

        inline T *getP(int row, int col) {
            return &mat[_index(row, col)];
        }


        inline void set(int row, int col, const T &value) {
            mat[_index(row, col)] = value;
        }
    //
    //    inline void set(int row, int col, const T* value)
    //    {
    //        mat[_index(row, col)] = *value;
    //    }









    };


}

#endif //DISPLAYIMAGE_MATRIX_H
