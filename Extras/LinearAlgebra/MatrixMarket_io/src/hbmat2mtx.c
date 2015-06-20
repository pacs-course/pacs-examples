/*
Fri Aug 15 16:29:47 EDT 1997

                     ***   CONVERSION ROUTINE    ***
                      
                 Harwell-Boeing            Matrix Market 
                   assembled       --> sparse coordinate file
                    matrix           
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
#include<stdio.h>
#include "iohb.h"
#include "mmio.h"
char* substr(const char* S, int pos, int len);

void main(int argc, char *argv[])
{
    FILE *in_file;
    char Title[73], Key[9], Rhstype[4];
    char Type[4];
    char Ptrfmt[17], Indfmt[17], Valfmt[21], Rhsfmt[21];
    int Ptrcrd, Indcrd, Valcrd, Rhscrd;
    int Indperline, Indwidth;
    int Valperline, Valwidth, Valprec;
    int Valflag;           /* Indicates 'E','D', or 'F' float format */
    int Nrow, Ncol, Nnzero;
    int Nrhs;
    char *valc;
    char *Valfmtc;
    char *tmp1;
    char *tmp2;
    char format[30];
    char rformat[30];
    char cformat[30];
    int *colptr, *rowind;
    int i,j, indxval, rowp1, colp1;
    MM_typecode matcode;

    if (argc != 2) {
      printf("Usage: %s HBfile \n", argv[0]);
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
    ParseIfmt(Indfmt,&Indperline,&Indwidth);
    ParseRfmt(Valfmt,&Valperline,&Valwidth,&Valprec,&Valflag);
    sprintf(format,"%%%dd %%%dd \n",Indwidth,Indwidth);
    sprintf(rformat,"%%%dd %%%dd %%%ds\n",Indwidth,Indwidth,Valwidth);
    sprintf(cformat,"%%%dd %%%dd %%%ds %%%ds\n",Indwidth,Indwidth,Valwidth,Valwidth);


   mm_set_matrix(&matcode);
   mm_set_coordinate(&matcode);
   if ( Type[0] == 'R' )
    mm_set_real(&matcode);
   else if ( Type[0] == 'C' )
    mm_set_complex(&matcode);
   else if ( Type[0] == 'P' )
    mm_set_pattern(&matcode);
   else {
    fprintf(stderr,"Unrecognized field in HB Type: %1s",Type);
    exit(1);
   }
   if ( Type[1] == 'U' || Type[1] == 'R' )
    mm_set_general(&matcode);
   else if ( Type[1] == 'S' )
    mm_set_symmetric(&matcode);
   else if ( Type[1] == 'Z' )
    mm_set_skew(&matcode);
   else if ( Type[1] == 'H' )
    mm_set_hermitian(&matcode);
   else {
    fprintf(stderr,"Unrecognized field in HB Type: %1s",&Type[1]);
    exit(1);
   }
   if ( Type[2] != 'A' ){
    fprintf(stderr,"Unrecognized format in HB Type: %1s",&Type[2]);
    exit(1);
   }
    mm_write_banner(stdout, matcode);
    fprintf(stdout,"%% RBTitle: %s\n",Title);
    fprintf(stdout,"%% RBKey:   %s\n",Key);
    mm_write_mtx_crd_size(stdout, Nrow, Ncol, Nnzero);

    if ( Type[0] == 'C' ) {
/*  Loop through columns */
    for (j = 0; j < Ncol ; j++)
       for (i=colptr[j];i<colptr[j+1];i++)
       {
          indxval = 2*(i-1);
          rowp1 = rowind[i-1]+1-1;
          colp1 = j + 1;
          tmp1 = substr(valc,indxval*Valwidth,Valwidth);
          tmp2 = substr(valc,(indxval+1)*Valwidth,Valwidth);
          fprintf(stdout,cformat,rowp1,colp1,tmp1,tmp2);
       }
    } else if ( Type[0] == 'R' ) {
/*  Loop through columns */
    for (j = 0; j < Ncol ; j++)
       for (i=colptr[j];i<colptr[j+1];i++)
       {
          rowp1 = rowind[i-1];
          colp1 = j + 1;
          tmp1 = substr(valc,(i-1)*Valwidth,Valwidth);
          fprintf(stdout,rformat,rowp1,colp1,tmp1);
       }
    } else {
/*  Loop through columns */
    for (j = 0; j < Ncol ; j++)
       for (i=colptr[j];i<colptr[j+1];i++)
       {
          rowp1 = rowind[i-1];
          colp1 = j + 1;
          fprintf(stdout,format,rowp1,colp1);
       }
    }
}
