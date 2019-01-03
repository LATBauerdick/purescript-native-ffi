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

//uncons :: forall a. Array a -> Maybe { head :: a, tail :: Array a }
//uncons = uncons' (const Nothing) \x xs -> Just { head: x, tail: xs }
//foreign import uncons'
//  :: forall a b
//   . (Unit -> b)
//  -> (a -> Array a -> b)
//  -> Array a
//  -> b
exports["uncons'"] = [](const boxed& empty_) -> boxed {
  return [=](const boxed& next_) -> boxed {
    return [=](const boxed& xs_) -> boxed {
      array_t xs = unbox<array_t>(xs_);
      return xs.empty() ? empty_(boxed()) : next_(boxed(xs.front()))(boxed(array_t(xs.cbegin()+1, xs.cend())));
    };
  };
};

//-- | Keep only a number of elements from the start of an array, creating a new
//-- | array.
//-- |
//-- | ```purescript
//-- | letters = ["a", "b", "c"]
//-- |
//-- | take 2 letters = ["a", "b"]
//-- | take 100 letters = ["a", "b", "c"]
//-- | ```
//-- |
//foreign import take :: forall a. Int -> Array a -> Array a
exports["take"] = [](const boxed& n_) -> boxed {
  const auto n = std::max(unbox<int>(n_), 0);
  return [=](const boxed& xs_) -> boxed {
    const auto& xs = unbox<array_t>(xs_);
    const long long size = xs.size();
    assert(size <= std::numeric_limits<decltype(n)>::max());
    const auto length = static_cast<decltype(n)>(size);
    const auto _n   = std::min(length, n);
    return array_t(xs.cbegin(), xs.cbegin() + _n);
  };
};

//-- | Drop a number of elements from the start of an array, creating a new array.
//foreign import drop :: forall a. Int -> Array a -> Array a
exports["drop"] = [](const boxed& n_) -> boxed {
  const auto n = unbox<int>(n_);
  return [=](const boxed& xs_) -> boxed {
    const auto& xs = unbox<array_t>(xs_);
    const auto l = xs.size();
    return n <= 0 ? xs : n >= l ? array_t() : array_t(xs.cbegin()+n, xs.cend());
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

//-- | Flatten an array of arrays, creating a new array.
//-- |
//-- | ```purescript
//-- | concat [[1, 2, 3], [], [4, 5, 6]] = [1, 2, 3, 4, 5, 6]
//-- | ```
//-- |
//foreign import concat :: forall a. Array (Array a) -> Array a
exports["concat"] = [](const boxed& xss_) -> boxed {
  array_t xss = unbox<array_t>(xss_);
  array_t result;
  for (auto it = xss.cbegin(), end = xss.cend(); it != end; it++) {
    const array_t& xs = unbox<array_t>(*it);
    for (auto iit = xs.cbegin(), iend = xs.cend(); iit != iend; iit++) {
      result.emplace_back(*iit);
    }
  }
  return result;
};

//index = indexImpl Just Nothing
//foreign import indexImpl
//  :: forall a
//   . (forall r. r -> Maybe r)
//  -> (forall r. Maybe r)
//  -> Array a
//  -> Int
//  -> Maybe a
exports["indexImpl"] = [](const boxed& just_) -> boxed {
  return [=](const boxed& nothing_) -> boxed {
    return [=](const boxed& xs_) -> boxed {
      const auto& xs = unbox<array_t>(xs_);
      const long long size = xs.size();
      return [=](const boxed& i_) -> boxed {
        const auto i = unbox<int>(i_);
        return i >= size || i<0 ? nothing_ : just_(xs[i]);
      };
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

//-- | Create an empty mutable array.
//foreign import empty :: forall h a. ST h (STArray h a)
exports["empty"] = []() -> boxed {
    return array_t();
};

//-- | Append the values in an immutable array to the end of a mutable array.
//-- | Returns the new length of the mutable array.
//foreign import pushAll
//  :: forall h a
//   . Array a
//  -> STArray h a
//  -> ST h Int
exports["pushAll"] = [](const boxed& as_) -> boxed {
  const auto& as = unbox<array_t>(as_);
  return [=](const boxed& sas_) -> boxed {
    const auto& sas = unbox<array_t>(sas_);
    array_t& ssas = const_cast<array_t&>(static_cast<const array_t&>(sas));
    ssas.insert(ssas.end(), as.cbegin(), as.cend());
    auto s = static_cast<int>(ssas.size());
    assert(s <= std::numeric_limits<int>::max());
    return [=]() -> boxed {
      return s;
    };
  };
};

//-- | Read the value at the specified index in a mutable array.
//peek :: forall h a . Int -> STArray h a -> ST h (Maybe a)
//peek = peekImpl Just Nothing
// foreign import peekImpl ::
// forall h a r .
// (a -> r)
// -> r
// -> Int
// -> STArray h a
// -> (ST h r)
exports["peekImpl"] = [](const boxed& just_) -> boxed {
  return [=](const boxed& nothing_) -> boxed {
    return [=](const boxed& i_) -> boxed {
      const auto i = unbox<int>(i_);
      return [=](const boxed& xs_) -> boxed {
        const auto& xs = unbox<array_t>(xs_);
        return [=]() -> boxed {
          return i >= 0 && i < xs.size() ? just_(xs[i]) : nothing_;
        };
      };
    };
  };
};

// foreign import poke :: forall h a. Int -> a -> STArray h a -> ST h Boolean
exports["poke"] = [](const boxed& i_) -> boxed {
  const auto i = unbox<int>(i_);
  return [=](const boxed& a_) -> boxed {
    return [&](const boxed& xs_) -> boxed {
      const array_t& xs = unbox<array_t>(xs_);
      array_t& xxs = const_cast<array_t&>(static_cast<const array_t&>(xs));
      const bool ret = i >= 0 && i < xs.size();
      if (ret) { xxs[i] = a_; }
      return [=]() -> boxed {
        return ret;
      };
    };
  };
};

FOREIGN_END

