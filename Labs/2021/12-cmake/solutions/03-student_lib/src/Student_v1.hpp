#include <string>

class Student
{
public:
  Student(const std::string &);
  void
  display();

private:
  std::string name;
};
