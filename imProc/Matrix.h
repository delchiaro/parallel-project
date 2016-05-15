//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_pmatRIX_H
#define DISPLAYIMAGE_pmatRIX_H


#include "imProc.h"



namespace imProc {


    template<class T> class Matrix
    {
    private:
        const int _rows;
        const int _cols;

        T *_pmat;
        // _pmat_created: true if _pmat is created dinamically with "new" (in this case destructor will delete _pmat)
        bool _pmat_created = false;



    protected:
        inline int _index(int row, int col) const {
            return col + row * this->_cols;
        }

    public:

        Matrix(int rows, int cols) : _rows(rows), _cols(cols) {
            _pmat = new T[rows * cols];
            _pmat_created = true;
        }
        Matrix(int rows, int cols, const T& initVal) : Matrix(rows, cols) {
            for(int i = 0; i < _cols*_rows; i++) _pmat[i] = initVal;
        }
        Matrix(T* rawMatrix, int rows, int cols)  : Matrix(rows, cols) {
            for (int i = 0; i < _rows*_cols; i++)
                this->_pmat[i] = rawMatrix[i];
        }

        // not safe:
        Matrix(T* rawMatrix, int rows, int cols, bool shallowCopy) : _rows(rows), _cols(cols), _pmat(rawMatrix) {

        }

        ~Matrix() {
            if( this->_pmat_created == true)
                delete _pmat;
        }


        // Copy Constructor
        Matrix(const Matrix<T>& m) : Matrix(m._rows, m._cols ){ operator=(m); }

        // not safe:
        Matrix(const Matrix<T>& m, bool shallowCopy ) :_rows(m._rows), _cols(m._cols), _pmat(m._pmat){
//            this->_pmat_created = true;
//            m._pmat_created = false;
        }

        Matrix& operator= (const Matrix &m)
        {
            for (int i = 0; i < _rows*_cols; i++)
                this->_pmat[i] = m._pmat[i];
            return *this;
        }
        Matrix<T> clone() const { return Matrix<T>(*this); }


        inline const int& rows() const { return this->_rows; }
        inline const int& cols() const { return this->_cols; }


        // ~ ~ GETTERS ~ ~
        //2D:
        inline const T& get (int row, int col) const { return _pmat[_index(row, col)]; }
        inline       T  getV(int row, int col) const { return _pmat[_index(row, col)]; }
        //1D:
        inline const T& get (int index) const { return _pmat[index]; }
        inline       T  getV(int index) const { return _pmat[index]; }
        //RAW:
        inline T* getRaw() const  { return _pmat; } // pointer to const data (immutable - read only)



        // ~ ~ SETTERS ~ ~
        //2D:
        inline void set(int row, int col, const T& value) { _pmat[_index(row, col)] = value; }
        //1D:
        inline void set(int index, const T& value) { _pmat[index] = value; }






        inline  cv::Mat toMatCV() const {
            return cv::Mat( rows(), cols(), CV_8U, (void*) getRaw() , 0);
            // NB: We hope in Return Value Optimization (see wikipedia)
        }

        Matrix<T> immersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue)
        {
            int newRows = _rows + topPadding + bottomPadding;
            int newCols = _cols + rightPadding + leftPadding;
            Matrix<T> m(newRows, newCols, borderValue);

            for(int i = 0 ; i < _rows; i++)
            {
                for( int j=0 ; j < _cols ; j++)
                {
                    m.set(i+topPadding,j+rightPadding, this->get(i,j) );
                }
            }

            return m;
        }

    };


}

#endif //DISPLAYIMAGE_pmatRIX_H
