#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <windows.h>

using namespace std;

const int screenX = 10,screenY=10;
const char char_not_dig = '?';
const char char_flag = 'F';
const char char_clear = '-';

enum STATE{
    PLAY,
    LOST,
    WIN
};

enum ACTION{
    FLAG,
    DIG
};

STATE currentState = PLAY;
ACTION currentAction = DIG;

class controller{
private:
    const int numberOfBombs = 15;
    int flagsLeft;

    vector<char> field;
    vector<int> bombs;
    map<int,char> flags;
    const int fieldSize = screenX*screenY;
    int currentPos;

    vector<int> dpBombs;

    void setTextColor(std::string color) {
        int colorID;
        if(color == "blue"){
            colorID = 3;
        } else if(color == "purple"){
            colorID = 5;
        }  else if(color == "red"){
            colorID = 4;
        } else if(color == "green"){
            colorID = 2;
        } else {
        colorID = 7;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorID);
    }

    void dig(int currentBlock){

        if(currentBlock < 0 || currentBlock >= fieldSize ){
           return;
        }

        if(dpBombs.at(currentBlock) != -1){
            return;
        }

        for(int i=0;i<numberOfBombs;i++){
            if(bombs[i] == currentBlock){
                return;
            }
        }

        field[currentBlock] = char_clear;

        //Calculate Number of bombs around the block
        int bombsAround = 0;
        for(int i=0;i<numberOfBombs;i++){
           if(  bombs[i] == currentBlock - screenX   || bombs[i] ==  currentBlock + screenX ){
                bombsAround++;
           }
           if((currentBlock+1)%screenX != 0 &&( bombs[i] == currentBlock + 1 || bombs[i] ==  currentBlock - screenX + 1 || bombs[i] ==  currentBlock + screenX + 1)){//Edge cases
                bombsAround++;
           }
           if(currentBlock%screenX != 0 && ( bombs[i] == currentBlock - 1 || bombs[i] ==  currentBlock - screenX - 1 || bombs[i] ==  currentBlock + screenX - 1)){
                bombsAround++;
           }
        }
        if(bombsAround > 0){
            char charBombsAround = (char)bombsAround + (char)'0';
            field[currentBlock] = charBombsAround;
        }

        dpBombs.at(currentBlock) = bombsAround;

        dig(currentBlock - screenX);
        dig(currentBlock + screenX);

        if((currentBlock+1)%screenX != 0){//Edge cases
            dig(currentBlock - screenX + 1);
            dig(currentBlock + 1);
            dig(currentBlock + screenX + 1);
        }
        if(currentBlock%screenX != 0){
            dig(currentBlock - 1);
            dig(currentBlock - screenX - 1);
            dig(currentBlock + screenX - 1);
        }
        return;
    }

    void action(){
        if(currentAction == DIG){
            for(int i=0;i<numberOfBombs;i++){
                if(bombs[i] == currentPos){
                    //Lose Game
                    field[currentPos] = 'X';
                    //currentState = LOST;
                    return;
                }
                dig(currentPos);
            }
        } else {//FLAG
            if(field[currentPos] != char_flag && flagsLeft > 0){
                flags[currentPos] = field[currentPos];
                field[currentPos] = char_flag;
                flagsLeft--;
            } else if(field[currentPos] == char_flag){
                field[currentPos] = flags[currentPos];
                flagsLeft++;
            }

        }
        return ;
    }

    void setBombs(){
        // Define range
        int minRandge = 0;
        int maxRange = screenX*screenY - 1;//99

        // Initialize a random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(minRandge, maxRange);

        for(int i=0;i<numberOfBombs;i++){
             int newBomb = distrib(gen);

             int j=0;
             while(j<i){
                if(bombs[j] == newBomb){
                    newBomb = distrib(gen);
                    j=0;
                }
                j++;
             }
             bombs[i] = newBomb;
        }


    }

public:
    controller(){
        field.resize(screenX * screenY, char_not_dig);
        bombs.reserve(numberOfBombs);
        flagsLeft = numberOfBombs;
        setBombs();

        dpBombs.resize(screenX * screenY,-1);

        currentPos = 0;
    }

