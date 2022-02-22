#ifndef PIPPO
#define PIPPO
#endif

#ifdef PIPPO
If PIPPO is defined this part of text will be contained in the output file
#endif

#ifdef PLUTO
  If PLUTO is defined this part of text will be contained in the output file
#endif

#ifdef __cplusplus
    Only if C++ compiler
#endif
/* Works only with newer compilers */

#if __cplusplus == 201103L
      We are compiling with a C++ 11 compliant compiler !
#endif

#if __cplusplus != 201103L
#error We need a c++11 compliant compiler
#endif
