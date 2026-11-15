#include "modBetaCipher.h"
#include <vector>

modBetaCipher::modBetaCipher(unsigned key) {
    if (key < 2)
        throw cipher_error("Key must be >= 2");
    cols = key;
}

std::wstring modBetaCipher::encrypt(const std::wstring& text) {
    if (text.empty())
        throw cipher_error("Empty text");

    unsigned n = text.size();
    unsigned rows = (n + cols - 1) / cols;

    std::vector<std::vector<wchar_t>> table(
        rows, std::vector<wchar_t>(cols, L'_'));

    unsigned k = 0;
    for (unsigned r = 0; r < rows; ++r)
        for (unsigned c = 0; c < cols; ++c)
            if (k < n) table[r][c] = text[k++];

    std::wstring result;
    for (int c = static_cast<int>(cols) - 1; c >= 0; --c)
        for (unsigned r = 0; r < rows; ++r)
            if (table[r][c] != L'_')
                result.push_back(table[r][c]);

    return result;
}

std::wstring modBetaCipher::decrypt(const std::wstring& text) {
    if (text.empty())
        throw cipher_error("Empty text");

    unsigned n = text.size();
    unsigned rows = (n + cols - 1) / cols;

    std::vector<std::vector<wchar_t>> table(
        rows, std::vector<wchar_t>(cols, L'_'));

    unsigned k = 0;
    for (int c = static_cast<int>(cols) - 1; c >= 0; --c) {
        for (unsigned r = 0; r < rows; ++r) {
            if (r * cols + c < n) {
                table[r][c] = text[k++];
            }
        }
    }

    std::wstring result;
    for (unsigned r = 0; r < rows; ++r)
        for (unsigned c = 0; c < cols; ++c)
            if (table[r][c] != L'_')
                result.push_back(table[r][c]);

    return result;
}
