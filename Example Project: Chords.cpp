#include <iostream>
#include <fstream>
using namespace std;

#define whiteHeightBig 9
#define whiteHeightSmall 5
#define blackHeightBig 5
#define blackHeightSmall 3
#define whiteWidthBig 6 // has to be even and at least 2 bigger than blackWidth
#define whiteWidthSmall 4 // has to be even and at least 2 bigger than blackWidth
#define blackWidthBig 4 // has to be even
#define blackWidthSmall 2 // has to be even
#define assumed4thNoteDist 3
#define bigNum 100
#define immobilityBias 0.5
#define maxNumChordInput 100
#define maxTokensPerQuality 10
#define maxNumQualities 100
#define qualFilename "qualities.txt"

class Key {
    bool black;
    int numImposingKeys;
public:
    Key(bool b, int n)  {
        black = b; numImposingKeys = n;
    }
    bool isBlack() { return black;}
    int countImposingKeys() {return numImposingKeys;}
};
class Keyboard {
    Key *keys[24];
    bool big;
    int whiteHeight, blackHeight, whiteWidth, blackWidth;
    int topWidthOf(int keyIndex) {
        if (!keys[keyIndex]->isBlack()) {
            return whiteWidth - (blackWidth / 2) * keys[keyIndex]->countImposingKeys(); // the width of the top of white keys depends on the black keys cutting into the space.
        } else return blackWidth;
    }
    int heightOf(int keyIndex) {
        return keys[keyIndex]->isBlack() ? blackHeight : whiteHeight;
    }
public:
    Keyboard();
    void toggleSize();
    bool print(string,int[4], bool, string);
    bool print(string label, int indicated[4]) { // simple parameters for printing to the console
        return print(label,indicated, false, "");
    }
};
class ChordQuality {
    string name;
    int noteOffsets[4]; // half step differences from tonic
    string repTokens[maxTokensPerQuality]; //representative tokens. Compared against input chords (E.g. maj, min, maj7)
    int numToks;
public:
    ChordQuality(string n, int r[4], string t[maxTokensPerQuality], int numT) {
        name = n;
        numToks = numT;
        for (int i = 0; i < 4; i++) {
            noteOffsets[i] = r[i];
        }
        for (int i = 0; i < maxTokensPerQuality; i++) {
            repTokens[i] = t[i];
        }
    }
    ChordQuality() {} // to create an uninitialized var
    string getName() {return name;}
    int* getNoteOffsets() {return noteOffsets;}
    string* getRepToks() {return repTokens;}
    int getNumToks() {return numToks;}
    void print();
};

