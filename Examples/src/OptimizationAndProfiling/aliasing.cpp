#include <vector>
#include<iostream>
typedef std::vector<double> Vet;
void covol(Vet const & A, Vet const & B , Vet & R)
{
  for (unsigned int i=0; i<A.size();++i)
    for(unsigned int k=B.size()-1; k!=0;--k)
      R[i]+=A[i]*B[k];
}
typedef std::vector<double> Vet;
void covol2(Vet const & A, Vet const & B , Vet & R)
{
  for (unsigned int i=0; i<A.size();++i){
    double tmp=A[i];
    for(unsigned int k=B.size()-1; k!=0;-- k)
      R[i]+=tmp*B[k];
  }
}
int main()
{
  Vet A(100,1.0);
  Vet B(100,10.0);
  Vet R(100,10.0);
  covol(A,R,R);
  std::cout<<R[50]<<std::endl;
  A.resize(100,1.0);
  R.resize(100,0.0);
  covol2(A,R,R);
  std::cout<<R[50]<<std::endl;
}
