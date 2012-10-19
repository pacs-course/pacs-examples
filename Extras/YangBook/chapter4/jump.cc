/* 
 * sample program illustrating the use of long jump (jump between functions) 
 */ 

#include <iostream>
#include <setjmp.h>


// The output of this program should be: 0 1 2 3

void f(jmp_buf envbuf) {
  std::cout << 1 << " ";

  // jump to the point of last call of setjmp() and set return status to 2
  longjmp(envbuf, 2);

  // never be executed
  std::cout << 5 << " ";
}

int main() {
  jmp_buf envbuf;           // jmp_buf is defined in <setjmp.h>
  bool first = true;

  // The first time program execution comes to this point, return status
  // is 0. Later a call longjmp(envbuf, 2) causes program execution to
  // come back to this point and return status is set to 2.

  int status = setjmp(envbuf);

  std::cout << status << " ";

  if (first) {
    first = false;
    f(envbuf);
  }

  std::cout << 3 << std::endl;
}

/*
// longjmp() causes program execution to resume at the point of last call
// to setjmp(). longjmp() must be called before the function that called
// setjmp() returns. Should not set "status to 0
// structure jmp_buf is defined in <setjmp.h>

void longjmp(jmp_buf envbuf, int status);

// setjmp(envbuf) saves the enviroment (stack) in "envbuf". It returns 0 if
// successful when it is first called. Later, a call to longjmp(envbuf, status)
// resets the stack as described in "envbuf" and causes the return value to
// be "status". The return value can be used to determine where the long
// jump came from.

int setjmp(jmp_buf envbuf);
*/
