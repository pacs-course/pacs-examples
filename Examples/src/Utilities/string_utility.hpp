#ifndef HHH_STRING_UTILITIES
#define HHH_STRING_UTILITIES
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
/*
  Part of this software has been taken from internet. I thank the 
  original (unknwown) authors.
  It has been changed to make it more readable using some C++11 fetures. Even if may be less efficient
  than original code.
 */
namespace Utility{
  
  //! Trim a string from left eliminating trailing spaces
  /**
     @param a A string
     @return the trimmed string

     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
  inline std::string &ltrim(std::string &s)
  {
    auto const & loc = std::locale();
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
				    [&loc]
				    (std::string::const_reference c)
				    {
				      return ! std::isspace(c,loc);
				    }
				    )
	    );
    return s;
  }
  
  
  //! Trims a string from right eliminating trailing spaces
  /**
     @param a A string
     @return the trimmed string

     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
    inline std::string &rtrim(std::string &s) {
    auto const & loc = std::locale();
    s.erase(std::find_if(s.rbegin(), s.rend(), 
			 [&loc]
			 (std::string::const_reference c)
			 {
			   return !std::isspace(c,loc);
			 }).base(),
	    s.end()
	    );
    return s;
  }
  
  //! trim from both ends
  /**
     @param a A string
     @return the trimmed string
     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
  static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
  }
  
  //! Converting a whole string using current locale
  /**
     @param s A string
     @return the modified string
     @note s is also returned trimmed: after a call to 
     toupper(a) string a is upper case!! 
   */

  /**@{*/
  std::string toupper(std::string & s);
  std::string tolower(std::string & s);
  /**@}*/
/*!
 * This is an helper class to allow the handling of a stringstream that holds a whole text file in a buffer. It is necessary
 * in order to have proper memory handling. No copy operations are available (but it can be moved) since it is meant only as to be an helper
 * class and the copy could be ambiguous (do you want deep or shallow copy of the text buffer?).
 *
 * You can extract the read data from the buffer using the streaming operator or by accessing (by reference) the stored
 * stringstream
 *
 *  Reading the file is faster since it is read in a whole block. At the price of more memory usage.
 *
 */
  class GlobbedTextReader
  {
  public:
    /*!
     * Class is default constructible
     */
    GlobbedTextReader()=default;
    /*!
     * This constructor also reads the content of a text file
     * @param fileName the name of the file
     * @throw a std runtime exception if file cannot be opened
     */
    GlobbedTextReader(std::string const & fileName);
    /*!
     * Reads the content of a whole text file
     * @param fileName the file name
     * @throw a std runtime exception if file cannot be opened
     */
    void read(std::string const & fileName);
    /*!
     * Allow extraction of the stared test using the classical streaming operator
     *
     * @tparam T the data type
     * @param data The data that will be read
     * @return This object
     */
    template <class T>
    GlobbedTextReader & operator>>(T & data);
    /*!
     * You can get the enclosed string stream to operate on it at wish.
     * @note you should extract it only by reference.
     * @return a reference to the stringstrem associated to the text buffer (const version)
     */
    std::stringstream const & globbedText() const {return MyGlobbedText;}
    /*!
      * You can get the enclosed string stream to operate on it at wish.
     * @note you should extract it only by reference
     *
     * @return the stringstrem associated to the text buffer
     */
    std::stringstream & globbedText() {return MyGlobbedText;}
    /*!
     * Release the buffer.
     *
     * This member function is set only to allow releasing memory after you have finished operated with the stringstream
     * After calling close the object is not usable anymore. Accessing the stringstream leads to undefined behaviour So use with care
     */
    void close(){MyBuffer.release(); MySize=0;}
    /*!
     * @return the size of the internal buffer (in bytes)
     */
    std::size_t size()const {return MySize;}
  private:
    //! the underlying stringstream
    std::stringstream MyGlobbedText;
    //! The buffer holding the read text, wrapped in a unique pointer.
    std::unique_ptr<char[]> MyBuffer;
    //!
    std::size_t MySize=0;
    //! Utility to get the buffer (used only internally)
    char * buffer(){return MyBuffer.get();}
    //! Reset reading from start. Used only internally
    void setAtStart(){MyGlobbedText.seekg (0, std::ios::beg);}
  };


  template<class T>
  GlobbedTextReader & GlobbedTextReader::operator>>(T & data)
  {
    this->MyGlobbedText>>data;
    return *this;
  }



  /*!
   * It chops a stringstream into a vector of strings, one for each line stored in the stringstream buffer
   *
   * @param sstream the stringstream (it must be in a valid state!)
   * @return the vector with strings
   */
  std::vector<std::string> chop (std::stringstream  & sstream);

}
#endif
