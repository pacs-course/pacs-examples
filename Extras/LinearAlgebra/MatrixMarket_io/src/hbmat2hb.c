/*
Fri Aug 15 16:29:47 EDT 1997

                     ***   TESTING ROUTINE    ***
                      
                    Harwell-Boeing        Harwell-Boeing
                     matrix file    -->     matrix file
              ( real data transferred as character strings )

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
#include<string.h>
#include<stdio.h>
#include "iohb.h"
#include "mmio.h"
char* substr(const char* S, const int pos, const int len);
char* Makename(const char* Basename, const char* Vectype, const int Nrhs);

void main(int argc, char *argv[])
{
    FILE *in_file;
    FILE *out_file;
    char Title[73], Key[9], Rhstype[4];
    char Type[4];
    char Ptrfmt[17], Indfmt[17], Valfmt[21], Rhsfmt[21];
    int Ptrcrd, Indcrd, Valcrd, Rhscrd;
    int Ptrperline, Ptrwidth, Indperline, Indwidth;
    int Rhsperline, Rhswidth;
    int Rhsflag;           /* Indicates 'E','D', or 'F' float format */
    int nvecs;
    int Nrow, Ncol, Nnzero;
    int Nrhs;
    char *valc;
    char *tmp1;
    char *tmp2;
    char *Rhsc;
    char *Estc;
    char *Slnc;
    char *Rhsi;
    char *Valfmtc;
    char *Rhsfmtc;
    char format[30];
    char rformat[30];
    char cformat[30];
    char *RhsFilename;
    char *SlnFilename;
    char *EstFilename;
    int *colptr, *rowind;
    int i, vectorp, j, indxval, rowp1, colp1;
    MM_typecode matcode;

    if (argc != 3) {
      printf("Usage: %s HBfile_in HBfile_out\n", argv[0]);
      printf("Function: Testing routine for writeHB_mat_char.\n");
      printf("          Reads matrix and auxillary vectors (if present) from\n");
      printf("          HBfile_in and writes to HBfile_out. \n");
      exit(-1);
    } 

    in_file = fopen( argv[1], "r");
    if (in_file == NULL)
    {
       fprintf(stderr,"Error: Cannot open file: %s\n",argv[1]);
       exit(1);
    }

    readHB_header(in_file, Title, Key, Type, &Nrow, &Ncol, &Nnzero, &Nrhs,
                  Ptrfmt, Indfmt, Valfmt, Rhsfmt,
                  &Ptrcrd, &Indcrd, &Valcrd, &Rhscrd, Rhstype);
    fclose(in_file);
    readHB_newmat_char(argv[1], &Nrow, &Ncol, &Nnzero, &colptr, &rowind, &valc,
                       &Valfmtc);
    if ( Nrhs > 0 ) {
      nvecs = 1;
      if ( Rhstype[0] != 'F' ) { 
         fprintf(stderr,"Right-hand-side type %c not supported.\n",Rhstype[0]);
         exit(1);
       }
       if ( Rhstype[1] == 'G' ) nvecs++;
       if ( Rhstype[2] == 'X' ) nvecs++;
       readHB_newaux_char(argv[1], 'F', &Rhsc, &Rhsfmtc);
       if ( Rhstype[1] == 'G' ) {
           printf("Reading guess in file %s\n",argv[1]);
           readHB_newaux_char(argv[1], 'G', &Estc, &Rhsfmtc);
       } else {
           printf("Reading RHS in file %s to be used as guess\n",argv[1]);
           readHB_newaux_char(argv[1],'F',&Estc,&Rhsfmtc);
       }
       if ( Rhstype[2] == 'X' ) {
           printf("Reading exact solution in file %s\n",argv[1]);
           readHB_newaux_char(argv[1], 'X', &Slnc, &Rhsfmtc);
       } else {
           printf("Reading RHS in file %s to be used as exact solution\n",argv[1]);
           readHB_newaux_char(argv[1],'F',&Slnc,&Rhsfmtc);
       }
     }

    writeHB_mat_char(argv[2], Nrow, Ncol, Nnzero, colptr, rowind, valc, 
         Nrhs, Rhsc, Estc, Slnc, "Test Title", "Test Key", Type, 
         Ptrfmt, Indfmt, Valfmtc, Rhsfmtc, "FGX");

}

char * Makename(const char *Basename, const char* Vectype, const int Nrhs) {
  char *tmp;
  char *Filename;
  tmp = strchr(Basename,'.');
  if ( tmp == NULL ) { 
     printf(stderr,"Makename: Cannot create filename _%3s%d from %s\n",Vectype,Nrhs,Basename);
     exit(1);
  }
  tmp = substr(Basename,0,tmp-Basename);
  Filename = (char *) malloc((strlen(tmp)+10));
  strcpy(Filename,tmp);
  sprintf(&Filename[strlen(tmp)],"_%3s%d.mtx\0",Vectype,Nrhs);
  return Filename;
}

    
