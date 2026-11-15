#include <iostream>
#include <locale>
#include "modAlphaCipher.h"

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());

    try {
        modAlphaCipher cipher(L"ПРИВЕТ");
        std::wstring text = L"РУКА";
        std::wstring enc  = cipher.encrypt(text);
        std::wstring dec  = cipher.decrypt(enc);

        std::wcout << L"Ключ:            ПРИВЕТ\n";
        std::wcout << L"Открытый текст:  " << text << L"\n";
        std::wcout << L"Шифротекст:      " << enc  << L"\n";
        std::wcout << L"Расшифровано:    " << dec  << L"\n";

        if (enc == L"ЯГТВ" && dec == text)
            std::wcout << L"OK\n";
        else
            std::wcout << L"ERR\n";
    } catch (const std::exception& e) {
        std::wcout << L"Исключение: " << e.what() << L"\n";
    }
    return 0;
}

