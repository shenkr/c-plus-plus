#include <iostream>

using namespace std;

int main()
{
    int a, b;
    cout << "Enter the a: ";
    cin >> a;
    switch(a) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            b = a;
            break;
        case 8:
        case 9:
        case 10:
            b = a + 2;
            break;
    }
    cout << b << endl;

    cout << "Hello world!" << endl;
    return 0;
}
