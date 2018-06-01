## Copyright (C) 2012 Carlo de Falco
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

savetommfile    = false
savetooctxtfile = false
plotspy         = true

tic ()
printf ("generate and partition mesh\n")

xx     = linspace (0, 1, 56);
msh    = bim3c_mesh_properties (msh3m_structured_mesh (xx, xx, xx, 1, 1:6));
nelem  = columns (msh.t);

xm     = sum (msh.p(1, :)(msh.t(1:4, :)), 1) / 4;   
ym     = sum (msh.p(2, :)(msh.t(1:4, :)), 1) / 4;  

els_0  = find (xm <= 1/2);
els_1  = find (xm >  1/2 & ym <= 1/2);
els_2  = find (xm >  1/2 & ym >  1/2);

dnodes   = bim3c_unknowns_on_faces (msh, 1:6);
nn       = columns (msh.p)
intnodes = setdiff (1:nn, dnodes);

msh_0            = msh;
msh_0.t          = msh.t(:, els_0);
msh_0.wjacdet    = msh.wjacdet(:, els_0);
msh_0.area       = msh.area(:, els_0);
msh_0.shg        = msh.shg(:, :, els_0);

msh_1            = msh;
msh_1.t          = msh.t(:, els_1);
msh_1.wjacdet    = msh.wjacdet(:, els_1);
msh_1.area       = msh.area(:, els_1);
msh_1.shg        = msh.shg(:, :, els_1);

msh_2            = msh;
msh_2.t          = msh.t(:, els_2);
msh_2.wjacdet    = msh.wjacdet(:, els_2);
msh_2.area       = msh.area(:, els_2);
msh_2.shg        = msh.shg(:, :, els_2);

toc ()
printf ("partition mesh nodes\n")

all_0    = unique (msh_0.t(1:4, :)(:));
all_1    = unique (msh_1.t(1:4, :)(:));
all_2    = unique (msh_2.t(1:4, :)(:));

int_0    = setdiff (all_0, [all_1(:); all_2(:); dnodes(:)]);
int_1    = setdiff (all_1, [all_0(:); all_2(:); dnodes(:)]);
int_2    = setdiff (all_2, [all_1(:); all_0(:); dnodes(:)]);

oth_0    = setdiff (all_0, int_0);
oth_1    = setdiff (all_1, [int_1(:); oth_0(:)]);
oth_2    = setdiff (all_2, [int_2(:); oth_0(:); oth_1]);

oth_0i   = setdiff (oth_0, dnodes(:));
oth_1i   = setdiff (oth_1, dnodes(:));
oth_2i   = setdiff (oth_2, dnodes(:));

ordering  = [int_0(:); oth_0(:); int_1(:); oth_1(:); int_2(:); oth_2(:)];
orderingi = [int_0(:); oth_0i(:); int_1(:); oth_1i(:); int_2(:); oth_2i(:)];

toc()
printf ("generate partitioned matrices\n")

A0               = sparse (nn, nn);
A0(all_0, all_0) = bim3a_laplacian (msh_0, 1, 1)(all_0, all_0);
A0               = A0;

A1               = sparse (nn, nn);
A1(all_1, all_1) = bim3a_laplacian (msh_1, 1, 1)(all_1, all_1);
A1               = A1(ordering, ordering);

A2               = sparse (nn, nn);
A2(all_2, all_2) = bim3a_laplacian (msh_2, 1, 1)(all_2, all_2);
A2               = A2(ordering, ordering);

toc ()
printf ("compute reference solution\n")

A       = bim3a_laplacian (msh, 1, 1);
Ai      = A(intnodes, intnodes);
b       = bim3a_rhs (msh, 1, 1);
bi      = b(intnodes);
x       = zeros (nn, 1);
x       = Ai \ bi;

toc ()
printf ("compute block-jacobi solution\n")

xbj     = zeros (numel (intnodes), 1);
resbj   = xbj;
presbj  = xbj;

resnrm  = inf;
for ibj=1:100
  
  resbj  = b(intnodes) - A(intnodes, intnodes) * xbj;
  resnrm = norm (resbj, inf);
  if (resnrm < 1e-10); break; endif
  
  presbj()

endfor


toc ()
printf ("postprocessing\n")

if (savetommfile)
  mmwrite ("A_3",   A(ordering, ordering));
  mmwrite ("A_3.0", A0(ordering, ordering));
  mmwrite ("A_3.1", A1(ordering, ordering));
  mmwrite ("A_3.2", A2(ordering, ordering));
  mmwrite ("b_3",   b(ordering));
  toc ()
endif

if (plotspy)
  close all; spy (A2(ordering, ordering), 'kd'); hold on; spy (A1(ordering, ordering), 'bo'); spy (A0(ordering, ordering), 'rx')
  toc ()
endif

if (savetommfile)
  A0 = A0(ordering, ordering); A1 = A1(ordering, ordering); A2 = A2(ordering, ordering);
  save ("-text", "Ab_3", "A", "b", "A0", "A1", "A2")
  toc ()
endif