Keyboard::Keyboard() {
    bool black; int numImposingKeys;
    int imposingKeysRec[12] = {1,0,2,0,1,1,0,2,0,2,0,1}; // how many keys cut into the top of each key in an octave (starting on C)
    for (int i = 0; i < 24; i++) { // assigning each key in a 2-octave range an index from 0-23.
        numImposingKeys = imposingKeysRec[i % 12];
        black = numImposingKeys == 0; // black keys are not cut into, but all white keys are.
        keys[i] = new Key(black, numImposingKeys);
    }
    //set keyboards to print small by default
    big = false;
    whiteHeight = whiteHeightSmall;
    blackHeight = blackHeightSmall;
    whiteWidth = whiteWidthSmall;
    blackWidth = blackWidthSmall;
}
void Keyboard::toggleSize() {
    if (big) {
        big = false;
        whiteHeight = whiteHeightSmall;
        blackHeight = blackHeightSmall;
        whiteWidth = whiteWidthSmall;
        blackWidth = blackWidthSmall;
        cout << "\nThe printed keyboards will now be small.\n";
    } else {
        big = true;
        whiteHeight = whiteHeightBig;
        blackHeight = blackHeightBig;
        whiteWidth = whiteWidthBig;
        blackWidth = blackWidthBig;
        cout << "\nThe printed keyboards will now be big.\n";
    }
}
bool Keyboard::print(string label, int indicated[4], bool toFile, string filename) { //T=success,F=failure
    ostream *stream;
    ofstream file; // decalring outside the if so can make sure to close it at the end.
    if (toFile) {
        file.open(filename, ios::out | ios::ate | ios::app);
        if (!file.is_open()) {return false;}
        stream = &file;
    } else { stream = &cout; } //figured it out!
    *stream << "\n" << label << ": \n"; 
    for (int _ = 0; _ <= 14 * whiteWidth; _ ++) { *stream << "_";} // print the top border
    *stream << "\n";
    for (int line = 0; line <= whiteHeight; line++) {
        *stream << "|";
        if (line <= blackHeight) { // black keys are in play for key widths
            for (int k = 0; k < 24; k++) { // Keys
                for (int c = 0; c < topWidthOf(k) - 1; c++) { // Character
                    if (line == (int)heightOf(k)/2 && c == (int)topWidthOf(k)/2 - 1 && (k == indicated[0] || k == indicated[1] || k == indicated[2] || k == indicated[3])) {
                        *stream << "X";
                    } else if (line == blackHeight) {
                        if (keys[k]->isBlack()) {
                            *stream << "_";
                        } else *stream << " ";
                    } else *stream << " ";
                }
                *stream << "|";
            }
        } else { // black keys are not in play for key widths
            for (int k = 0; k < 14; k++) { // Keys
                for (int c = 0; c < whiteWidth - 1; c++) { // Characters
                    *stream << " ";
                }
                *stream << "|";
            }
        }
        *stream << "\n";
    }
    for (int _ = 0; _ <= 14 * whiteWidth; _ ++) { *stream << "_";} // print the bottom border
    *stream << "\n";
    if (file.is_open()) {
        file.close();
    }
    return true;
}
void ChordQuality::print() {
    cout << "Name: " << name << "\n";
    cout << "Notes: ";
    for (int i = 0; i < 4; i++) {
        if (!(i == 3 && noteOffsets[i] == -1)) {
            cout << noteOffsets[i] << " ";
        }
    }
    cout << "\nTokens: ";
    for (int i = 0; i < numToks; i++) {
        cout << repTokens[i] << " ";
    }
    cout << "\n-----------------\n";
}
int lineParse(string input, string delimiters, int maxNumWords, string *output) {
    int indexToSearchFrom = 0;
    int numWordsProcessed = 0;
    int startIndex, endIndex;
    for (int i = 0; i < maxNumWords; i++) {
        startIndex = input.find_first_not_of(delimiters, indexToSearchFrom);
        endIndex = input.find_first_of(delimiters, startIndex);
        if (endIndex == -1) {
            output[i] = input.substr(startIndex, input.size() - startIndex);
            numWordsProcessed += 1;
            break;
        }
        output[i] = input.substr(startIndex, endIndex-startIndex);
        numWordsProcessed += 1;
        indexToSearchFrom = endIndex + 1;
    }
    return numWordsProcessed;
}
int readQualFile(ChordQuality *quals[]) { //provide an array to put the pointers in, returns number of qualities (or -1 if failure).
    string line, chunks[3], noteOffsetsTemp[4], repTokens[maxTokensPerQuality];
    int noteOffsets[4], numQualities;
    ifstream file;
    file.open(qualFilename);
    if (file.is_open()) {
        for (int i = 0; i < maxNumQualities; i++){
            getline(file,line);
            lineParse(line,":;",3,chunks); //split quality line of form name:x,x,x,x;tok1,tok2 into the 3 chunks 'name' 'x,x,x,x' 'toks'.
            lineParse(chunks[1],",",4,noteOffsetsTemp); // seperate each noteOffset
            int numToks = lineParse(chunks[2],",",maxTokensPerQuality,repTokens); // seperate each token and count how many
            for (int j = 0; j < 4; j++) {
                noteOffsets[j] = stoi(noteOffsetsTemp[j]);
            }
            quals[i] = new ChordQuality(chunks[0],noteOffsets,repTokens, numToks);
            if (file.eof()) {
                numQualities = i + 1;
                break;
            }
        }
        return numQualities;
    }
    return -1;
}
void displayQualities(ChordQuality *quals[], int numQuals) {
    cout << "\n-----------------\n";
    for (int i = 0; i < numQuals; i++) {
        quals[i]->print();
    }
}
int nameToIndex(string name) { // ex. A# ->: 10, always chooses lower octave. Returns the note index (or -1 if failure).
    //turn the ascii value into the note without # or b
    int note = (int)name[0] - 67; // ASCII letters are 65-71, C is at 67
    if (note < -2 || note > 4) {cout << "Error: Tried to find the pitch of a note with an invalid root.\n"; return -1;} 
    else if (note < 0) {note += 7;} // loop A and B back around
    note *= 2; // most whole notes are two half steps apart
    if (note >4) note -= 1; // ^ but not E&F
    // slap on the accidental
    if (name[1] == '#') {note += 1;}
    else if (name[1] == 'b') {
        note -= 1;
        if (note < 0) note += 12;
    }
    else if (name.size() == 1) ; // do nothing bc no accidental
    else {cout << "Error: Tried to find the pitch of a note with an invalid accidental symbol.\n"; return -1;}
    return note;
}
int makeBetterOct(int *in, int ref) { // returns the distance between the reference note and the better octave. Modifies the input.
    int altOct = *in;
    if (*in < 12) {
        altOct += 12;
    } else altOct -=12;
    int dist = ref - *in;
    if (dist < 0) {dist *= -1;}
    int altOctDist = ref - altOct;
    if (altOctDist < 0) {altOctDist *= -1;}
    if (dist < altOctDist) return dist;
    else {
        *in = altOct;
        return altOctDist;
    }
}
int findCenter( int chord[4]) {
    int center = 0;
    for (int i = 0; i < 4; i++) { // averaging the indices
        if (chord[i] != -1) center += chord[i];
    }
    center /= chord[3] == -1 ? 3 : 4;
    return center;
}
bool parseChord(string chordName, int output[], ChordQuality *quals[], int numQuals) { // Provide array of quals to use. returns T if sucess. Fills in 4-item array with root position notes and puts -1 if there is an extra slot
    // parse string
    int root;
    string quality;
    if (chordName[1] == '#' || chordName[1] == 'b') {
        quality = chordName.substr(2, chordName.size() - 2);
        chordName[0] = toupper(chordName[0]); // use capital letter bc it will have the right ASCII value
        root = nameToIndex(chordName.substr(0,2));
    } else {
        quality = chordName.substr(1, chordName.size() - 1);
        chordName[0] = toupper(chordName[0]);
        root = nameToIndex(chordName.substr(0,1));
    }
    if (root == -1) return false; // figurig out the root went wrong.
    // assign notes
    ChordQuality currentQual;
    for (int i = 0; i < numQuals; i ++) {
        currentQual = *quals[i];
        for (int j = 0; j < currentQual.getNumToks(); j++) {
            if (quality == currentQual.getRepToks()[j]) { // input matched a quality token
                for (int k = 0; k < 4; k++) {
                    if (k == 3 && currentQual.getNoteOffsets()[3] == -1) { // there is no 4th note in the chord
                        output[k] = -1;
                    } else {
                        output[k] = root + currentQual.getNoteOffsets()[k];
                    }
                }
                if (output[0] < 0) { // if the lowest note is off the keyboard, move everything up an octave.
                        for (int k = 0; k < 4; k ++) {
                            if (!(output[k] == -1 && k == 3)) output[k] += 12;
                    }
                }
                return true;
            }
        }
    }
    return false;
}
void invertToMatch(int chord[4], int position) { // modifies the input
    int note;
    for (int i = 0; i < 4; i++) { // j is index of input note
        if (chord[i] == -1) continue;
        makeBetterOct(&chord[i], position);
    }
}
void invertOnce(int chord[4]) {
    if (chord[3] == -1) {
        chord[3] = chord[0];
        chord[0] = chord[1];
        chord[1] = chord[2];
        chord[2] = chord[3] + 12;
        chord[3] = -1; 
    } else {
        int temp = chord[0];
        chord[0] = chord[1];
        chord[1] = chord[2];
        chord[2] = chord[3];
        chord[3] = temp + 12;
    }
    if (chord[0] >= 12) {
        for(int i = 0; i < 4; i++) {
            if(chord[i] == -1 && i == 3) continue;
            chord[i] -= 12;
        }
    }

}
void addQuality(ChordQuality *quals[], int *numQuals) { //adds the quality and increments the numQuals variable
    string name, tokensTemp, tokens[maxTokensPerQuality];
    int numToks, noteOffsets[4];
    cout << "Enter the proper name for this chord quality (Ex. Major 7th chord): ";
    getline(cin,name);
    cout << "Enter the distance in half steps from the tonic to the lowest note (Ex. 0 or -1): ";
    cin >> noteOffsets[0];
    cout << "Enter the distance in half steps from the tonic to the second-lowest note (Ex. 4): ";
    cin >> noteOffsets[1];
    cout << "Enter the distance in half steps from the tonic to the next note up (Ex. 7): ";
    cin >> noteOffsets[2];
    cout << "If your chord quality has a fourth note, enter than distance. Otherwise, enter -1: ";
    cin >> noteOffsets[3];
    cout << "Now enter the 'tokens' to be used for this chord quality. This is how the quality is recognized from input. \nFor example, a major triad has tokens maj or M. Enter your tokens seperated by spaces (no spaces within tokens allowed): ";
    getline(cin, tokensTemp); // dummy to eat the \n.
    getline(cin, tokensTemp);
    numToks = lineParse(tokensTemp, " ", maxTokensPerQuality, tokens);
    quals[*numQuals] = new ChordQuality(name, noteOffsets, tokens, numToks);
    *numQuals += 1;
    //write to text the text file so persists next time run
    ofstream file;
    file.open(qualFilename, ios::ate | ios::app | ios::out);
    if (file.is_open()) {
        file << "\n" << name << ":";
        for (int i = 0; i < 4; i ++) {
            file << noteOffsets[i];
            if (i != 3) {file << ",";} else file << ";";
        }
        for (int i = 0; i < numToks; i++) {
            file << tokens[i];
            if (i != numToks - 1) file << ",";
        }
        cout << "Your quality has been added.\n";
    } else cout << "Could not add the new chord quality to the save file.\n";

}

