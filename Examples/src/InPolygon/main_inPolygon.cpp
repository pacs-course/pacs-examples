//
// Created by forma on 31/01/23.
//
#include "inPolygon.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include<string>
int
main()
{
  std::array<std::array<double, 2>, 3> tria1{
    {{{1., 0.}}, {{0.5, 0.5}}, {{-0.5, 0.}}}};
  double x{0};
  double y;
  while(x != -9999.)
    {
      std::istringstream iline("");
      std::string line;
      std::cout << "gimme x and y. Quit to exit\n";
      std::getline(std::cin,line);
      if (line.find("uit")!=std::string::npos) return 0;
      iline.str(line);
      iline >> x >> y;
      std::array<double, 2> p{x, y};
      bool                  in = apsc::inPolygon(tria1, p);
      std::cout << "Point is for standard algoritm ";
      if(in)
        std::cout << "Inside!";
      else
        std::cout << "Outside!";
      std::cout << std::endl;
      std::cout << "Point is for fast algoritm ";
      in = apsc::inPolygonFast(tria1, p);
      if(in)
        std::cout << "Inside!";
      else
        std::cout << "Outside!";
      std::cout << std::endl;
    }
}