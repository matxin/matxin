#include "matxin_string_utils.h"

#include <cassert>
#include <cerrno>
#include <cwctype>

#include <vector>

int watoi(const wchar_t *buffer) { return wcstol(buffer, NULL, 10); }

void Tokenize(const wstring &str, vector<wstring> &tokens,
              const wstring &delimiters) {
  // Skip delimiters at beginning
  size_t lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter"
  size_t pos = str.find_first_of(delimiters, lastPos);

  while (pos != wstring::npos || lastPos != wstring::npos) {
    // Found a token, add it to the vector
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

void Tokenize2(const wstring &str, wstring &tokens, const wstring &delimiters) {
  // Skip delimiters at beginning
  size_t lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter"
  size_t pos = str.find_first_of(delimiters, lastPos);

  while (pos != wstring::npos || lastPos != wstring::npos) {
    // Found a token, add it to the resulting string
    tokens += str.substr(lastPos, pos - lastPos);
    // Skip delimiters
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

wstring v2s(vector<wstring> vector, wstring delimiter) {
  wstring result = L"";

  for (size_t i = 0; i < vector.size(); i++)
    result += delimiter + vector[i];

  return result;
}

vector<wstring> split_multiattrib(wstring str) {
  vector<wstring> tokens;
  vector<wstring> result;
  wstring resultStr;

  Tokenize(str, tokens);
  for (size_t i = 0; i < tokens.size(); i++) {
    vector<wstring> attribs;
    vector<wstring> valueparts;
    wstring values;

    Tokenize(tokens[i], attribs, L"=");
    Tokenize2(attribs[1], values, L"'");
    Tokenize(values, valueparts, L"\\");

    for (size_t j = 0; j < valueparts.size(); j++) {
      resultStr = attribs[0] + L"='" + valueparts[j] + L"' ";
      if (result.size() > j)
        result[j].append(resultStr);
      else {
        result.resize(valueparts.size());
        result[j] = resultStr;
      }
    }
  }

  return result;
}

wstring towstring(xmlChar const *input) {
  wstring result = L"";

  for (int i = 0, limit = xmlStrlen(input); i != limit; i++) {
    int val = 0;
    if (((unsigned char)input[i] & 0x80) == 0x0) {
      val = static_cast<wchar_t>(input[i]);
    } else if (((unsigned char)input[i] & 0xE0) == 0xC0) {
      val = (input[i] & 0x1F) << 6;
      i++;
      val += input[i] & 0x7F;
    } else if (((unsigned char)input[i] & 0xF0) == 0xE0) {
      val = (input[i] & 0x0F) << 6;
      i++;
      val += input[i] & 0x7F;
      val = val << 6;
      i++;
      val += input[i] & 0x7F;
    } else if (((unsigned char)input[i] & 0xF8) == 0xF0) {
      val = (input[i] & 0x07) << 6;
      i++;
      val += input[i] & 0x7F;
      val = val << 6;
      i++;
      val += input[i] & 0x7F;
      val = val << 6;
      i++;
      val += input[i] & 0x7F;
    } else {
      wcerr << L"UTF-8 invalid string" << endl;
      exit(EXIT_FAILURE);
    }

    result += static_cast<wchar_t>(val);
  }
  return result;
}

wstring stows(string const &str) {
  wchar_t *result = new wchar_t[str.size() + 1];
  size_t retval = mbstowcs(result, str.c_str(), str.size());
  result[retval] = L'\0';
  wstring result2 = result;
  delete[] result;
  return result2;
}

std::size_t size(const wchar_t *wchar_t_what_) {
  std::mbstate_t ps = {0};
  errno = 0;
  std::size_t size_ = std::wcsrtombs(NULL, &wchar_t_what_, 0, &ps);

  if (errno == EILSEQ)
    throw;

  return size_ + 1;
}

string wstos(wstring const &src) {
  std::size_t size_ = size(src.c_str());
  std::vector<char> dst(size_);
  std::mbstate_t ps = {0};
  errno = 0;
  const wchar_t *src_c_str = src.c_str();
  std::wcsrtombs(dst.data(), &src_c_str, size_, &ps);

  if (errno == EILSEQ)
    throw;

  return std::string(dst.data());
}
