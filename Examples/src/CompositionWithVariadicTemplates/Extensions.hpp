#ifndef __HH_EXTENSIONS_HH__
#define __HH_EXTENSIONS_HH__
#include <utility>
namespace apsc
{
  template <typename... Extensions>
  struct Extension : public Extensions...
  {
    //! @brief default constructor. It constucts all components with theit own default constructor
    //!
    //! @pre Extensions must be default constructable
    Extension()=default;
    /*!
     * @fn  Extension(T&&...)
     * @brief COntructor taking Exensions objects
     *
     * @tparam T The extensions
     * @param ext The Extensions objects
     *
     * @pre it assumes that all Exensionss are copy/move constructable
     */
    template <typename... T>
    Extension(T &&... ext)
      : Extensions(std::forward<T>(ext))...{};
};

} // namespace apsc
#endif
