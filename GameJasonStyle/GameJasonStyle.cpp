#include<iostream>
#include<cstdlib>
#include<stdlib.h>
#include<cmath>

using namespace std;

const int NRow=10;
const int NCol=20;
const char PlayerSprite='$';
const char TresureSprite='*';
const char EnemySprite='@';


bool ERROR=false;
bool END=false;
bool WinFlag=false;

int Tr0,Tc0;

char Map[NRow][NCol];

const int NumberOfEnemies=10;
int EnemyRange=5;
int EnemyPosition[NumberOfEnemies][2];

void InitialiaseCharArray(char Array[][NCol]){
    for(int i=0;i<NRow;i++)
        for(int j=0;j<NCol;j++) Array[i][j]='.';
}
void AddWalls(char Array[NRow][NCol]){
    for(int i=0;i<NCol;i++){
        Array[0][i]='-';
        Array[NRow-1][i]='-';
    }
    for(int i=0;i<NRow;i++){
        Array[i][0]='|';
        Array[i][NCol-1]='|';
    }
    Array[0][NCol-1]='|';
}
void AddPlayer(int Pr0,int Pc0){
    if(Pr0<0 || Pr0>NRow || Pc0<0 || Pc0>NCol) ERROR=true;
    else Map[Pr0][Pc0]=PlayerSprite;
}
void RandomSpawn(int &Spawnr0,int &Spawnc0){
    bool Stop=false;
    while(!Stop){
        Spawnr0=rand() % 10;//Generate Random Number between 0-10
        Spawnc0=2*rand() % 10;//Generate Random Number between 0-20
        for(int i=0;i<NRow;i++)
            for(int j=0;j<NCol;j++)
                if(Map[i][j]=='.' && i==Spawnr0 && j==Spawnc0) Stop=true;
    }
    return;
}
void AddTresure(){
    RandomSpawn(Tr0,Tc0);
    Map[Tr0][Tc0]=TresureSprite;
}
void FindCharPosition(int &Pr,int &Pc,char CharToFind){
    for(int i=0;i<NRow;i++)
        for(int j=0;j<NCol;j++)
            if(Map[i][j]==CharToFind){
                Pr=i;
                Pc=j;
                return;
            }
}
void UpdateMoveEnemy(int EnemyFlag,int Enemyr,int Enemyc,int Direction){
    if(Direction==0 && Map[Enemyr+1][Enemyc]!=TresureSprite){
        Map[Enemyr+1][Enemyc]=EnemySprite;
        Map[Enemyr][Enemyc]='.';
        EnemyPosition[EnemyFlag][0]=Enemyr+1;
        return;
    } else if(Direction==1 && Map[Enemyr-1][Enemyc]!=TresureSprite){
        Map[Enemyr-1][Enemyc]=EnemySprite;
        Map[Enemyr][Enemyc]='.';
        EnemyPosition[EnemyFlag][0]=Enemyr-1;
        return;
    }else if(Direction==2 && Map[Enemyr][Enemyc-1]!=TresureSprite){
        Map[Enemyr][Enemyc-1]=EnemySprite;
        Map[Enemyr][Enemyc]='.';
        EnemyPosition[EnemyFlag][1]=Enemyc-1;
        return;
    }else if(Direction==3 && Map[Enemyr][Enemyc+1]!=TresureSprite){
        Map[Enemyr][Enemyc+1]=EnemySprite;
        Map[Enemyr][Enemyc]='.';
        EnemyPosition[EnemyFlag][1]=Enemyc+1;
        return;
    }
}
void MoveEnemy(int EnemyFlag){
    int Enemyr=EnemyPosition[EnemyFlag][0];
    int Enemyc=EnemyPosition[EnemyFlag][1];

    int Pr,Pc;
    int RandomDiagonalDirection=trunc(rand() % 2);
    FindCharPosition(Pr,Pc,PlayerSprite);
    if(Pr>Enemyr && Pr-Enemyr<=EnemyRange && Pc==Enemyc){
        UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,0);
        return;
    }else if(Pr<Enemyr && Enemyr-Pr<=EnemyRange && Pc==Enemyc){
        UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,1);
        return;
    }else if(Pc<Enemyc && Enemyc-Pc<=EnemyRange && Pr==Enemyr){
        UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,2);
        return;
    }else if(Pc>Enemyc && Pc-Enemyc<=EnemyRange && Pr==Enemyr){
        UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,3);
        return;
    }else if(Pr>Enemyr && Pr-Enemyr<=EnemyRange && Pc<Enemyc && Enemyc-Pc<=EnemyRange){
        if(RandomDiagonalDirection==0) UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,0);
        else UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,2);
        return;
    }else if(Pr>Enemyr && Pr-Enemyr<=EnemyRange && Pc>Enemyc && Pc-Enemyc<=EnemyRange){
        if(RandomDiagonalDirection==0) UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,0);
        else UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,3);
        return;
    }else if(Pr<Enemyr && Enemyr-Pr<=EnemyRange && Pc>Enemyc && Pc-Enemyc<=EnemyRange){
        if(RandomDiagonalDirection==0) UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,1);
        else UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,3);
        return;
    }else if(Pr<Enemyr && Enemyr-Pr<=EnemyRange && Pc<Enemyc && Enemyc-Pc<=EnemyRange){
        if(RandomDiagonalDirection==0) UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,1);
        else UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,2);
        return;
    }


    while(true){
        int RandomDirection=trunc(rand() % 4);//Generate Random Number between 0-3
        //Check if New Position is Valid
        if(RandomDirection==0){ //Down
            if(Map[Enemyr+1][Enemyc]== '.' || Map[Enemyr+1][Enemyc]==PlayerSprite){
                    UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,0);
                    return;
            }
        }else if(RandomDirection==1){ //Up
            if(Map[Enemyr-1][Enemyc]=='.'|| Map[Enemyr-1][Enemyc]==PlayerSprite){
                    UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,1);
                    return;
            }
        }else if(RandomDirection==2){ //Left
            if(Map[Enemyr][Enemyc-1]=='.'|| Map[Enemyr][Enemyc-1]==PlayerSprite){
                    UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,2);
                    return;
            }
        }else if(RandomDirection==3) //Right
            if(Map[Enemyr][Enemyc+1]=='.' || Map[Enemyr][Enemyc+1]==PlayerSprite){
                    UpdateMoveEnemy(EnemyFlag,Enemyr,Enemyc,3);
                    return;
            }
        return;
    }

}
void AddEnemy(int EnemyFlag){
    int Er0,Ec0;
    RandomSpawn(Er0,Ec0);
    EnemyPosition[EnemyFlag][0]=Er0;
    EnemyPosition[EnemyFlag][1]=Ec0;
    Map[Er0][Ec0]=EnemySprite;

}
void InitialiseMap(){
    InitialiaseCharArray(Map);

    AddWalls(Map);

    int Pr0=NRow/2,Pc0=NCol/2;
    AddPlayer(Pr0,Pc0);

    AddTresure();
    for(int i=0;i<NumberOfEnemies;i++)
        AddEnemy(i);
}

