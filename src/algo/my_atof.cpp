#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>       //isfinite
#include <utility>
#include <vector>

#ifdef _WIN32
#define strcasecmp _strcmpi 
#endif

inline double doubleFromBits(int64_t n)
{
    double f = 0;
    memcpy(&f, &n, sizeof(n));
    return f;
}

const double NaN = doubleFromBits(0x7FF8000000000001);
const double Inf = doubleFromBits(0x7FF0000000000000);
const double NegInf = doubleFromBits(0xFFF0000000000000);


inline bool is_space(int ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

inline bool is_digit(int ch)
{
    return ch >= '0' && ch <= '9';
}

inline int tenpow(int n)
{
    int v = 1;
    for (int i = 0; i < n; i++)
    {
        v *= 10;
    }
    return v;
}

int my_atoi(const char* s)
{
    if (s == NULL || s[0] == '\0')
        return 0;
    
    // skip leading space
    while (is_space(*s))
        s++;

    // negative sign
    int sign = (*s == '-');
    if (sign || *s == '+')
        s++;

    int64_t n = 0;
    const char* p = s;
    while (is_digit(*p))
    {
        if (p - s > 10) {
            return sign ? INT_MIN : INT_MAX;
        }
        n = n * 10 + (*p - '0');
        p++;
    }
    if (sign) {
        n = -n < INT_MIN ? INT_MIN : -n;
    } else {
        n = n > INT_MAX ? INT_MAX: n;
    }
    return n;
}

double my_atof(const char* s, int len)
{
    int sign = 0;
    double v = 0;
    const char* end = s + len;
    char c;

    if (len <= 0)
        return 0;

    // skip leading space
    while (s != end && is_space(c = *s))
        s++;

    if (len == 3)
    {
        if (strcasecmp(s, "nan") == 0)
            return NaN;
    }
    
    // sign flag
    sign = (c == '-');
    if (sign || c == '+')
        s++;

    if (len >= 3) 
    {
        if (strcasecmp(s, "inf") == 0)
        {
            return sign ? NegInf : Inf;
        }
    }
    
    while (is_digit(*s))
    {
        v = v * 10 + (*s++ - '0');
    }
    // process fraction
    if (*s == '.')
    {
        s++;
        double fr = 10;
        while (is_digit(*s))
        {
            v += (*s++ - '0') / fr;
            fr *= 10;
        }
    }

    while (is_space(*s))
        s++;

    // process exp
    if (*s == 'e' || *s == 'E')
    {
        c = *(++s);
        int f = (c == '-');
        if (f || c == '+')
            s++;
        int n = my_atoi(s);
        double base = tenpow(n);
        if (f) {
            base = 1.0 / base;
        }
        v *= base;
    }
    return sign ? -v : v;
}

inline bool is_float_equal(double x, double y)
{
    double f = x - y;
    if (f < 0)
    {
        f = -f;
    }
    return f <= 0.000001;
}

int main()
{
    std::vector<std::pair<const char*, double>> test_cases = {
        {"NaN", NaN},
        {"inf", Inf},
        {"-inf", NegInf},
        {"  314.159", 314.159},
        {"314.159 ", 314.159},
        {" +314.159 ", 314.159},
        {" -314.159 ", -314.159},
        {"0.314", 0.314},
        {" 3.141E2", 314.1},
        {" +3.141E-2", 0.03141},
        {" -3.141E2", -314.1},
    };
    
    for (size_t i = 0; i < test_cases.size(); i++)
    {
        const char* s = test_cases[i].first;
        double expect = test_cases[i].second;
        double d = my_atof(s, strlen(s));  
        if (isfinite(expect)) 
        {
            bool f = is_float_equal(expect, d);
            assert(f);
        }
        printf("%s --> %f\n", s, d);
    }
    printf("OK\n");
}