#include <iostream>
#include <locale>
#include <limits>
#include <stdexcept>
#include "modAlphaCipher.h"
using namespace std;

bool isValid(const wstring& s)
{
    for (wchar_t c : s)
        if (c < L'А' || c > L'Я')
            return false;
    return true;
}

int main()
{
    locale::global(locale("ru_RU.UTF-8"));
    wcout.imbue(locale());
    wcin.imbue(locale());

    wstring key, text;
    unsigned op;

    try {
        wcout << L"Шифр Гронсфельда готов. Введите ключ: ";
        wcin >> key;

        if (!isValid(key))
            throw invalid_argument("Ключ содержит недопустимые символы");

        modAlphaCipher cipher(key);
        wcout << L"Ключ принят\n";

        do {
            wcout << L"\nВыберите действие "
                     L"(0 - выход, 1 - зашифровать, 2 - расшифровать): ";
            wcin >> op;

            if (op > 2) {
                wcout << L"Ошибка: недопустимая операция\n";
            } else if (op > 0) {
                wcout << L"Введите текст: ";
                wcin >> text;

                if (!isValid(text))
                    throw invalid_argument(
                        "Текст содержит недопустимые символы");

                if (op == 1)
                    wcout << L"Зашифрованный текст: "
                          << cipher.encrypt(text) << endl;
                else
                    wcout << L"Расшифрованный текст: "
                          << cipher.decrypt(text) << endl;
            }
        } while (op != 0);
    }
    catch (const exception& e) {
        wcerr << L"Ошибка " << e.what() << endl;
        return 1;
    }

    return 0;
}