#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include "modAlphaCipher.h"

int total_tests = 0;
int passed_tests = 0;

void check_equal(const std::wstring& expected, const std::wstring& actual, const std::string& test_name) {
    total_tests++;
    if (expected == actual) {
        passed_tests++;
        std::wcout << L"[OK]   " << test_name.c_str() << L"\n";
    } else {
        std::wcout << L"[FAIL] " << test_name.c_str() << L" | Ожидалось: " << expected << L" | Получено: " << actual << L"\n";
    }
}
template <typename Func>
void check_throw(Func func, const std::string& test_name) {
    total_tests++;
    try {
        func();
        std::wcout << L"[FAIL] " << test_name.c_str() << L" (не было исключения)\n";
    } catch (const cipher_error&) {
        passed_tests++;
        std::wcout << L"[OK]   " << test_name.c_str() << L"\n";
    } catch (...) {
        std::wcout << L"[FAIL] " << test_name.c_str() << L" (неверный тип исключения)\n";
    }
}

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale());
    
    std::wcout << L"=== ТЕСТИРОВАНИЕ ШИФРА ГРОНСФЕЛЬДА ===\n\n";

    std::wcout << L"--- Тесты конструктора ---\n";
    check_equal(L"БВГБВ", modAlphaCipher(L"БВГ").encrypt(L"ААААА"), "ValidKey");
    check_equal(L"БВГДЕ", modAlphaCipher(L"БВГДЕЖЗИЙК").encrypt(L"ААААА"), "LongKey");
    check_equal(L"БВГБВ", modAlphaCipher(L"бвг").encrypt(L"ААААА"), "LowCaseKey");
    check_throw([]{ modAlphaCipher cp(L"Б1"); }, "DigitsInKey");
    check_throw([]{ modAlphaCipher cp(L"Б,В"); }, "PunctuationInKey");
    check_throw([]{ modAlphaCipher cp(L"Б В"); }, "WhitespaceInKey");
    check_throw([]{ modAlphaCipher cp(L""); }, "EmptyKey");
    check_throw([]{ modAlphaCipher cp(L"ААА"); }, "WeakKey");

    std::wcout << L"\n--- Тесты encrypt ---\n";
    modAlphaCipher cipherB(L"Б");
    check_equal(L"РСЙГЖУНЙС", cipherB.encrypt(L"ПРИВЕТМИР"), "UpCaseString");
    check_equal(L"РСЙГЖУНЙС", cipherB.encrypt(L"приветмир"), "LowCaseString");
    check_equal(L"РСЙГЖУНЙС", cipherB.encrypt(L"ПРИВЕТ, МИР!"), "StringWithWhitspaceAndPunct");
    check_equal(L"ТОПГЬНДПЕПН", cipherB.encrypt(L"С Новым 2024 Годом"), "StringWithNumbers");
    check_throw([&]{ cipherB.encrypt(L""); }, "EmptyString");
    check_throw([&]{ cipherB.encrypt(L"1234+8765=9999"); }, "NoAlphaString");
    check_equal(L"ОПЗБДС", modAlphaCipher(L"Я").encrypt(L"ПРИВЕТ"), "MaxShiftKey");

    std::wcout << L"\n--- Тесты decrypt ---\n";
    check_equal(L"ПРИВЕТМИР", cipherB.decrypt(L"РСЙГЖУНЙС"), "UpCaseString");
    check_throw([&]{ cipherB.decrypt(L"рсйгжунйс"); }, "LowCaseString");
    check_throw([&]{ cipherB.decrypt(L"РСЙГЖУ НЙС"); }, "WhitespaceString");
    check_throw([&]{ cipherB.decrypt(L"ТОПХСН2024ГОДПН"); }, "DigitsString");
    check_throw([&]{ cipherB.decrypt(L"РСЙГЖУ,НЙС"); }, "PunctString");
    check_throw([&]{ cipherB.decrypt(L""); }, "EmptyString");
    check_equal(L"ПРИВЕТ", modAlphaCipher(L"Я").decrypt(L"ОПЗБДС"), "MaxShiftKey");

    std::wcout << L"\n=== ИТОГОВЫЕ РЕЗУЛЬТАТЫ ===\n";
    std::wcout << L"Всего тестов: " << total_tests << L"\n";
    std::wcout << L"Пройдено:     " << passed_tests << L"\n";
    std::wcout << L"Не пройдено:  " << (total_tests - passed_tests) << L"\n";
    
    if (total_tests == passed_tests) {
        std::wcout << L"\nУСПЕХ: ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n";
    } else {
        std::wcout << L"\nОШИБКА: ЕСТЬ НЕПРОЙДЕННЫЕ ТЕСТЫ.\n";
    }
    return 0;
}
