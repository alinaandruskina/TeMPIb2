#include "modBetaCipher.h"
#include <vector>
#include <stdexcept>
using namespace std;

modBetaCipher::modBetaCipher(unsigned key)
{
    if (key < 2)
        throw invalid_argument("Ключ должен быть >= 2");
    cols = key;
}

wstring modBetaCipher::encrypt(const wstring& text)
{
    if (text.empty())
        throw invalid_argument("Шифруемый текст пуст");

    unsigned n = text.size();
    unsigned rows = (n + cols - 1) / cols;

    vector<vector<wchar_t>> table(
        rows, vector<wchar_t>(cols, L'_'));

    unsigned k = 0;
    for (unsigned r = 0; r < rows; r++)
        for (unsigned c = 0; c < cols; c++)
            if (k < n)
                table[r][c] = text[k++];

    wstring result;
    // Считывание: сверху вниз, справа налево
    for (int c = static_cast<int>(cols) - 1; c >= 0; c--)
        for (unsigned r = 0; r < rows; r++)
            if (table[r][c] != L'_')
                result.push_back(table[r][c]);

    return result;
}

wstring modBetaCipher::decrypt(const wstring& text)
{
    if (text.empty())
        throw invalid_argument("Расшифровываемый текст пуст");

    unsigned n = text.size();
    unsigned rows = (n + cols - 1) / cols;

    vector<vector<wchar_t>> table(
        rows, vector<wchar_t>(cols, L'_'));

    // Заполнение по тому же маршруту считывания
    unsigned k = 0;
    for (int c = static_cast<int>(cols) - 1; c >= 0; c--)
        for (unsigned r = 0; r < rows; r++)
            if (k < n)
                table[r][c] = text[k++];

    // Обратное считывание: слева направо, сверху вниз
    wstring result;
    for (unsigned r = 0; r < rows; r++)
        for (unsigned c = 0; c < cols; c++)
            if (table[r][c] != L'_')
                result.push_back(table[r][c]);

    return result;
}