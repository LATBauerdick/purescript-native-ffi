#include <vector>
#include <purescript.h>

FOREIGN_BEGIN( Data_Unfoldable )

// foreign import unfoldrArrayImpl
//   :: forall a b
//    . (forall x. Maybe x -> Boolean)
//   -> (forall x. Maybe x -> x)
//   -> (forall x y. Tuple x y -> x)
//   -> (forall x y. Tuple x y -> y)
//   -> (b -> Maybe (Tuple a b))
//   -> b
//   -> Array a
//
// unfoldr = unfoldrArrayImpl isNothing (unsafePartial fromJust) fst snd
exports["unfoldrArrayImpl"] =
          [](const boxed& isNothing_) -> boxed {
  return [=](const boxed& fromJust_) -> boxed {
  return [=](const boxed& fst_) -> boxed {
  return [=](const boxed& snd_) -> boxed {
  return [=](const boxed& f_) -> boxed {
  return [=](const boxed& b_) -> boxed {
  array_t result;
  auto b = b_;
  while (true) {
    const auto maybe = f_(b);
    if (unbox<bool>(isNothing_(maybe))) {
      return result;
    }
    const auto tuple = fromJust_(maybe);
    result.push_back(fst_(tuple));
    b = snd_(tuple);
  }
};
};
};
};
};
};
FOREIGN_END
