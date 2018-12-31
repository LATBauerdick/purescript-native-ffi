
#include <purescript.h>

FOREIGN_BEGIN( Control_Monad_ST_Internal )

// foreign import pure_ :: forall r a. a -> ST r a
exports["pure_"] = [](const boxed& a_) -> boxed {
  return [=]() -> boxed {
    return a_;
  };
};

FOREIGN_END
