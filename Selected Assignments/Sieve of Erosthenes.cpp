#include <iostream>
#include <iomanip>
using namespace std;

#define upperLimit 999

int main() {
    int numbers[upperLimit + 1];
    for (int i = 0; i<=upperLimit; i++) {
        if (i < 2) { 
            numbers[i] = 0;
        } else {
            numbers[i] = 1;
        }
    }
    for (int i = 2; i<upperLimit; i++) {
        if (numbers[i] == 1) {
            for (int j = 2; j <= upperLimit/i; j++) {
                numbers[i*j] = 0;
            }
        }
    } 
    for (int i = 1; i<=upperLimit; i++) { // print  table
        if (numbers[i] == 1) {
            i-100>0 ? cout << setw(4) << i : cout << setw(3) << i << " ";
        } else {
            cout << setw(4) << "- ";
        }
        if (i % 10 == 0 && i != 0) {
            cout << "\n";
        }
    }
    return 0;
}