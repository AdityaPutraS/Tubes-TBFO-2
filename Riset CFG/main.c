#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include "boolean.h"

void funcExpression1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcExpression2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcTerm1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcTerm2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcFactor1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcFactor2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcComplex(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcBilangan(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcUnsigned(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);
void funcDigit(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung);

long double complex to_ldc(char c)
{
    int angka = c - '0';
    return (angka+0*I);
}

boolean isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

void funcExpression1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // E -> TE' | -TE'
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '-')
    {
        // -TE'
        *letak += 1;
        long double complex temp = 0;
        funcTerm1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung = (0+0*I) - temp;
            funcExpression2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else
    {
        // TE'
        long double complex temp = 0;
        funcTerm1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung = temp;
            funcExpression2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
}

void funcExpression2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    //E' -> -TE' | +TE' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '-')
    {
        // -TE'
        *letak += 1;
        long double complex temp = *hasilHitung;
        funcTerm1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung -= temp;
            funcExpression2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else if (c == '+')
    {
        // +TE'
        *letak += 1;
        long double complex temp = *hasilHitung;
        funcTerm1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung += temp;
            funcExpression2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else
    {
        //epsilon
        *berhasil = true;
    }
}

void funcTerm1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // T -> FT'
    int letakAwal = *letak;
    long double complex temp = 0;
    funcFactor1(s, letak, berhasil, mathError, &temp);
    if (*berhasil)
    {
        *hasilHitung = temp;
        funcTerm2(s, letak, berhasil, mathError, hasilHitung);
    }
    else
    {
        *letak = letakAwal;
    }
}

void funcTerm2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // T'-> *FT' | /FT' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '*')
    {
        // *FT'
        *letak += 1;
        long double complex temp = *hasilHitung;
        funcFactor1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung *= temp;
            funcTerm2(s, letak, berhasil, mathError, hasilHitung);
        }
    }
    else if (c == '/')
    {
        // /FT'
        *letak += 1;
        long double complex temp = *hasilHitung;
        funcFactor1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            if (temp != 0)
            {
                *hasilHitung /= temp;
                funcTerm2(s, letak, berhasil, mathError, hasilHitung);
            }
            else
            {
                *mathError = true;
            }
        }
    }
    else
    {
        //epsilon
        *berhasil = true;
    }
}

