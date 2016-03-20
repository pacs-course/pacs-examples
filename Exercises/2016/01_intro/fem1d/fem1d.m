a = 0; 
b = 1;
L = b-a;
nnodes = 50;
nels   = nnodes - 1;
h      = L / nels;
nodes  = [a:h:b];
elements = [1:nnodes-1; 2:nnodes];

%% Initialize the coefficients matrix
A = zeros (nnodes);

%% loop over the intervals to assemble the coefficient matrix:
for iel=1:nels

  %% build local matrix
  mloc = zeros (2);
  
  %% loop over the rows of the local matrix
  for inode =1:2
    
    %% compute gradient of the inode-th test function
    igrad = (-1)^inode / h;

    %% loop over the columns of the local matrix
    for jnode =1:2
      
      %% compute gradient of the jnode-th shape function
      jgrad = (-1)^jnode/h;
      
      %% integrate igrad*jgrad over the element iel and store at mloc (inode, jnode)            
      mloc(inode,jnode) = igrad * jgrad * h;
      
    end 
  end 
  
  %% Assembly:
  for inode =1:2
    for jnode =1:2
      A(elements(inode,iel), elements(jnode,iel)) = ...
      A(elements(inode,iel), elements(jnode,iel)) + ...
      mloc(inode,jnode);
    end
  end
end


%% Initialize the rhs vector
f = zeros (nnodes, 1);

%% loop over the elements to assemble the rhs
for iel = 1:nels

  %% build local vector
  vloc = zeros (2, 1);

  %% loop over the rows of local vector
  for inode =1:2
    
    %% compute the integral of the forcing term
    %% times the inode-th test function integrated
    %% on the interval iel 
    vloc(inode) = h / 2;
    
  end 

  %% Assembly:
  for inode = 1:2
    f(elements(inode, iel)) += ...
    vloc(inode);
  end
end
%% Impose boundary conditions via Iron's trick
f(1)   = 0;
f(end) = 0;

A(1, 1) = 1;
A(1, 2:end) = 0;

A(end, end) = 1;
A(end, 1:end-1) = 0;

%% Solve linear system
uh = A \ f;

%% plot solution
plot (nodes, uh, nodes, nodes .* (1 - nodes) / 2, 'x')
