#ifndef HH_READCVSHEADER_HH
#define HH_READCVSHEADER_HH
#include "string_utility.hpp"
#include <string>
#include <vector>
#include <iostream>
namespace Utility
{
  /*!
   * Class forreading a csv file. It recognizes text cells ignoring separation character
   * for within tokens (cells) marked by " "
   */
  class ReadCSV
  {
  public:
    //! The type used for characters
    using CharT=char;
    //! The type used for strings
    using StringType=std::string;
    //! The type used for the Token, i.e. the content of a cell
    using Token=StringType;
    //! The type used to store a record
    using Record=std::vector<Token>;
    //! The type used to store all records
    using RecordList=std::vector<Record>;
    ReadCSV()=default;
    /*!
     * Change the separator character
     * The default is the comma
     */
    void setSeparator(CharT s=CharT{','}){sep=s;}

    /*
     * When reading a token protected by double quotation marks (" ") strip the marks
     * from the stored tokens (default false: quotations are kept)
     */
    void stripQuotation(bool strip=false){stripApexes=strip;}
    /*!
     * Change the minimum number of Tokens in a record (default 1)
     * If we read a number of tokens lesss than the minumum, the record is
     * completed with empy cells.
     * If different from 1, it is also used to preallocate memory
    */
    void setMinTokens(unsigned n=1u){minTokens=n;}
    /*!
     * Change the minimum number of records
     * if different from 0 it is used to preallocate memory
     */
    void setMinRecords(unsigned n=0u){minRecords=n;}
    //! Change the number of lines to be skipped (default 0)
    void setSkippedLines(unsigned s=0u){skip=s;}
    //! Change verbosity (default false)
    void setVerbose(bool v=false){verbose=v;}
    /*!
     * Get the reead records. It returns a matrix
     * so that using [i][j] you have the j-th token of record i
     * (remember, starting from 0).
     * @return The matrix containing all redords and tokens
     */
    RecordList getTokens(){return allRecords;}
    //! Clear all records and tokens
    void clear(){allRecords.clear();allRecords.shrink_to_fit();}
    /*!
     * Get all tokens form an input stream
     * @param in The input stream
     */
    void read(std::basic_istream<CharT> & in);
    //! Writes all the tokens on a file
    void writeAllRecords(std::basic_ostream<CharT> &) const;
  private:
    CharT sep=CharT{','};
    unsigned int minTokens=1u;
    unsigned int skip=0u;
    bool verbose=false;
    RecordList allRecords;
    unsigned int minRecords=0u;
    bool stripApexes=false;
    static constexpr CharT textMark=CharT{'"'};
    void writeRecord(std::basic_ostream<CharT> &, Record const &)const;
  };

}

#endif
