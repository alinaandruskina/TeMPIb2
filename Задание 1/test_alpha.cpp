#include <UnitTest++/UnitTest++.h>
#include <locale>
#include "modAlphaCipher.h"

struct KeyB_fixture {
    modAlphaCipher * p;
    KeyB_fixture()  { p = new modAlphaCipher(L"Б"); }
    ~KeyB_fixture() { delete p; }
};

SUITE(KeyTest)
{
    TEST(ValidKey)         { CHECK(modAlphaCipher(L"БВГ").encrypt(L"ААААА") == L"БВГБВ"); }
    TEST(LongKey)          { CHECK(modAlphaCipher(L"БВГДЕЖЗИЙК").encrypt(L"ААААА") == L"БВГДЕ"); }
    TEST(LowCaseKey)       { CHECK(modAlphaCipher(L"бвг").encrypt(L"ААААА") == L"БВГБВ"); }
    
    TEST(DigitsInKey)      { CHECK_THROW(modAlphaCipher cp(L"Б1"), cipher_error); }
    TEST(PunctuationInKey) { CHECK_THROW(modAlphaCipher cp(L"Б,В"), cipher_error); }
    TEST(WhitespaceInKey)  { CHECK_THROW(modAlphaCipher cp(L"Б В"), cipher_error); }
    TEST(EmptyKey)         { CHECK_THROW(modAlphaCipher cp(L""), cipher_error); }
    TEST(WeakKey)          { CHECK_THROW(modAlphaCipher cp(L"ААА"), cipher_error); }
}

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString)   { CHECK(p->encrypt(L"ПРИВЕТМИР") == L"РСЙГЖУНЙС"); }
    TEST_FIXTURE(KeyB_fixture, LowCaseString)  { CHECK(p->encrypt(L"приветмир") == L"РСЙГЖУНЙС"); }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) { CHECK(p->encrypt(L"ПРИВЕТ, МИР!") == L"РСЙГЖУНЙС"); }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) { CHECK(p->encrypt(L"С Новым 2024 Годом") == L"ТОПГЬНДПЕПН"); }
    
    TEST_FIXTURE(KeyB_fixture, EmptyString)    { CHECK_THROW(p->encrypt(L""), cipher_error); }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString)  { CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error); }
    
    TEST(MaxShiftKey) { CHECK(modAlphaCipher(L"Я").encrypt(L"ПРИВЕТ") == L"ОПЗБДС"); }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString)  { CHECK(p->decrypt(L"РСЙГЖУНЙС") == L"ПРИВЕТМИР"); }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) { CHECK_THROW(p->decrypt(L"рсйгжунйс"), cipher_error); }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) { CHECK_THROW(p->decrypt(L"РСЙГЖУ НЙС"), cipher_error); }
    TEST_FIXTURE(KeyB_fixture, DigitsString)  { CHECK_THROW(p->decrypt(L"ТОПХСН2024ГОДПН"), cipher_error); }
    TEST_FIXTURE(KeyB_fixture, PunctString)   { CHECK_THROW(p->decrypt(L"РСЙГЖУ,НЙС"), cipher_error); }
    TEST_FIXTURE(KeyB_fixture, EmptyString)   { CHECK_THROW(p->decrypt(L""), cipher_error); }
    
    TEST(MaxShiftKey) { CHECK(modAlphaCipher(L"Я").decrypt(L"ОПЗБДС") == L"ПРИВЕТ"); }
}

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    return UnitTest::RunAllTests();
}
