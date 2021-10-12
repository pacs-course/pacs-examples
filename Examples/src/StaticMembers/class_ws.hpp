#ifndef __WS_HH_
#define __WS_HH_
/*!
  @brief a simple example of use static variables.
  @details This class stores a double, counts how many objects
  have been created and is able to access the average of the
  value of all elements of the class
*/
class WS
{
public:
  //! The default constructor.
  /*!
    It is also a constructor that takes an argument, the value
    assigned to the object. Definition in the cpp file.
  */
  WS(double const &x = 0.0);
  //! Copy constructor.
  WS(const WS &);
  //! Assignment operator.
  WS &operator=(WS const &);
  //! Destructor.
  /*!
    I need to define it since it has to change the value of the
    static variables. Definition in the cpp file
  */
  ~WS();
  //! Return the stored value.
  double
  getValue() const
  {
    return myValue;
  };
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
    If you want to initialize in class a static member variable you have to
    declare it inline!
   */
  inline static int counter = 0;
  //! Stores the sum of the values.
  /*!
      If you want to initialize in class a static member variable you have to
     declare it inline!
   */
  inline static double sum = 0.0;
  //! The object value.
  /*!
    It could be in-class initialised, but since the constructor
    takes a default value it will be initialized by the constructor
    directly.
   */
  double myValue;
};

#endif
