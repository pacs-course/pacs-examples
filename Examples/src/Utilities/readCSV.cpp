
/*
 * readCSV.cpp
 *
 *  Created on: Jun 25, 2021
 *      Author: forma
 */
#include "readCSV.hpp"
namespace Utility
{
  // Implementations

  void ReadCSV::read(std::basic_istream<CharT> & in)
  {
    StringType line;
    unsigned count=0u;
    unsigned maxtokens=0u;
    unsigned mintokens=999999u;
    Token token;
    Record tokens;
    StringType word;
    bool incomplete=false;

    // set capacity to reduce memory reallocations
    if(minRecords !=0 && allRecords.capacity()<minRecords)
      allRecords.reserve(minRecords);
    if(minTokens>1u)tokens.reserve(minTokens);

    for (auto i= 0u; i<skip;++i)getline(in,line);

    while(!in.eof())
      {
        std::getline(in,line);
        ++count;
        // skip empty lines
        if(line.empty())
          {
            std::cerr<<"Warning: Empty line n. "<<count<<std::endl;
            continue;
          }
        // Check if we are inside a text token
        if (!incomplete)
          line=Utility::trim(line);
        std::stringstream s(line);
        while(std::getline(s,word,sep))
          {
            word=Utility::trim(word);
            if (incomplete)
              {
                // end of text token
                if(word.back()==textMark)
                  {
                    if(stripApexes)word.pop_back();
                    token+=word;
                    tokens.push_back(token);
                    token.clear();
                    incomplete=false;
                  }
                else
                  token+=word;
              }
            // Start of text token
            else if(word.front()==textMark)
              {
                if(stripApexes)
                  token=word.substr(1,word.size());
                else
                  token=word;
                if(token.back()==textMark)
                  {
                    if(stripApexes)token.pop_back();
                    tokens.push_back(token);
                    token.clear();
                  }
                else
                  {
                    incomplete=true;
                  }
              }
            else
              {
                token=word;
                tokens.push_back(token);
                token.clear();
              }
          }

        int check;
        // Check if I have read something
        for (auto const & t : tokens)
          check+=t.size();
        if(check!=0)
          {
            if(!incomplete)
              {
                if(tokens.size()<minTokens)
                  {
                    if(verbose)
                      {
                        std::clog<<"Line "<<count<<"Incomplete:\n";
                        std::clog<<" Has only"<<tokens.size()<<"Tokens. Autocompleted\n";
                      }
                    for (unsigned int i=0; i<minTokens-tokens.size();++i)
                      {
                        tokens.push_back(std::string{}.assign(1,sep));
                      }
                  }
                tokens.shrink_to_fit();
                allRecords.push_back(tokens);
                maxtokens=tokens.size()>maxtokens?tokens.size():maxtokens;
                mintokens=tokens.size()<mintokens?tokens.size():mintokens;
                tokens.clear();
              }
            else
              {
                std::cerr<<"Warning: broken line n. "<<count<<std::endl;
              }
          }
        else
          {
            std::cerr<<"Warning: empty line n. "<<count<<std::endl;
          }
      }

    if(verbose)
      {
        std::clog<<" Read "<<count<<" lines in file"<<std::endl;
        std::clog<<" Max/Min number of tokens "<<maxtokens<<"/"<<mintokens<<std::endl;
        std::clog<<" Read "<<allRecords.size()<<" valid records"<<std::endl;
        std::clog<<" Last meaningful line read:\n";
        this->writeRecord(std::clog, allRecords.back());
        std::clog<<"\n";
      }
    // reduce memory
    allRecords.shrink_to_fit();
  }


  void
  ReadCSV::writeRecord(std::basic_ostream<CharT> & out,
                              const Record& tokens) const
  {
    StringType s;
    StringType separator(1,sep);
    for (auto const & token : tokens)
      s+=token+separator;
    s.pop_back();
    out<<s;
  }


   void
   ReadCSV::writeAllRecords(std::basic_ostream<CharT> & out) const
  {
    for (auto const & t: allRecords)
      {
        writeRecord(out,t);
        out<<"\n";
      }
  }

}// end namespace



