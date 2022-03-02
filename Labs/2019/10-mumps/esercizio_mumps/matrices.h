/*
Copyright (C) 2006-2011 Evgenii Rudnyi, http://MatrixProgramming.com
http://Evgenii.Rudnyi.Ru/

This software is a copyrighted work licensed under the terms, described
in the file "FREE_LICENSE". 
*/

#ifndef MATRICES
#define MATRICES

#include <map>
#include <cmath>
#include <algorithm>
#include "auxiliary.h"

namespace MatrixError {

struct MatrixError : public BaseError
{
	MatrixError(const string &str1, const string &str2) : BaseError(string("MatrixError::") + str1, str2) {}
};

struct ReadError : public MatrixError
{
	ReadError(const string &str) : MatrixError("ReadError", str) {}
};

struct WriteError : public MatrixError
{
	WriteError(const string &str) : MatrixError("WriteError", str) {}
};

} //end namespace MatrixError

/// Dense matrix
class Matrix : public vector<double>
{
  size_t m;
  size_t n;

public:
	Matrix() : m(0), n(0) {}
  Matrix(size_t m_, size_t n_) : vector<double>(m_*n_), m(m_), n(n_) {}
  Matrix(size_t m_, size_t n_, double val) : vector<double>(m_*n_, val), m(m_), n(n_) {}

  void resize(size_t m_, size_t n_)
    {m = m_; n = n_; vector<double>::resize(m_*n_);}
  void reserve(size_t m_, size_t n_)
    {vector<double>::reserve(m_*n_);}
  void clear()
    {m = n = 0; vector<double>::clear();}

  size_t NRows() const {return m;}
  size_t NCols() const {return n;}

	double* column(size_t j)
	{
		return &*(begin() + j*m);
	}	
	size_t nnz()
	{
		return size() - count(begin(), end(), 0.);
	}
	size_t nnz(size_t j)
	{
		return NRows() - count(column(j), column(j) + NRows(), 0.);
	}
  double& operator()(size_t i, size_t j)
  {
    return operator[](i + j*m);
  }
  const double& operator()(size_t i, size_t j) const
  {
    return operator[](i + j*m);
  }
	void swap(Matrix &y)
	{
		vector<double>::swap(y);
		std::swap(n, y.n);
		std::swap(m, y.m);
	}
	void clearMemory()
	{
		Matrix empty;
	  swap(empty);
	}
	void read(istream &in);
	void readData(istream &in, size_t n, size_t m, bool IsSymmetric);
	void readDataSparse(istream &in, size_t nnz, bool IsSymmetric);
	void write(ostream &out);
};

/// Sparse matrix, support general or symmetric storage
class SparseMatrix : public vector< map<int, double> >
{
	void init()
	{
		nnz = 0;
		IsSymmetric = false;
		m = 0;
	}
public:	
	size_t nnz;
//m may be not consistent	
	size_t m;
	bool IsSymmetric;
	
	SparseMatrix() {init();}
  explicit SparseMatrix(size_t n_) : vector< map<int, double> >(n_) {init();}
	void clear()
	{
		vector< map<int, double> >::clear();
		init();
	}

	void setProperties();
	void makeNotSymmetric();

  size_t NRows() const {return size();}
  size_t NCols() const {return m;}
	
	void swap(SparseMatrix &y)
	{
		vector< map<int, double> >::swap(y);
		std::swap(m, y.m);
		std::swap(nnz, y.nnz);
		std::swap(IsSymmetric, y.IsSymmetric);
	}
	void clearMemory()
	{
		SparseMatrix empty;
	  swap(empty);
	}

	void read(istream &in);
	void readData(istream &in, size_t nnz);
	void write(ostream &out);
};

#endif
