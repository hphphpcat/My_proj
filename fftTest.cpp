#include <bits/stdc++.h>
using namespace std;
class complex_
{
public:
    double real;
    double fake;
    complex_(double real, double fake)
    {
        this->real = real;
        this->fake = fake;
    }
    complex_()
    {
        this->real = 0;
        this->fake = 0;
    }
    complex_ operator*(const complex_ &c)
    {
        complex_ ret(c.real * real - c.fake * fake, c.fake * real + fake * c.real);
        return ret;
    }
    complex_ operator+(const complex_ &c)
    {
        complex_ ret(c.real + real, c.fake + fake);
        return ret;
    }
    complex_ operator-(const complex_ &c)
    {
        complex_ ret(real - c.real, fake - c.fake);
        return ret;
    }
    complex_ operator=(const complex_ &c)
    {
        real = c.real;
        fake = c.fake;
        return *this;
    }
    void print()
    {
        cout << real << " qwqq" << fake << endl;
    }
};
const double pi = acos(-1);

void fft(vector<complex_> &A, int n, int inv)
{
    if (n == 1)
        return;
    vector<complex_> A1(n / 2), A2(n / 2);
    for (int i = 0; i < n; i += 2)
    {
        A1[i / 2] = A[i];
        A2[i / 2] = A[i + 1];
    }
    fft(A1, n / 2, inv);
    fft(A2, n / 2, inv);
    complex_ w(cos(2 * pi / n), inv * sin(2 * pi / n));
    complex_ wn(1, 0);
    for (int i = 0; i < n / 2; i++, wn = wn * w)
    {
        A[i] = A1[i] + wn * A2[i];
        A[i + n / 2] = A1[i] - wn * A2[i];
    }
}
int main()
{
    int n = 4;
    int m = 4;
    vector<complex_> A = {{1, 0}, {1, 0}, {1, 0}, {1, 0}};
    vector<complex_> B = {{1, 0}, {1, 0}, {1, 0}, {1, 0}};
    for (int i = 0; i < m; i++)
        A.push_back(complex_(0, 0));
    for (int i = 0; i < n; i++)
        B.push_back(complex_(0, 0));
    n += m;
    fft(A, n, 1);
    fft(B, n, 1);
    for (int i = 0; i < n; i++)
        A[i] = A[i] * B[i];
    fft(A, n, -1);
    for (int i = 0; i < n; i++)
        cout << (A[i].real / n) << " ";
}