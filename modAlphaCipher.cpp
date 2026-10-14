#include "modAlphaCipher.h"
#include <locale>
#include <stdexcept>

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Проверка: ключ не пустой
    if (skey.empty())
        throw std::invalid_argument("Ключ не может быть пустым");

    // Заполнение алфавита
    for (size_t i = 0; i < numAlpha.size(); i++)
        alphaNum[numAlpha[i]] = static_cast<int>(i);

    // Проверка допустимости символов ключа
    for (wchar_t c : skey) {
        if (alphaNum.find(c) == alphaNum.end())
            throw std::invalid_argument(
                "Ключ содержит недопустимый символ");
    }

    key = convert(skey);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (wchar_t c : s) {
        auto it = alphaNum.find(c);
        if (it == alphaNum.end())
            throw std::invalid_argument(
                "Обнаружен недопустимый символ");
        result.push_back(it->second);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (int i : v) {
        if (i < 0 || i >= static_cast<int>(numAlpha.size()))
            throw std::out_of_range(
                "Индекс выходит за границы алфавита");
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    if (open_text.empty())
        throw std::invalid_argument(
            "Шифруемый текст не может быть пустым");

    std::vector<int> work = convert(open_text);

    for (size_t i = 0; i < work.size(); i++)
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();

    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    if (cipher_text.empty())
        throw std::invalid_argument(
            "Расшифровываемый текст не может быть пустым");

    std::vector<int> work = convert(cipher_text);

    for (size_t i = 0; i < work.size(); i++)
        work[i] = (work[i] + alphaNum.size()
                   - key[i % key.size()]) % alphaNum.size();

    return convert(work);
}