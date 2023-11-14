#include <stdio.h>
#include <stdarg.h>

void show_var(const char *format_str, ...)
{
    const char *p = format_str;

    va_list vl;
    va_start(vl, format_str);
    while (*p != '\0')
    {
        union printable_t
        {
            int i;
            double f;
            int c;
            char *s;
        } printable;

        switch (*p++)
        {
        case 'i':
            printable.i = va_arg(vl, int);
            printf("%i\n", printable.i);
            break;
        case 'f':
            printable.f = va_arg(vl, double);
            printf("%f\n", printable.f);
            break;
        case 'c':
            printable.c = (char) va_arg(vl, int);
            printf("%c\n", printable.c);
            break;
        case 's':
            printable.s = va_arg(vl, char *);
            printf("%s\n", printable.s);
            break;
        default:
            break;
        }
    }
    va_end(vl);
}

int main()
{
    show_var("ifcs", 10, 32.888, 'a', "Test string");
    return 0;
}
