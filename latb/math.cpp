
#include <cmath>
#include "purescript.h"

namespace Math {
  using namespace purescript;
  constexpr double pi = 3.141592653589793; //atan2(0, -1);
};


FOREIGN_BEGIN( Math )

exports["pi"] = pi;

exports["sin"] = [](const boxed& x) -> boxed {
    return std::sin(unbox<double>(x));
};
exports["cos"] = [](const boxed& x) -> boxed {
    return std::cos(unbox<double>(x));
};
exports["atan"] = [](const boxed& x) -> boxed {
    return std::atan(unbox<double>(x));
};
exports["atan2"] = [](const boxed& y_) -> boxed {
  const auto y = unbox<double>(y_);
  return [=](const boxed& x_) -> boxed {
    return std::atan2(y, unbox<double>(x_));
  };
};
exports["sqrt"] = [](const boxed& x) -> boxed {
    return std::sqrt(unbox<double>(x));
};
exports["floor"] = [](const boxed& x) -> boxed {
    return std::floor(unbox<double>(x));
};
exports["log"] = [](const boxed& x) -> boxed {
    return std::log(unbox<double>(x));
};

FOREIGN_END
