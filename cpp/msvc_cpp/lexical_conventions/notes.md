# lexical conventions


## literals

##
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

```cpp

```
