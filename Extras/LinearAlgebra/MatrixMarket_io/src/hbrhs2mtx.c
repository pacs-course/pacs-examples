/*
Fri Aug 15 16:29:47 EDT 1997

                     ***   CONVERSION ROUTINE    ***
                      
                    Harwell-Boeing        Matrix Market 
                   right-hand-sides --> dense array files
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
    int i, j;
    int Ptrcrd, Indcrd, Valcrd, Rhscrd;
    int Rhsperline, Rhswidth, Rhsprec;
    int Rhsflag;           /* Indicates 'E','D', or 'F' float format */
    int nvecs;
    int Nrow, Ncol, Nnzero;
    int Nrhs;
    char *tmp;
    char *Rhsc;
    char *Rhsi;
    char *Rhsfmtc;
    char rformat[30];
    char cformat[30];
    char *RhsFilename;
    char *SlnFilename;
    char *EstFilename;
    char Title[73], Key[9], Rhstype[4];
    char Type[4];
    char Ptrfmt[17], Indfmt[17], Valfmt[21], Rhsfmt[21];
    MM_typecode matcode;

    if (argc != 2) {
      printf("Usage: %s HBfile\n", argv[0]);
      printf("Function: reads auxillary vectors (if present) from HBfile\n");
      printf("          and writes to Matrix Market (dense array) files,\n");
      printf("          one for each auxillary vector.\n");
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

    if ( Nrhs > 0 ) {
      nvecs = 1;
      if ( Rhstype[0] != 'F' ) { 
         fprintf(stderr,"Right-hand-side type %c not supported.\n",Rhstype[0]);
         exit(1);
       }
       if ( Rhstype[1] == 'G' ) nvecs++;
       if ( Rhstype[2] == 'X' ) nvecs++;
       readHB_newaux_char(argv[1], 'F', &Rhsc, &Rhsfmtc);
       ParseRfmt(Rhsfmtc,&Rhsperline,&Rhswidth,&Rhsprec,&Rhsflag);
       sprintf(rformat,"%%%ds\n",Rhswidth);
       sprintf(cformat,"%%%ds\n",2*Rhswidth+1);
       Rhsi = Rhsc;
       for (i=0;i<Nrhs;i++) {
          RhsFilename = Makename(argv[1],"rhs",i+1);
          mm_set_matrix(&matcode);
          mm_set_array(&matcode);
          mm_set_general(&matcode);
          if ( Type[0] == 'R' )
            mm_set_real(&matcode);
          else if ( Type[0] == 'C' )
            mm_set_complex(&matcode);
          else {
            fprintf(stderr,"Unrecognized field in HB Rhs-Type: %1s",Type);
            exit(1);
          }

          out_file = fopen(RhsFilename,"w");
          mm_write_banner(out_file, matcode);
          fprintf(out_file,"%% RBTitle: %s\n",Title);
          fprintf(out_file,"%% RBKey:   %s\n",Key);
          fprintf(out_file,"%% Right-hand-side: %d\n",i+1);
          mm_write_mtx_array_size(out_file, Nrow, 1);
            
    /*    Loop through columns */
          for (j = 0; j < Nrow ; j++) {
            tmp = substr(Rhsi,0,Rhswidth);
            Rhsi+=Rhswidth;
            if ( Type[0] == 'R' ) fprintf(out_file,rformat,tmp);
            else if ( Type[0] == 'C' ) {
              tmp = strcat(tmp," ");
              tmp = strncat(tmp,Rhsi,Rhswidth);
              Rhsi+=Rhswidth;
              fprintf(out_file,cformat,tmp);
            }
          }
          fclose(out_file);
       }

       if ( Rhstype[1] == 'G' ) {
          Rhsi = Rhsc;
          readHB_aux_char(argv[1], 'G', Rhsc);
          for (i=0;i<Nrhs;i++) {
            EstFilename = Makename(argv[1],"est",i+1);
            out_file = fopen(EstFilename,"w");
            mm_write_banner(out_file, matcode);
            fprintf(out_file,"%% RBTitle: %s\n",Title);
            fprintf(out_file,"%% RBKey:   %s\n",Key);
            fprintf(out_file,"%% Solution Estimate: %d\n",i);
            mm_write_mtx_array_size(out_file, Nrow, 1);
            
    /*      Loop through columns */
            for (j = 0; j < Nrow ; j++) {
              tmp = substr(Rhsi,0,Rhswidth);
              Rhsi+=Rhswidth;
              if ( Type[0] == 'R' ) fprintf(out_file,rformat,tmp);
              else if ( Type[0] == 'C' ) {
                tmp = strcat(tmp," ");
                tmp = strncat(tmp,Rhsi,Rhswidth);
                Rhsi+=Rhswidth;
                fprintf(out_file,cformat,tmp);
              }
            }
            fclose(out_file);
         }
       }
       if ( Rhstype[2] == 'X' ) {
          Rhsi = Rhsc;
          readHB_aux_char(argv[1], 'X', Rhsc);
          for (i=0;i<Nrhs;i++) {
            SlnFilename = Makename(argv[1],"sln",i+1);
            out_file = fopen(SlnFilename,"w");
            mm_write_banner(out_file, matcode);
            fprintf(out_file,"%% RBTitle: %s\n",Title);
            fprintf(out_file,"%% RBKey:   %s\n",Key);
            fprintf(out_file,"%% Exact Solution: %d\n",i);
            mm_write_mtx_array_size(out_file, Nrow, 1);
            
      /*    Loop through columns */
            for (j = 0; j < Nrow ; j++) {
              tmp = substr(Rhsi,0,Rhswidth);
              Rhsi+=Rhswidth;
              if ( Type[0] == 'R' ) fprintf(out_file,rformat,tmp);
              else if ( Type[0] == 'C' ) {
                tmp = strcat(tmp," ");
                tmp = strncat(tmp,Rhsi,Rhswidth);
                Rhsi+=Rhswidth;
                fprintf(out_file,cformat,tmp);
              }
            }
            fclose(out_file);
         }
       }
     }
}

char * Makename(const char *Basename, const char* Vectype, const int Nrhs) {
  char *tmp;
  char *tail;
  char *Filename;
  tail = strrchr(Basename,'/')+1;
  tmp = strchr(Basename,'.');
  if ( tmp == NULL ) { 
     fprintf(stderr,"Makename: Cannot create filename _%3s%d from %s\n",Vectype,Nrhs,Basename);
     exit(1);
  }
  tmp = substr(tail,0,tmp-tail);
  Filename = (char *) malloc((strlen(tmp)+10));
  strcpy(Filename,tmp);
  sprintf(&Filename[strlen(tmp)],"_%3s%d.mtx",Vectype,Nrhs);
  return Filename;
}

    
