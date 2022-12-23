#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_CARDS 108
#define Cards_Init_Number 7

/*
    Si la carte est utiliser son etat egale a 1 sinon egale a 0
    +2 ==>10 !:2 carte pour chaque color
    inversement de sens ==>11 !:2 carte pour chaque color
    passe to tour ==>12 !:2 carte pour chaque color
    Joker ==>13 !:une seul carte pour chaque color
    +4 ==>14 !:une seul carte pour chaque color
*/

typedef struct {
    char color;
    int value;
    int etat;
}Card;

typedef struct {
    char name[100];
    Card *CardsList[NUM_CARDS];
    int CardsNumber;
}Player;

void init_Game_Cards(Card *Game_Cards){
    int Cards_index;
    char *colors[]={'R','Y','B','G','N'};

    Cards_index=0;
    for(int i=0;i<4;i++){
        (Game_Cards+Cards_index)->color=colors[i];
        (Game_Cards+Cards_index)->etat=0;
        Cards_index=Cards_index+1;
        for(int j=0;j<2;j++){
            //+2
            (Game_Cards+Cards_index)->color=colors[i];
            (Game_Cards+Cards_index)->value=10;
            (Game_Cards+Cards_index)->etat=0;
            Cards_index=Cards_index+1;
            //inversement de sens
            (Game_Cards+Cards_index)->color=colors[i];
            (Game_Cards+Cards_index)->value=11;
            (Game_Cards+Cards_index)->etat=0;
            Cards_index=Cards_index+1;
            //passe to tour
            (Game_Cards+Cards_index)->color=colors[i];
            (Game_Cards+Cards_index)->value=12;
            (Game_Cards+Cards_index)->etat=0;
            Cards_index=Cards_index+1;

            for(int k=1;k<=9;k++){
                (Game_Cards+Cards_index)->color=colors[i];
                (Game_Cards+Cards_index)->value=k;
                (Game_Cards+Cards_index)->etat=0;
                Cards_index=Cards_index+1;
            }
        }
        //Joker
        (Game_Cards+Cards_index)->color=colors[4];
        (Game_Cards+Cards_index)->value=13;
        (Game_Cards+Cards_index)->etat=0;
        Cards_index=Cards_index+1;
        //+4
        (Game_Cards+Cards_index)->color=colors[4];
        (Game_Cards+Cards_index)->value=14;
        (Game_Cards+Cards_index)->etat=0;
        Cards_index=Cards_index+1;
    }
}

void print_Game_Cards(Card *Game_Cards){
    for(int i=0;i<108;i++){
        printf("%c %d %d\t",(Game_Cards+i)->color,(Game_Cards+i)->value,(Game_Cards+i)->etat);
        Print_Card((Game_Cards+i));
        printf("\n");
    }
}

int *Get_Card(Card *Game_Cards){
    int index;
    do{
        index = rand()%NUM_CARDS;
    }while((Game_Cards+index)->etat!=0);
    (Game_Cards+index)->etat=1;

    return (Game_Cards+index);
}

void init_Game_Players(Player *Game_Players,Card *Game_Cards,int *PlayersNumber){
    int Player_index,*Card_Pointer;

    Player_index=0;

    //avoir le nombre de jeueur attention le nombre comporte entre 1 et 4
    do{
        printf("Donner le nombre des jeueur : ");
        scanf("%d",PlayersNumber);
    }while(*PlayersNumber>4 || *PlayersNumber<1);

    //boucle pour remplir les donner de liste des jeueur le nom les carte des jeueur le nombre des carte pour chaque jeueur
    for(int i=0;i<*PlayersNumber;i++){
        //init name
        printf("Donner le nom du jeur num %d : ",i+1);
        scanf("%s",(Game_Players+Player_index)->name);

        //cards list
        for(int j=0;j<=Cards_Init_Number;j++){
            Card_Pointer=Get_Card(Game_Cards);
            (Game_Players+Player_index)->CardsList[j]=Card_Pointer;
        }

        //cards Number
        (Game_Players+Player_index)->CardsNumber=Cards_Init_Number;

        //incerment Player index
        Player_index=Player_index+1;
    }
}

void print_Game_Players(Player *Game_Players,int PlayersNumber){
    Card *Cards_Number_Pointer;
    for(int i=0;i<PlayersNumber;i++){
        printf("%s\t %d\n",(Game_Players+i)->name,(Game_Players+i)->CardsNumber);
        for(int j=0;j<(Game_Players+i)->CardsNumber;j++){
            Cards_Number_Pointer = (Game_Players+i)->CardsList[j];
            Print_Card(Cards_Number_Pointer);
        }
    }
}

