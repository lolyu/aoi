#include <iostream>

using namespace std;
/*
 * https://www.ibm.com/docs/en/i/7.3?topic=specifiers-decltypeexpression-type-specifier-c11
 * If expression is an unparenthesized id-expression or class member, decltype(expression) is the type of the entity named by expression.
 *   If there is no such entity, or if expression names a set of overloaded functions, the program is ill formed.
 * Otherwise, if expression is a function call or an invocation of an overloaded operator (parentheses around expression are ignored), decltype(expression) is the return type of the statically chosen function.
 * Otherwise, if expression is an lvalue, decltype(expression) is T&, where T is the type of expression.
 * Otherwise, decltype(expression) is the type of expression.
 */

template <typename T, typename U>
auto eff(T t, U u) -> decltype(t * u)
{
    return t * u;
}

template <typename T, typename U>
decltype(auto) add(T t, U u)
{
    return u + t;
}

int main()
{
    // NOTE: string literals are lvalues
    decltype("decltype") var0 = "decltype"; // const char(&)[9]
    auto var1 = "auto";                     // const char *
    auto var2 = eff(100, 0.1);              // double
    auto var3 = add(100, 123123123123);     // long
    return 0;
}