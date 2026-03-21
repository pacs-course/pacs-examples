#include "fun1_fixed.hpp"

namespace myspace {
    //! Same implementation logic as the failing variant.
    double fun3(double x) {
        return 2. + myspace::fun1(x) - myspace::fun2(x);
    }
}
