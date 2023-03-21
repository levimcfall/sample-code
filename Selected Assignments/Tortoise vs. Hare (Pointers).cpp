#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(NULL));
    char racetrack[42] = "H..T....................................|";
    const char *finLine = racetrack + 40;
    char *h = racetrack;
    char *t = racetrack + 3;
    cout << "Are you ready to witness the legendary race between the tortoise and the hare, frame by frame?\nThe hare was feeling confident and gave the toroise a 3m head start.\nWill this be a fatal miscalculation? You'll just have to see for youself: ";
    cout << "\n" << racetrack;
    while (true) {
        cout << "\nPress enter to see the next frame.";
        cin.ignore();
        // move tortoise
        *t = '.';
        t += 2;
        if (t < finLine) *t = 'T';
        //move hare
        *h = '.';
        switch (rand()%4) {
            case 0:
                h += 6;
                cout << "The hare had a burst of energy.";
                break;
            case 1:
                h += 4;
                cout << "The hare is running.";
                break;
            case 2:
                cout << "The hare fell asleep...";
                break;
            case 3:
                cout << "The hare ran the wrong way; the tortoise was smart to put carrots at the starting line!";
                if (h-2 >= racetrack) h -= 2;
                break; 
        }
        if (h < finLine) *h = 'H';
        // check for victory and declare a winner if warranted.
        if (h >= finLine || t >= finLine) {
            cout << "\n" << racetrack;
            if (h >= finLine && t >= finLine) {
                if (h > t) cout << "\nIt was a photo finish, but the hare was victorious in the end.";
                else cout << "\nIt was a photo finish, but the tortoise was victorious in the end.";
                break;
            }
            if (h >= finLine) cout << "\nThe hare was victorious!";
            else cout << "\nThe tortoise was victorious!";
            break;
        }
        cout << "\n" << racetrack;
    }
    
    return 0;
}