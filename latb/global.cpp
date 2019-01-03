#include <cstdlib>
#include <sstream>
#include <limits>
#include <cmath>
#include "purescript.h"

FOREIGN_BEGIN( Global )

exports["readFloat"] = [](const boxed& s_) -> boxed {
    const std::string s = unbox<string>(s_);
    std::istringstream i(s);
    double x;
    i >> x;
    x = i.fail() || !i.eof() ? std::numeric_limits<double>::quiet_NaN() : x;
    return x;
};

exports["isFinite"] = [](const boxed& x) -> boxed {
  return isfinite(unbox<double>(x));
};

exports["infinity"] = std::numeric_limits<double>::infinity();

FOREIGN_END

