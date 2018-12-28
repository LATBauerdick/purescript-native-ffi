#include <limits>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <purescript.h>

namespace Data_Array {
    using namespace purescript;
    static auto cons(const boxed& e_) -> boxed {
      return [=](const boxed& xs_) -> boxed {
        array_t xs = unbox<array_t>(xs_);
        array_t result(xs);
        result.emplace_front(e_);
        return result;
      };
    };
}

FOREIGN_BEGIN( Data_Array )

// foreign import fromFoldableImpl
//   :: forall f a
//    . (forall b. (a -> b -> b) -> b -> f a -> b)
//   -> f a
//   -> Array a
exports["fromFoldableImpl"] = [](const boxed& foldr_) -> boxed {
  return [=](const boxed& xs_) -> boxed {
    // Since 'any::array' is actually a std::deque, we can cheat and
    // just pass it through
    //
    const auto& xs = unbox<array_t>(xs_);
    /* for(auto const& value: xs) { */
    /*     std::cout << value << ","; */
    /* } */
    /* std::cout << "\n"; */
    return array_t(xs);//foldr_($cons)(array_t())(xs);
  };
};
// foreign import cons :: forall a. a -> Array a -> Array a
//
exports["cons"] = [](const boxed& e_) -> boxed {
  return [=](const boxed& xs_) -> boxed {
    array_t xs = unbox<array_t>(xs_);
    array_t result(xs);
    result.emplace_front(e_);
    return result;
  };
};


FOREIGN_END
