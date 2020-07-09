function [M] = getFullMatrix(dirname)
mmat = strcat(dirname,"/M.mat");
bmat = strcat(dirname,"/B.mat");
tmat = strcat(dirname,"/T.mat");
M = mmread(mmat);
B = mmread(bmat);
T = mmread(tmat);
M = [M B';B T];

