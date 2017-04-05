/*
 * smallSMatrix.hpp
 *
 *  Created on: Oct 3, 2010
 *      Author: forma
 */

#ifndef SMALLMATRIX_HPP_
#define SMALLMATRIX_HPP_
#include <cmath> // abs for floating types
#include <cstdlib> // abs for int types
namespace LinearAlgebra{
template<typename T, unsigned int NROW, unsigned int NCOL>
class SMatrix{
private:
  typedef T value_type;
    typedef unsigned int size_type;
    static const size_type M_size=NROW*NCOL;
    T data[NROW*NCOL];
    int skip(size_type i,size_type j) const{
    	return j+i*NCOL;}; //returns offset
  public:
    SMatrix(){};
    SMatrix(T const & init){
    	for(unsigned int i=0;i<M_size;++i)data[i]=init;}
    SMatrix(SMatrix<T,NROW,NCOL> const & m){
    	for(unsigned int i=0;i<M_size;++i)data[i]=m.data[i];};
    SMatrix<T,NROW,NCOL> &operator=(SMatrix<T,NROW,NCOL> const & m){
    	if(this!=&m)for(size_type i=0;i<M_size;++i)data[i]=m.data[i];
    	return *this;
    }
    T* operator[](size_type i){return data+i*NCOL;}
    T const * operator[](size_type i)const {return data+i*NCOL;}
    T & operator ()(size_type i, size_type j){return data[skip(i,j)];}
    T const & operator ()(size_type i, size_type j) const {return data[skip(i,j)];}
    size_type nrow()const{ return NROW;};
    size_type ncol()const{ return NCOL;};
    SMatrix<T,NROW,1> operator *(SMatrix<T,NROW,1>const & v);
    void clear(){for (unsigned int i=0;i<NROW*NCOL;++i)data[i]=0.0;}
  //  *******************
    //! Computes the sum of absolute values of a row
    T sumAbsRow(size_type i)const;
    //! Lump by summing the rows
    void lump();
    //! Set to zero the rows
    /*! Sets to zero the row indicated by the mask (value !=0)
    * The diagonal is set to the value in the mask.
    * Used to impose Dirichlet bc
    * */
    void cleanRows(SMatrix<T,NROW,1> const & mask);
    //! Set to zero the rows and columns
     /*! Sets to zero the rows and columns indicated
      * by the mask (value !=0)
      * The diagonal is set to the given value. Return
      * The matrix with the columns. Used to impose Dirichlet bc
      */
    void reduceMatrix(SMatrix<T,NROW,1> const & mask,
    		SMatrix<T,NROW,NCOL> & colmatrix);
};

// Specialization for vectors
template<typename T, unsigned int NROW>
class SMatrix<T,NROW,1>{
  private:
	typedef T value_type;
	typedef unsigned int size_type;
	static const size_type M_size=NROW;
    T data[NROW];
  public:
    SMatrix(){};
    SMatrix(T const & init){
    	for(unsigned int i=0;i<M_size;++i)data[i]=init;}
    SMatrix(SMatrix<T,NROW,1> const & m){
    	for(unsigned int i=0;i<M_size;++i)data[i]=m.data[i];};
    SMatrix<T,NROW,1> &operator=(SMatrix<T,NROW,1> const & m){
    	if(this!=&m)for(size_type i=0;i<M_size;++i)data[i]=m.data[i];
    	return *this;
    }
    T & operator[](size_type i){return data[i];}
    T const & operator[](size_type i)const {return data[i];}
    T & operator ()(size_type i){return data[i];}
    T const & operator ()(size_type i) const {return data[i];}
    T & operator ()(size_type i,size_type j){return data[i];}
    T const & operator ()(size_type i,size_type j) const {return data[i];}
    size_type nrow()const{ return NROW;};
    size_type ncol()const{ return 1;};
    void clear(){for (unsigned int i=0;i<NROW;++i)data[i]=0.0;}
};

template<typename T, unsigned int NCOL>
class SMatrix<T,1,NCOL>{
  private:
	typedef T value_type;
	typedef unsigned int size_type;
	static const size_type M_size=NCOL;
    T data[NCOL];
  public:
    SMatrix(){};
    SMatrix(T const & init){
    	for(unsigned int i=0;i<M_size;++i)data[i]=init;}
    SMatrix(SMatrix<T,1,NCOL> const & m){
    	for(unsigned int i=0;i<M_size;++i)data[i]=m.data[i];};
    SMatrix<T,1,NCOL> &operator=(SMatrix<T,1,NCOL> const & m){
    	if(this!=&m)for(size_type i=0;i<M_size;++i)data[i]=m.data[i];
    	return *this;
    }
    T & operator[](size_type i){return data[i];}
    T const & operator[](size_type i)const {return data[i];}
    T & operator ()(size_type i){return data[i];}
    T const & operator ()(size_type i) const {return data[i];}
    T & operator ()(size_type i,size_type j){return data[j];}
    T const & operator ()(size_type i,size_type j) const {return data[j];}
    size_type nrow()const{ return 1;};
    size_type ncol()const{ return NCOL;};
    void clear(){for (size_type i=0;i<NCOL;++i)data[i]=0.0;}
};

//!        *****   IMPLEMENTATIONS     ***********
template<typename T, unsigned int NROW, unsigned int NCOL>
    SMatrix<T,NROW,1>
SMatrix<T,NROW,NCOL>::operator *(SMatrix<T,NROW,1>const & v){
	SMatrix<T,NROW,1> tmp;
	for (size_type i=0;i<NROW;++i){
		tmp[i]=v[0] * this->data(skip(i,0));
		for (size_type j=1;j<NCOL;++j)
			tmp[i]+=this->data(skip(i,j))*v[j];
	}
	return tmp;
}

template<typename T, unsigned int NROW, unsigned int NCOL>
T SMatrix<T,NROW,NCOL>::sumAbsRow(size_type i)const {
	T sum;
	for (size_type j=0;j<NCOL;++j)
		sum+=std::abs(this->data(skip(i,j)));
	return sum;
}

template<typename T, unsigned int NROW, unsigned int NCOL>
void SMatrix<T,NROW,NCOL>::lump(){
	SMatrix<T,NROW,1> sum(0);
	// Collect sum
	for (size_type i=0;i<NROW;++i)
		for (size_type j=0;i<NCOL;++i)sum[i]+=this->data[skip(i,j)];
	this->clear();
	for (size_type i=0;i<NROW;++i)this->data[skip(i,i)]=sum[i];
}

template<typename T, unsigned int NROW, unsigned int NCOL>
void SMatrix<T,NROW,NCOL>::reduceMatrix(SMatrix<T,NROW,1> const & mask,
		SMatrix<T,NROW,NCOL> & colmatrix){
	colmatrix.clear();
	for (unsigned int i=0;i<NROW;++i){
		if (mask[i]!=0){
		 for (unsigned int j=0;j<NROW;++j)
			 colmatrix(j,i)=this->data[skip(j,i)];
		}
	}
		for (unsigned int i=0;i<NROW;++i){
			for (unsigned int j=0;i<NCOL;++i){
				this->data[skip(j,i)]=0;
				this->data[skip(i,j)]=0;
			}
			this->data[skip(i,i)]=mask[i];
			colmatrix(i,i)=0.0;
		}
}
}
#endif /* SMALLMATRIX_HPP_ */
