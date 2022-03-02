/*
Copyright (C) 2006-2011 Evgenii Rudnyi, http://MatrixProgramming.com
http://Evgenii.Rudnyi.Ru/

This software is a copyrighted work licensed under the terms, described
in the file "FREE_LICENSE". 
*/

#include <iostream>
#include <iomanip>
#include <set>
#include "matrices.h"

// global functions for Matrix Market

bool isDefined(const set<string> &opt, const string &name)
{
	if (opt.find(name) == opt.end())
		return false;
	else
		return true;	
}

void readHeader(istream &in, set<string> &opt)
{
	string line;
	getline(in, line);
	if (line.substr(0, 15) != "%%MatrixMarket ")
		throw MatrixError::ReadError("header: Not a Matrix Market file");
	istringstream in2(line.substr(15));
	string word;
	while (in2)
	{
		in2 >> word;
		for (int i = 0; i < word.size(); ++i)
			word[i] = tolower(word[i]);
		opt.insert(word);
	}
	while (in.peek() == '%')
		getline(in, line);
	if (!isDefined(opt, "matrix"))
		throw MatrixError::ReadError("header: Not a matrix");
	if (!isDefined(opt, "real"))
		throw MatrixError::ReadError("header: Not a real matrix");
}

void writeComment(ostream &out)
{
	out << "% File is created http://MatrixProgramming.com/" << endl;
}

// member functions
void Matrix::read(istream &in)
{
	clear();
	set<string> opt;
	bool IsSymmetric;	
	readHeader(in, opt);
	if (isDefined(opt, "symmetric"))
	{
		IsSymmetric = true;
	}
	else if (isDefined(opt, "general"))
	{
		IsSymmetric = false;
	}
	else
		throw MatrixError::ReadError("Matrix: Not general, neither symmetric");
	size_t n, m, nnz;
	in >> n >> m;
	if (IsSymmetric && (m != n))
		throw MatrixError::ReadError(stringAndTwoNumbers("Matrix: Symmetric but number of columns is not equal to number of rows", n, m));
	resize(n, m);

	if (isDefined(opt, "array"))
	{
		readData(in, n, m, IsSymmetric);
	}
	else if (isDefined(opt, "coordinate"))
	{
		in >> nnz;
		readDataSparse(in, nnz, IsSymmetric);
	}
	else
		throw MatrixError::ReadError("Matrix: Not array, neither coordinate");
}

void Matrix::readData(istream &in, size_t n, size_t m, bool IsSymmetric)
{
	double val;
	if (IsSymmetric)
	{
		for (int j = 0; j < n; ++j)
			for (int i = j; i < n; ++i)
			{
				in >> val;
				if (!in)
					throw MatrixError::ReadError("Matrix: Symmetric and end of file too early");
				(*this)(j, i) = (*this)(i, j) = val;
			}			
	}
	else
		for (int j = 0; j < m; ++j)
			for (int i = 0; i < n; ++i)
			{
				in >> val;
				if (!in)
					throw MatrixError::ReadError("Matrix: End of file too early");
				(*this)(i, j) = val;
			}
}

void Matrix::readDataSparse(istream &in, size_t nnz, bool IsSymmetric)
{
	double val;
	size_t i, j;
	fill(begin(), end(), 0.);
	for (int k = 0; k < nnz; ++k)
	{
		in >> i >> j >> val;
		if (!in)
				throw MatrixError::ReadError("Matrix: End of file too early");
		(*this)(i - 1, j - 1) = val;
		if (IsSymmetric)
			(*this)(j - 1, i - 1) = val;
	}		
}

void Matrix::write(ostream &out)
{
	out << "%%MatrixMarket matrix array real general" << endl;
	writeComment(out);
	out << NRows() << " " << NCols() << endl;
	for (vector<double>::const_iterator i = begin(); i < end(); ++i)
		out << *i << endl;
}

void SparseMatrix::setProperties()
{
	nnz = 0;
	for (int i = 0; i < size(); ++i)
		nnz += (*this)[i].size();
}

void SparseMatrix::makeNotSymmetric()
{
	if (IsSymmetric)
	{
		for (int i = 0; i < size(); ++i)
			for (map<int, double>::iterator j = (*this)[i].begin(); j != (*this)[i].end(); ++j)
				if ((*j).first > i)
					(*this)[(*j).first][i] = (*j).second;
		IsSymmetric = false;
		setProperties();
	}
}

void SparseMatrix::read(istream &in)
{
	clear();
	set<string> opt;	
	readHeader(in, opt);
	if (!isDefined(opt, "coordinate"))
		throw MatrixError::ReadError("SparseMatrix: Not coordinate");
	bool sym;
	if (isDefined(opt, "general"))
		sym = false;
	else if (isDefined(opt, "symmetric"))
		sym = true;
	else
		throw MatrixError::ReadError("SparseMatrix: neither general, nor symmetric");
	size_t n, mm, nnz;
	in >> n >> mm >> nnz;
	resize(n);
	if (sym && n != mm)
		throw MatrixError::ReadError(stringAndTwoNumbers("SparseMatrix: symmetric but number of rows is not equal to number of columns", n, mm));		
	IsSymmetric = sym;
	m = mm;
	readData(in, nnz);
}

void SparseMatrix::readData(istream &in, size_t nnzz)
{
  int i, j;
	double val;
	for (int ii = 0; ii < nnzz; ++ii)
	{
		in >> i >> j >> val;
		if (!in)
				throw MatrixError::ReadError("SparseMatrix: End of file too early");
		--i;
		--j;
		if (IsSymmetric)
		{
			if (j <= i)
			(*this)[j][i] = val;
		}
		else
			(*this)[i][j] = val;
	}
	setProperties();
	if (nnz != nnzz)
		warning(stringAndTwoNumbers("SparseMatrix: After reading the number of nonzeros is different from that in the file", nnz, nnzz));
}

void SparseMatrix::write(ostream &out)
{
	out << "%%MatrixMarket matrix coordinate real";
	if (IsSymmetric)
		out << " symmetric" << endl;
	else
		out << " general" << endl;
	writeComment(out);
	setProperties();
	out << NRows() << " " << NCols() << " " << nnz << endl;
	if (IsSymmetric && NRows() != NCols())
		throw MatrixError::WriteError(stringAndTwoNumbers("SparseMatrix: symmetric but number of rows is not equal to number of columns", NRows(), NCols()));		
	int ii, jj;
	double val;
	for (int i = 0; i < size(); ++i)
		for (map<int, double>::const_iterator j = (*this)[i].begin(); j != (*this)[i].end(); ++j)
		{
			if (IsSymmetric)
			{
				ii = (*j).first + 1;
				jj = i + 1;
			}
			else
			{
				jj = (*j).first + 1;
				ii = i + 1;
			}
			val = (*j).second;	
			out << ii << " " << jj << " " << val << endl;
		}
//	out << endl;
}