void funcFactor1(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // F -> CF' | (E)F'
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '(')
    {
        // (E)F'
        *letak += 1;
        long double complex temp = 0;
        funcExpression1(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *letak += 1;
            *hasilHitung = temp;
            funcFactor2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else
    {
        // BF'
        long double complex temp = 0;
        funcComplex(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            *hasilHitung = temp;
            funcFactor2(s, letak, berhasil, mathError, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
}

void funcFactor2(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // F'-> ^(E)F' | ^CF' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '^')
    {
        char c2 = s[*letak + 1];
        if (c2 == '(')
        {
            // ^(E)F'
            long double complex temp = 0;
            *letak += 2;
            funcExpression1(s, letak, berhasil, mathError, &temp);
            if (*berhasil)
            {
                *letak += 1;
                funcFactor2(s, letak, berhasil, mathError, &temp);
                if (*berhasil)
                {
                    *hasilHitung = cpowl(*hasilHitung, temp);
                }
                else
                {
                    *letak = letakAwal;
                }
            }
            else
            {
                *letak = letakAwal;
            }
        }
        else
        {
            // ^CF'
            long double complex temp = 0;
            *letak += 1;
            funcComplex(s, letak, berhasil, mathError, &temp);
            if (*berhasil)
            {
                funcFactor2(s, letak, berhasil, mathError, &temp);
                if (*berhasil)
                {
                    *hasilHitung = cpowl(*hasilHitung, temp);
                }
                else
                {
                    *letak = letakAwal;
                }
            }
            else
            {
                *letak = letakAwal;
            }
        }
    }
    else
    {
        // epsilon
        *berhasil = true;
    }
}

void funcComplex(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // C -> B | Bi | i
    int letakAwal = *letak;
    long double complex temp = 0;
    char cekI = s[*letak];
    if (cekI == 'i')
    {
        *berhasil = true;
        *letak += 1;
        *hasilHitung = I;
    }
    else
    {
        funcBilangan(s, letak, berhasil, mathError, &temp);
        if (*berhasil)
        {
            char c = s[*letak];
            if (c == 'i')
            {
                *letak += 1;
                *hasilHitung = temp * I;
            }
            else
            {
                *hasilHitung = temp + 0*I;
            }
        }
        else
        {
            *letak = letakAwal;
        }
    }
}

void funcBilangan(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // B -> U | U.U
    int letakAwal = *letak;
    long double complex temp1 = 0;
    funcUnsigned(s, letak, berhasil, mathError, &temp1);
    if (*berhasil)
    {
        *hasilHitung = temp1;
        char c = s[*letak];
        if (c == '.')
        {
            int cekPanjang = *letak;
            *letak += 1;
            long double complex temp2 = 0;
            funcUnsigned(s, letak, berhasil, mathError, &temp2);
            if (*berhasil)
            {
                cekPanjang = *letak - cekPanjang;
                long double complex bilKoma = temp2;
                while (cekPanjang > 1)
                {
                    bilKoma /= (10+0*I);
                    cekPanjang -= 1;
                }
                *hasilHitung += bilKoma;
            }
        }
    }
}

void funcUnsigned(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // U -> D | DU
    int letakAwal = *letak;
    long double complex temp = 0;
    funcDigit(s, letak, berhasil, mathError, &temp);
    boolean dummy = true;
    while (dummy && *berhasil)
    {
        *hasilHitung *= (10+0*I);
        *hasilHitung += temp;
        *letak += 1;
        funcDigit(s, letak, &dummy, mathError, &temp);
    }
}

void funcDigit(char s[], int *letak, boolean *berhasil, boolean *mathError, long double complex *hasilHitung)
{
    // D -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    char cek = s[*letak];
    *berhasil = isDigit(cek);
    if (*berhasil)
    {
        *hasilHitung = to_ldc(cek);
    }
    else
    {
        *hasilHitung = -999+0*I;
    }
}

void eval(char s[])
{
    long double complex hasil = 0;
    int letak = 0;
    boolean berhasil = true, mathError = false;
    funcExpression1(s, &letak, &berhasil, &mathError, &hasil);
    if (berhasil && letak == strlen(s))
    {
        if (!mathError)
        {
            long double realPart = creall(hasil);
            long double imagPart = cimagl(hasil);
            printf("%s = ", s);
            if (fabsl(realPart) > 0.000000000000001)
            {
                if (fabsl(imagPart) > 0.000000000000001)
                {
                    printf("%.15Lf ", realPart);
                    if (imagPart < 0)
                    {
                        printf("-");
                        imagPart *= -1;
                    }
                    else
                    {
                        printf("+");
                    }
                    printf(" %.15Lfi\n", imagPart);
                }
                else
                {
                    printf("%.15Lf\n", realPart);
                }
            }
            else
            {
                if (fabsl(imagPart) > 0.000000000000001)
                {
                    printf("%.15Lf i\n", imagPart);
                }
                else
                {
                    printf("0\n");
                }
            }
        }
        else
        {
            printf("%s = Math Error!\n", s);
        }
    }
    else
    {
        printf("%s = Syntax Error!\n", s);
    }
}

int main()
{
    char s[100] = "";
    int n = 1;
    while (n != -1)
    {
        printf("Banyak persamaan : ");
        scanf("%d", &n);
        char pers[100][100];
        for (int i = 0; i < n; i++)
        {
            printf("Pers. %d : ", i + 1);
            scanf("%s", pers[i]);
        }
        for (int i = 0; i < n; i++)
        {
            eval(pers[i]);
        }
    }
}