#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include "modBetaCipher.h"

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
    
    std::wcout << L"=== ТЕСТИРОВАНИЕ ШИФРА МАРШРУТНОЙ ПЕРЕСТАНОВКИ ===\n\n";

    std::wcout << L"--- Тесты конструктора ---\n";
    check_equal(L"ИТРЕПВ", modBetaCipher(3).encrypt(L"ПРИВЕТ"), "ValidKey_3");
    check_equal(L"БА", modBetaCipher(2).encrypt(L"АБ"), "MinValidKey");
    check_throw([]{ modBetaCipher cp(1); }, "KeyEqualsOne");
    check_throw([]{ modBetaCipher cp(0); }, "KeyEqualsZero");
    check_equal(L"ДГВБА", modBetaCipher(10).encrypt(L"АБВГД"), "BigKey");
    check_equal(L"ВИРТПЕ", modBetaCipher(4).encrypt(L"ПРИВЕТ"), "ValidKey_4");

    std::wcout << L"\n--- Тесты encrypt ---\n";
    check_equal(L"ИТРЕПВ", modBetaCipher(3).encrypt(L"ПРИВЕТ"), "FullTable");
    check_equal(L"А", modBetaCipher(3).encrypt(L"А"), "SingleChar");
    check_throw([]{ modBetaCipher cp(3); cp.encrypt(L""); }, "EmptyString");
    check_equal(L"ВИРТПЕ", modBetaCipher(4).encrypt(L"ПРИВЕТ"), "PartialTable");
    check_equal(L"ДГВБА", modBetaCipher(5).encrypt(L"АБВГД"), "SingleRow");

    std::wcout << L"\n--- Тесты decrypt ---\n";
    check_equal(L"ПРИВЕТ", modBetaCipher(3).decrypt(L"ИТРЕПВ"), "Decrypt_FullTable");
    check_throw([]{ modBetaCipher cp(3); cp.decrypt(L""); }, "Decrypt_EmptyString");
    
    modBetaCipher cp4(4);
    check_equal(L"ПРИВЕТ", cp4.decrypt(cp4.encrypt(L"ПРИВЕТ")), "Decrypt_PartialTable");
    
    modBetaCipher cp5(5);
    check_equal(L"АБВГД", cp5.decrypt(cp5.encrypt(L"АБВГД")), "Decrypt_SingleRow");

    std::wcout << L"\n--- Тесты циклов ---\n";
    
    modBetaCipher cp3(3);
    check_equal(L"КРИПТОГРАФИЯ", cp3.decrypt(cp3.encrypt(L"КРИПТОГРАФИЯ")), "Cycle_3");
    check_equal(L"АБВГДЕЁЖЗИЙ", cp3.decrypt(cp3.encrypt(L"АБВГДЕЁЖЗИЙ")), "Cycle_3_Long");
    
    modBetaCipher cp7(7);
    std::wstring fullAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    check_equal(fullAlpha, cp7.decrypt(cp7.encrypt(fullAlpha)), "FullAlphabet");

    bool all_ok = true;
    for (int len = 1; len <= 10; ++len) {
        std::wstring t;
        for (int i = 0; i < len; ++i) t.push_back(L'А' + i);
        if (cp3.decrypt(cp3.encrypt(t)) != t) { all_ok = false; break; }
    }
    total_tests++;
    if (all_ok) {
        passed_tests++;
        std::wcout << L"[OK]   VariousLengths\n";
    } else {
        std::wcout << L"[FAIL] VariousLengths\n";
    }

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
