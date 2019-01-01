
#include <cmath>
#include "purescript.h"

// Tested with package v2.1.1

FOREIGN_BEGIN( Math )

exports["sqrt"] = [](const boxed& x) -> boxed {
    return std::sqrt(unbox<double>(x));
};

FOREIGN_END
