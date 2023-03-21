#include <iostream>
using namespace std;

#define defaultInitPeg 1
#define defaultFinPeg 3
#define deafultTempPeg 2

void move(int numDisks, int initPeg, int finPeg, int tempPeg) {
    if (numDisks == 1) {cout <<  initPeg << "->" << finPeg << "\n";}
    else if (numDisks > 1) {
    move(numDisks-1, initPeg, tempPeg, finPeg);
    cout <<  initPeg << "->" << finPeg << "\n";
    move(numDisks-1, tempPeg, finPeg, initPeg);
    }
}
int main() {
    int totalNumDisks;
    cout << "Welcome to the Tower of Hanoi solver.\nHow many disks? ";
    cin >> totalNumDisks;
    move(totalNumDisks,defaultInitPeg, defaultFinPeg, deafultTempPeg);
}