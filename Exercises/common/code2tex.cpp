#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc < 2) { // At least one input file is needed
        std::cerr << "Usage: code2tex input1.[ch]pp [input2.[ch]pp] ... [inputn.[ch]pp] [-o output_dir]" << std::endl;
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        std::string ifilename( argv[i] );
        std::string ofilename = ifilename + ".tex";

        std::ifstream ifile( ifilename.c_str(), std::ios_base::in );
        std::ofstream ofile( ofilename.c_str(), std::ios_base::out );
        char line[1024];

        std::cout << ifilename << "..." << std::flush;
        ofile << '\\' << "begin{verbatim}" << std::endl;
        while( ifile.getline(line, 1024, '\n' ) ) ofile << line << std::endl;
        ofile << '\\' << "end{verbatim}" << std::endl;
        std::cout << ofilename << std::endl;

        ifile.close();
        ofile.close();
    }

    return 0;
}
