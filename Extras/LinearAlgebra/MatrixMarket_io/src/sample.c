/*
Fri Aug 15 16:29:47 EDT 1997

                       ***   SAMPLE USAGE    ***
                    For Harwell I/O package iohb.c.

           National Institute of Standards and Technology, MD.
                             K.A. Remington

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                NOTICE

 Permission to use, copy, modify, and distribute this software and
 its documentation for any purpose and without fee is hereby granted
 provided that the above copyright notice appear in all copies and
 that both the copyright notice and this permission notice appear in
 supporting documentation.

 Neither the Author nor the Institution (National Institute of Standards
 and Technology) make any representations about the suitability of this 
 software for any purpose. This software is provided "as is" without 
 expressed or implied warranty.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#include<stdlib.h>
#include<stdio.h>
#include "iohb.h"

void main(int argc, char *argv[])
{
   int M, N, nonzeros, Nrhs;
   int *colptr, *rowind;
   double *val;
   double *rhs;
   int rhsentries;
   double *guess;
   double *exact;
   char *Type;
   char Ptrfmt[]="(13I6)";
   char Indfmt[]="(16I5)";
   char Valfmt[]="(3E26.18)";
   char Rhsfmt[]="(3E26.18)";
   int i=0;

   if (argc < 3)
   {
     fprintf(stderr,"Usage: %s HBfile HBfileout\n", argv[0]);
     exit(-1);
   }

/* Get information about the array stored in the file specified in the  */
/* argument list:                                                       */

   fprintf(stderr,"Reading matrix info from %s...\n",argv[1]);
   readHB_info(argv[1], &M, &N, &nonzeros, &Type, &Nrhs);

   fprintf(stderr,"Matrix in file %s is %d x %d, with %d nonzeros with type %s;\n",
                         argv[1], M, N, nonzeros, Type);
   fprintf(stderr,"%d right-hand-side(s) available.\n",Nrhs);

/* Read the matrix information, generating the associated storage arrays  */ 
   fprintf(stderr,"Reading the matrix from %s...\n",argv[1]);
   readHB_newmat_double(argv[1], &M, &N, &nonzeros, &colptr, &rowind, &val);
   
/* If a rhs is specified in the file, read one, generating the associate storage */
   if (Nrhs > 0) {
      fprintf(stderr,"Reading right-hand-side vector(s) from %s...\n",argv[1]);
      readHB_newaux_double(argv[1], 'F', &rhs);
   }
 
/* Generate a new Guess vector (all zeros) */

  if ( Type[0] == 'R' ) rhsentries = M*Nrhs;
  else if ( Type[0] == 'C' ) rhsentries = 2*M*Nrhs;
  guess = (double *) malloc(rhsentries*sizeof(double));
  for (i=0;i<rhsentries;i++) guess[i] = 0;
  if (Nrhs > 0) {
  fprintf(stderr,"Writing the matrix and right-hand-side/guess vector(s) from %s...\n",argv[1]);
  } else {
  fprintf(stderr,"Writing the matrix from %s...\n",argv[1]);
  }
  writeHB_mat_double(argv[2], M, N, nonzeros, colptr, rowind, val, Nrhs, rhs, 
           guess, exact, "Test Title", "Test Key", Type, Ptrfmt,Indfmt,Valfmt,Rhsfmt,"FGN");

  fprintf(stderr,"**** Successful completion of %s. Generated %s. ****\n",argv[0],argv[2]);

}
