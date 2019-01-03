
#include <limits>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <purescript.h>

FOREIGN_BEGIN( Effect )

//-- | Loop over a consecutive collection of numbers.
//-- |
//-- | `forE lo hi f` runs the computation returned by the function `f` for each
//-- | of the inputs between `lo` (inclusive) and `hi` (exclusive).
//foreign import forE :: Int -> Int -> (Int -> Effect Unit) -> Effect Unit
exports["forE"] = [](const boxed& lo_) -> boxed {
  return [=](const boxed& hi_) -> boxed {
    return [=](const boxed& f_) -> boxed {
      return [=]() -> boxed {
        const auto lo = unbox<int>(lo_);
        const auto hi = unbox<int>(hi_);
        for (int i = lo; i < hi; i++) {
          f_(boxed(i))();
        };
        return boxed();
      };
    };
  };
};

FOREIGN_END



