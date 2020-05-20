/*
 * loadPolyFactory.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: forma
 */
#include "loadPolyFactory.hpp"

namespace Geometry
{
bool loadPolyFactory::load (std::string pluginFile)
{
    bool good = true;
    std::ifstream pFile (pluginFile);
    if (pFile.bad() )
    {
        std::cerr << " Cannot open plugin files" << std::endl;
        return false;
    }
    while (pFile.good() && (! pFile.eof() ) )
    {
        std::string line;
        //! get a line
        std::getline (pFile, line);
        // I am assuming no spaces in file name and I gat start and end of alphabetic characters
        // This way I account for possible initial or training blanks.
        auto start = find_if (line.begin(), line.end(), [] (unsigned char c)
        {
            return !std::isblank (c);
        });
        auto end  = find_if (start, line.end(), [] (unsigned char c)
        {
            return std::isblank (c);
        });
        if (start != end)
        {
            std::string libName (start, end);
            // open the dynamic library
            void* sdl_library = dlopen (libName.c_str(), RTLD_NOW);
            // check if error
            if (sdl_library == nullptr)
            {
                std::cerr << " Cannot load Library " << libName << " " << dlerror() << std::endl;
                good = false;
            }
            else
            {
                std::clog << libName << " Library opened" << std::endl;
                good = (good && true);
                loadedLibs.push_back (sdl_library);
            }
        }
        else
        {
            std::clog << "Empty line in plugins file\n";
        }
    }
    return good;
}

void
loadPolyFactory::close ()
{
    // close all hold libraries
    for (auto l : this->loadedLibs)
    {
        dlclose (l);
    }
    this->loadedLibs.clear();
}
}     // end namespace