int main() {
    Keyboard keyboard = Keyboard();
    ChordQuality * qualities[maxNumQualities];
    int numQuals = readQualFile(qualities); //puts the qualities from the file into 'qualities' and stores how many there were.
    string chordNames[maxNumChordInput];
    int bestSequence[maxNumChordInput][4], numChords; // declaring here so the export to txt option can access the last result
    string input, error = "";

    cout << "Enter up to " << maxNumChordInput << " chord names seperated by commas and/or spaces (no space between tonic and quality) \nand we will invert them for the satisfaction of even the laziest players.\n";
    cout << "\nTo begin, just start entering progressions. Type 'qualities' to see a list of all recognized chord quality notation. \nType 'add quality' to enter a custom chord quality. Type 'size' to toggle between large and small keyboards. \nType 'save' to save the previous result to a text file. Type 'example' to see example input. Type 'quit' to exit.\n\n";
    while (true) {
        error = ""; // reset the error message so that new input works
        getline(cin, input);
        if (input == "quit") {
            cout << "Thank you for using this software. Consider making a donation to Levi's savings account.\n";
            break;
        } else if (input == "help") {
            cout << "Type a chord progression sperated by commas and/or spaces to have it analyzed. Type 'qualities' to see a list of all\nrecognized chord quality notation. Type 'add quality' to enter a custom chord quality.\nType 'size' to toggle between large and small keyboards. Type 'save' to save the previous result to a text file.\nType 'example' to see example input. Type 'quit' to exit.\n";
        } else if (input == "qualities") {
            displayQualities(qualities, numQuals);
        } else if (input == "add quality") {
            addQuality(qualities, &numQuals);
        } else if (input == "example") {
            cout << "Example Input: Dm7, G7, CM7\n";
        } else if (input == "size") {
            keyboard.toggleSize();
        } else if (input == "save") {
            string filename;
            cout << "What would you like to call the file? If you enter the name of an existing file, \nthe new chords will be appended. (Include .txt) Enter it here: "; // helpful for adding to a song file in sections
            getline(cin, filename); // I could use cin, but there is some nonsense about getline not ignoring the newline from cin, causing my getline at the start of the looop to get confused if I use cin here. 
            for (int i = 0; i < numChords; i++) {
                if (!keyboard.print(chordNames[i], bestSequence[i], true, filename)) { // try to output the chord to a file, but complain if fail.
                    cout << "Unable to use that file name. Please try again.\n";
                    break;
                } else if (i == numChords - 1) cout << "File Saved.\n";
            }
        } else {
            //Treat the input as a chord progression (failed parsings should stop extraneous input from wreaking havoc).
            numChords = lineParse(input, ", ", maxNumChordInput,chordNames); // seperate the chords, using commas and/or spaces as delimiter
            if (numChords == maxNumChordInput) {
                cout << "By the way, you have reached the maxiumum of " << maxNumChordInput << " chords. \nSome chords may have been cut off.\n";
            }
            //Try process with all inversions of first chord and see which one requires the least travel overall
            int smallestTravel = bigNum; //want to have all of the versions to beat this. (Also want same logic for the first inversion, which is why not uninitialized.)
            int currentTravel = 0, currentCenter, progressionCenter;
            int currentSequence[maxNumChordInput][4]; // a place to store the sequence as it is generated so can compare against what was the best sequence.
            if (!parseChord(chordNames[0], currentSequence[0], qualities, numQuals)) { // tyr to parse the first chord and complain if fail.
                error = "The chord " + chordNames[0] + " could not be parsed. Please enter your chord(s) or input again: \n";
            }
            for (int i = 0; i < 4; i++) {
                if(error != "") break; // inside multiple things, might need to break, and would like to provide error message. Everything stops if something adds an error message.
                if (i == 3 && currentSequence[0][3] == -1) break;
                progressionCenter = findCenter(currentSequence[0]);
                currentCenter = progressionCenter;
                for (int j = 1; j < numChords; j++) {
                    if (i == 0) { // check that all the chords are valid. (only need to do once)
                        if (!parseChord(chordNames[j], currentSequence[j], qualities, numQuals)) {
                            error = "The chord " + chordNames[j] + " could not be parsed. Please enter your chord(s) again or enter a new command: \n";
                            break;
                        }
                    }
                    invertToMatch(currentSequence[j], ((int)(currentCenter + (progressionCenter * immobilityBias)) / (1 + immobilityBias))); // arbitrarily weighted average for matching to the previous chord's center and the overall center so the chords don't run away. 
                    int center = findCenter(currentSequence[j]);
                    int travel = currentCenter - center; // find the distance in the chords' center positions
                    if (travel < 0) travel *= -1;
                    currentTravel += travel; // add that distnce to the sequence's total.
                    currentCenter = center;
                }
                if(error != "") break;
                if (currentTravel < smallestTravel || (currentTravel == smallestTravel && currentSequence[0][0] < bestSequence[0][0])) { // update the best sequence. If they are tied, favor the lower one. 
                    smallestTravel = currentTravel;
                    for (int j = 0; j < numChords; j++) {
                        for (int k = 0; k < 4; k++) {
                            bestSequence[j][k] = currentSequence[j][k];
                        }
                    }
                }
                invertOnce(currentSequence[0]); // invert the original chord so the process can be tried with that inversion as the seed. 
                currentTravel = 0;
            }
            if (error != "") {
                cout << error;
                continue;
            }
            for (int i = 0; i < numChords; i++) {
                keyboard.print(chordNames[i], bestSequence[i]);
            }
        }
        cout << "\n-------------------(Type 'help' to see the options)---------------------\n\n";
    }

}
