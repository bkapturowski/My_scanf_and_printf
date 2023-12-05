
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

//Funkcja my_printf obsluguje następujące symbole formatujące:
//
//%s - tekst (wskaźnik na znak, terminator='\x0'),
//%d - liczba całkowita 32-bitowa ze znakiem,
//%f - liczba zmiennoprzecinkowa podwójnej precyzji, do wyświetlenia z dokładnością 5 miejsc po przecinku,
//%p - struktura point_t (z zadania 2.1 W punkt) - punkt o dwóch współrzędnych całkowitych, do wyświetlenia w formie (x y),
//Wartość zwracana:
//
//Funkcja zwraca liczbę wyświetlonych znaków.

//Funkcja my_scanf obsluguje następujące formaty:
//
//%d - liczba całkowita 32-bitowa ze znakiem,
//%f - liczba zmiennoprzecinkowa podwójnej precyzji, wpisywana tylko w formie dziesiętnej (nie naukowej!),
//%p - struktura point_t (z zadania 2.1 W punkt) - punkt o dwóch współrzędnych całkowitych w postaci "(x y)". Punkt należy liczyć jako dwa elementy pobrane.
//Wartość zwracana:
//
//Funkcja zwraca liczbę poprawnie pobranych elementów.

struct point_t {
    int x;
    int y;
};

#include <math.h>

long long zamien_miejscami(long long liczba) {


    long long reverse = 0, remainder;
    while (liczba != 0) {
        remainder = liczba % 10;
        reverse = reverse * 10 + remainder;
        liczba /= 10;
    }

    return reverse;
}


long long liczba_cyfr(long long liczba) {
    int ilosc_cyfr = 0;

    // Jeśli liczba jest 0, to ma jedną cyfrę
    if (liczba == 0) {
        return 1;
    }

    // Obliczanie ilości cyfr
    while (liczba != 0) {
        liczba /= 10;
        ilosc_cyfr++;
    }

    return ilosc_cyfr;
}

int my_printf(char *format, ...) {
    int count = 0;
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    while (*str != '\0') {
                        putchar(*str);
                        str++;
                        count++;
                    }
                    break;
                }
                case 'd': {

                    int flag = 0;
                    int num = va_arg(args, int);
                    if(num % 10 == 0 && num != 0) flag = 1;
                    if(num % 100 == 0 && num != 0) flag = 2;
                    if (num < 0) {
                        putchar('-');
                        count++;
                        num = -num;
                        if(num % 10 == 0 && num != 0) flag = 1;
                        if(num % 100 == 0 && num != 0) flag = 2;
                    }

                    long long rev = zamien_miejscami(num);
                    long long digit_count = liczba_cyfr( rev);

                    while (digit_count > 0) {
                        long long digit = rev % 10;
                        putchar('0' + (int) digit);
                        rev /= 10;
                        digit_count--;
                        count++;
                    }
                    if(flag ==1){
                        putchar('0');
                        count++;
                    }
                    if(flag == 2){
                        putchar('0');
                        putchar('0');
                        count+=2;
                    }
                    break;
                }
                case 'f': {
                    double num = va_arg(args, double);
                    if (num < 0) {
                        putchar('-');
                        count++;
                        num = -num;
                    }
                    int integer_part = (int)num;
                    double fraction_part = num - integer_part;
                    count += my_printf("%d", integer_part);
                    putchar('.');
                    count++;
                    int len = 5;
                    while (len > 0) {
                        fraction_part *= 10;
                        int digit = (int)fraction_part;
                        putchar('0' + digit);
                        fraction_part -= digit;
                        len--;
                        count++;
                    }
                    break;
                }
                case 'p': {
                    struct point_t point = va_arg(args, struct point_t);
                    putchar('(');
                    count += my_printf("%d", point.x);
                    putchar(' ');
                    count += my_printf("%d", point.y);
                    putchar(')');
                    count += 3;
                    break;
                }
                default:
                    putchar(*format);
                    count++;
                    break;
            }
        } else {
            putchar(*format);
            count++;
        }
        format++;
    }

    va_end(args);
    return count;
}



