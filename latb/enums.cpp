
#include <purescript.h>

FOREIGN_BEGIN( Data_Enum )

// foreign import toCharCode :: Char -> Int
exports["toCharCode"] = [](const boxed& c_) -> boxed {
  const int i = unbox<char>(c_);
  return i;
};

FOREIGN_END