void print_Player_Cards(Player *Game_Players,int PlayerIndex){
    Card *Cards_Number_Pointer;

    printf("Jeueur : %s\t Nombre des cardes : %d\n",(Game_Players+PlayerIndex)->name,(Game_Players+PlayerIndex)->CardsNumber);
    for(int i=0;i<(Game_Players+PlayerIndex)->CardsNumber;i++){
        Cards_Number_Pointer = (Game_Players+PlayerIndex)->CardsList[i];
        Print_Card(Cards_Number_Pointer);
    }
}

void Print_Card(Card *_Card){
    switch ((_Card)->value) {
        case 10:printf("%c\t +2\t\n",(_Card)->color);break;
        case 11:printf("%c\t Inverce\t\n",(_Card)->color);break;
        case 12:printf("%c\t Passe\t\n",(_Card)->color);break;
        case 13:printf("%c\t Joker\t\n",(_Card)->color);break;
        case 14:printf("%c\t +4\t\n",(_Card)->color);break;
        default:printf("%c\t %d\t\n",(_Card)->color,(_Card)->value);
    }
}

void game(Card *Game_Cards,Card *Last_Card,Player *Game_Players,int PlayersNumber,int inv){
    int PlayerIndex;

    PlayerIndex=0;
    inv=0;
    // check if the first card in the game is +4 or joker
    if((Last_Card)->color=='N'){
        do{
            printf("Choisir la couleur R:rouge Y:jaune B:bleu G:vert : ");
            scanf("%c",Last_Card->color);
        }while(Last_Card->color!='R' && Last_Card->color!='Y' && Last_Card->color!='B' && Last_Card->color!='G');
    }
    //boucle de jeu
    do{
        PlayerTour(Game_Cards,Last_Card,Game_Players,PlayersNumber,PlayerIndex,inv);
        PlayerIndexBoucle(&PlayerIndex,PlayersNumber,inv);
        Print_Card(Last_Card);
    }while(!check_End(Game_Players,PlayersNumber));

}

void PlayerIndexBoucle(int *PlayerIndex,int PlayersNumber,int inv){
    //il assure le boucle de tour des jeueur
    if(inv==0){
        if(*PlayerIndex==PlayersNumber-1){
            *PlayerIndex=0;
        }else{
            *PlayerIndex+=1;
        }
    }else{
        if(*PlayerIndex==0){
            *PlayerIndex=PlayersNumber-1;
        }else{
            *PlayerIndex-=1;
        }
    }
}

void PlayerTour(Card *Game_Cards,Card **Last_Card,Player *Game_Players,int PlayersNumber,int PlayerIndex,int inv){
    int choix;

    //afficher le tour du jeueur
    printf("=====C'est le tour du %s =====\n",Game_Players[PlayerIndex].name);
    printf("Dans la Table : \n");
    Print_Card(Last_Card);
    printf("Vous cartes : \n");
    printf("Si Vous n'a pas autre solution saisir 0\n");
    print_Player_Cards(Game_Players,PlayerIndex);

    //choisir la carte qui vous voulez jouer et verifier si la carte est compatible avec la derniere carte dans la table ou pas si non il faut choisir une autre carte compatible avec la derniere carte dans la table et si oui il faut changer la derniere carte dans la table et changer l'etat de la carte qui a etait dans la table et changer l'etat de la carte qui a etait jouer et changer le nombre des carte du jeueur qui a jouer la carte
    do{
        printf("Donner La posision de la carte qui vous choisir : ");
        scanf("%d",&choix);
        choix-=1;
    }while(!(0<=choix && choix<=Game_Players[PlayerIndex].CardsNumber && Card_Compatibility(Last_Card,Game_Players[PlayerIndex].CardsList[choix])));

    // change etat du la carte qui a etait dans la table
    RetrunToCards(Last_Card);

    //change la derniere carte
    PutCard(Game_Players,Last_Card,PlayerIndex,choix);

    //les fonction des cartes spesiale
    switch (Game_Players[PlayerIndex].CardsList[choix]->value) {
        case 10:plus2(Game_Cards,Game_Players,PlayerIndex,PlayersNumber,inv);break;
        case 11:if(inv==0){inv=1;}else{inv=0;};break;
        case 12:PlayerIndexBoucle(&PlayerIndex,PlayersNumber,inv);break;
        case 13:break;
        case 14:plus4(Game_Cards,Game_Players,PlayerIndex,inv);break;
        default:break;
    }

}

