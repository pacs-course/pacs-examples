#ifndef __HH_EXTENSIONS_HH__
#define __HH_EXTENSIONS_HH__
#include <utility>
#include <concepts>
namespace apsc
{
/*!
 * This class is just a class to collect components. A sort of composer class
 * via variadic templates
 * @tparam Extensions the set of components to be assembled
 * @pre components must be default initializable and copy-contructible
 * @note I have used concepts to cehct the preconditions. Not how to use fold expression to combine
 * more than one concepts with variadic parameters!
 */
  template <typename... Extensions>
  requires ((std::default_initializable<Extensions> && ...) and (std::copy_constructible<Extensions> && ...) )
  struct Extension : public Extensions...
  {
    //! @brief default constructor. It constucts all components with theit own default constructor
    /*!
     * The explicit declaration of the default constructor is needed here to overload
     * the other constructor. Indeed the other constructor may potentially act as default constructor
     * when T is empty!
     */
    Extension()=default;
    /*!
     * @fn  Extension(T&&...)
     * @brief COntructor taking Extensions objects
     *
     * @tparam T The extensions
     * @param ext The Extensions objects
     *
     * */
    template <typename... T>
    Extension(T &&... ext)
      : Extensions(std::forward<T>(ext))...{};
};

} // namespace apsc
#endif