int my_scanf(char *format, ...) {
    va_list args;
    va_start(args, format);

    int count = 0;
    char c;
    int *integer;
    double *flooat;
    struct point_t *point;

    while ((c = *format++) != '\0') {

        if (c != '%') {
            continue;
        }

        switch (*format++) {
            case 'd':
                integer = va_arg(args, int *);
                *integer = 0;

                // Sprawdź, czy liczba jest ujemna
                int flag = 0;
                c = getchar();
                if(!(c >= '0' && c <= '9') && c != ' ' && c != '-') return 0;
                if(c == ' ') c = getchar();
                if (c == '-') {
                    flag = 1;
                    c = getchar();
                }

                // Pobierz kolejne znaki cyfry i konwertuj na liczbę całkowitą
                while (c != EOF && c >= '0' && c <= '9') {
                    *integer = (*integer * 10) + (c - '0');
                    c = getchar();
                }

                if (flag == 1) {
                    *integer *= -1;
                }

                if (c != EOF) {
                    ungetc(c, stdin); // Przywróć ostatni pobrany znak do strumienia
                    count++;
                }
                break;
            case 'f':
                flooat = va_arg(args, double *);
                *flooat = 0.0;

                // Sprawdź, czy liczba jest ujemna
                int flag_float = 0;
                c = getchar();
                if(!(c >= '0' && c <= '9') && c != ' ' && c != '-' && c != '.') return 0;


                if(c == ' ') c = getchar();

                if (c == '-') {
                    flag_float = 1;
                    c = getchar();
                }

                // Pobierz część całkowitą liczby
                while (c != EOF && c >= '0' && c <= '9') {
                    *flooat = (*flooat * 10) + (c - '0');
                    c = getchar();
                }

                // Pobierz część dziesiętną liczby
                if (c == '.') {
                    double fraction = 0.1;

                    c = getchar();
                    while (c != EOF && c >= '0' && c <= '9') {
                        *flooat += (c - '0') * fraction;
                        fraction *= 0.1;
                        c = getchar();
                    }
                }

                if (flag_float == 1) {
                    *flooat *= -1;
                }

                if (c != EOF) {
                    ungetc(c, stdin); // Przywróć ostatni pobrany znak do strumienia
                    count++;
                }
                break;
            case 'p':

                point = va_arg(args, struct point_t *);
                point->x = 0;
                point->y = 0;
                c = getchar();
                if(!(c >= '0' && c <= '9') && c != ' ' && c != '-' && c != '.' && c != '('&& c != ')') return 0;
                if(c == ' ') c = getchar();

                // Sprawdź, czy znak '(' jest obecny
                if (c != '(') {
                    break;
                }

                int flag_x = 0;
                c = getchar();
                if (c == '-') {
                    flag_x = 1;
                }
                else{
                    ungetc(c,stdin);
                }


                // Pobierz wartość x
                while ((c = getchar()) != EOF && c >= '0' && c <= '9') {
                    point->x = (point->x * 10) + (c - '0');
                }

                if(flag_x == 1){
                    point->x *= -1;
                }

                // Sprawdź, czy znak ' ' jest obecny
                if (c != ' ') {
                    break;
                }

                int flag_y = 0;
                c = getchar();
                if(!(c >= '0' && c <= '9') && c != ' ' && c != '-' && c != '.' && c != '('&& c != ')') return 0;
                if(c == ' ') c = getchar();
                if (c == '-') {
                    flag_y = 1;
                }
                else{
                    ungetc(c,stdin);
                }
                // Pobierz wartość y
                while ((c = getchar()) != EOF && c >= '0' && c <= '9') {
                    point->y = (point->y * 10) + (c - '0');
                }

                if(flag_y == 1){
                    point->y *= -1;
                }

                // Sprawdź, czy znak ')' jest obecny
                if (c != ')') {
                    break;
                }

                count+=2;
                break;
        }
    }

    va_end(args);

    return count;
}



int main() {


    int a;
    my_printf("Podaj liczbe:\n");
    if(my_scanf("%d",&a) != 0){
        my_printf("Moja liczba to: %d\n",a);
    }


    return 0;
}