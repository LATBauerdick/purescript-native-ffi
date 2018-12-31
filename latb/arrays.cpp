#include <limits>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <purescript.h>

namespace Data_Array {
    using namespace purescript;
    static auto cons_(const boxed& e_) -> boxed {
      return [=](const boxed& xs_) -> boxed {
        array_t xs = unbox<array_t>(xs_);
        array_t result(xs);
        result.emplace_front(e_);
        return result;
      };
    };
}

FOREIGN_BEGIN( Data_Array )

// foreign import replicate :: forall a. Int -> a -> Array a
//
exports["replicate"] = [](const boxed& n_) -> boxed {
  const auto n = unbox<int>(n_);
  return [=](const boxed& a) -> boxed {
    return array_t(n < 0 ? 0 : n, a);
  };
};

// foreign import fromFoldableImpl
//   :: forall f a
//    . (forall b. (a -> b -> b) -> b -> f a -> b)
//   -> f a
//   -> Array a
exports["fromFoldableImpl"] = [](const boxed& foldr_) -> boxed {
  return [=](const boxed& xs_) -> boxed {
    const auto& xs = unbox<array_t>(xs_);
    return foldr_(cons_)(array_t())(xs);
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

// foreign import zipWith
//   :: forall a b c
//    . (a -> b -> c)
//   -> Array a
//   -> Array b
//   -> Array c
//
exports["zipWith"] = [](const boxed& f_) -> boxed {
  return [=](const boxed& xs_) -> boxed {
  return [=](const boxed& ys_) -> boxed {
    array_t xs = unbox<array_t>(xs_);
    array_t ys = unbox<array_t>(ys_);
    const auto length = std::min(xs.size(), ys.size());
    array_t result;
    auto itx = xs.cbegin();
    auto ity = ys.cbegin();
    for (auto i = 0; i < length; i++) {
      result.emplace_back(f_(*itx++)(*ity++));
    }
    return result;
  };
  };
};

FOREIGN_END

FOREIGN_BEGIN( Data_Array_ST )
// foreign import copyImpl :: forall h a b. a -> ST h bi
//
exports["copyImpl"] = [](const boxed& xs_) -> boxed {
  const auto& xs = unbox<array_t>(xs_);
  return [=]() -> boxed {
    return array_t(xs);
  };
};
FOREIGN_END

