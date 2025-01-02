#include <iostream>
#include <strings.h>
#include <vector>
#include <map>
#include <random>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

const int screenX = 10,screenY=10;
const char char_not_dig = '?';
const char char_flag = 'F';
const char char_clear = '-';

const int gameDepth = 5;

enum STATE{
    MENU,
    PLAY,
    LOST,
    WIN,
    END
};

enum MENU_STATE{
    MAIN,
    MENU_PLAY,
    STATISTICS,
    NEW_GAME
};

enum ACTION{
    FLAG,
    DIG
};

STATE currentState = MENU;
ACTION currentAction = DIG;
MENU_STATE currentMenuState = MAIN;

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

class timer{
private:
    chrono::time_point<chrono::steady_clock> startTime, endTime;
    bool running;
public:
    timer(){
        running = false;
    }
    void start(){
        startTime = chrono::steady_clock::now();
        running = true;
        return;
    }
    void stop(){
        running = false;
        endTime = chrono::steady_clock::now();
    }
    bool getRunning(){
        return running;
    }
    int CalculateTime() const{
        if (!running){
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
            static_cast<int>(duration.count());
        }
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        return static_cast<int>(duration.count());
    }

    void print(CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        coord.X = (short)(csbi.srWindow.Right / 2);
        coord.Y = (short)((csbi.srWindow.Top ) / 2) +  2 * screenY;
        SetConsoleCursorPosition(hConsoleOutput,coord);

        cout<<"Timer: "<< CalculateTime()<<" s ";
    }
};

class controller{
private:
    const int numberOfBombs = 1;
    int flagsLeft;

    vector<char> field;
    vector<int> bombs;
    map<int,char> flags;
    const int fieldSize = screenX*screenY;
    int currentPos;

    vector<int> dpBombs;

    void dig(int currentBlock,int depth){

        if(depth <= 0){
            return;
        }

        if(field[currentBlock] == char_flag){
            return;
        }

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

        depth --;

        field[currentBlock] = char_clear;

        //Calculate Number of bombs around the block
        int bombsAround = 0;
        for(int i=0;i<numberOfBombs;i++){
           if(  bombs[i] == currentBlock - screenX   || bombs[i] ==  currentBlock + screenX ){
                bombsAround++;
           }

           if((currentBlock+1)%screenX != 0 &&( bombs[i] == currentBlock + 1 || bombs[i] == currentBlock + 1 - screenX || bombs[i] == currentBlock + 1 + screenX )){//Edge cases
               bombsAround++;
           }

           if(currentBlock%screenX != 0 && ( bombs[i] == currentBlock - 1 || bombs[i] == currentBlock - 1 - screenX || bombs[i] == currentBlock - 1 + screenX )){
                bombsAround++;
           }



        }

        if(bombsAround > 0){
            char charBombsAround = (char)bombsAround + (char)'0';
            field[currentBlock] = charBombsAround;
        }

        dpBombs.at(currentBlock) = bombsAround;

        dig(currentBlock - screenX,depth);
        dig(currentBlock + screenX,depth);

        if((currentBlock+1)%screenX != 0){//Edge cases
            dig(currentBlock + 1,depth);
        }
        if(currentBlock%screenX != 0){
            dig(currentBlock - 1,depth);
        }
        return;
    }

