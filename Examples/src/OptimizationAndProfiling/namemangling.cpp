// Just a declaration
double aFunction(double const &);

float
pippo(float a)
{
  return a * a;
}

double
pippo(double a)
{
  return a * aFunction(a);
}
