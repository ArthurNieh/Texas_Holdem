#include "card.h"
#include <iostream>
using namespace std;

void card::showcard(){
	switch(number){
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			cout<<number;
			break;
		case 10:
			cout<<"T";
			break;
		case 1:
			cout<<"A";
			break;
		case 11:
			cout<<"J";
			break;
		case 12:
			cout<<"Q";
			break;
		case 13:
			cout<<"K";
			break;
		case 14:
			cout<<"Joker\n";
			break;
	}

	if(color != 4){
		cout<<" of ";
	}
	else{
		return;
	}

	switch(color){
		case 0:
			cout<<"spade"<<endl;
			break;
		case 1:
			cout<<"heart"<<endl;
			break;
		case 2:
			cout<<"diamond"<<endl;
			break;
		case 3:
			cout<<"club"<<endl;
			break;
	}
	return;
}
std::string card::picaddress(){
	string address = "pictures/";
	switch(color){
		case 0:
			address += "spade";
			break;
		case 1:
			address += "heart";
			break;
		case 2:
			address += "diamond";
			break;
		case 3:
			address += "club";
			break;
	}
	if(number<10){
		address += '0' + number;
	}
	else{
		address += '1';
		address += '0' + number-10;
	}
	address += ".png";
	return address;
}
