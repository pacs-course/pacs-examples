template <typename T >
T const  Max(T const & a, T const &b)
{ return a<b?a:b;}


template <typename T >
T  const  Max(T * a, T * b)
{ return *a<*b?*a:*b;}

/* CANCELED (LF) Cannot return a reference to a temporary!
template <>
void *   const & Max(void * & a, void * &b)
{ void * p=0;
  return p;}
*/
int main(){
  double a=Max(3,4);
  double b(5);
  double d(6);
  double e=Max(&b,&d);
}
