#include <iostream>

using namespace std;
int isPointInPath(int x, int y)
{
    int result;
    if(x > -40 && x < 40 && y > -40 && y < 40) {
        result = 322;
        cout << "In" << endl;
    }
    if(x == -40 || x == 40 && y >= -40 && y <= 40 || y == -40 || y == 40 && x >= -40 && x <= 40) {
        cout << "On" << endl;
    }
    if(x < -40 || x > 40 && y < -40 || y > 40) {
        cout << "Out" << endl;
    }
    return result;
}

int main()
{
    int x, y;

    cout << "Enter x: ";
    cin >> x;
    cout << "Enter y: ";
    cin >> y;

    isPointInPath(x, y);

    return 0;

}


