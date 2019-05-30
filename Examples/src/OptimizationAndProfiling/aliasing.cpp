#include <vector>
#include<iostream>
typedef std::vector<double> Vet;
//! Sums to R the convolution of A and B
//! Does not work if you alias the last two arguments
void covol(Vet const & A, Vet const & B , Vet & R)
{
  for (unsigned int i=0; i<A.size();++i)
    for(unsigned int k=B.size()-1; k!=0;--k)
      R[i]+=A[i]*B[k];
}
//! Sums to R the convolution of A and B
//! This works correctly even if you alias the fist and last args
/*!
  @note. This is just an example of the effect of argument aliasing
  Indeed, this function still is not giving the correct results if
  you alias the second and third argument. If you really want a alias
  proof function you have to pass by value!
 */
void covol2(Vet const & A, Vet const & B , Vet & R)
{
  for (unsigned int i=0; i<A.size();++i){
    double tmp=A[i];
    for(unsigned int k=B.size()-1; k!=0;-- k)
      R[i]+=tmp*B[k];
  }
}
//! Sums to R the convolution of A and B
//! This version is argument alias proof! But uses more memory
/*!
  @note I could have not returned the new R with the return statement.
  I just wanted to maintain the same signature as the previous functions.
 */
void covol3(Vet const  A, Vet const  B , Vet & R)
{
  Vet Rt=R; // copy content of R
  for (unsigned int i=0; i<A.size();++i){
    double tmp=A[i];
    for(unsigned int k=B.size()-1; k!=0;-- k)
      Rt[i]+=tmp*B[k];
  }
  R=Rt; // return the result
}



int main()
{
  Vet B(100,1.0);
  Vet A(100,10.0);
  Vet R=A;
  covol(R,B,R);
  std::cout<<R[50]<<std::endl;
  R=A;
  covol2(R,B,R);
  std::cout<<R[50]<<std::endl;
  R=A;
  covol3(R,B,R);
  std::cout<<R[50]<<std::endl;

}
