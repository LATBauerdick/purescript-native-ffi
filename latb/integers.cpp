
#include <iostream>
#include <sstream>
#include <cmath>


#include <purescript.h>

FOREIGN_BEGIN( Data_Int )

//-- | Creates an `Int` from a `Number` value. The number must already be an
//-- | integer and fall within the valid range of values for the `Int` type
//-- | otherwise `Nothing` is returned.
//fromNumber :: Number -> Maybe Int
//fromNumber = fromNumberImpl Just Nothing
//foreign import fromNumberImpl
//  :: (forall a. a -> Maybe a)
//  -> (forall a. Maybe a)
//  -> Number
//  -> Maybe Int

  exports["fromNumberImpl"] = [](const boxed& just_) -> boxed {
    return [=](const boxed& nothing_) -> boxed {
      return [=](const boxed& x_) -> boxed {
        auto x = unbox<double>(x_);
        return (x >= std::numeric_limits<int>::min() &&
                x <= std::numeric_limits<int>::max() &&
            std::trunc(x) == x) ? just_(std::lround(x)) : nothing_;
      };
    };
  };

//-- | Like `fromString`, but the integer can be specified in a different base.
//-- |
//-- | Example:
//-- | ``` purs
//-- | fromStringAs binary      "100" == Just 4
//-- | fromStringAs hexadecimal "ff"  == Just 255
//-- | ```
//fromStringAs :: Radix -> String -> Maybe Int
//fromStringAs = fromStringAsImpl Just Nothing
// foreign import fromStringAsImpl
//   :: (forall a. a -> Maybe a)
//   -> (forall a. Maybe a)
//   -> Radix
//   -> String
//   -> Maybe Int
//
exports["fromStringAsImpl"] = [](const boxed& just_) -> boxed {
  return [=](const boxed& nothing_) -> boxed {
    return [=](const boxed& radix_) -> boxed {
      auto radix = unbox<int>(radix_);
      return [=](const boxed& s_) -> boxed {
        const string& s = unbox<string>(s_);
        int i;
        std::stringstream ss;
        switch (radix) {
          case 2:
            try {
              const auto n = std::bitset<32>(s).to_ulong();
              return n <= std::numeric_limits<int>::max() ? just_(static_cast<int>(n)) : nothing_;
            } catch (std::exception&) {
              return nothing_;
            }
            break;
          case 8:
            ss << std::oct << s;
            break;
          case 10:
            ss << std::dec << s;
            break;
          case 16:
            ss << std::hex << s;
            break;
          default:
            std::cerr << "fromStringAsImpl radix " << radix << " not supported" << std::endl;
            return nothing_;
            break;
        }
        ss >> i;
        return ss.fail() || !ss.eof() ? nothing_ : just_(i);
      };
    };
  };
};
FOREIGN_END
