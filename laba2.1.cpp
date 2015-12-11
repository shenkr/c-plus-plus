#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    float x, y, z, F;

    cout << "Please enter x: ";
    cin >> x;
    if(x <= 2.5) {
        y = log(abs(2*x))/exp(3*x*x);
        z = cos(2*x)/pow(cos(x), 2);
    }
    if(x > 2.5) {
        y = 2.1*x*log10(x)/sqrt(2*x-3)+10;
        z = sin(2*x)/(x+(M_PI/3));
    }
    F = log(abs(y + z));
    cout << F;

}
