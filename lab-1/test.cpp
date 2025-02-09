#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <compare>

#define TEST 123
#define EMPTY
#if TEST
#define EMPTY
#elif
#define NOT_EMPTY 1
#endif

double func(double *a, int n)
{
    for (auto i = 0; i < n; i++)
    {
        a[i]++;
    }

    return a[n - 1];
}

/*comment*/
/*
    Multiline comments
    Multiline comments
    Multiline comments
    Multiline comments
    Multiline comments
*/

int main()
{
    // INTEGERS
    int d = 42, dd = -42;
    int o = 052;
    int x = 0x2a;
    int X = 0X2A;
    int b = 0b101010;
    // unsigned long long bad_digit = 1''2345678901234567890u;
    auto test = 0xFAULL;
    auto test_two = 0B0010;
    unsigned long long a = 12345678901234567890u;
    unsigned long long l1 = 18446744073709550592ull;
    unsigned long long l2 = 18'446'744'073'709'550'592llu;
    unsigned long long l3 = 1844'6744'0737'0955'0592uLL;
    unsigned long long l4 = 184467'440737'095505'92LLU;

    // COMPARISON
    bool bool1 = d == dd;
    bool bool2 = d != dd;
    bool bool3 = d < dd;
    bool bool4 = d > dd;
    bool bool5 = d <= dd;
    bool bool6 = d >= dd;
    auto bool7 = d <=> dd;

    // CHARS
    char char_a = 'a';
    char char_b = u8'B';
    char char_c = L'C';
    char char_new_line = '\n';
    char char_d = '\13';
    auto char_unicode = u'\x732b';

    // STRINGS
    auto str_a = R"(\)";
    auto str_b = R"(\n\n\n\n)";
    const auto *s2 = "\nHello\n  World\n";
    const auto *s4 = L"ABC";
    const auto *s6 = U"GHI";
    const auto *s9 = "MN";
    const auto *sA = u"MNOPQR";

    // ASSIGNMENT
    a = l1;
    a += l1;
    a -= l1;
    a *= l1;
    a /= l1;
    a %= l1;
    a &= l1;
    a |= l1;
    a ^= l1;
    a <<= l1;
    a >>= l1;

    double arr[] = {1, 2, 3};
    int idx = 0;
    // member access
    arr[idx] = 1;
    *arr = 1;
    struct temp
    {
        int b;
        int *c;
    } s;
    temp *sptr = &s;

    s.b = 1;
    sptr->b = 13;

    // a->d<1>;
}
