%%  N = 1000; Sum = 3.141592736923126e+00
%%  N = 10000;Sum = 3.141592654423132e+00
%%             pi = 3.141592653589793

T=clock; 
MPI_ANY_SOURCE = -1;
MPI_Init ();  

rnk   = MPI_Comm_rank (MPI_COMM_WORLD);
siz   = MPI_Comm_size (MPI_COMM_WORLD);

SLV = logical (rnk);                 
MST = ~ SLV;

width = 1/N; lsum=0;
i=rnk:siz:N-1;    
x=(i+0.5)*width;  
lsum=sum(4./(1+x.^2));
TAG=7;                
if SLV                
  MPI_Send (lsum, 0, TAG, MPI_COMM_WORLD);
else                  
  Sum =lsum;          
  for slv=1:siz-1     
    lsum = MPI_Recv (MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD);
    Sum += lsum;      
  endfor              
endif

if MST
  format long e
  disp (Sum / N)
endif

MPI_Finalize ();

