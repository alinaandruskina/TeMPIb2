#include <iostream>
#include <locale>
#include <limits>
#include <stdexcept>
#include "modBetaCipher.h"
using namespace std;

bool isValid(const wstring& s)
{
    if (s.empty()) return false;
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

    unsigned key;
    wstring text;
    unsigned op;

    try {
        wcout << L"Шифр маршрутной перестановки\n";
        wcout << L"Введите ключ (количество столбцов, >=2): ";
        wcin >> key;

        if (wcin.fail())
            throw invalid_argument("Ключ должен быть числом");

        modBetaCipher cipher(key);
        wcout << L"Ключ принят\n";

        do {
            wcout << L"\nВыберите операцию "
                     L"(0 - выход, 1 - шифрование, 2 - дешифрование): ";
            wcin >> op;

            if (wcin.fail()) {
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: недопустимая операция\n";
                continue;
            }

            if (op > 2) {
                wcout << L"Ошибка: недопустимая операция\n";
            } else if (op > 0) {
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Введите текст "
                         L"(ЗАГЛАВНЫМИ русскими буквами без пробелов): ";
                getline(wcin, text);

                if (!isValid(text))
                    throw invalid_argument(
                        "Текст содержит недопустимые символы "
                        "или пуст");

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
        wcerr << L"Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
