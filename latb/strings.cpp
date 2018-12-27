#include <algorithm>
#include <locale>
#include <codecvt>
#include <regex>
#include <iostream>

#include <purescript.h>

static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;

FOREIGN_BEGIN( Data_String_Common )

// foreign import replace :: Pattern -> Replacement -> String -> String
//
exports["replace"] = [](const boxed& s1) -> boxed {
  return [=](const boxed& s2) -> boxed {
    return [=](const boxed& s3) -> boxed {
      string ss(unbox<string>(s3));
      string sp(unbox<string>(s1));
      const auto found = ss.find(unbox<string>(sp));
      return found == string::npos ? ss : ss.replace(found, sp.size(), unbox<string>(s2));
    };
  };
};

FOREIGN_END


FOREIGN_BEGIN( Data_String_CodeUnits )


// foreign import length :: String -> Int
//
exports["length"] = [](const boxed& s_) -> boxed {
  const string& s = unbox<string>(s_);
  const auto sz = s.size();
  assert(sz <= std::numeric_limits<int>::max());
  return sz;
};

// foreign import singleton :: Char -> String
//
exports["singleton"] = [](const boxed& c_) -> boxed {
  return unbox<string>(c_);
};

// foreign import toCharArray :: String -> Array Char
//
exports["toCharArray"] = [](const boxed& s_) -> boxed {
  array_t cs;
  const string& s = unbox<string>(s_);
  /* const auto utf32 = utf32conv.from_bytes(s); */
  /* auto it = utf32.cbegin(); */
  /* auto end = utf32.cend(); */
  /* for (; it != end; it++) { */
  /*   cs.emplace_back(*it); */
  /* }; */
  for (char c : s) {
    cs.emplace_back(c);
  };
  return cs;
};

// foreign import fromCharArray :: Array Char -> String
//
exports["fromCharArray"] = [](const boxed& xs_) -> boxed {
    array_t xs = unbox<array_t>(xs_);
    std::u32string utf32;
    utf32.reserve(xs.size());
    for (auto it = xs.cbegin(), end = xs.cend(); it != end; it++) {
      utf32.push_back(unbox<char>(*it));
    }
    return utf32conv.to_bytes(utf32);
    /* return "test"; */
};

// foreign import drop :: Int -> String -> String
//
exports["drop"] = [](const boxed& n_) -> boxed {
  return [=](const boxed& s_) -> boxed {
    auto n = unbox<int>(n_);
    const string& s = unbox<string>(s_);
    return n <= 0 ? s : n >= s.size() ? "" : s.substr(n);
  };
};

// foreign import countPrefix :: (Char -> Boolean) ->  String -> Int
//
exports["countPrefix"] = [](const boxed& f_) -> boxed {
  return [=](const boxed& s_) -> boxed {
    const string& s = unbox<string>(s_);
    size_t i = 0;
    while ( i<s.length() && unbox<bool>(f_(boxed(s.substr(i, 1)))) ) i++;
    return i;
  };
};
/* PURS_FFI_FUNC_2(Data_String_CodeUnits_countPrefix, f, s0, { // only works with ASCII... */
/*   const char * s = purs_any_get_string(s0); */
/*   int i = 0; */
/*   while ( i<strlen(s) && purs_any_is_true(purs_any_app(f, purs_any_char_new(s[i])))) { */
/*     i++; */
/*   } */
/*   return purs_any_int_new(i); */
/* }); */


// foreign import _indexOf :: (forall a. a -> Maybe a) -> (forall a. Maybe a) -> Pattern -> String -> Maybe Int
//
exports["_indexOf"] = [](const boxed& just_) -> boxed {
  return [=](const boxed& nothing_) -> boxed {
    return [=](const boxed& p_) -> boxed {
      return [=](const boxed& s_) -> boxed {
        const string& p = unbox<string>(p_);
        const string& s = unbox<string>(s_);
        std::size_t found = s.find(p);
        return found==std::string::npos ? nothing_ : just_(found);
      };
    };
  };
};
/*   const char * p = purs_any_get_string(p0); */
/*   const char * s = purs_any_get_string(s0); */
/*   const char * found = strstr(s, p); */
/*   if (!found) return nothing; */
/*   return purs_any_app(just, purs_any_int_new(found-s)); */

