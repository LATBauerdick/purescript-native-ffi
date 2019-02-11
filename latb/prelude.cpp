#include <iostream>
#include "purescript.h"
FOREIGN_BEGIN( Prelude_Extended )

// foreign import unsafePerformEffect :: forall a. Effect a -> a
//
exports["unsafeLog"] = [](const boxed& s_) -> boxed {
  const string s(unbox<string>(s_));
  std::cout << "TRACE: " << s << std::endl;
  return s;
};

// foreign import toCharCode :: Char -> Int
exports["toCharCode"] = [](const boxed& c_) -> boxed {
  const int i = unbox<string>(c_)[0];
  return i;
};

FOREIGN_END

FOREIGN_BEGIN( Control_Bind )

// foreign import arrayBind :: forall a b. Array a -> (a -> Array b) -> Array b
exports["arrayBind"] = [](const boxed& xs_) -> boxed {
  const auto& xs = unbox<array_t>(xs_);
  return [=](const boxed& f_) -> boxed {
    array_t result;
    for (auto it = xs.cbegin(), end = xs.cend(); it != end; it++) {
      auto ys_ = unbox<array_t>(f_(*it));
      const array_t& ys = ys_;
      for (auto iit = ys.begin(), iend = ys.end(); iit != iend; iit++) {
        result.emplace_back(std::move(*iit));
      }
    }
    return result;
  };
};

FOREIGN_END

FOREIGN_BEGIN( Record_Unsafe )

//-- | Unsafely gets a value from a record, using a string for the key.
//-- |
//-- | If the key does not exist this will cause a runtime error elsewhere.
//foreign import unsafeGet :: forall r a. String -> Record r -> a
exports["unsafeGet"] = [](const boxed& key_) -> boxed {
  const string key(unbox<string>(key_));
  return [=](const boxed& rec_) -> boxed {
    return key;
};
};

FOREIGN_END
