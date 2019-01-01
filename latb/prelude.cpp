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
