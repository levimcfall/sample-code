#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

#define maxGroupSize 100
#define numGroupsPerSize 200
#define smallestGroup 10
#define largestGroup 40

class Date {
    int month; // 1-12
    int day; //1-31
public:
    Date();
    int getm() {return month;}
    int getd() {return day;}
};
Date::Date() {
    const int numDaysinMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    day = 1 + rand() % 365;
    month = 1;
    int daysInCurrMonth;
    while (true) {
        daysInCurrMonth = numDaysinMonth[month - 1];
        if (day > daysInCurrMonth) {
            day -= daysInCurrMonth;
            month += 1;
        } else break;
    }
}
class DateGroup {
    int size;
    Date *dates[maxGroupSize];
public:
    DateGroup(int s) {
        size = s;
        for (int i = 0; i < s; i++) {
            dates[i] = new Date();
        }
    }
    ~DateGroup(){
        for (int i = 0; i < size; i++) {
            delete dates[i];
        }
    }
    bool containsDuplicates();
    void printDates() {
        for (int i = 0; i < size; i++) {
            cout << "m: " << dates[i]->getm() << ", d: " << dates[i]->getd() << ".\n";
        }
    }
};
bool DateGroup::containsDuplicates() {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (dates[i]->getm() == dates[j]->getm()) {
                if (dates[i]->getd() == dates[j]->getd()) {
                return true;
                }
            }
        }
    }
    return false;
}

int main() {
    srand(time(NULL));
    double percetages[largestGroup-smallestGroup + 1];
    for (int s = smallestGroup; s <= largestGroup; s++) {
        percetages[s-smallestGroup] = 0;
        for (int g = 0; g < numGroupsPerSize; g++) {
            DateGroup group = DateGroup(s);
            if (group.containsDuplicates()) percetages[s-smallestGroup] += 1;
        }
        percetages[s-smallestGroup] /= (numGroupsPerSize / 100);
    }
    cout << "Group Size and Chance of Duplicate Date:\n";
    for (int s = smallestGroup; s <= largestGroup; s++) {
        cout << s << ": " << setw(4) << percetages[s-smallestGroup] << " %\n";
    }
    return 0;
}