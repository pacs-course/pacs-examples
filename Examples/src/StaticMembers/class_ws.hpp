#ifndef __WS_HH_
#define __WS_HH_
/*!
  @brief a simple example of static variables.
  @details This class counts how many objects
  have been created and is able to access the mean of the
  value of all elements of the class
*/
class WS{
public:
  //! It is also a default constructor. 
  /*! It may take as argument the value attributed to the
   WS object
  */
  WS(double value=0.0);
  //! Copy constructor.
  WS(const WS&);
  //! Assignment operator.
  WS & operator =(WS const &);
  //! Destructor. 
  /*! 
    I need it explicitely since it has to change the value of the
    static variables.
  */
  ~WS();
  //! Returns the average value of the existing objects.
  /*! 
    It is callable from objects of the class. It is defined in class
    so it is eligible for inlining.
   */
  double getAverage() const{
    return WS::sum/static_cast<double>(WS::counter);}
  //! Return the stored value.
  double getValue() const{return myValue;};
  //! Returns the average value.
  /*! Being a static method it doeds not require an object
    to be called. The const keyword is not usable with static
    methods because they do not operate on objects.
  */
  static double average();
  //! Returns how many WS objects we have.
  /*! Being a static method it does not require an object
    to be called.
  */
  static int howmany();
  //! Resets the sum of values to zero.
  static void reset();  
private:
  //! Counts the number of WS objects.
  /*!
    Cannot be initialised in class since it is not const.
   */
  static int counter; 
  //! Stores the sum of the values.
  /*!
     Cannot be initialised in class since it is not const and it is not an 
     int. (so even if it were const we could not initilize it in class).
   */
  static double sum;
  //! The object value.
  double myValue;
};

#endif
