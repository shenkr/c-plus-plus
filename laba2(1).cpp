#include <iostream>
using namespace std;

int main()
{
    int a[4][4] = {
        {1, 2, 3, 4},
        {1, 2, 3, 4},
        {1, 2, 3, 5},
        {1, 5, 3, 4}
    };
    bool flag = false;


    int array_max = a[0][0];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
           if(a[i][j] > array_max) array_max = a[i][j];
        }
    }

    int number = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
           if(a[i][j] == array_max) number++;
        }
    }
    if(number > 1) flag = true;
    else {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                if(a[i][j] = array_max) a[i][j] = 0;
            }
        }
    }
    cout << "Max Number: " << array_max;

    return 0;
}
