#include <iostream>
#include <strings.h>
#include <vector>
#include <map>
#include <random>
#include <windows.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <limits.h>
#include <cmath>

using namespace std;


const char char_not_dig = '?';
const char char_flag = 'F';
const char char_clear = '-';

const int EasyScreenX = 10;
const int EasyScreenY = 10;
const int EasyBombs = 20;
const int EasyDepth = 5;

const int MediumScreenX = 20;
const int MediumScreenY = 20;
const int MediumBombs = 60;
const int MediumDepth = 7;

const int HardScreenX = 30;
const int HardScreenY = 30;
const int HardBombs = 120;
const int HardDepth = 10;

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
    LOAD,
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
int currentFile;

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

int stringToInt(string text, bool negative = false) {
    int number = 0;
    int multiplier = 1;

    if(text.size() == 1){
        number = text[0] - '0';
        return negative ? -number : number;
    }

    // Iterate from the end of the string
    for (int i = text.size() - 1; i >= 0; --i) {
        if (text[i] >= '0' && text[i] <= '9') {
            int charInt = text[i] - '0';
            number += charInt * multiplier;
            multiplier *= 10;
        }
    }

    return negative ? -number : number;
}

class timer{
private:
    chrono::time_point<chrono::steady_clock> startTime, endTime;
    bool running;

    int prevTime;

    string timeFileName = "timer.dat";

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void setCoord(){
        coord.X = (short)(oldcsbi.srWindow.Right / 2) + 8;
        coord.Y = (short)((oldcsbi.srWindow.Bottom )) + 2;
    }
public:
    timer(CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        running = false;
        oldcsbi = setOldcsbi;
        coord = setCoord;
    }

    void saveTime(){
        ofstream saveFile(timeFileName);

        saveFile<<prevTime + CalculateTime();

        saveFile.close();
    }

    void loadTime(){
        ifstream readFile(timeFileName);

        string line;
        getline(readFile,line);
        prevTime = stringToInt(line);

        readFile.close();
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

    void print(){
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        setCoord();
        SetConsoleCursorPosition(hConsoleOutput,coord);

        cout<<"Timer: "<< CalculateTime() + prevTime<<" s ";
    }
};

class controller{
private:
    int numberOfBombs;
    int flagsLeft;

    int depth;

    int difficulty;

    timer* timerPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    int screenX,screenY;

    vector<char> field;
    vector<int> bombs;
    map<int,char> flags;
    int fieldSize;
    int currentPos;

    vector<int> dpBombs;

