//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_MATRIX_H
#define DISPLAYIMAGE_MATRIX_H


#include "imProc.h"



namespace imProc {


    template<class T> class Matrix
    {
    private:
        const int _rows;
        const int _cols;
        T *_mat;

    protected:
        inline int _index(int row, int col) const {
            return col + row * this->_cols;
        }


    public:

        Matrix(int rows, int cols) : _rows(rows), _cols(cols) {
            _mat = new T[rows * cols];
        }
        Matrix(int rows, int cols, const T& initVal) : Matrix(rows, cols) {
            for(int i = 0; i < _cols*_rows; i++) _mat[i] = initVal;
        }
        Matrix(const Matrix<T>& m) : _rows(m._rows), _cols(m._cols) {
            this->_mat = new T[_rows * _cols];
            for (int i = 0; i < _rows*_cols; i++) {
                    this->_mat[i] = m._mat[i];
            }
        }

        ~Matrix() {
            delete _mat;
        }


        Matrix<T>* clone() const { return new Matrix<T>(*this); }


        inline const int& rows() const { return this->_rows; }
        inline const int& cols() const { return this->_cols; }


        inline const T* getRawMatrix() const        { return _mat; }

        inline const T& get (int row, int col) const { return _mat[_index(row, col)]; }
        inline       T  getV(int row, int col) const { return _mat[_index(row, col)]; }
        inline       T* getP(int row, int col)       { return &_mat[_index(row, col)]; }


        inline void set(int row, int col, const T &value) { _mat[_index(row, col)] = value; }
        inline void set(int row, int col, const T* value) { _mat[_index(row, col)] = *value; }


    };


}

#endif //DISPLAYIMAGE_MATRIX_H
