template <typename T, typename S> bool in(T x, S s, S t)
{
  return s <= x && x <= t;
}
bool isUtf8Tail(unsigned char ch) { return in(ch, 0x80u, 0xbfu); }

bool isUtf8(const std::string& str)
{
  for (std::string::const_iterator s = str.begin(), eos = str.end(); s != eos;
       ++s) {
    unsigned char firstChar = *s;
    // See ABNF in http://tools.ietf.org/search/rfc3629#section-4
    if (in(firstChar, 0x20u, 0x7eu) || firstChar == 0x08u || // \b
        firstChar == 0x09u ||                                // \t
        firstChar == 0x0au ||                                // \n
        firstChar == 0x0cu ||                                // \f
        firstChar == 0x0du                                   // \r
    ) {
      // UTF8-1 (without ctrl chars)
    }
    else if (in(firstChar, 0xc2u, 0xdfu)) {
      // UTF8-2
      if (++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (0xe0u == firstChar) {
      // UTF8-3
      if (++s == eos || !in(static_cast<unsigned char>(*s), 0xa0u, 0xbfu) ||
          ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (in(firstChar, 0xe1u, 0xecu) || in(firstChar, 0xeeu, 0xefu)) {
      // UTF8-3
      if (++s == eos || !isUtf8Tail(*s) || ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (0xedu == firstChar) {
      // UTF8-3
      if (++s == eos || !in(static_cast<unsigned char>(*s), 0x80u, 0x9fu) ||
          ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (0xf0u == firstChar) {
      // UTF8-4
      if (++s == eos || !in(static_cast<unsigned char>(*s), 0x90u, 0xbfu) ||
          ++s == eos || !isUtf8Tail(*s) || ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (in(firstChar, 0xf1u, 0xf3u)) {
      // UTF8-4
      if (++s == eos || !isUtf8Tail(*s) || ++s == eos || !isUtf8Tail(*s) ||
          ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else if (0xf4u == firstChar) {
      // UTF8-4
      if (++s == eos || !in(static_cast<unsigned char>(*s), 0x80u, 0x8fu) ||
          ++s == eos || !isUtf8Tail(*s) || ++s == eos || !isUtf8Tail(*s)) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  return true;
}




bool isAlpha(const char c)
{
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool isDigit(const char c) { return '0' <= c && c <= '9'; }

bool isHexDigit(const char c)
{
  return isDigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

bool isHexDigit(const std::string& s)
{
  for (const auto& c : s) {
    if (!isHexDigit(c)) {
      return false;
    }
  }
  return true;
}




std::string toHex(const unsigned char* src, size_t len)
{
  std::string out(len * 2, '\0');
  std::string::iterator o = out.begin();
  const unsigned char* last = src + len;
  for (const unsigned char* i = src; i != last; ++i) {
    *o = (*i >> 4);
    *(o + 1) = (*i) & 0x0fu;
    for (int j = 0; j < 2; ++j) {
      if (*o < 10) {
        *o += '0';
      }
      else {
        *o += 'a' - 10;
      }
      ++o;
    }
  }
  return out;
}

std::string toHex(const char* src, size_t len)
{
  return toHex(reinterpret_cast<const unsigned char*>(src), len);
}

std::string toHex(const std::string& src)
{
  return toHex(reinterpret_cast<const unsigned char*>(src.c_str()), src.size());
}



unsigned int hexCharToUInt(unsigned char ch)
{
  if ('a' <= ch && ch <= 'f') {
    ch -= 'a';
    ch += 10;
  }
  else if ('A' <= ch && ch <= 'F') {
    ch -= 'A';
    ch += 10;
  }
  else if ('0' <= ch && ch <= '9') {
    ch -= '0';
  }
  else {
    ch = 255;
  }
  return ch;
}


std::string secfmt(time_t sec)
{
  time_t tsec = sec;
  std::string str;
  if (sec >= 3600) {
    str = fmt("%" PRId64 "h", static_cast<int64_t>(sec / 3600));
    sec %= 3600;
  }
  if (sec >= 60) {
    str += fmt("%dm", static_cast<int>(sec / 60));
    sec %= 60;
  }
  if (sec || tsec == 0) {
    str += fmt("%ds", static_cast<int>(sec));
  }
  return str;
}


std::string toUpper(std::string src)
{
  uppercase(src);
  return src;
}

std::string toLower(std::string src)
{
  lowercase(src);
  return src;
}


void uppercase(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), toUpperChar);
}

void lowercase(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), toLowerChar);
}

char toUpperChar(char c)
{
  if ('a' <= c && c <= 'z') {
    c += 'A' - 'a';
  }
  return c;
}

char toLowerChar(char c)
{
  if ('A' <= c && c <= 'Z') {
    c += 'a' - 'A';
  }
  return c;
}







