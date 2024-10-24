#include <iostream>
#include <string>
#include <map>
#include<vector>
#include<cstdlib>
#include <ctime>
#include <stdlib.h>
#include<conio.h>

using namespace std;

string cardSuitesList[4] ={"Club","Diamond","Heart","Spades"};
const int deckSize = 52;

struct Card{
    int id;
    char number;
    string suite;
};
Card startingDeck[deckSize];

class player{
private:
    vector<Card> hand;
    int id,balance,bet;

    int aceDrawn(){//Fix in the future
        return 11;
    }

    int handNumberToInt(int position){
        int num;
        char handNumber = hand.at(position).number;

        if(handNumber == 'A') num = aceDrawn();
        else if(handNumber == 'J' || handNumber == 'Q' ||handNumber == 'K' ||handNumber == 'X') num=10;
        else num = (int)(handNumber - 48);

        return num;
    }

public:
    player(int setId,int setBalance){
        id = setId;
        balance = setBalance;
        bet = 0;
    }
    void drawCard(vector<Card>& deck){
        hand.push_back(deck.back());
        deck.pop_back();
    };

    int getId(){
        return id;
    }
    int getBalance(){
        return balance;
    }
    int getBet(){
        return bet;
    }
    void setBalance(int newBalance){
        balance = newBalance;
    }
    void setBet(int newBet){

        bet = newBet;
        balance -=bet;
    }
    void win(){
        balance += 2*bet;
    }

    int sumHand(){
        int sum=0;
        for(int i=0;i<hand.size();i++){
            sum += handNumberToInt(i);
        }
        if(sum>21){
            return -1;
        }
        return sum;
    }
    void clearHand(){
        hand.clear();
    }
    void showHand(){
        for(int i=0;i<hand.size();i++)
            cout<<hand[i].number<<' '<<hand[i].suite<<' ';
        cout<<endl;
    }
};

void initialiseDeck(){
    int cardId=0;
    for(int j=0;j<4;j++){
        for(int i=0;i<13;i++){
            cardId = i+j*13;

            startingDeck[cardId].suite = cardSuitesList[j];
            startingDeck[cardId].id = cardId;

            if(i<9 && i>1) startingDeck[cardId].number = char(i+48+1);
            else if (i==1) startingDeck[cardId].number = 'X';
            else if(i==9) startingDeck[cardId].number='J';
            else if(i==10) startingDeck[cardId].number='Q';
            else if(i==11) startingDeck[cardId].number='K';
            else startingDeck[cardId].number='A';
        }
    }
}

void shuffleDeck(vector<Card>& deck){

	srand(time(0));

	map<int,Card> newDeck;

    for(int i=0;i<deckSize;i++){
        int random = rand() % deckSize;
        while(newDeck.find(random) != newDeck.end()){
            if(++random >= deckSize) random = 0;
        }
        newDeck[random] = startingDeck[i];
    }
    for(int i=0;i<deckSize;i++){
        deck.push_back(newDeck[i]);
    }
}

void showDeck(vector<Card> deck){
    for(int i=0;i<deck.size();i++){
        cout<<deck.at(i).number<<' '<<deck.at(i).suite<<endl;
    }
}

void PrintUI(vector<player> playerList,int numberOfPlayers){
    system("cls");
    cout<<"Dealer: ";
    playerList[0].showHand();
    cout<<"Sum of dealers hand = "<<playerList[0].sumHand()<<endl;

    for(int i=1;i<numberOfPlayers;i++){
        cout<<endl<<"Player "<<i<<": ";
        playerList[i].showHand();
        cout<<"Sum of player's " << i;
        if(playerList[i].sumHand() == -1) cout<<" You are out!"<<endl;
        else cout<<" hand = "<<playerList[i].sumHand();
        if(playerList[i].sumHand() == 21) cout<<" Black Jack!"<<endl;
        else cout<<endl;
        cout<<"Bet: "<<playerList[i].getBet()<<" Balance: "<<playerList[i].getBalance()<<endl;
    }
    cout<<endl;
}

void ChooseBets(vector<player>& playerList,int numberOfplayers){
    int bet;
    int dealersBalance = playerList.at(0).getBalance();
    for(int i=1;i<numberOfplayers;i++){
        int currentPlayerBalance = playerList.at(i).getBalance();
        cout<<"Player's "<<i<<" choice..."<<endl;
        cout<<"How much do you want to bet? Current balance is "<<currentPlayerBalance<<" $"<<endl;
        cin>>bet;
        if(bet > currentPlayerBalance){
            cout<<"You don't have that much money..."<<endl;
            cout<<"Do you want to go below 0?"<<endl;
            cout<<"...So, how much do you bet?"<<endl;
            cin>>bet;
        }
        if(bet > currentPlayerBalance) cout<<"Good luck soldier"<<endl;
        if(bet == currentPlayerBalance && currentPlayerBalance>=100) cout<<"ALL IN? You must feel really lucky!"<<endl;
        playerList.at(i).setBet(bet);

        dealersBalance += bet;
    }
    playerList.at(0).setBalance(dealersBalance);
    getch();
}


