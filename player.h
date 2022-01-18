#ifndef PLAYER_H
#define PLAYER_H
#include<string>
#include "card.h"
#include "basic.h"

class player
{
	friend class card;
	public:
		player();
		std::string name;
		card* handcard ;
		int cardNumber;//number of card in hand

		bool comCheck;// check if this player a computer player
		void setcardNumber(int, int);//set number, playerNumber

		void set_chips(int moneyw){chips = moneyw;}
		int get_chips(){return chips;}
		void plus_chips(int);

		void sethandcard(int*, int);
		void sorthandcard();//to sort handcard

		void set_bitThisRound(int);
		int get_bitThisRound();
		void plus_bitThisRound(int);

		void setposition(int, int);
		int getposx(){return posx;}
		int getposy(){return posy;}

		//SDL references
		void showplayerindex(int index);
		void showplayerindex(int index, int posx, int posy);
		void showcardontable();
		void showhandcardInPic();
		void showmychipsInPic();
		

	protected:
		int number;//�s��
		int chips;//��
		int bitThisRound;
		int posx;
		int posy;
};



#endif
