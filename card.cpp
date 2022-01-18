#include "card.h"


std::string card::picaddress(){
	std::string address = "pictures/";
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
