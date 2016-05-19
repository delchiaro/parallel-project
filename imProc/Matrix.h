//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_pmatRIX_H
#define DISPLAYIMAGE_pmatRIX_H


#include "imProc.h"
#include <opencv2/opencv.hpp>


namespace imProc {


    template<class T> class Matrix
    {
    private:
        int _rows;
        int _cols;
        T *_pmat;


        void _empty() {
            _rows = _cols = 0;
            _pmat = nullptr;
        }

    protected:
        inline int _index(int row, int col) const {
            return col + row * this->_cols;
        }

    public:
        Matrix(int rows, int cols) : _rows(rows), _cols(cols) {
            _pmat = new T[rows * cols];
        }
        Matrix(int rows, int cols, const T& initVal) : Matrix(rows, cols) {
            for(int i = 0; i < _cols*_rows; i++) _pmat[i] = initVal;
        }

        /**
            Deep copy of rawMatrix, with dimension (rows, cols).

            @param digit the single digit to encode.
            @return a bar code of the digit using "|" as the long
            bar and "," as the half bar.
        */
        Matrix(T* rawMatrix, int rows, int cols)  : Matrix(rows, cols) {
            for (int i = 0; i < _rows*_cols; i++)
                this->_pmat[i] = rawMatrix[i];
        }



        // Copy, Assignment, Move
        Matrix(const Matrix<T>& m) : Matrix(m._rows, m._cols ){
            for (int i = 0; i < _rows*_cols; i++)
                this->_pmat[i] = m._pmat[i];
        }
        Matrix& operator= (const Matrix &m) {
            if(this->_pmat != nullptr)
                delete[] _pmat;
            _rows = m._rows;
            _cols = m._cols;
            _pmat = new T[_rows * _cols];
            for (int i = 0; i < _rows*_cols; i++)
                this->_pmat[i] = m._pmat[i];
            return *this;
        }
        Matrix(Matrix<T>&& m) : _rows(m._rows), _cols(m._cols), _pmat(m._pmat) {
            m._empty();

        }
        Matrix& operator= (Matrix&& m) {
            if(_pmat != nullptr)
                delete[] _pmat;
            _rows = m._rows;
            _cols = m._cols;
            _pmat = m._pmat;
             m._empty();
            return *this;
        }

        ~Matrix() {
            if( _pmat != nullptr)
                delete[] _pmat;
        }






        inline const int& rows() const { return this->_rows; }
        inline const int& cols() const { return this->_cols; }


        // ~ ~ GETTERS ~ ~
        inline const T& get (int row, int col)  const { return _pmat[_index(row, col)]; }
        inline       T  getV(int row, int col)  const { return _pmat[_index(row, col)]; }
        inline const T& get (int index)         const { return _pmat[index]; }
        inline       T  getV(int index)         const { return _pmat[index]; }
        inline       T* getRaw() const  { return _pmat; } // pointer to const data (immutable - read only)



        // ~ ~ SETTERS ~ ~
        inline void set(int row, int col, const T& value)   { _pmat[_index(row, col)] = value; }
        inline void set(int index, const T& value)          { _pmat[index] = value; }



        void immerge(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            this->operator=( makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue) );
        }

        Matrix<T> makeImmersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue)
        {
            int newRows = _rows + topPadding + bottomPadding;
            int newCols = _cols + rightPadding + leftPadding;
            Matrix<T> m(newRows, newCols, borderValue);

             for(int i = 0 ; i < _rows; i++)
            {
                // TODO: optimize -> for each row copy all columns from leftPadding to _cols-rightPadding
                for( int j=0 ; j < _cols ; j++)
                {
                    m.set(i+topPadding,j+rightPadding, this->get(i,j) );
                }
            }

            return m;
        }



        cv::Mat toMatCV(bool deepCopy = false) const
        {
            if(deepCopy)
            {
                cv::Mat ret(rows(), cols(), CV_8U, 0, 0);
                for(int i = 0; i < rows() ; i++)
                    for(int j = 0; j < cols() ; j++)
                        // TODO: debug.. error at runtime
                        *(ret.row(i).col(j).data) = this->get(i,j);

                return ret;
            }
            else return cv::Mat( rows(), cols(), CV_8U, (void*) getRaw() , 0);
            // NB: We hope in Return Value Optimization
        }




    };


}

#endif //DISPLAYIMAGE_pmatRIX_H


//        bool _pmat_created = false; // true if _pmat is created dinamically with "new"


// Matrix() /*: _rows(0), _cols(0), _pmat(nullptr), _pmat_created(false) */ { _empty(); }


//        /**
//           Shallow copy of rawMatrix, with dimension (rows, cols).
//
//           @param manageShallowCopyPTR if true, this class will manage the destruction of the rawMatrix.
//        */
//        Matrix(T* rawMatrix, int rows, int cols, bool manageShallowCopyPTR)
//                : _rows(rows), _cols(cols), _pmat(rawMatrix), _pmat_created(manageShallowCopyPTR) {}
