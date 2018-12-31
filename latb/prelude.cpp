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
