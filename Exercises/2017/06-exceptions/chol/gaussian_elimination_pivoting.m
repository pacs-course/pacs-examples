function [U,v,x] = gaussian_elimination_pivoting(A,b)
  %GAUSSIAN_ELIMINATION_PIVOTING Perform Gaussian elimination with pivoting on a linear
  %system
  %   [U,v,x] = gaussian_elimination(A,b)
  %
  %   Inputs  : A = system coefficient matrix
  %             b = right-hand side vector
  %   Outputs : U = obtained upper triangle matrix
  %             v = obtained right-hand side vector
  %             x = solution vector

  n = size(A, 1);

  U = A
  v = b;

  for (k = 1:n-1)   % cycle over the columns
    disp(['Unknown ', num2str(k)]);
    % Pivoting
    aux = zeros(n, 1);
    aux(k:end) = abs( U(k:end, k) );
    [max_value, k_max] = max(aux);
    if (k_max > k)
		U([k k_max], :) = U([k_max k], :);
		v([k k_max]) = v([k_max k]);
		disp(['Pivoting with equation ', num2str(k_max)]);
		U, v, pause()
    end
    % cycle in the [k+1:end] times [k:end] submatrix
    for (i = k+1:n) 
      l_ik = U(i,k) / U(k,k);
      for (j = k:n)
	      U(i,j) = U(i,j) - l_ik*U(k,j);
      end
	  v(i) = v(i) - l_ik*v(k);
      disp(['Linear combination with equation ', num2str(i)]);
      U, v, pause()
    end
  end

  % Backward substitution
  x = U \ v;

end