void initaliseGame(vector<player>& playerList,int numberOfplayers, vector<Card>& deck){
    shuffleDeck(deck);
    for(int i=0;i<numberOfplayers;i++){
            playerList[i].clearHand();
        }
    for(int j=0;j<2;j++){
        for(int i=0;i<numberOfplayers;i++){
            playerList[i].drawCard(deck);
        }
    }
    ChooseBets(playerList,numberOfplayers);
    PrintUI(playerList,numberOfplayers);
}

vector<int> maxPositionInArray(int arr[],int arrSize){
    int Max = arr[0];
    vector<int> MaxPos;
    for(int i=0;i<arrSize;i++){
        if(Max<=arr[i]){
            Max=arr[i];
        }
    }
    for(int i=0;i<arrSize;i++){
        if(Max == arr[i])
         MaxPos.push_back(i);
    }
    return MaxPos;
}

bool playerDecisionDraw(player p){
    char desition;

    if(p.sumHand() == -1 || p.sumHand() == 21) return false;

    cout<<endl;
    cout<<"Player "<<p.getId()<<" is playing..."<<endl;
    cout<<"Do you want to hit?"<<endl;
    cout<<"Y/N ... ";
    cin>>desition;
    cout<<endl;
    if(desition=='Y' || desition=='y') return true;
    return false;
}

void nextPlayer(player& p,vector<Card>& deck,vector<player> playerList,int numberOfPlayers){
    bool play = playerDecisionDraw(p);
    while(play){
        p.drawCard(deck);
        PrintUI(playerList,numberOfPlayers);
        play = playerDecisionDraw(p);
    }
}

void endGame(vector<player>& playerList,int numberOfPlayers){
    int sumPlayersHands[numberOfPlayers];
    for(int i=0;i<numberOfPlayers;i++){
        sumPlayersHands[i] = playerList[i].sumHand();
    }
    vector<int> maxPositions = maxPositionInArray(sumPlayersHands,numberOfPlayers);

    if(maxPositions.size() == 1){
        if(maxPositions[0] == 0) cout<<"The Dealer Wins!";
        else{
            cout<<"Player "<<maxPositions.at(0)<<" Wins!";
            playerList[maxPositions.at(0)].win();

            int dealersBalance = playerList[0].getBalance();
            dealersBalance -= playerList[maxPositions.at(0)].getBet();
            playerList[0].setBalance(dealersBalance);
        }
    }else{
        int dealersBalance = playerList[0].getBalance();
        for(int i=0;i<maxPositions.size();i++){
            if(maxPositions[1] == 0) cout<<"dealer,";
            else{
                cout<<" player "<<maxPositions.at(i)<<", ";

                playerList[maxPositions.at(i)].win();

                dealersBalance -= playerList[maxPositions.at(i)].getBet();
                playerList[0].setBalance(dealersBalance);
            }
        }
        cout<<" Win!"<<endl;
    }
    cout<<endl<<"Dealer's profit = "<<playerList[0].getBalance()<<endl;
    cout<<"New Balances: "<<endl;
    for(int i=1; i<numberOfPlayers;i++){
        cout<<"Player "<<i<<"'s Balance = "<<playerList[i].getBalance()<<endl;
    }
}

void playGame(vector<player>& playerList,int numberOfPlayers,vector<Card>& deck){

    initaliseGame(playerList,numberOfPlayers,deck);

    for(int i=1;i<numberOfPlayers;i++){
        nextPlayer(playerList[i],deck,playerList,numberOfPlayers);
        PrintUI(playerList,numberOfPlayers);
    }
    //Dealer's Turn
    while(playerList[0].sumHand()<=16 && playerList[0].sumHand()>0){
        playerList[0].drawCard(deck);
    }
    PrintUI(playerList,numberOfPlayers);

    endGame(playerList,numberOfPlayers);
}
bool noMoreMoney(vector<player> playerList,int numberOfPlayers){
    int numberofBrokePlayers = 0;
    for(int i=1;i<numberOfPlayers;i++){
        if(playerList[i].getBalance()<= 0) numberofBrokePlayers++;
    }
    if(numberofBrokePlayers >= numberOfPlayers-2) return true;
    return false;
}
void setUpPlayerList(vector<player>& playerList,int numberOfPlayers){
    cout<<endl<<"Input player's data"<<endl;
    player Dealer(0,0);
    playerList.push_back(Dealer);
    for(int i=1;i<numberOfPlayers;i++){
        int newBalance;
        cout<<"Enter the Balance of player "<<i<<" : ";
        cin>>newBalance;
        player newPlayer(i,newBalance);
        playerList.push_back(newPlayer);
    }
}
int main(){
    vector<Card> deck;

    initialiseDeck();

    int numberOfPlayers;

    cout<<"How many players will be in the game? "<<endl<<"Input number of players: ";
    cin>>numberOfPlayers;
    numberOfPlayers++;//+1 is the dealer

    vector<player> playerList;
    setUpPlayerList(playerList,numberOfPlayers);

    bool playAgain = true;
    while(playAgain){
        playGame(playerList,numberOfPlayers,deck);
        if(noMoreMoney(playerList,numberOfPlayers)){
            cout<<"Sorry you cant play if you don't have any money..."<<endl;
            getch();
            playAgain = false;
        } else {
            cout<<"Do you want to play again?"<<endl;
            cout<<"Press Y if you want...";
            char answer;
            cin>>answer;
            if(answer =='y' || answer=='Y'){
                playAgain = true;
            } else playAgain = false;
            system("cls");
        }
    }
}
