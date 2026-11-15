#pragma once
#include <string>
#include <vector>
#include "cipher_error.h"

class modBetaCipher {
private:
    unsigned cols;

public:
    modBetaCipher() = delete;
    modBetaCipher(unsigned key);
    std::wstring encrypt(const std::wstring& text);
    std::wstring decrypt(const std::wstring& text);
};
