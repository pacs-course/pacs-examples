#ifdef STUDENT_VERSION_2
#  include "Student_v2.hpp"
#else
#  include "Student_v1.hpp"
#endif

int
main(int argc, char **argv)
{
#ifdef STUDENT_VERSION_2
  Student s("Joe", 25);
#else
  Student s("Joe");
#endif

  s.display();

  return 0;
}
