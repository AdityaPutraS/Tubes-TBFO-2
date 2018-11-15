#include <stdio.h>
#include <string.h>
#include <math.h>
#include "boolean.h"

void funcExpression1(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcExpression2(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcTerm1(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcTerm2(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcFactor1(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcFactor2(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcBilangan(char s[], int *letak, boolean *berhasil, long double *hasilHitung);
void funcUnsigned(char s[], int *letak, boolean *berhasil, long int *hasilHitung);
void funcDigit(char s[], int *letak, boolean *berhasil, int *hasilHitung);

int to_int(char c)
{
    return c - '0';
}

boolean isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

void funcExpression1(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // E -> TE'
    int letakAwal = *letak;
    funcTerm1(s, letak, berhasil, hasilHitung);
    if (*berhasil)
    {
        funcExpression2(s, letak, berhasil, hasilHitung);
    }
    else
    {
        *letak = letakAwal;
    }
}

void funcExpression2(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    //E' -> -TE' | +TE' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '-')
    {
        // -TE'
        *letak += 1;
        long double temp = 0;
        funcTerm1(s, letak, berhasil, &temp);
        *hasilHitung -= temp;
        if (*berhasil)
        {
            funcExpression2(s, letak, berhasil, hasilHitung);
        }
    }
    else if (c == '+')
    {
        // +TE'
        *letak += 1;
        long double temp = 0;
        funcTerm1(s, letak, berhasil, &temp);
        *hasilHitung += temp;
        if (*berhasil)
        {
            funcExpression2(s, letak, berhasil, hasilHitung);
        }
    }
    else
    {
        //epsilon
        *berhasil = true;
    }
}

void funcTerm1(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // T -> FT'
    int letakAwal = *letak;
    funcFactor1(s, letak, berhasil, hasilHitung);
    if (*berhasil)
    {
        funcTerm2(s, letak, berhasil, hasilHitung);
    }
    else
    {
        *letak = letakAwal;
    }
}

void funcTerm2(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // T'-> *FT' | /FT' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '*')
    {
        // *FT'
        *letak += 1;
        long double temp = 0;
        funcFactor1(s, letak, berhasil, &temp);
        *hasilHitung *= temp;
        if (*berhasil)
        {
            funcTerm2(s, letak, berhasil, hasilHitung);
        }
    }
    else if (c == '/')
    {
        // /FT'
        *letak += 1;
        long double temp = 0;
        funcFactor1(s, letak, berhasil, &temp);
        *hasilHitung /= temp;
        if (*berhasil)
        {
            funcTerm2(s, letak, berhasil, hasilHitung);
        }
    }
    else
    {
        //epsilon
        *berhasil = true;
    }
}

void funcFactor1(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // F -> BF' | -BF' | (E)F' | -(E)F'
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '-')
    {
        char c2 = s[*letak + 1];
        if (c2 == '(')
        {
            // -(E)F'
            *letak += 2;
            funcExpression1(s, letak, berhasil, hasilHitung);
            if (*berhasil)
            {
                *letak += 1;
                funcFactor2(s, letak, berhasil, hasilHitung);
                *hasilHitung *= -1;
            }
            else
            {
                *letak = letakAwal;
            }
        }
        else
        {
            // -BF'
            *letak += 1;
            funcBilangan(s, letak, berhasil, hasilHitung);
            if (*berhasil)
            {
                funcFactor2(s, letak, berhasil, hasilHitung);
                *hasilHitung *= -1;
            }
            else
            {
                *letak = letakAwal;
            }
        }
    }
    else if (c == '(')
    {
        // (E)F'
        *letak += 1;
        funcExpression1(s, letak, berhasil, hasilHitung);
        if (*berhasil)
        {
            *letak += 1;
            funcFactor2(s, letak, berhasil, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else
    {
        // BF'
        funcBilangan(s, letak, berhasil, hasilHitung);
        if (*berhasil)
        {
            funcFactor2(s, letak, berhasil, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
}

void funcFactor2(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // F'-> ^(E)F' | ^BF' | ^-BF' | epsilon
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '^')
    {
        char c2 = s[*letak + 1];
        if (c2 == '(')
        {
            // ^(E)F'
            long double temp = 0;
            *letak += 2;
            funcExpression1(s, letak, berhasil, &temp);
            *letak += 1;
            funcFactor2(s, letak, berhasil, &temp);
            *hasilHitung = powl(*hasilHitung, temp);
        }
        else if (c2 == '-')
        {
            // ^-BF'
            long double temp = 0;
            *letak += 2;
            funcBilangan(s, letak, berhasil, &temp);
            temp *= -1;
            funcFactor2(s, letak, berhasil, &temp);
            *hasilHitung = powl(*hasilHitung, temp);
        }
        else
        {
            // ^BF'
            long double temp = 0;
            *letak += 1;
            funcBilangan(s, letak, berhasil, &temp);
            funcFactor2(s, letak, berhasil, &temp);
            *hasilHitung = powl(*hasilHitung, temp);
        }
    }
    else
    {
        // epsilon
        *berhasil = true;
    }
}

void funcBilangan(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // B -> U | U.U
    int letakAwal = *letak;
    long int temp1 = 0;
    funcUnsigned(s, letak, berhasil, &temp1);
    if (*berhasil)
    {
        *hasilHitung = (long double)temp1;
        char c = s[*letak];
        if (c == '.')
        {
            int cekPanjang = *letak;
            *letak += 1;
            long int temp2 = 0;
            funcUnsigned(s, letak, berhasil, &temp2);
            if (*berhasil)
            {
                cekPanjang = *letak - cekPanjang;
                long double bilKoma = temp2;
                while (cekPanjang > 1)
                {
                    bilKoma /= 10;
                    cekPanjang -= 1;
                }
                *hasilHitung += bilKoma;
            }
        }
    }
}

void funcUnsigned(char s[], int *letak, boolean *berhasil, long int *hasilHitung)
{
    // U -> D | DU
    int letakAwal = *letak;
    int temp = 0;
    funcDigit(s, letak, berhasil, &temp);
    boolean dummy = true;
    while (dummy && *berhasil)
    {
        *hasilHitung *= 10;
        *hasilHitung += temp;
        *letak += 1;
        funcDigit(s, letak, &dummy, &temp);
    }
}

void funcDigit(char s[], int *letak, boolean *berhasil, int *hasilHitung)
{
    // D -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    char cek = s[*letak];
    *berhasil = isDigit(cek);
    if (*berhasil)
    {
        *hasilHitung = to_int(cek);
    }
    else
    {
        *hasilHitung = -999;
    }
}

int main()
{
    char s[100] = "";
    while(strcmp(s,"exit")){
        printf("Masukan string : ");
        scanf("%s", s);
        long double hasil = 0;
        int letak = 0;
        boolean berhasil;
        funcExpression1(s, &letak, &berhasil, &hasil);
        if (berhasil && letak==strlen(s))
        {
            printf("%.15Lf\n", hasil);
        }
        else
        {
            printf("gagal, lol\n.");
        }
    }
}