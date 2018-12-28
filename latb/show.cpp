#include "purescript.h"
#include <string>         // std::string, std::u32string
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <cstdint>        // std::uint_least32_t

FOREIGN_BEGIN( Data_Show )

exports["showCharImpl"] = [](const boxed& c_) -> boxed {
    std::string c =  "\'";
    c += unbox<char>(c_);
    c += "\'";
    /* if (c < 128) { */
    /*   s.push_back(c); */
    /* } else { */
    /*   static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv; */
    /*   s += utf32conv.to_bytes(c); */
    /* } */
    /* s.push_back('\''); */
    return c;
};

FOREIGN_END
