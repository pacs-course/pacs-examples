#include <string>

class Student
{
public:
  Student(const std::string &, const unsigned int &);
  void
  display();

private:
  std::string  name;
  unsigned int age;
};
