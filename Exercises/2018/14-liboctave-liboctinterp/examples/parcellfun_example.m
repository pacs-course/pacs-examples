## Copyright (C) 2011 Carlo de Falco
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## Author: Carlo de Falco <carlo@guglielmo.local>
## Created: 2011-12-24


%% you need at least version 3.4 of Octave to run this script

clear all
close all

%% uncomment the following line if you don't have the required packages installed
%% pkg install -forge general bim fpl msh splines
pkg load general bim fpl msh splines

%%%%%%%%%%%%%%%%%%%%%
%%%    SERIAL VERSION
%%%%%%%%%%%%%%%%%%%%%

tic ();
nel = 100;
U0 = randn (200, 1);
us = zeros (101, 200);
for ii=1:numel (U0)

  x = transpose (linspace (0, 1, nel+1));
  A = bim1a_laplacian (x, 1, 1);
  b = bim1a_rhs (x, 1, 1);

  us(:,ii) = zeros (size (x));
  us(1,ii) = U0(ii);
  
  res = @(X) A(2:end-1, 2:end-1) * X - (b(2:end-1) - A(2:end-1, [1 end]) * us([1 end], ii));
  us(2:end-1,ii) = fsolve (res, us(2:end-1,ii));

endfor
toc ()




%%%%%%%%%%%%%%%%%%%%%
%%%  PARALLEL VERSION
%%%%%%%%%%%%%%%%%%%%%

%% solve  - u'' + exp (u) = 1 in (0, 1) with u(0)=u0 and u(1)=0
function u = poisson1d (u0)
  nel = 100;
  x = transpose (linspace (0, 1, nel+1));
  A = bim1a_laplacian (x, 1, 1);
  b = bim1a_rhs (x, 1, 1);

  u = zeros (size (x));
  u(1) = u0;
  
  res = @(X) A(2:end-1, 2:end-1) * X - (b(2:end-1) - A(2:end-1, [1 end]) * u([1 end]));
  u(2:end-1) = fsolve (res, u(2:end-1));
endfunction
tic ();
U0 = num2cell (randn (1, 200));
up = parcellfun (2, @poisson1d, U0, "UniformOutput", true, "VerboseLevel", 2);
toc ()

%subplot (1, 2, 1)
%plot (x, us)
%subplot (1, 2, 2)
%plot (x, up)