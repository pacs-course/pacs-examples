g++ -std=c++11 -c fem1d.cpp -I.
g++ -std=c++11 -c mesh.cpp -I.
g++ -std=c++11 -c gauss_seidel.cpp -I.
g++ -o fem1d fem1d.o gauss_seidel.o mesh.o
rm fem1d.o gauss_seidel.o mesh.o
