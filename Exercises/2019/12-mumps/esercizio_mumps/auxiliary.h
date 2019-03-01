/*
Copyright (C) 2006-2011 Evgenii Rudnyi, http://MatrixProgramming.com
http://Evgenii.Rudnyi.Ru/

This software is a copyrighted work licensed under the terms, described
in the file "FREE_LICENSE". 
*/

#ifndef AUXILIARY
#define AUXILIARY

#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
	
using namespace std;

/// Transforms any object to a string.
template <class T>
string ObjToString(const T &x)
{
  ostringstream out;
  out << x;
  return out.str();
}

inline string stringAndNumber(const string &str1, const double &x)
{
	return str1 + string(" ") + ObjToString(x);
}

inline string stringAndTwoNumbers(const string &str1, const double &x, const double &y)
{
	return str1 + string(" ") + ObjToString(x) + string(" ") + ObjToString(y);
}

/// Base object for error handling
struct BaseError : public string 
{
	BaseError(const string &str1) 
	{
		append("ERROR: ").append(str1).append(".");
		cout << *this << endl;
	}
	BaseError(const string &str1, const string &str2) 
	{
		append("ERROR: ").append(str1).append(": ").append(str2).append(".");
		cout << *this << endl;
	}
};

inline void warning(const string &str)
{
	cout << "WARNING: " << str << "." << endl;
}

inline void information(const string &str)
{
	cout << str << "." << endl;
}

/// Simple timing
class Timing
{
	double st;
	double gettime()
	{
		return clock();
	}
public:
	Timing() 
	{
		st = gettime(); 
	}
	double time() {return (gettime()- st)/CLOCKS_PER_SEC;}
	void write(const string &mes)
	{
		information(string("TIME: ") + mes + stringAndNumber(" is done for", time()) + string(" s"));
	}
};

#endif
