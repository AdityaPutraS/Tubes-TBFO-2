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
    // E -> TE' | -TE'
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '-')
    {
        // -TE'
        *letak += 1;
        long double temp = 0;
        funcTerm1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung = -1*temp;
            funcExpression2(s, letak, berhasil, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
    }
    else
    {
        // TE'
        long double temp = 0;
        funcTerm1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung = temp;
            funcExpression2(s, letak, berhasil, hasilHitung);
        }
        else
        {
            *letak = letakAwal;
        }
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
        long double temp = *hasilHitung;
        funcTerm1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung -= temp;
            funcExpression2(s, letak, berhasil, hasilHitung);
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
        long double temp = *hasilHitung;
        funcTerm1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung += temp;
            funcExpression2(s, letak, berhasil, hasilHitung);
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

void funcTerm1(char s[], int *letak, boolean *berhasil, long double *hasilHitung)
{
    // T -> FT'
    int letakAwal = *letak;
    long double temp = 0;
    funcFactor1(s, letak, berhasil, &temp);
    if (*berhasil)
    {
        *hasilHitung = temp;
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
        long double temp = *hasilHitung;
        funcFactor1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung *= temp;
            funcTerm2(s, letak, berhasil, hasilHitung);
        }
    }
    else if (c == '/')
    {
        // /FT'
        *letak += 1;
        long double temp = *hasilHitung;
        funcFactor1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung /= temp;
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
    // F -> BF' | (E)F'
    int letakAwal = *letak;
    char c = s[*letak];
    if (c == '(')
    {
        // (E)F'
        *letak += 1;
        long double temp = 0;
        funcExpression1(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *letak += 1;
            *hasilHitung = temp;
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
        long double temp = 0;
        funcBilangan(s, letak, berhasil, &temp);
        if (*berhasil)
        {
            *hasilHitung = temp;
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
    // F'-> ^(E)F' | ^BF' | epsilon
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

void eval(char s[])
{
    long double hasil = 0;
    int letak = 0;
    boolean berhasil;
    funcExpression1(s, &letak, &berhasil, &hasil);
    if (berhasil && letak == strlen(s))
    {
        printf("%s = %.15Lf\n",s, hasil);
    }
    else
    {
        printf("%s = Syntax Error!\n",s);
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
        for(int i = 0; i < n; i++)
        {
            printf("Pers. %d : ",i+1);
            scanf("%s",pers[i]);
        }
        for(int i = 0; i < n;i++)
        {
            eval(pers[i]);
        }
    }
}