int solution(Card *Last_Card,Card *PlayerCards,int PlayerCardsNumber){
    int i=0;
    while(i<PlayerCardsNumber){
        if(Card_Compatibility(Last_Card,PlayerCards[i])){
            return i;
        }
        i++;
    }
    return 0;
}

// void Rech_Plus4OuJoker(Card *Last_Card,Card *PlayerCards,int PlayerCardsNumber,int *choix){
//     int i=0;
//     while(i<PlayerCardsNumber){
//         if(PlayerCards[i]->value==13 || PlayerCards[i]->value==14){
//             *choix=i;
//             return;
//         }
//         i++;
//     }
// }


void plus2(Card *Game_Cards,Player *Game_Players,int LastPlayerIndex,int PlayersNumber,int inv){
    int PlayerIndex,CardsNumber;

    PlayerIndex=LastPlayerIndex;
    CardsNumber=(Game_Players[LastPlayerIndex].CardsNumber);

    PlayerIndexBoucle(&PlayerIndex,PlayersNumber,inv);

    Game_Players[PlayerIndex].CardsList[CardsNumber+1] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsList[CardsNumber+2] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsNumber+=2;

}

void plus4(Card *Game_Cards,Player *Game_Players,int LastPlayerIndex,int PlayersNumber,int inv){
    int PlayerIndex,CardsNumber;

    PlayerIndex=LastPlayerIndex;
    CardsNumber=(Game_Players[LastPlayerIndex].CardsNumber);

    PlayerIndexBoucle(&PlayerIndex,PlayersNumber,inv);

    Game_Players[PlayerIndex].CardsList[CardsNumber+1] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsList[CardsNumber+2] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsList[CardsNumber+3] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsList[CardsNumber+4] = Get_Card(Game_Cards);
    Game_Players[PlayerIndex].CardsNumber+=4;

}


void RetrunToCards(Card *CardReturn){
    // change l'etat de la carte qui a etait dans la table
    (CardReturn)->etat=0;
    // change la couleur de la carte qui a etait dans la table
    if((CardReturn)->value==13 || (CardReturn)->value==14){
        (CardReturn)->color='N';
    }

}


void PutCard(Player *Game_Players, Card *Last_Card, int PlayerIndex, int choix) {
    // Change l'etat de la carte qui a etait jouer dans la table si elle +4 ou joker elle change la couleur
    RetrunToCards(Last_Card);

    // Change the last card
    (*(Last_Card))=(*(Game_Players[PlayerIndex].CardsList[choix]));

    // Supprimer la carte de la liste des cartes du joueur
    for (int i = choix; i < Game_Players[PlayerIndex].CardsNumber; i++) {
        Game_Players[PlayerIndex].CardsList[i] = Game_Players[PlayerIndex].CardsList[i+1];
    }
    Game_Players[PlayerIndex].CardsList[Game_Players[PlayerIndex].CardsNumber] = NULL;
    Game_Players[PlayerIndex].CardsNumber -= 1;
}


int Card_Compatibility(Card *Last_Card,Card *Choosen_Card){
    if(Last_Card->color==Choosen_Card->color || Last_Card->value==Choosen_Card->value){
        return 1;
    }else{
        return 0;
    }
}

int check_End(Player *Game_Players,int PlayersNumber){
    int Total_Complite;

    Total_Complite=0;
    for(int i=0;i<PlayersNumber;i++){
        Total_Complite=Total_Complite+Game_Players[i].CardsNumber;
    }

    if(Total_Complite==1){
        return 1;
    }else{
        return 0;
    }
}

int main(){
    //declaration
    Card *Game_Cards,*Last_Card;
    Player *Game_Players;
    int PlayersNumber,inv;// inv si inv = 1 donc le jeux est inverce
    srand(time(0));

    //reservation dans la memoire
    Game_Cards = (Card*)calloc(NUM_CARDS,sizeof(Card));
    Last_Card = (Card*)calloc(1,sizeof(Card));
    Game_Players = (Player*)calloc(Cards_Init_Number,sizeof(Player));

    // initialisation du tableux des carte pour jeux
    init_Game_Cards(Game_Cards);
    print_Game_Cards(Game_Cards);

    // initialisation du tableux des jeueur pour jeux
    init_Game_Players(Game_Players,Game_Cards,&PlayersNumber);
    print_Game_Players(Game_Players,PlayersNumber);

    // initialisation du la premiere carte
    Last_Card=Get_Card(Game_Cards);
    print_Game_Cards(Game_Cards);

    // game start
    game(Game_Cards,Last_Card,Game_Players,PlayersNumber,&inv);

    return 0;
}
