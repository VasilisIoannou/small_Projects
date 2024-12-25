#include <iostream>
#include <vector>
#include<map>
#include <windows.h>

using namespace std;

const int screenX = 30,screenY=10;
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
    const int numberOfBombs = screenY;
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
        //Calculate random numbers
        bombs[0] = 10;
        for(int i=1;i<screenY;i++){
            bombs[i] = bombs[i-1] + screenX;
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

    void print(){
        system("cls");
        for(int i=0;i<screenX*screenY;i++){
            if(field[i] == char_flag){
                setTextColor("red");
            } else if(field[i] == char_clear){
                setTextColor("green");
            }
            if(currentPos == i){
                setTextColor("purple");
            }
            cout<<field.at(i)<<' ';
            if((i+1)%screenX == 0) cout<<endl;

            setTextColor("normal");
        }
        cout<<"Current Action ";
        if(currentAction == DIG) cout<<"DIG"<<endl;
        else cout<<"FLAG"<<endl;

        cout<<endl<<"Number of Flags: "<<flagsLeft<<endl;
        cout<<endl<<"Move with Arrow keys or WASD"<<endl<<"press SPACE to toggle between DIG/FLAG"<<endl<<"Press ENTER for action";
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

    INPUT_RECORD inputRecord;
    DWORD eventsRead;

    while (currentState == PLAY) {
        if (ReadConsoleInput(hStdin,&inputRecord, 1, &eventsRead)) {
            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {

                KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;

                Ctrl.handleKeyInput(keyEvent);
                Ctrl.win();
                Ctrl.print();
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
