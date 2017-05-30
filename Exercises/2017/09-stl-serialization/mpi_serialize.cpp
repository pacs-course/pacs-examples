#include "serialize.h"
#include <iostream>
#include <mpi.h>

int
main (int argc, char **argv)
{

  MPI_Init (&argc, &argv);
  int size, rank;
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  
  // write and read back an std::vector<double>
  if (rank == 0)
    {
      std::vector<double> a(10, 79.1234);  
      std::vector<unsigned char> sa = serialize::write (a);
      std::cout << "sa = " << std::endl;

      for (int ii = 0; ii < sa.size (); ++ii)
        std::cout << sa[ii];
      std::cout << std::endl;

      int tmp = sa.size ();

      MPI_Send (&tmp, 1,
                MPI_INT, 1,
                1974, MPI_COMM_WORLD);
      
      MPI_Send (&(sa[0]), sa.size (),
                MPI_CHAR, 1,
                1974, MPI_COMM_WORLD); 
    }
  else if (rank == 1)
    {
      MPI_Status status;
      int tmp = 0;
      MPI_Recv (&tmp, 1,
                MPI_INT, 0,
                1974, MPI_COMM_WORLD, &status);

      std::vector<unsigned char> sa (tmp);
        
      MPI_Recv (&(sa[0]), sa.size (),
                MPI_CHAR, 0,
                1974, MPI_COMM_WORLD, &status); 

      
      std::vector<double> b;
      serialize::read (sa, b);
  
      std::cout << "b = " << std::endl;
      for (int ii = 0; ii < b.size (); ++ii)
        std::cout << b[ii] <<  std::endl;
    }

  MPI_Barrier (MPI_COMM_WORLD);

  // // write and read back an std::map<int, double>
  if (rank == 0)
    {
      std::map<int, double> c; c[0] = 1.; c[12] = 3.14;
      std::vector<unsigned char> sc = serialize::write (c);

      std::cout << "sc = " << std::endl;
      for (int ii = 0; ii < sc.size (); ++ii)
        std::cout << sc[ii];
      std::cout << std::endl;

      std::cout << "c = " << std::endl;
      for (auto ii = c.begin (); ii != c.end (); ++ii)
        std::cout << ii->first << " " << ii->second << std::endl;
      std::cout << std::endl;

      int tmp = sc.size ();
            
      MPI_Send (&tmp, 1,
                MPI_INT, 1,
                1974, MPI_COMM_WORLD);
      
      MPI_Send (&(sc[0]), sc.size (),
                MPI_CHAR, 1,
                1974, MPI_COMM_WORLD); 
    }
  else if (rank == 1)
    {

      MPI_Status status;
      int tmp = 0;
      MPI_Recv (&tmp, 1,
                MPI_INT, 0,
                1974, MPI_COMM_WORLD, &status);

      std::vector<unsigned char> sc(tmp);

      MPI_Recv (&(sc[0]), sc.size (),
                MPI_CHAR, 0,
                1974, MPI_COMM_WORLD, &status); 

      std::map<int, double> d;
      serialize::read (sc, d);
      std::cout << "d = " << std::endl;
      for (auto ii = d.begin (); ii != d.end (); ++ii)
        std::cout << ii->first << " " << ii->second << std::endl;
      std::cout << std::endl;  
    }

  MPI_Barrier (MPI_COMM_WORLD);

  // write and read back an std::vector<std::map<int, double>>
  if (rank == 0)
    {
      std::vector<std::map<int, double>> vm (5); vm[3][2] = 3.14; vm[4][8] = 31.4;
      std::vector<unsigned char> svm = serialize::write (vm);

      std::cout << "vm = " << std::endl;
      for (int ii = 0; ii < vm.size (); ++ii)
        {
          std::cout << "map #" << ii << std::endl;
          for (auto jj = vm[ii].begin (); jj != vm[ii].end (); ++jj)
            std::cout << jj->first << " " << jj->second << std::endl;
        }
      std::cout  << std::endl;

      std::cout << "svm = " << std::endl;
      for (int ii = 0; ii < svm.size (); ++ii)
        std::cout << svm[ii];
      std::cout << std::endl;

      int tmp = svm.size ();
            
      MPI_Send (&tmp, 1,
                MPI_INT, 1,
                1974, MPI_COMM_WORLD);
      
      MPI_Send (&(svm[0]), svm.size (),
                MPI_CHAR, 1,
                1974, MPI_COMM_WORLD); 

    }
  else if (rank == 1)
    {
      MPI_Status status;
      int tmp = 0;
      MPI_Recv (&tmp, 1,
                MPI_INT, 0,
                1974, MPI_COMM_WORLD, &status);

      std::vector<unsigned char> svm (tmp);

      MPI_Recv (&(svm[0]), svm.size (),
                MPI_CHAR, 0,
                1974, MPI_COMM_WORLD, &status); 
      
      std::vector<std::map<int, double>> e;
      serialize::read (svm, e);
              
      std::cout << "e = " << std::endl;
      for (int ii = 0; ii < e.size (); ++ii)
        {
          std::cout << "map #" << ii << std::endl;
          for (auto jj = e[ii].begin (); jj != e[ii].end (); ++jj)
            std::cout << jj->first << " " << jj->second << std::endl;
        }
    }

  MPI_Barrier (MPI_COMM_WORLD);
  MPI_Finalize ();
  return 0;
}