    void action(){
        if(currentAction == DIG){
            for(int i=0;i<numberOfBombs;i++){
                if(bombs[i] == currentPos){
                    //Lose Game
                    field[currentPos] = 'X';
                    currentState = LOST;
                    return;
                }
                dig(currentPos,gameDepth);
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
            if(currentPos >= screenX)
                currentPos-= screenX;
            return;
        }
        return;
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

public:
    controller(){
        field.resize(screenX * screenY, char_not_dig);
        bombs.reserve(numberOfBombs);
        flagsLeft = numberOfBombs;
        setBombs();

        dpBombs.resize(screenX * screenY,-1);

        currentPos = 0;
    }


    void mainControl(KEY_EVENT_RECORD event,CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){
        handleKeyInput(event);
        win();
        print(oldcsbi, coord);
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
            short verticalOffset = (short)((csbi.srWindow.Top ) / 2) + screenY;

            coord.X = horizontalOffset + screenX;
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
                coord.X += 3;
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

};

class playMenu;

class newGameMenu{
private:
    int currentPosition;
    const int numberOfDifficultyOptions = 3;
    string DifficultyOptions[3] = {"Easy","Medium","Hard"};

    string fileName;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void action(){
        if(currentPosition == 0){ //Easy

            return;
        } else if(currentPosition == 1){ // Medium

            return;
        } else if (currentPosition == 2){ // Hard
            return;
        }
    }

    void handleKeyInput(KEY_EVENT_RECORD event){
        WORD VirtualKey = event.wVirtualKeyCode;
        char c = event.uChar.AsciiChar;
        fileName+=c;

        if(VirtualKey == VK_RETURN){
            action();
            return;
        }
        if(VirtualKey == VK_RIGHT){
            if(currentPosition < numberOfDifficultyOptions - 1){
                currentPosition ++ ;
            }
            return;
        }
        if(VirtualKey == VK_LEFT ){
            if(currentPosition > 0)
                currentPosition-- ;
        }
        if(VirtualKey == VK_DOWN ){
            if(currentPosition < numberOfDifficultyOptions)
                currentPosition = numberOfDifficultyOptions ;
            else
                currentPosition = numberOfDifficultyOptions + 1 ;
        }
        if(VirtualKey == VK_UP ){
            if(currentPosition <= numberOfDifficultyOptions)
                currentPosition = 0 ;
            else
                currentPosition = numberOfDifficultyOptions;
        }
    }
public:
    newGameMenu(CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPosition = 0;
        setOldcsbi = setOldcsbi;
        coord = setCoord;
    }

    void mainControl(KEY_EVENT_RECORD event,CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){
        handleKeyInput(event);
        if(currentMenuState == NEW_GAME){
            print();
        }
    }

    void print(){

        system("cls");

        //get Console Size
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        oldcsbi = csbi;

        const int Xoffset = 10;

        coord.X = (short)(csbi.srWindow.Right / 2) - Xoffset;
        coord.Y = (short)((csbi.srWindow.Bottom) / 3);

        SetConsoleCursorPosition(hConsoleOutput, coord);

        cout<<"Mine sweeper";

        coord.Y+= 4;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        cout<<"File Name: ";
        cout<<fileName;

        coord.Y += 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        string Diff = "Difficulty:";
        cout<<Diff;

        coord.X += Diff.length() + 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        int DiffixultyOffsetX = Diff.length() + 2;
        for(int i=0;i<numberOfDifficultyOptions;i++){
            if(currentPosition == i){
                setTextColor("red");
            }
            cout<<DifficultyOptions[i];

            int wordSize = DifficultyOptions[i].length() + 2;
            coord.X+= wordSize;
            DiffixultyOffsetX += wordSize;
            SetConsoleCursorPosition(hConsoleOutput, coord);
            setTextColor("normal");
        }

        coord.X -= DiffixultyOffsetX;
        coord.Y += 4;
        SetConsoleCursorPosition(hConsoleOutput, coord);
        if(currentPosition == numberOfDifficultyOptions){
            setTextColor("red");
        }
        cout<<"Save";

        coord.Y+= 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);
        setTextColor("normal");

        if(currentPosition == numberOfDifficultyOptions + 1){
            setTextColor("red");
        }
        cout<<"Back";
        setTextColor("normal");
    }
};

class mainMenu{
private:
    int currentPosition;

    playMenu* playMenuPointer;
    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    const int numberOfOptions = 3;
    string Options[3] = {"Play","Statistics","Exit"};


    void action();

    void handleKeyInput(KEY_EVENT_RECORD event);

public:
    mainMenu(playMenu* setPlayMenu,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord);

    void mainControl(KEY_EVENT_RECORD event);

    void print();
};

class playMenu{
private:
    int currentPosition;
    const int numberOfPlayOptions = 3;
    string PlayOptions[3] = {"New Game","Load Game","Back"};

    mainMenu* mainMenuPointer;
    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void action(){
        if(currentPosition == 0){ //New
            currentMenuState = NEW_GAME;
            return;
        } else if(currentPosition == 1){ // Load

            return;
        } else if (currentPosition == 2){ // Back
            currentMenuState = MAIN;
            mainMenuPointer -> print();
            return;
        }
    }

    void handleKeyInput(KEY_EVENT_RECORD event){
        WORD VirtualKey = event.wVirtualKeyCode;
        char c = event.uChar.AsciiChar;

        if(VirtualKey == VK_RETURN){
            action();
            return;
        }
        if(VirtualKey == VK_DOWN || c =='s'){
            if(currentPosition < numberOfPlayOptions - 1){
                currentPosition ++ ;
            }
            return;
        }
        if(VirtualKey == VK_UP || c =='w'){
            if(currentPosition > 0)
                currentPosition-- ;
        }
    }
public:
    playMenu(CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPosition = 0;
        setOldcsbi = setOldcsbi;
        coord = setCoord;
    }

    void setMainMenuPointer(mainMenu* setMainMenuPointerVariable){
        mainMenuPointer = setMainMenuPointerVariable;
    }

    void mainControl(KEY_EVENT_RECORD event,CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){
        handleKeyInput(event);
        if(currentMenuState == MENU_PLAY){
            print();
        }
    }

    void print(){

        system("cls");

        //get Console Size
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        oldcsbi = csbi;

        const int Xoffset = 10;

        coord.X = (short)(csbi.srWindow.Right / 2) - Xoffset;
        coord.Y = (short)((csbi.srWindow.Bottom) / 3);

        SetConsoleCursorPosition(hConsoleOutput, coord);

        cout<<"Mine sweeper";

        coord.Y+= 4;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        for(int i=0;i<numberOfPlayOptions;i++){
            if(currentPosition == i){
                setTextColor("red");
            }
            cout<<PlayOptions[i];

            coord.Y+= 2;
            SetConsoleCursorPosition(hConsoleOutput, coord);
            setTextColor("normal");
        }
    }
};

void mainMenu::action(){
    if(currentPosition == 0){//Play
        currentMenuState = MENU_PLAY;
        playMenuPointer -> print();
        return;
    } else if(currentPosition == 1){ // Statistic
        currentMenuState = STATISTICS;
        return;
    } else if (currentPosition == 2){ // Exit
        currentState = END;
    }
}
void mainMenu::handleKeyInput(KEY_EVENT_RECORD event){
    WORD VirtualKey = event.wVirtualKeyCode;
    char c = event.uChar.AsciiChar;

    if(VirtualKey == VK_RETURN){
        action();
        return;
    }
    if(VirtualKey == VK_DOWN | c =='s'){
          if(currentPosition < numberOfOptions - 1)
            currentPosition ++ ;
        return;
    }
    if(VirtualKey == VK_UP || c =='w'){
        if(currentPosition > 0)
            currentPosition-- ;
        return;
    }
    return;
}

mainMenu::mainMenu(playMenu* setPlayMenu,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
    currentPosition = 0;
    currentMenuState = MAIN;
    playMenuPointer = setPlayMenu;

    oldcsbi = setOldcsbi;
    coord = setCoord;
}

void mainMenu::mainControl(KEY_EVENT_RECORD event){
    handleKeyInput(event);
    if(currentMenuState == MAIN){
        print();
    }
}

void mainMenu::print(){

    system("cls");

    //get Console Size
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

    oldcsbi = csbi;

    const int Xoffset = 10;

    coord.X = (short)(csbi.srWindow.Right / 2) - Xoffset;
    coord.Y = (short)((csbi.srWindow.Bottom) / 3);

    SetConsoleCursorPosition(hConsoleOutput, coord);

    cout<<"Mine sweeper";

    coord.Y+= 4;
    SetConsoleCursorPosition(hConsoleOutput, coord);

    for(int i=0;i<numberOfOptions;i++){
        if(currentPosition == i){
            setTextColor("red");
        }

        cout<<Options[i];

        coord.Y+= 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);
        setTextColor("normal");
    }
}




void printEnd(bool win,int TimePlayed, CONSOLE_SCREEN_BUFFER_INFO oldcsbi, COORD coord){

    system("cls");

    //get Console Size
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

    coord.X = (short)(csbi.srWindow.Right / 2);
    coord.Y = (short)(csbi.srWindow.Bottom / 2);

    SetConsoleCursorPosition(hConsoleOutput,coord);

    if(win){
        cout<<"You Won :) ";
    } else {
        cout<<"YOu Lost :( " ;
    }

    coord.Y += 2;
    SetConsoleCursorPosition(hConsoleOutput,coord);

    cout<<"You played for "<<TimePlayed<<" seconds";

}

int main(){
    controller Ctrl;
    timer Timer;

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

    playMenu PlayMenu(oldcsbi,coord);
    mainMenu MainMenu(&PlayMenu,oldcsbi,coord);
    newGameMenu NewGameMenu(oldcsbi,coord);

    PlayMenu.setMainMenuPointer(&MainMenu);

    INPUT_RECORD inputRecord;
    DWORD eventsRead;

    MainMenu.print();
    while (currentState == PLAY || currentState == MENU) {
        if(currentState == PLAY){
            Timer.print(oldcsbi,coord);
        }
        if (PeekConsoleInput(hStdin,&inputRecord, 1, &eventsRead) && eventsRead > 0 )  {
            ReadConsoleInput(hStdin, &inputRecord, 1, &eventsRead);
            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {

                KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;

                if(currentState == MENU){
                    if(currentMenuState == MAIN){
                        MainMenu.mainControl(keyEvent);
                    } else if(currentMenuState == MENU_PLAY){
                        PlayMenu.mainControl(keyEvent,oldcsbi,coord);
                    } else if(currentMenuState == NEW_GAME){
                        NewGameMenu.mainControl(keyEvent,oldcsbi,coord);
                    }


                } else if(currentState == PLAY){
                    if(!Timer.getRunning()){
                        Timer.start();
                    }

                    Ctrl.mainControl(keyEvent,oldcsbi,coord);
                }

            }
        }
    }
    Timer.stop();
    int TimePlayed = Timer.CalculateTime();
    if(currentState == LOST){
         printEnd(false,TimePlayed,oldcsbi,coord);
    } else if(currentAction == WIN){
         printEnd(true,TimePlayed,oldcsbi,coord);
    }

    SetConsoleMode(hStdin,originalMode);

    return 0;
}
