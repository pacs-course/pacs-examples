/*
Fri Aug 15 16:29:47 EDT 1997
          This is a STREAMING translator to be used for *LARGE*
             Harwell-Boeing files with REAL or COMPLEX data.
                   (use 'hbmat2mtx' for PATTERN matrices)

                     ***   CONVERSION ROUTINE    ***
                      
                 Harwell-Boeing            Matrix Market 
                   assembled       --> sparse coordinate file
                    matrix           
              ( real data transferred as character strings )
                                         ---------



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
#define _SP_base 1
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "iohb.h"
#include "mmio.h"
char* substr(const char* S, int pos, int len);
FILE * readHB_newind(const char*, int*, int*, int*, int**, int**);
FILE * readHB_ind(const char*, int[], int[]);

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
    char* ThisElement;
    char format[30];
    char rformat[30];
    int *colptr, *rowind;
    int *colcount;
    int i,j, repeat, count, col, ind, items, last;
    char line[BUFSIZ];
    MM_typecode matcode;

    if (argc != 2) {
      printf("Usage: %s HBfile\n\n", argv[0]);
      printf("   Sends Matrix Market formatted output to stdout\n");
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

    if (Type[0] == 'P' ) {
      fprintf(stderr,"This is a streaming translator for LARGE files with ");
      fprintf(stderr,"REAL or COMPLEX data.  Use 'hbmat2mtx' for PATTERN matrices.\n");
      exit(1);
    }
    in_file = readHB_newind(argv[1], &Nrow, &Ncol, &Nnzero, &colptr, &rowind);
    ParseIfmt(Indfmt,&Indperline,&Indwidth);
    sprintf(format,"%%%dd %%%dd ",Indwidth,Indwidth);
    ParseRfmt(Valfmt,&Valperline,&Valwidth,&Valprec,&Valflag);
    sprintf(rformat,"%%%ds ",Valwidth);
    ThisElement = (char *) malloc(Valwidth+1);


/*  Skip to values in hb file: */

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

    if ( ThisElement == NULL ) IOHBTerminate("Insufficient memory for ThisElement.\nhb2mtxstrm.c: Line 117 approx.");
    repeat=0;
    if ( Type[0] == 'C') repeat=1;
    count = 0;
    colcount = &colptr[1];
    items = 0; 
    for (i=0;i<Valcrd;i++)
    {
       fgets(line, BUFSIZ, in_file);
       if ( sscanf(line,"%*s") < 0 ) {
         fprintf(stderr,"iohb.c: Null (or blank) line in value data region of HB file.\n");
         exit(1);
       }
       if (Valflag == 'D') {
          while( strchr(line,'D') ) *strchr(line,'D') = 'E';
       }
       col =  0;
       for (ind = 0;ind<Valperline;ind++)
       {
          if (count == Nnzero) break;
          if (count == *colcount-1) colcount++;
          strncpy(ThisElement,line+col,Valwidth);
          *(ThisElement+Valwidth) = (char) NULL;
          if ( Valflag != 'F' && strchr(ThisElement,'E') == NULL ) {
             /* insert a char prefix for exp */
             last = strlen(ThisElement);
             for (j=last+1;j>=0;j--) {
                ThisElement[j] = ThisElement[j-1];
                if ( ThisElement[j] == '+' || ThisElement[j] == '-' ) {
                   ThisElement[j-1] = Valflag;
                   break;
                }
             }
          }
         items++;
         if (items==1) fprintf(stdout,format,rowind[count],colcount-colptr);
         if (items > repeat) { 
            fprintf(stdout,rformat,ThisElement);
            fprintf(stdout,"\n");
            count++; 
            items=0;
         } else {
            fprintf(stdout,rformat,ThisElement);
         }
         col += Valwidth;
       }
    }
       
}