FOREIGN_END


/* PURS_FFI_FUNC_1(Data_String_CodeUnits_length, x, { */
/*     return purs_any_int_new(strlen( purs_any_get_string(x))); */
/* }); */

/* PURS_FFI_FUNC_1(Data_String_CodeUnits_fromCharArray, xs, { */
/*   const purs_vec_t * zs = purs_any_get_array(xs); */
/*   const purs_any_t * tmp; */
/*   int i; */
/*   const int mxbytes = 4; // ???? I have no idea... */
/*   char * out = (char *) malloc(mxbytes*(zs->length) + 1); */
/*   out[0] = '\0'; */
/*   char * s = (char *) malloc(mxbytes + 1); */
/*   purs_vec_foreach(zs, tmp, i) { */
/*     utf8_int32_t chr = purs_any_get_char(tmp); */
/*     size_t bytes = utf8codepointsize(chr); */
/*     utf8catcodepoint(s, chr, bytes); */
/*     s[bytes + 1] = '\0'; */
/*     strcat(out, s); */
/*   } */
/*   free(s); */
/*   return purs_any_string_new(out); */
/* }); */

/* PURS_FFI_FUNC_1(Data_String_CodeUnits_toCharArray, s, { */
/*   const char * ts = purs_any_get_string(s); */
/*   const purs_any_int_t count = strlen(ts); */
/*   purs_vec_t * result = (purs_vec_t *) purs_vec_new(); */
/*   for (purs_any_int_t i = 0; i < count; i++) { */
/*     char c = ts[i]; */
/*     purs_vec_push_mut(result, purs_any_char_new(c)); */
/*   } */
/*   return purs_any_array_new(result); */
/* }); */

/* PURS_FFI_FUNC_1(Data_String_CodeUnits_singleton, c, { */
/*   const int mxbytes = 4; // ???? I have no idea... */
/*   char * s = (char *) malloc(mxbytes + 1); */
/*   utf8_int32_t chr = purs_any_get_char(c); */
/*   size_t bytes = utf8codepointsize(chr); */
/*   utf8catcodepoint(s, chr, bytes); */
/*   s[bytes + 1] = '\0'; */
/*   return purs_any_string_new(s); */
/* }); */

/* PURS_FFI_FUNC_2(Data_String_CodeUnits_drop, n0, s0, { //??? does not work with unicode chars */
/*   size_t n = purs_any_get_int(n0); */
/*   if (n <= 0) return s0; //???? not sure this is ok re/ memory allocation etc? */
/*   const char * s = purs_any_get_string(s0); */
/*   size_t sl = strlen(s); */
/*   if (n >= sl) return purs_any_string_new(""); */
/*   size_t srl = sl-n; */
/*   char * sr = (char *) malloc(srl + 1); */
/*   strlcpy(sr, s+n, srl+1); // strlcpy makes sure there's a null character at the end */
/*   const purs_any_t * out = purs_any_string_new(sr); */
/*   free(sr); */
/*   return out; */
/* }); */

/* PURS_FFI_FUNC_2(Data_String_CodeUnits_countPrefix, f, s0, { // only works with ASCII... */
/*  // foreign import countPrefix :: (Char -> Boolean) ->  String -> Int */
/*   const char * s = purs_any_get_string(s0); */
/*   int i = 0; */
/*   while ( i<strlen(s) && purs_any_is_true(purs_any_app(f, purs_any_char_new(s[i])))) { */
/*     i++; */
/*   } */
/*   return purs_any_int_new(i); */
/* }); */

/* PURS_FFI_FUNC_4(Data_String_CodeUnits__indexOf, just, nothing, p0, s0, { // only works with ASCII... */
/* // foreign import _indexOf :: (forall a. a -> Maybe a) -> (forall a. Maybe a) -> Pattern -> String -> Maybe Int */
/*   const char * p = purs_any_get_string(p0); */
/*   const char * s = purs_any_get_string(s0); */
/*   const char * found = strstr(s, p); */
/*   if (!found) return nothing; */
/*   return purs_any_app(just, purs_any_int_new(found-s)); */
/* }); */

