#include "player.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#define initialMoney 100
using namespace std;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Scene textures
extern LTexture hand1texture;
extern LTexture hand2texture;
extern LTexture gBackgroundTexture;
extern LButton gbottum[5];

player::player(){

	set_chips(initialMoney);

	handcard = NULL;
	bitThisRound=0;
	cardNumber = 0;
}

void player::setcardNumber(int num, int playernum){
	number = num;
	if(num<(53%playernum)){
		cardNumber = 53/playernum +1;
	}
	else{
		cardNumber = 53/playernum;
	}
}
void player::initializer(int num){

	cout<<"Welcome player "<<num<<"\n";
	cout<<"please type in your name"<<endl;
	string yourname;
	cin>>yourname;
	name = yourname;
	cout<<"Hellow "<<yourname<<"\n";
	comCheck = 0;
}
void player::cominitializer(int order){
	char comorder = 'A' + order;
	name = "Computer ";
	name[8] = comorder;
	cout<<name<<endl;
	comCheck = 1;
}
void player::sethandcard(int* cardnotprocessed, int ncards){

	handcard = new card[ncards];
	for(int i=0;i<ncards;i++){
		handcard[i].color = cardnotprocessed[i] % 4;
		if(cardnotprocessed[i] == 52){
			handcard[i].number = 14;
			handcard[i].color = 4;
		}
		else if(cardnotprocessed[i] / 4 == 0){
			handcard[i].number = 13;
		}
		else{
			handcard[i].number = cardnotprocessed[i] / 4;
		}

	}
}
bool cardcompare(card a, card b){
	if(a.number != b.number){
		return (a.number < b.number);
	}
	else{
		return (a.color < b.color);
	}
}
void player::sorthandcard(){
	sort(handcard, handcard + cardNumber, cardcompare);
}
void player::showhandcards(){
	if(cardNumber == 0){
		cout<<"\n--------------------\n\n";
		cout<<"There is no card in "<<name<<"'s hand."<<endl;
		return;
	}
	for(int i=0;i<cardNumber;i++){
		handcard[i].showcard();
	}
}
void player::dealpairedcards(){
	if(!comCheck){
		cout<<"throwing out the paired cards";
		for(int i=0;i<3;i++){
			cout<<".";
			Sleep(200);
		}
		cout<<endl;
	}

	int index = 0;
	while(index < cardNumber - 1){
		if(handcard[index].number == handcard[index+1].number){
			getridofcard(index);
			getridofcard(index);
		}
		else{
			index++;
		}
	}
}
void player::getridofcard(int index){
	card* temp = new card[cardNumber-1];
	int counttemp = 0;
	for(int i=0;i<cardNumber;i++){
		if(i != index){
			temp[counttemp] = handcard[i];
			counttemp++;

		}
	}
	delete handcard;
	handcard = temp;
	cardNumber-=1;
}
void player::addhandcard(card thecard){
	card* temp = new card[cardNumber+1];
	for(int i=0;i<cardNumber;i++){
		temp[i] = handcard[i];
	}
	temp[cardNumber] = thecard;
	delete handcard;
	handcard = temp;
	sorthandcard();
	cardNumber++;
	return;
}
void player::plus_chips(int chip){
	chips += chip;
}
void player::set_bitThisRound(int b){
	bitThisRound = b;
}
int player::get_bitThisRound(){
	return bitThisRound;
}
void player::plus_bitThisRound(int b){
	bitThisRound += b;
}
void player::setposition(int x, int y){
	posx = x;
	posy = y;
}
void player::showplayerindex(int index){
	hand1texture.loadFromFile("pictures/player.png");//player
	hand1texture.render( 0 , 0 , SCREEN_WIDTH/6, SCREEN_HEIGHT/6 );
	
	if(index<10){
		hand1texture.loadFromFile(numtoaddress(index));//number
		hand1texture.render( SCREEN_WIDTH/6 , 0 , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
	}
	else{
		hand1texture.loadFromFile(numtoaddress(index/10));//number
		hand1texture.render( SCREEN_WIDTH/6 , 0 , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
		hand1texture.loadFromFile(numtoaddress(index%10));//number
		hand1texture.render( SCREEN_WIDTH/6 + SCREEN_WIDTH/20 , 0 , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
	}
	
	SDL_RenderPresent( gRenderer );
}
void player::showplayerindex(int index, int posx, int posy){
	hand1texture.loadFromFile("pictures/player.png");//player
	hand1texture.render( posx , posy , SCREEN_WIDTH/6, SCREEN_HEIGHT/6 );
	
	if(index<10){
		hand1texture.loadFromFile(numtoaddress(index));//number
		hand1texture.render( posx + SCREEN_WIDTH/6 , posy , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
	}
	else{
		hand1texture.loadFromFile(numtoaddress(index/10));//number
		hand1texture.render( posx + SCREEN_WIDTH/6 , posy , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
		hand1texture.loadFromFile(numtoaddress(index%10));//number
		hand1texture.render( posx + SCREEN_WIDTH/6 + SCREEN_WIDTH/20 , posy , SCREEN_WIDTH/20, SCREEN_HEIGHT/6 );
	}
	
	SDL_RenderPresent( gRenderer );
}

void player::showcardontable(){
	hand1texture.loadFromFile("pictures/card.png");
	hand1texture.render( getposx() , getposy() , 30, 50 );
   	hand1texture.render( getposx()+15 , getposy() , 30, 50 );
   	SDL_RenderPresent( gRenderer );
}

void player::showhandcardInPic(){
	hand1texture.loadFromFile(handcard[0].picaddress() );//number
	hand1texture.render( SCREEN_WIDTH/4 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/4, SCREEN_WIDTH/4*5/3 );
    hand1texture.loadFromFile(handcard[1].picaddress() );//number
	hand1texture.render( SCREEN_WIDTH/2 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/4, SCREEN_WIDTH/4*5/3 );
	SDL_RenderPresent( gRenderer );
}

void player::showmychipsInPic(){
	
	int x = SCREEN_WIDTH*12/20;
	int y = 0;
	
	int moneywidth = SCREEN_WIDTH/20;
	int moneyheight = SCREEN_HEIGHT/6;
	int mynum = chips;
	hand1texture.loadFromFile("pictures/money.png");
	hand1texture.render( x , y , moneywidth*5 , moneyheight );
	
	if(mynum<10){
		hand1texture.loadFromFile(numtoaddress(mynum));
		hand1texture.render( x + moneywidth*5 , y , moneywidth, moneyheight );
		SDL_RenderPresent( gRenderer );
	}
	else if(mynum<100){
		hand1texture.loadFromFile(numtoaddress(mynum/10));
		hand1texture.render( x + moneywidth*5 , y , moneywidth, moneyheight );
		hand1texture.loadFromFile(numtoaddress(mynum%10));
		hand1texture.render( x + moneywidth*6 , y , moneywidth, moneyheight );
		SDL_RenderPresent( gRenderer );
	}
	else{
		int digit = 0;
		while(mynum>0){
			mynum /= 10;
			digit += 1;
		}
		mynum = chips;
		for(int i =digit-1;i>=0;i--){
			hand1texture.loadFromFile(numtoaddress(mynum%10));
			hand1texture.render( x + moneywidth*5 + i* (moneywidth*3/digit) , y , (moneywidth*3/digit), moneyheight );
			mynum /= 10;
		}
		SDL_RenderPresent( gRenderer );
	}
	
}
