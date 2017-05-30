function Dk = deriv_1d (uk, Qk)
  
  Duk = diff (uk);
  ak  = Duk(1:end-1) ./ (Duk(1:end-1) + Duk(2:end)); 
  qk  = diff (Qk, 1, 2);
  dk  = qk ./ Duk;

  Dk  = zeros (size (Qk));
  Dk(:, 2:end-1) = (1 - ak) .* dk(:, 1:end-1) + ak .* dk(:, 2:end);
  Dk(:, 1)   = 2 * dk(:, 1)   - Dk(:, 2);
  Dk(:, end) = 2 * dk(:, end) - Dk(:, end-1);

end