    void handleKeyInput(KEY_EVENT_RECORD event){
        WORD VirtualKey = event.wVirtualKeyCode;
        char c = event.uChar.AsciiChar;
        if(VirtualKey == VK_RETURN){
            action();
            return;
        }
        if(VirtualKey == VK_SPACE){
            if(currentAction == FLAG){
                currentAction = DIG;
            } else  currentAction = FLAG;
            return;
        }
        if(VirtualKey == VK_LEFT || c =='a'){
            if(currentPos > 0 &&  currentPos%screenX != 0)
                currentPos--;
            return;
        }
        if(VirtualKey == VK_RIGHT || c =='d' &&  (currentPos + 1)%screenX != 0){
            if(currentPos < fieldSize)
                currentPos++;
            return;
        }
        if(VirtualKey == VK_DOWN | c =='s'){
            if(currentPos < fieldSize - screenX)
                currentPos += screenX;
            return;
        }
        if(VirtualKey == VK_UP || c =='w'){
            if(currentPos > screenX)
                currentPos-= screenX;
            return;
        }
        return;
    }

    void print(CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){

        system("cls");

        //get Console Size
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        if (csbi.srWindow.Bottom != oldcsbi.srWindow.Bottom || csbi.srWindow.Right != oldcsbi.srWindow.Right)
        {
            oldcsbi = csbi;

            short horizontalOffset = (short)(csbi.srWindow.Right / 2) -   screenX;
            short verticalOffset = (short)((csbi.srWindow.Top + 2* screenY) / 2);

            coord.X = horizontalOffset + 3;
            SetConsoleCursorPosition(hConsoleOutput, coord);

            cout<<"Mine sweeper";

            coord.X = horizontalOffset;
            coord.Y+= 2;

            SetConsoleCursorPosition(hConsoleOutput, coord);

            for(int i=0;i<screenX*screenY;i++){
                if(field[i] == char_flag){
                    setTextColor("red");
                } else if(field[i] == char_clear){
                    setTextColor("green");
                } else if(field[i] == '?'){
                    setTextColor("blue");
                }
                if(currentPos == i){
                    setTextColor("purple");
                }

                cout<<field.at(i);
                coord.X += 2;
                if((i+1)%screenX == 0){
                   coord.X = horizontalOffset;
                   coord.Y += 1;
                }
                SetConsoleCursorPosition(hConsoleOutput,coord);
                setTextColor("normal");
            }
            const int infoSize = 6;
            string info[infoSize] = {"Current Action: ","Number of Flags: ","How to play: "," - Move with Arrow keys or WASD"," - press SPACE to toggle between DIG/FLAG"," - Press ENTER for action" };


            coord.X = horizontalOffset ;
            coord.Y = verticalOffset + 2;

            for(int i=0;i<infoSize;i++){
                coord.Y += 1;
                SetConsoleCursorPosition(hConsoleOutput, coord);
                setTextColor("normal");

                if(i ==0 || i == 1){
                    setTextColor("green");
                }

                cout<<info[i];

                if(i == 0){
                    setTextColor("red");
                    if(currentAction == DIG) cout<<"DIG";
                    else cout<<"FLAG";
                }

                if(i == 1){
                    setTextColor("red");
                    cout<<flagsLeft;
                    setTextColor("normal");
                    coord.Y ++;
                }
            }
        }
    }

    void win(){
        if(flagsLeft > 0){
            return ;
        }
        for(int i=0;i<numberOfBombs;i++){
            if(field[bombs[i]] != char_flag){
                return ;
            }
        }
        currentState = WIN;
    }

};

int main(){
    controller Ctrl;

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (hStdin == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to get the console input handle.\n";
        return 1;
    }

    DWORD originalMode;
    if (!GetConsoleMode(hStdin, &originalMode)) {
        std::cerr << "Error: Unable to get console mode.\n";
        return 1;
    }

    // Modify the console mode to disable line input and echo input
    DWORD newMode = originalMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    if (!SetConsoleMode(hStdin, newMode)) {
        std::cerr << "Error: Unable to set console mode.\n";
        return 1;
    }

    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    if (GetConsoleCursorInfo(hConsoleOutput, &cursorInfo)) {
        cursorInfo.bVisible = false; // Set caret visibility
        SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    } else {
        std::cerr << "Error: Unable to retrieve console cursor info.\n";
    }

    //get console Size
    CONSOLE_SCREEN_BUFFER_INFO oldcsbi{};
    COORD coord{};


    INPUT_RECORD inputRecord;
    DWORD eventsRead;

    Ctrl.print(oldcsbi, coord);
    while (currentState == PLAY) {
        if (ReadConsoleInput(hStdin,&inputRecord, 1, &eventsRead)) {
            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {

                KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;

                Ctrl.handleKeyInput(keyEvent);
                Ctrl.win();
                Ctrl.print(oldcsbi, coord);
            }
        }
    }
    if(currentState == LOST){
        cout<<"YOU LOST :(";
    } else {
         cout<<"YOU WON :)";
    }
    SetConsoleMode(hStdin,originalMode);

    return 0;
}