bool PlayerDied(int NewPr,int NewPc){
    if(Map[NewPr][NewPc]=='-' || Map[NewPr][NewPc]=='|' || Map[NewPr][NewPc]==EnemySprite){ //Player Collides with wall or Enemy
        END=true;
        return true;
    }
    return false;
}
void MovePlayer(char Key){
    int Pr=-1,Pc=-1;
    FindCharPosition(Pr,Pc,PlayerSprite);
    if(Pr==-1) END = true;

    if(Key=='W' || Key=='S' || Key=='D' || Key=='A') Map[Pr][Pc]='.';
    if(Key=='W'){
        if(!PlayerDied(Pr-1,Pc)) Map[Pr-1][Pc]=PlayerSprite;
    }
    else if(Key=='S') {
        if(!PlayerDied(Pr+1,Pc)) Map[Pr+1][Pc]=PlayerSprite;
    }
    else if(Key=='D'){
        if(!PlayerDied(Pr,Pc+1)) Map[Pr][Pc+1]=PlayerSprite;
    }
    else if(Key=='A'){
        if(!PlayerDied(Pr,Pc-1)) Map[Pr][Pc-1]=PlayerSprite;
    }
    else if(Key!='E')ERROR=true;
}
void Win(){
    int Pr=-1,Pc=-1,Tr=-1,Tc=-1;
    FindCharPosition(Pr,Pc,PlayerSprite);

    if(Pr==Tr0 && Pc==Tc0){
        END=true;
        WinFlag=true;
    }
}
void Player(char KeyEntered){
    MovePlayer(KeyEntered);
    Win();
}
void PrintMap(){
    for(int i=0;i<NRow;i++){
        for(int j=0;j<NCol;j++) cout<<Map[i][j];
        cout<<endl;
    }
}

int main(){
    char KeyEntered;
    srand(time(0));// Initialize random number generator

    while(true){
        InitialiseMap();
        PrintMap();
        while(!END && !ERROR){
            cin>>KeyEntered;
            for(int i=0;i<NumberOfEnemies;i++)
                MoveEnemy(i);
            Player(KeyEntered);
            system("CLS");
            PrintMap();
        }
        if(ERROR){
            cout<<endl<<"ERROR";
            ERROR=false;
        }
        else if(END){
            if(WinFlag) cout<<endl<<"You Won:)";
            else cout<<endl<<"You Lost :(";
            END=false;
            WinFlag=false;
        }
        KeyEntered=getchar();
        KeyEntered=getchar();
        system("CLS");
    }


    return 0;
}
