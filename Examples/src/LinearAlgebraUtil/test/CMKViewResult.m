A_raw=load("Original.mat");
A=spconvert(A_raw);
B_raw=load("Processed.mat");
B=spconvert(B_raw);
spy(A);
figure;
spy(B);

