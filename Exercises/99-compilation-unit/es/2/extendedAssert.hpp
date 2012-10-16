/*---------------------------------------------------------------------*
| header file  extendedAssert                                      |
| #Purposes Defines typedefs and macros common to ALL lifeV.h software |
|           it must be includes in all translation units.              |
| THIS IS A REDUCED VERSION FOR EDUCATIONAL PURPOSES ONLY              |
| Copyright Luca Formaggia 2005                                        |

  Questo file definisce delle macro che funzionano in modo simile ad
  assert ed in piu` controllano la keyword inline

  Quando le macro sono attivare hanno il comportamento seguente
  
        INLINE          -> inline
        SURE_ASSERT(cond,"mesg") -> se cond e` false stampa "mesg" e
	                              abortisce
        ASSERTM(cond,"mesg") come SURE_ASSERT
        ASSERT(cond)  come ASSERTM ma senza messaggio

 Variabili del preprocessore

  NOINLINE    INLINE e` disattivata
  NOASSERT    ASSERTM e` disattivata
  NDEBUG      come NOINLINE + NOASSERT

*----------------------------------------------------------------------*/
# include<iostream>
# ifndef __cplusplus
# error You must use C++ 
# endif

# ifndef _EXTENDEDASSERT_HH_
# define _EXTENDEDASSERT_HH_

#ifdef NDEBUG
#undef NOINLINE
#define NOASSERT
#endif

#ifdef NOINLINE
#define INLINE
#else
#define INLINE inline
#endif


//
# define SURE_ASSERT(X,A) if ( !(X) ) \
    do{ std::cerr << std::endl << A<<std::endl << " In file " << __FILE__ \
		  << " line " << __LINE__<<" : "; abort() ; } while (0) ;


#ifdef NOASSERT
#define ASSERTM(X,A)
#else
#define ASSERTM(X,A)  SURE_ASSERT(X,A)
#endif

#define ASSERT(X) ASSERTM(X," ")

//  end of  _EXTENDEDASSERT_HH_
#endif
