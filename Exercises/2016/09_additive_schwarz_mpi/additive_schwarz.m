

n_div     = 10;
n_ovp     = 2;
mpi_size  = 3;

a       = 0;
b       = 1;
L       = b - a;
L_loc   = L / mpi_size;
nel     = 300;
overlap = 30;
h       = L_loc / ceil (nel / mpi_size);

a_coef = 1;
r_coef = 1;

tol = 1e-6;
maxit = 1000;

for mpi_rank = 1:mpi_size

  a_loc(mpi_rank) = a + (mpi_rank - 1) * L_loc;
  b_loc(mpi_rank) = a_loc(mpi_rank) + L_loc;
  nel_loc(mpi_rank) = ceil (nel / mpi_size);

  if (mpi_rank) > 1
    a_loc(mpi_rank) -= overlap * h;
    nel_loc(mpi_rank) += overlap;
  end

  if (mpi_rank) < mpi_size
    b_loc(mpi_rank) += overlap * h;
    nel_loc(mpi_rank) += overlap;
  end

  u_left(mpi_rank) = 0;
  u_leftright(mpi_rank) = 0;

  ndof_loc(mpi_rank) = nel_loc(mpi_rank) + 1;
  u_loc{mpi_rank} = zeros (ndof_loc(mpi_rank), 1);
  x_loc{mpi_rank} = linspace (a_loc(mpi_rank),
                              b_loc(mpi_rank),
                              ndof_loc(mpi_rank)) .';

  rhs{mpi_rank} = bim1a_rhs (x_loc{mpi_rank},
                             r_coef, 1);

  stiff{mpi_rank} = bim1a_laplacian (x_loc{mpi_rank},
                                     a_coef, 1);
  
  stiff{mpi_rank}([1 end], :) = 0;
  stiff{mpi_rank}(1, 1) = 1;
  stiff{mpi_rank}(end, end) = 1;

  normdiff(mpi_rank) = inf;
endfor

for it = 1 : maxit

  if (all (normdiff <= tol))
    break
  endif

  for mpi_rank = 1:mpi_size
    if mpi_rank > 1
      rhs{mpi_rank}(1) = u_loc{mpi_rank - 1}(end - 2 * overlap);
    else
      rhs{mpi_rank}(1) = 0;
    endif

    if mpi_rank < mpi_size
      rhs{mpi_rank}(end) = u_loc{mpi_rank + 1}(1 + 2 * overlap);
    else
      rhs{mpi_rank}(end) = 0;
    endif
  endfor
  for mpi_rank = 1:mpi_size
    if normdiff(mpi_rank) > tol
      u_loc_new = stiff{mpi_rank} \ rhs{mpi_rank};
      normdiff(mpi_rank) = norm (u_loc{mpi_rank} - u_loc_new);
      u_loc{mpi_rank} = u_loc_new;
    endif
    plot (x_loc{mpi_rank}, u_loc{mpi_rank})
    axis([a b 0 .15])
    hold all
  endfor
  title (sprintf ("iteration %d", it))
  pause (.1)
  
  hold off
endfor


  

