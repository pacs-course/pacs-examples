/*
 * QuadParameters.cpp
 *
 *  Created on: Dec 20, 2021
 *      Author: forma
 */

#include <stdexcept>
#include "GetPot"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include "QuadParameters.hpp"

namespace apsc::NumericalIntegration
{
  QuadParameters readQuadParameters_GP(std::string const & filename)
  {
    using namespace std;
    QuadParameters param;
    GetPot cl(filename.c_str()); // open getpot file

    // Read quadratur rule libraries
    auto nlibs = cl.vector_variable_size("library");
    if(nlibs == 0)
      {
        throw std::runtime_error("You need to specify at least one plugin library");
      }
    else
      {
        cout << "Reading " << nlibs << " plugin libraries\n";
      }

    std::vector<string> libs(nlibs);
    for(unsigned int i = 0; i < nlibs; ++i)
      {
        string quadlib = cl("library", i, "NONE");
        if(quadlib == string("NONE"))
          {
            throw std::runtime_error("Getpot file wrongly parsed,. Cannot read library");
          }
        else
          {
            cout << "Reading plugin library " << quadlib << std::endl;
          }
        libs[i]=quadlib;
      }
    param.library=libs;
    libs.clear();

    // read integrand libraries
    nlibs = cl.vector_variable_size("udflib");
    libs.resize(nlibs);
    if(nlibs == 0)
      {
        throw std::runtime_error("You need to specify at least one integrand library");
      }
    else
      {
        cout << "Reading " << nlibs << " integrand libraries\n";
      }

    for(unsigned int i = 0; i < nlibs; ++i)
      {
        string intlib = cl("udflib", i, "NONE");
        if(intlib == string("NONE"))
          {
            throw("Getpot file wrongly parsed. Cannot read library");
          }
        else
          {
            cout << "Integrands library " << intlib << std::endl;
          }
        libs[i]=intlib;
      }
    param.udflib=std::move(libs);

    std::string fun_name = cl("integrand", "NULL");
    if(fun_name == "NULL")
      {
        throw std::runtime_error("Getpot file must contain integrand=integrandName\n or integrand=?");
      }
    param.integrand=std::move(fun_name);

    param.a=cl("a",param.a);
    param.b=cl("b",param.b);
    param.nint=cl("nint",param.nint);
    param.maxIter=cl("maxIter",param.maxIter);
    param.targetError=cl("targetError",param.targetError);
    param.rule=cl("rule","?");
    return param;
  }

  QuadParameters readQuadParameters_json(std::string const & filename)
  {
    using namespace std;
    QuadParameters param;
    std::ifstream file(filename);
    nlohmann::json cl;
    file >>cl;
    file.close();


    // Read quadratur rule libraries
    // The second argument is there just to deduce the type
    auto libs = cl.value("library",param.library);
    auto nlibs = libs.size();
    if(nlibs == 0)
      {
        throw std::runtime_error("You need to specify at least one plugin library");
      }
    else
      {
        cout << "Reading " << nlibs << " plugin libraries\n";
      }

    for(unsigned int i = 0; i < nlibs; ++i)
      {
        cout << "Reading plugin library " << libs[i] << std::endl;
      }
    param.library=libs;
    libs.clear();

    // read integrand libraries
    libs = cl.value("udflib", param.udflib);
    nlibs = libs.size();
    if(nlibs == 0)
      {
        throw std::runtime_error("You need to specify at least one integrand library");
      }
    else
      {
        cout << "Reading " << nlibs << " integrand libraries\n";
      }

    for(unsigned int i = 0; i < nlibs; ++i)
      {
        cout << "Integrands library " << libs[i] << std::endl;
      }
    param.udflib=std::move(libs);

    // @note: I must tell that the II argument is a std::string, since it is used to derive
    //        the return type of exec();
    std::string fun_name = cl.value("integrand", std::string("NULL"));

    if(fun_name == "NULL")
      {
        throw std::runtime_error("Json file must contain \"integrand\": integrandName\n or integrand=?");
      }
    param.integrand=std::move(fun_name);
    // The last parameters
    param.a=cl.value("a",param.a);
    param.b=cl.value("b",param.b);
    param.nint=cl.value("nint",param.nint);
    param.maxIter=cl.value("maxIter",param.maxIter);
    param.targetError=cl.value("targetError",param.targetError);
    param.rule=cl.value("rule",std::string("?"));
    return param;
  }

}

