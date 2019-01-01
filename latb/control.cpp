
#include <purescript.h>

FOREIGN_BEGIN( Control_Monad_ST_Internal )

// foreign import pure_ :: forall r a. a -> ST r a
exports["pure_"] = [](const boxed& a_) -> boxed {
  return [=]() -> boxed {
    return a_;
  };
};

// Loop over a consecutive collection of numbers
//
// `ST.for lo hi f` runs the computation returned by the function `f` for each
// of the inputs between `lo` (inclusive) and `hi` (exclusive).
// foreign import for :: forall r a. Int -> Int -> (Int -> ST r a) -> ST r Unit

exports["for"] = [](const boxed& lo_) -> boxed {
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
