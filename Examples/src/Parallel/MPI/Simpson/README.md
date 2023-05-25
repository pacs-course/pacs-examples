# Numerical integration with a parallel Simpson composite quatrature implementation.


This code illustrates

- a possible parallel implementation of ano of the most classic quadrature rules;
- the use of the `json.hpp` utility to read a (very simple) json file with some data;
- the use of `std::function<>`, the C++ function wrapper.

Composite Simpson rule compute the approximation of a definite integral by summing contributions coming by a subdivision of the original interval into n subintervals (often called elements). A MPI parallel implementation is rather straighforward: ech process computes the integral on a subset of the elements and, eventually, a reduction is made to collect the result. The algorithm is thus almost embarassingly parallel, so we expect a reasonable speed-up even on a ordinary multicore PC (until the number of processes exceeeds the number of cores).

In the json file we store the interval ends, `a` and `b`, and the number of interfal. For instance

```
{ 
	"n":1000000,
	"a":0.0,
	"b":1000.0
} 
```
The integrand is given in the `main()` program as a lambda expession. Not very general, but sufficient for this example, and it is conveniently wrapped into a 
C+++ function wrapper:

	  std::function<double (double const &)> f=[](double const & x){return std::sin(x)*x*x;};
	
json file is read thanks to the *Json for modern C++* utility that is provided in the `Utility` folder (remember to do `make install` in that folder):

      std::ifstream ifile("data.json"); //open file
      using json=nlohmann::json; // to make life easier
      json j1; // create a json object
      ifile >>j1;//load the file in the json object
      auto a = j1["a"].get<double>(); // get data from json object
      auto b = j1["b"].get<double>();
      auto n = j1["n"].get<unsigned int>();
 
 Alternatively, one may use the method `value()` that allows to provide a default:
 
      auto a = j1.value("a",0.0);
      auto b = j1.value("b"],10.0);
      auto n = j1.value("n",1000u);
 Remember that in this case the returned type is determind by the type of the default value, so if you use `auto` you need to take care! For instance if you do `auto a = j1.value("a",0);`, `a` will be an integer and not a double!!
 
 Another thing thet we whow in this code t=is the use of `std::tuple<>` to pack heterogenous variables in a single message. It is an alternative to
 `MPI_Pack`. We explain the main features. Here `a`, `b`, and `h` are doubles and `n` is an integer. I define (for all processes)
 
	std::tuple<double,double,double,unsigned int> databuf;
only the root process fills the buffer with the data to be broadcasted, using the `std::tie` C++ utility.

	if(my_rank==0)
	{
	double a,b,h;// here they are local variable
	int n;
	...
      databuf=std::tie(a,b,h,n);
	...
	}
	
Then we broadcast the tuple (as a buffer of bytes), relying on `sizeof` to get the size in bytes of the tuple.

	MPI_Bcast(&databuf, sizeof(databuf),MPI_BYTE,0,mpi_comm);
 and recover the data:
   
	auto [a,b,h,n]=databuf;
	
We use here structured binding because at this point of the code the variables have not been declared, and with structured binding we are going to create and initialize them. If the variables were already
declared we cannot use structure binding, but we can use `std::tie` again,
 
	std::tie(a,b,h,n)=databuf;
 
 **Note:** Remember that for this trick to work, the data packed in the tuple must be either plain old data or aggregates. More precisely, they should be [Trivially Copyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable). A tuple of trivially copyable elements is itself trivially copyable, so it can be trivially serialised and sent as a single buffer.
 
 
 The code is then compiled with `make` and then run as usual. For instance,
 
 ```
 mpirun -n 2 ./main_simpsonMP
 ```
 if you want 2 processes. 
 
#What do I learn here?
 
- A simple usage  of the json reader. Much, much more in [Json for modern C++](https://github.com/nlohmann/json)
- The use of `MPI_Reduce()` to collect partial sums
- The C++ function wrapper
- The use of a C++ `std::tuple<>` to pack heterogeneous data. 

 
 