    void dig(int currentBlock,int currentDepth){

        if(currentDepth <= 0){
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

        currentDepth --;

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

        dig(currentBlock - screenX,currentDepth);
        dig(currentBlock + screenX,currentDepth);

        if((currentBlock+1)%screenX != 0){//Edge cases
            dig(currentBlock + 1,currentDepth);
        }
        if(currentBlock%screenX != 0){
            dig(currentBlock - 1,currentDepth);
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
                dig(currentPos,depth);
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
        int maxRange = screenX*screenY - 1;

        // Initialize a random number generator
        random_device rd;
        mt19937 gen(static_cast<unsigned int>(time(nullptr)));
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

    void saveFile(){

        timerPointer -> saveTime();

        char charFileNumber = '0' + currentFile + 1;
        string fileName = "SaveFile_";
        fileName += charFileNumber;
        fileName += ".dat";
        ofstream fileToSave(fileName) ;

        //First Line Difficulty
        //Second Line Field
        //Third Line the Bomb Positions with # in between each number
        //Forth Line Flags Left
        //Fifth dpBombs

        fileToSave << difficulty << '\n';
        for(int i=0;i<fieldSize;i++){
            fileToSave << field.at(i);
        }
        fileToSave<<'\n';
        for(int i=0;i<numberOfBombs;i++){
            fileToSave << bombs[i] <<'#';
        }
        fileToSave <<'\n';

        fileToSave << flagsLeft;
        fileToSave <<'\n';

        for(int i=0;i<fieldSize;i++){
            fileToSave << dpBombs.at(i) << '#';
        }

        return;
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
        if(VirtualKey == VK_ESCAPE){
            saveFile();
            currentState = MENU;
            currentMenuState = MAIN;
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

    void initialise(int setScreenX,int setScreenY,int bombNumber, int setDepth){

        screenX = setScreenX;
        screenY = setScreenY;
        numberOfBombs = bombNumber;

        depth = setDepth;

        fieldSize = screenX*screenY;
        field.resize(screenX * screenY, char_not_dig);
        bombs.reserve(numberOfBombs);
        flagsLeft = numberOfBombs;
        setBombs();

        dpBombs.resize(screenX * screenY,-1);

        currentPos = 0;

        print();
    }

    void initialiseCreate(){
        switch (difficulty){
         case 0:
            screenX = EasyScreenX;
            screenY = EasyScreenY;
            numberOfBombs = EasyBombs;
            depth = EasyDepth;
            break;
         case 1:
            screenX = MediumScreenX;
            screenY = MediumScreenY;
            numberOfBombs = MediumBombs;
            depth = MediumDepth;
            break;
         case 2:
            screenX = HardScreenX;
            screenY = HardScreenY;
            numberOfBombs = HardBombs;
            depth = HardDepth;
            break;
        }

        fieldSize = screenX*screenY;
        field.resize(screenX * screenY);
        bombs.resize(numberOfBombs);
        dpBombs.resize(screenX * screenY);

    }
public:
    controller(timer* setTimerPointer, CONSOLE_SCREEN_BUFFER_INFO setOldcsbi,COORD setCoord){
        oldcsbi = setOldcsbi;
        coord = setCoord;

        timerPointer = setTimerPointer;
    }

    void create(int diff){
        difficulty = diff;
        switch(difficulty){
         case 0:
            initialise(EasyScreenX,EasyScreenY,EasyBombs,EasyDepth);
            break;
         case 1:
            initialise(MediumScreenX,MediumScreenY,MediumBombs,MediumDepth);
            break;
         case 2:
            initialise(HardScreenX,HardScreenY,HardBombs,HardDepth);
            break;
        }
    }

    int getDifficulty(){
        return difficulty;
    }

    void loadFile(){
        char charFileNumber = '0' + currentFile + 1;
        string fileName = "SaveFile_";
        fileName += charFileNumber;
        fileName += ".dat";
        ifstream fileToRead(fileName) ;

        ofstream temp("temp.txt");

        if (fileToRead.fail()) {
            cerr << "unable to open file for reading" << endl;
            exit(2);
        }

        string line;
        int lineNumber = 0;
        int currentBomb = 0 , currentDpBomb = 0;
        while(getline(fileToRead,line)){
            if(lineNumber == 0){
                int c_int = (int)line[0] - (int)'0';
                difficulty = c_int;
                initialiseCreate();
                temp << c_int;
            }
            else if(lineNumber == 1){
                for(int i=0;i<line.size();i++){
                    if(i < fieldSize){
                        field.at(i) = line[i];
                        temp<<field.at(i);
                    }
                }
            }
            else if(lineNumber == 2){
                string NumStr = "";
                for(int i=0;i<line.size();i++){
                    if(line[i] >= '0' && line[0] <= '9'){
                        NumStr += line[i];
                    }
                    if(line[i] == '#'){
                        bombs[currentBomb] = stringToInt(NumStr);
                        temp<<bombs[currentBomb]<<'#';
                        currentBomb++;
                        NumStr = "";
                    }
                }
            }
            else if(lineNumber == 3){
                flagsLeft = stringToInt(line);
                temp<<flagsLeft;
            }
            else if(lineNumber == 4){
                string NumDpStr = "";
                bool negative = false;
                for(int i=0;i<line.size()-1;i++){
                    if(line[i] >= '0' && line[0] <= '9'){
                        NumDpStr += line[i];
                    }
                    if(line[i] == '-'){
                        negative = true;
                    }
                    if(line[i] == '#'){
                        dpBombs[currentDpBomb] = stringToInt(NumDpStr,negative);
                        temp<<dpBombs[currentDpBomb] <<'#';

                        currentDpBomb++;
                        NumDpStr = "";
                        negative = false;
                    }
                }
            }
            lineNumber++;
            temp<<'\n';
        }

        temp.close();
        fileToRead.close();
    }

    void mainControl(KEY_EVENT_RECORD event){
        handleKeyInput(event);
        win();
        print();
    }

    void print(){

        system("cls");

        //get Console Size
        HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

        oldcsbi = csbi;

        short horizontalOffset = (short)(csbi.srWindow.Right / 2) -   screenX;
        short verticalOffset = (short)((csbi.srWindow.Top ) / 2) + screenY;

        coord.X = horizontalOffset + screenX;
        coord.Y = (short)(csbi.srWindow.Top );
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
            if(i == 1){
                coord.Y += 2;
                SetConsoleCursorPosition(hConsoleOutput, coord);
            }
        }
        coord.Y += 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        cout<<"Press ESC to save and exit";
    }

};

class playMenu;

class statisticsMenu;

class mainMenu{
private:
    int currentPosition;

    playMenu* playMenuPointer;
    statisticsMenu* statisticsMenuPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    const int numberOfOptions = 3;
    string Options[3] = {"Play","Statistics","Exit"};


    void action();

    void handleKeyInput(KEY_EVENT_RECORD event);

public:
    mainMenu(playMenu* setPlayMenu,statisticsMenu* setStatisticsMenuPointer,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord);

    void mainControl(KEY_EVENT_RECORD event);

    void print();
};

class newGameMenu{
private:
    int currentPositionX,currentPositionY;
    int chosenFile,chosenDifficulty;
    const int numberOfDifficultyOptions = 3;
    string DifficultyOptions[3] = {"Easy","Medium","Hard"};

    const int numberOfFiles = 3;

    mainMenu* mainMenuPointer;
    controller* CtrlPointer;
    playMenu* playMenuPointer;
    timer* TimerPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void action(){
        if(currentPositionY == 0){
            chosenFile = currentPositionX;
            return;
        }
        if(currentPositionY == 1){
            chosenDifficulty = currentPositionX;
            return;
        }
        if(currentPositionY == 2){
            //Save Game
            if(chosenDifficulty>=0 && chosenDifficulty <=3){
                CtrlPointer -> create(chosenDifficulty);
                TimerPointer -> start();

                currentState = PLAY;
            }
            currentFile = chosenFile;
            return;
        }
        if(currentPositionY == 3){
            currentMenuState = MAIN;
            mainMenuPointer->print();
        }
        return;
    }

    void handleKeyInput(KEY_EVENT_RECORD event){
        WORD VirtualKey = event.wVirtualKeyCode;
        char c = event.uChar.AsciiChar;

        if(VirtualKey == VK_RETURN){
            action();
            return;
        }
        if(VirtualKey == VK_RIGHT || c == 'd'){
            if(currentPositionY == 0){
                if(currentPositionX < numberOfFiles - 1){
                    currentPositionX ++ ;
                }
            } else if(currentPositionY == 1){
                if(currentPositionX < numberOfDifficultyOptions - 1){
                    currentPositionX ++ ;
                }
            }

            return;
        }
        if(VirtualKey == VK_LEFT || c == 'a'){
            if(currentPositionX > 0)
                currentPositionX -- ;
        }
        if(VirtualKey == VK_DOWN || c == 's'){
            if(currentPositionY < 3)
                currentPositionY ++ ;
        }
        if(VirtualKey == VK_UP || c == 'w'){
            if(currentPositionY > 0)
                currentPositionY--;
        }
    }
public:
    newGameMenu(mainMenu* setMainMenuPtr,controller* setCtrlPointer,timer* setTimerPointer,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPositionX = currentPositionY = 0;
        setOldcsbi = setOldcsbi;
        coord = setCoord;

        mainMenuPointer = setMainMenuPtr;
        CtrlPointer = setCtrlPointer;
        TimerPointer = setTimerPointer;
    }

    void mainControl(KEY_EVENT_RECORD event){
        handleKeyInput(event);
        if(currentMenuState == NEW_GAME && currentState != PLAY){
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

        string ChooseFile = "Choose File:";

        cout<<ChooseFile;
        coord.X+= ChooseFile.length() + 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        for(int i=0;i<numberOfFiles;i++){
            if(chosenFile == i){
                setTextColor("purple");
            }
            if(currentPositionX == i && currentPositionY == 0){
                setTextColor("red");
            }

            cout<<"File "<<i+1;

            setTextColor("normal");

            coord.X+= 8;
            SetConsoleCursorPosition(hConsoleOutput, coord);
        }
        coord.X-= 8 * numberOfFiles + ChooseFile.length() + 2;
        coord.Y += 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        string Diff = "Difficulty:";
        cout<<Diff;

        coord.X += Diff.length() + 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        int DiffixultyOffsetX = Diff.length() + 2;
        for(int i=0;i<numberOfDifficultyOptions;i++){
            if(chosenDifficulty == i){
                setTextColor("purple");
            }
            if(currentPositionX == i && currentPositionY == 1){
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
        if(currentPositionY == 2){
            setTextColor("red");
        }
        cout<<"Save";

        coord.Y+= 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);
        setTextColor("normal");

        if(currentPositionY == 3){
            setTextColor("red");
        }
        cout<<"Back";
        setTextColor("normal");
    }
};

class loadGameMenu{
private:
    int currentPosition;
    const int numberOfOptions = 3;
    string options[3] = {"File 1" , "File 2", "File 3"};

    controller* CtrlPointer;
    mainMenu* mainMenuPointer;
    timer* timerPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void loadFile(int fileNumber){
        currentFile = fileNumber;
        CtrlPointer -> loadFile();
        timerPointer -> loadTime();
        timerPointer -> start();
        currentState = PLAY;
        CtrlPointer ->print();
        return;
    }

    void action(){
        if(currentPosition >= 0 && currentPosition < numberOfOptions ){
            loadFile(currentPosition);
            return;
        }
        if(currentPosition == numberOfOptions){
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
        if(VirtualKey == VK_DOWN | c =='s'){
              if(currentPosition < numberOfOptions)
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
public:
    loadGameMenu(controller* setCtrlPointer,timer* setTimerPointer,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPosition = 0;

        oldcsbi = setOldcsbi;
        coord = setCoord;

        CtrlPointer = setCtrlPointer;
        timerPointer = setTimerPointer;

        return;
    }

    void mainControl(KEY_EVENT_RECORD event){
        handleKeyInput(event);
        if(currentMenuState == LOAD && currentState != PLAY && currentMenuState != MAIN){
            print();
        }
    }

    void setMainMenuPointer(mainMenu* setPointer){
        mainMenuPointer = setPointer;
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

        cout<<"Choose File:";

        coord.Y+= 2;
        SetConsoleCursorPosition(hConsoleOutput, coord);

        for(int i=0;i<numberOfOptions;i++){
            if(currentPosition == i){
                setTextColor("red");
            }

            cout<<options[i];

            setTextColor("normal");

            coord.Y+= 2;
            SetConsoleCursorPosition(hConsoleOutput, coord);
        }

        if(currentPosition == numberOfOptions ){
            setTextColor("red");
        }
        cout<<"Back";
        setTextColor("normal");

    }
};

class playMenu{
private:
    int currentPosition;
    const int numberOfPlayOptions = 3;
    string PlayOptions[3] = {"New Game","Load Game","Back to Main Menu"};

    mainMenu* mainMenuPointer;
    newGameMenu* newGameMenuPointer;
    loadGameMenu* loadGameMenuPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void action(){
        if(currentPosition == 0){ //New
            currentMenuState = NEW_GAME;
            newGameMenuPointer -> print();
            return;
        } else if(currentPosition == 1){ // Load
            currentMenuState = LOAD;
            loadGameMenuPointer -> print();

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
    playMenu(loadGameMenu* setLoadGameMenuPointer,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPosition = 0;
        setOldcsbi = setOldcsbi;
        coord = setCoord;

        loadGameMenuPointer = setLoadGameMenuPointer;
    }

    void setNewGameMenuPointer(newGameMenu* setPointer){
        newGameMenuPointer = setPointer;
    }

    void setMainMenuPointer(mainMenu* setPointer){
        mainMenuPointer = setPointer;
    }

    void mainControl(KEY_EVENT_RECORD event){
        handleKeyInput(event);
        if(currentMenuState == MENU_PLAY && currentMenuState != LOAD){
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

class statisticsMenu{
private:
    int currentPosition;
    const int numberOfOptions = 5;
    string statsOptions[5] = {"Best Time","Easy: ","Medium: ","Hard: " ,"Back to Main Menu"};

    int bestEasyTime = INT_MAX,bestMediumTime = INT_MAX,bestHardTime = INT_MAX;

    mainMenu* mainMenuPointer;

    CONSOLE_SCREEN_BUFFER_INFO oldcsbi;
    COORD coord;

    void action(){
        currentMenuState = MAIN;
        mainMenuPointer -> print();
        return;
    }

    void handleKeyInput(KEY_EVENT_RECORD event){
        WORD VirtualKey = event.wVirtualKeyCode;

        if(VirtualKey == VK_RETURN){
            action();
            return;
        }
    }

    void loadScore(){
        ifstream scores("scores.dat");

        string line;

        int i=0;
        int stats[3];
        while(getline(scores,line)){
            if(i <3){
                stats[i] = stringToInt(line);
            }
            i++;
        }
        bestEasyTime = stats[0];
        bestMediumTime = stats[1];
        bestHardTime = stats[2];

        scores.close();
    }
    void saveScore(){
        ofstream scores("scores.dat");

        scores << bestEasyTime <<'\n' << bestMediumTime <<'\n' << bestHardTime;

        scores.close();
    }
public:
    statisticsMenu(CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
        currentPosition = 0;
        setOldcsbi = setOldcsbi;
        coord = setCoord;

        temp();
    }

    void temp(){
        loadScore();
    }

    void CompareTime(int newTime, int diff){
        switch(diff){
         case 0://Easy
             if(bestEasyTime > newTime){
                bestEasyTime = newTime;
             }
             break;
         case 1://Medium
             if(bestMediumTime > newTime){
                bestMediumTime = newTime;
             }
            break;
         case 2://Hard
             if(bestHardTime > newTime){
                bestHardTime = newTime;
             }
             break;

        }
        saveScore();
    }

    void setMainMenuPointer(mainMenu* setPointer){
        mainMenuPointer = setPointer;
    }

    void mainControl(KEY_EVENT_RECORD event){
        handleKeyInput(event);
        if(currentMenuState == STATISTICS && currentMenuState != MAIN){
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

        for(int i=0;i<numberOfOptions;i++){
            if(i == numberOfOptions-1){
                setTextColor("red");
            }
            cout<<statsOptions[i];

            if(i == 1){
                if(bestEasyTime != INT_MAX)
                    cout<<bestEasyTime<<"s";
                else
                    cout<<"Not Set Yet";
            } else if(i == 2){
                if(bestMediumTime != INT_MAX)
                    cout<<bestMediumTime<<"s";
                else
                    cout<<"Not Set Yet";
            } else if(i == 3){
                if(bestHardTime != INT_MAX)
                    cout<<bestHardTime<<"s";
                else
                    cout<<"Not Set Yet";
            }

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
        statisticsMenuPointer -> print();
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

mainMenu::mainMenu(playMenu* setPlayMenu,statisticsMenu* setStatisticsMenuPointer,CONSOLE_SCREEN_BUFFER_INFO setOldcsbi, COORD setCoord){
    currentPosition = 0;

    playMenuPointer = setPlayMenu;
    statisticsMenuPointer = setStatisticsMenuPointer;

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
        cout<<"You Lost :( " ;
    }

    coord.Y += 2;
    SetConsoleCursorPosition(hConsoleOutput,coord);

    cout<<"You played for "<<TimePlayed<<" seconds";

    coord.Y += 20;
    SetConsoleCursorPosition(hConsoleOutput,coord);

}

int main(){
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

    timer Timer(oldcsbi,coord);
    controller Ctrl(&Timer,oldcsbi,coord);

    statisticsMenu StatisticsMenu(oldcsbi,coord);
    loadGameMenu LoadGameMenu(&Ctrl,&Timer,oldcsbi,coord);
    playMenu PlayMenu(&LoadGameMenu,oldcsbi,coord);
    mainMenu MainMenu(&PlayMenu,&StatisticsMenu,oldcsbi,coord);
    newGameMenu NewGameMenu(&MainMenu,&Ctrl,&Timer,oldcsbi,coord);


    StatisticsMenu.setMainMenuPointer(&MainMenu);

    LoadGameMenu.setMainMenuPointer(&MainMenu);

    PlayMenu.setNewGameMenuPointer(&NewGameMenu);
    PlayMenu.setMainMenuPointer(&MainMenu);

    INPUT_RECORD inputRecord;
    DWORD eventsRead;

    MainMenu.print();
    while (currentState == PLAY || currentState == MENU || currentState == WIN || currentState == LOST) {
        if(currentState == PLAY){
            Timer.print();
        }
        if (PeekConsoleInput(hStdin,&inputRecord, 1, &eventsRead) && eventsRead > 0 )  {
            ReadConsoleInput(hStdin, &inputRecord, 1, &eventsRead);
            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
                KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;

                if(currentState == MENU){
                    if(currentMenuState == MAIN){
                        MainMenu.mainControl(keyEvent);
                    } else if(currentMenuState == MENU_PLAY){
                        PlayMenu.mainControl(keyEvent);
                    } else if(currentMenuState == STATISTICS){
                        StatisticsMenu.mainControl(keyEvent);
                    } else if(currentMenuState == LOAD){
                        LoadGameMenu.mainControl(keyEvent);
                    } else if(currentMenuState == NEW_GAME){
                        NewGameMenu.mainControl(keyEvent);
                    }


                } else if(currentState == PLAY){
                    if(!Timer.getRunning()){
                        Timer.start();
                    }

                    Ctrl.mainControl(keyEvent);
                }
            }
            if(currentState == WIN || currentState ==LOST){
                Timer.stop();
                int TimePlayed = Timer.CalculateTime();
                if(currentState == LOST){
                    printEnd(false,TimePlayed,oldcsbi,coord);
                    //Ctrl -> deleteSave();
                } else if(currentState == WIN){
                    StatisticsMenu.CompareTime(TimePlayed,Ctrl.getDifficulty());
                    printEnd(true,TimePlayed,oldcsbi,coord);
                }
                if(inputRecord.Event.KeyEvent.bKeyDown){
                    currentState = MENU;
                    currentMenuState = MAIN;
                }
            }
        }
    }


    SetConsoleMode(hStdin,originalMode);

    return 0;
}
