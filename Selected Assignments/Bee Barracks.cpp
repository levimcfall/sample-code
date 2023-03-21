#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define honey 0
#define carpenter 1
#define wasp 2
#define asianGiantHornet 3

class Bee {
    string name;
    int type;
    int polinationCapacity, mandibleStrength, speed, attack;
public:
    Bee();
    Bee(int);
    string getName() {return name;}
    int getPol() {return polinationCapacity;}
    int getMandStren() {return mandibleStrength;}
    int getSpeed() {return speed;}
    int getAttack() {return attack;}
};
class Barrack {
    Bee * slots[3];
    bool slotFilled[3];
public:
    Barrack() {
        for (int i = 0; i < 3; i++) {
            slotFilled[i] = false;
        }
    }
    void addBee(Bee*);
    void dismis(int s) {
        slotFilled[s-1] = false; // input is 1-3.
    }
    void print(bool, string);

};
Bee::Bee() {
        type = rand() % 4;
        polinationCapacity = (1 + rand() % 6) + (1 + rand() % 6);
        attack = (1 + rand() % 6) + (1 + rand() % 6);
        speed = (1 + rand() % 6) + (1 + rand() % 6);
        mandibleStrength = (1 + rand() % 6) + (1 + rand() % 6);
        cout << "Please name your ";
        switch (type) {
            case honey: 
                cout << "honey bee: ";
                polinationCapacity += 8;
                break;
            case carpenter: 
                cout << "carpenter bee: ";
                mandibleStrength += 8;
                break;
            case wasp: 
                cout << "wasp: \n";
                speed += 8;
                break;
            case asianGiantHornet: 
                cout << "aisan giant hornet: ";
                attack += 8;
                break;
            default: 
                cout << "The world just blew up...\n";
                break;
        }
        cin >> name;

    }
Bee::Bee(int t) {
        type = t;
        polinationCapacity = (1 + rand() % 6) + (1 + rand() % 6);
        attack = (1 + rand() % 6) + (1 + rand() % 6);
        speed = (1 + rand() % 6) + (1 + rand() % 6);
        mandibleStrength = (1 + rand() % 6) + (1 + rand() % 6);
        cout << "Please name your ";
        switch (type) {
            case honey: 
                cout << "honey bee: ";
                polinationCapacity += 8;
                break;
            case carpenter: 
                cout << "carpenter bee: ";
                mandibleStrength += 8;
                break;
            case wasp: 
                cout << "wasp: ";
                speed += 8;
                break;
            case asianGiantHornet: 
                cout << "aisan giant hornet: ";
                attack += 8;
                break;
            default: 
                cout << "The world just blew up...\n";
                break;
        }
        cin >> name;
    }
void Barrack::print(bool toFile, string filename) {
        char lines[7][68] = {
            "+-------space-1-------+-------space-2-------+--------space-3------+",
            "|Name:                |Name:                |Name:                |",
            "|Polination:          |Polination:          |Polination:          |",
            "|Mandible Strength:   |Mandible Strength:   |Mandible Strength:   |",
            "|Speed:               |Speed:               |Speed:               |",
            "|Attack:              |Attack:              |Attack:              |",
            "+---------------------+---------------------+---------------------+"
        };
        for (int i = 0; i < 3; i++) {
            if (slotFilled[i]) {
                string name = slots[i]->getName();
                for (int j = 0; j<name.length(); j++) {
                    lines[1][i * 22 + 1 + 6 + j] = name[j];
                }
                string pol = to_string(slots[i]->getPol());
                for (int j = 0; j<pol.length(); j++) {
                    lines[2][i * 22 + 1 + 12 + j] = pol[j];
                }
                string mandstrn = to_string(slots[i]->getMandStren());
                for (int j = 0; j<mandstrn.length(); j++) {
                    lines[3][i * 22 + 1 + 19 + j] = mandstrn[j];
                }
                string speed = to_string(slots[i]->getSpeed());
                for (int j = 0; j<speed.length(); j++) {
                    lines[4][i * 22 + 1 + 7 + j] = speed[j];
                }
                string attack = to_string(slots[i]->getAttack());
                for (int j = 0; j<attack.length(); j++) {
                    lines[5][i * 22 + 1 + 8 + j] = attack[j];
                }
            }
        }
        if (toFile) {
            ofstream output;
            output.open(filename);
            if (output.is_open()) {
                for (int i = 0; i < 7; i++) {
                    output << lines[i] << "\n";
                }
            }
        } else {
            for (int i = 0; i < 7; i++) {
                    cout << lines[i] << "\n";
            }
        }
    }
void Barrack::addBee(Bee *b) {
    int slot = 0;
    while (slotFilled[slot] == true && slot < 3) slot += 1;
    if (slot > 2) {
        cout << "Sorry, there are no empty slots.\n";
        return;
    } else {
        slots[slot] = b;
        slotFilled[slot] = true;
    }
}
int main() {
    srand(time(NULL));
    Barrack barrack = Barrack();
    cout << "Welcome to the Bee Barracks.\nWhat would you like to do?\n";
    int input;
    bool active = true;
    while (active) {
        cout << "Your choices are:\n";
        cout << "(1) Add a Bee of a particular type.\n";
        cout << "(2) Add a Bee of a random type.\n";
        cout << "(3) Send a Bee packing.\n";
        cout << "(4) Output a record of the barracks to a text file\n";
        cout << "(5) Quit.\n";
        cout << "Enter your choice here: ";
        cin >> input;
        Bee * newBee;
        if (input == 5) {
            active = false;
        } else if (input > 0 && input < 5) {
            switch (input) {
                case 1: 
                    int type;
                    cout << "Enter the code for the type of your bee:\n(1) Honey Bee\n(2) Carpenter Bee\n(3) Wasp\n(4) Asian Giant Hornet\n";
                    cin >> type;
                    if (type > 0 && type < 5) {
                        type -= 1;
                        newBee = new Bee(type);
                        barrack.addBee(newBee);
                        barrack.print(false, "");
                    } else {
                        cout << "Unrecognized code. Please try again.\n";
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                    break;
                case 2: 
                    newBee = new Bee();
                    barrack.addBee(newBee);
                    barrack.print(false, "");
                    break;
                
                case 3:
                    int space;
                    cout << "Which bee would you like to send packing?\nEnter their space number: ";
                    cin >> space;
                    if (space > 0 && space < 4) {
                        barrack.dismis(space);
                        barrack.print(false, "");
                    } else {
                        cout << "\nInvalid space number. Please try again.\n";
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                    break;
                case 4: 
                    string fileName;
                    cout << "Enter the name of a file to output to. If it does not exist, it will be created. \nDo not include an extension. Enter it here: ";
                    cin >> fileName;
                    barrack.print(true, fileName + ".txt");
                    cout << "\nThe record has been output.\n";
                    break; 
            }
            
        } else {
            cout << "Invalid input. Please try again.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    return 0;
}