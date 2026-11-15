#include "modAlphaCipher.h"
#include <cwctype>
#include <algorithm>

inline std::wstring modAlphaCipher::getValidKey(const std::wstring& s) {
    if (s.empty())
        throw cipher_error("Empty key");

    std::wstring tmp;
    for (wchar_t c : s) {
        wchar_t u = std::towupper(c);
        if (alphaNum.find(u) == alphaNum.end())
            throw cipher_error("Invalid key characters");
        tmp.push_back(u);
    }

    if (tmp.size() > 1) {
        bool allSame = true;
        for (size_t i = 1; i < tmp.size(); ++i) {
            if (tmp[i] != tmp[0]) { allSame = false; break; }
        }
        if (allSame)
            throw cipher_error("WeakKey");
    }
    return tmp;
}

inline std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s) {
    if (s.empty())
        throw cipher_error("Empty open text");

    std::wstring tmp;
    for (wchar_t c : s) {
        wchar_t u = std::towupper(c);
        if (alphaNum.find(u) != alphaNum.end())
            tmp.push_back(u);
    }
    if (tmp.empty())
        throw cipher_error("No valid characters in text");
    return tmp;
}

inline std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s) {
    if (s.empty())
        throw cipher_error("Empty cipher text");
    for (wchar_t c : s) {
        if (alphaNum.find(c) == alphaNum.end())
            throw cipher_error("Invalid cipher text characters");
    }
    return s;
}

modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    for (size_t i = 0; i < numAlpha.size(); ++i)
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    key = convert(getValidKey(skey));
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    for (size_t i = 0; i < work.size(); ++i)
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    for (size_t i = 0; i < work.size(); ++i)
        work[i] = (work[i] + alphaNum.size()
                   - key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s) {
    std::vector<int> result;
    for (wchar_t c : s) result.push_back(alphaNum[c]);
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring result;
    for (int i : v) result.push_back(numAlpha[i]);
    return result;
}