FILE * readHB_ind(const char* filename, int colptr[], int rowind[])
{
/****************************************************************************/
/*  This function opens and reads the specified file, interpreting its      */
/*  contents as a sparse matrix stored in the Harwell/Boeing standard       */
/*  format and creating compressed column storage scheme vectors to hold    */
/*  the index information.                                                  */
/*                                                                          */
/*    ----------                                                            */
/*    **CAVEAT**                                                            */
/*    ----------                                                            */
/*  Parsing real formats from Fortran is tricky, and this file reader       */
/*  does not claim to be foolproof.   It has been tested for cases when     */
/*  the real values are printed consistently and evenly spaced on each      */
/*  line, with Fixed (F), and Exponential (E or D) formats.                 */
/*                                                                          */
/*  **  If the input file does not adhere to the H/B format, the  **        */
/*  **             results will be unpredictable.                 **        */
/*                                                                          */
/****************************************************************************/
    FILE *in_file;
    int i,ind,col,offset,count;
    int Nrow,Ncol,Nnzero,Nrhs;
    int Ptrcrd, Indcrd, Valcrd, Rhscrd;
    int Ptrperline, Ptrwidth, Indperline, Indwidth;
    int Valperline, Valwidth, Valprec;
    int Valflag;           /* Indicates 'E','D', or 'F' float format */
    char* ThisElement;
    char line[BUFSIZ];
    char Title[73], Key[8], Type[4], Rhstype[4];
    char Ptrfmt[17], Indfmt[17], Rhsfmt[21], Valfmt[21];

    if ( (in_file = fopen( filename, "r")) == NULL ) {
       fprintf(stderr,"Error: Cannot open file: %s\n",filename);
       return 0;
    }

    readHB_header(in_file, Title, Key, Type, &Nrow, &Ncol, &Nnzero, &Nrhs,
                  Ptrfmt, Indfmt, Valfmt, Rhsfmt,
                  &Ptrcrd, &Indcrd, &Valcrd, &Rhscrd, Rhstype);

/*  Parse the array input formats from Line 3 of HB file  */
    ParseIfmt(Ptrfmt,&Ptrperline,&Ptrwidth);
    ParseIfmt(Indfmt,&Indperline,&Indwidth);
    ParseRfmt(Valfmt,&Valperline,&Valwidth,&Valprec,&Valflag);
    if (Valflag == 'D') {
          *strchr(Valfmt,'D') = 'E';
    }

/*  Read column pointer array:   */

    offset = 1-_SP_base;  /* if base 0 storage is declared (via macro definition), */
                          /* then storage entries are offset by 1                  */

    ThisElement = (char *) malloc(Ptrwidth+1);
    if ( ThisElement == NULL ) IOHBTerminate("Insufficient memory for ThisElement.\nhb2mtxstrm.c: Line 216 approx.");
    count=0;
    for (i=0;i<Ptrcrd;i++)
    {
       fgets(line, BUFSIZ, in_file);
       if ( sscanf(line,"%*s") < 0 ) 
         IOHBTerminate("iohb.c: Null (or blank) line in pointer data region of HB file.\n");
       col =  0;
       for (ind = 0;ind<Ptrperline;ind++)
       {
          if (count > Ncol) break;
          strncpy(ThisElement,line+col,Ptrwidth);
          *(ThisElement+Ptrwidth) = (char) NULL;
/*          ThisElement = substr(line,col,Ptrwidth); */
          colptr[count] = atoi(ThisElement)-offset;
          count++; col += Ptrwidth;
       }
    }
    free(ThisElement);

/*  Read row index array:  */

    ThisElement = (char *) malloc(Ptrwidth+1);
    if ( ThisElement == NULL ) IOHBTerminate("Insufficient memory for ThisElement.\nhb2mtxstrm.c: Line 238 approx.");
    count = 0;
    for (i=0;i<Indcrd;i++)
    {
       fgets(line, BUFSIZ, in_file);
       if ( sscanf(line,"%*s") < 0 ) 
         IOHBTerminate("iohb.c: Null (or blank) line in index data region of HB file.\n");
       col =  0;
       for (ind = 0;ind<Indperline;ind++)
       {
          if (count == Nnzero) break;
          strncpy(ThisElement,line+col,Indwidth);
          *(ThisElement+Indwidth) = (char) NULL;
          /* ThisElement = substr(line,col,Indwidth);*/
          rowind[count] = atoi(ThisElement)-offset;
          count++; col += Indwidth;
       }
    }
    free(ThisElement);
    return in_file;
}


FILE * readHB_newind(const char* filename, int* M, int* N, int* nonzeros, int** colptr, int** rowind)
{
    FILE *in_file;
    int Nrhs;
    int Ptrcrd, Indcrd, Valcrd, Rhscrd;
    char Title[73], Key[9], Type[4], Rhstype[4];
    char Ptrfmt[17], Indfmt[17], Rhsfmt[21],Valfmt[21];

    if ((in_file = fopen( filename, "r")) == NULL) {
      fprintf(stderr,"Error: Cannot open file: %s\n",filename);
      return 0;
     }
    
    readHB_header(in_file, Title, Key, Type, M, N, nonzeros, &Nrhs,
                  Ptrfmt, Indfmt, Valfmt, Rhsfmt,
                  &Ptrcrd, &Indcrd, &Valcrd, &Rhscrd, Rhstype);
    fclose(in_file);

        *colptr = (int *)malloc((*N+1)*sizeof(int));
        if ( *colptr == NULL ) IOHBTerminate("Insufficient memory for colptr.\nhb2mtxstrm.c: Line 279 approx.");
        *rowind = (int *)malloc(*nonzeros*sizeof(int));
        if ( *rowind == NULL ) IOHBTerminate("Insufficient memory for rowind.\nhb2mtxstrm.c: Line 281 approx.");
	return readHB_ind(filename, *colptr, *rowind);
}
