#include <UnitTest++/UnitTest++.h>
#include <locale>
#include "modBetaCipher.h"

struct Key3_fixture {
    modBetaCipher * p;
    Key3_fixture()  { p = new modBetaCipher(3); }
    ~Key3_fixture() { delete p; }
};

SUITE(KeyTest)
{
    TEST(ValidKey)     { CHECK(modBetaCipher(3).encrypt(L"ПРИВЕТ") == L"ИТРЕПВ"); }
    TEST(MinValidKey)  { CHECK(modBetaCipher(2).encrypt(L"АБ") == L"БА"); }
    TEST(KeyEqualsOne) { CHECK_THROW(modBetaCipher cp(1), cipher_error); }
    TEST(KeyEqualsZero){ CHECK_THROW(modBetaCipher cp(0), cipher_error); }
    TEST(BigKey)       { CHECK(modBetaCipher(10).encrypt(L"АБВГД") == L"ДГВБА"); }
    TEST(ValidKey_4)   { CHECK(modBetaCipher(4).encrypt(L"ПРИВЕТ") == L"ВИРТПЕ"); }
}

SUITE(EncryptTest)
{
    TEST_FIXTURE(Key3_fixture, FullTable)  { CHECK(p->encrypt(L"ПРИВЕТ") == L"ИТРЕПВ"); }
    TEST_FIXTURE(Key3_fixture, SingleChar) { CHECK(p->encrypt(L"А") == L"А"); }
    TEST_FIXTURE(Key3_fixture, EmptyString){ CHECK_THROW(p->encrypt(L""), cipher_error); }

    TEST(PartialTable) { CHECK(modBetaCipher(4).encrypt(L"ПРИВЕТ") == L"ВИРТПЕ"); }
    TEST(SingleRow)    { CHECK(modBetaCipher(5).encrypt(L"АБВГД") == L"ДГВБА"); }
    TEST(MinimalText)  { CHECK(modBetaCipher(2).encrypt(L"АБ") == L"БА"); }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(Key3_fixture, FullTable) { CHECK(p->decrypt(L"ИТРЕПВ") == L"ПРИВЕТ"); }
    TEST_FIXTURE(Key3_fixture, EmptyString){ CHECK_THROW(p->decrypt(L""), cipher_error); }

    // Проверка неполной таблицы (через цикл, так как строка зашифрованного текста сложная)
    TEST(PartialTable) {
        modBetaCipher cp(4);
        CHECK(cp.decrypt(cp.encrypt(L"ПРИВЕТ")) == L"ПРИВЕТ");
    }
    TEST(SingleRow) {
        modBetaCipher cp(5);
        CHECK(cp.decrypt(cp.encrypt(L"АБВГД")) == L"АБВГД");
    }
    TEST(MinimalText) {
        modBetaCipher cp(2);
        CHECK(cp.decrypt(cp.encrypt(L"АБ")) == L"АБ");
    }
}

SUITE(CycleTest)
{
    TEST_FIXTURE(Key3_fixture, CycleShort) {
        CHECK(p->decrypt(p->encrypt(L"КРИПТОГРАФИЯ")) == L"КРИПТОГРАФИЯ");
    }
    TEST_FIXTURE(Key3_fixture, CycleLong) {
        CHECK(p->decrypt(p->encrypt(L"АБВГДЕЁЖЗИЙ")) == L"АБВГДЕЁЖЗИЙ");
    }
    TEST(FullAlphabet) {
        modBetaCipher cp(7);
        std::wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
        CHECK(cp.decrypt(cp.encrypt(alphabet)) == alphabet);
    }
    TEST_FIXTURE(Key3_fixture, VariousLengths) {
        bool all_ok = true;
        for (int len = 1; len <= 10; ++len) {
            std::wstring t;
            for (int i = 0; i < len; ++i) t.push_back(L'А' + i);
            if (p->decrypt(p->encrypt(t)) != t) { all_ok = false; break; }
        }
        CHECK(all_ok);
    }
}

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    return UnitTest::RunAllTests();
}
