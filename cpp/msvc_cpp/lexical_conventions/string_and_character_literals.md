# lexical conventions


## literals

### number and boolean
* integer literals, prefix to specify form(binary, octal, decimal or hex), suffix to specify the type
    * when no prefix/suffix, default to `int`(32bits) if the value could fit, otherwise `long`(64bits)
    * suffix:
        * `u`: unsigned
        * `l`: long
        * `ll`: long long
    * prefix:
        * `0b`: binary
        * `0`: octal
        * `0x`: hex
* float literals, suffix to specify the type
    * default to `double`
    * suffix:
        * `f`: float
        * `l`: long double
* boolean:
    * `true`
    * `false`

### character and string
* character literal: enclosed between single quote
    * default to `char`
    * prefix:
        * `u8`: UTF-8 `char`
        * `L`: `wchar_t`
        * `u`: `char16_t`
        * `U`: `char32_t`
* string literal: used to represent a NULL-terminated string, enclosed between double quote
    * default to `const char*`
    * prefix:
        * `u8`: UTF-8 `char`
        * `L`: `wchar_t`
        * `u`: `char16_t`
        * `U`: `char32_t`
        * `R`: raw string literal
            * raw string literal must be of format `R "delimiter( raw_characters )delimiter"`
    * suffix:
        * `s`: string
        * 
#### string concat
```cpp
char str[] = "12" "34";
char atr[] = "1234";
char atr[] =  "12\
34";
