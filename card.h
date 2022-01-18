#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
#include<string>
class card
{

	public:
		int color;//0 for spade, 1 for heart, 2 for diamond, 3 for club, 4 for jokers
		int number;//14 for jokers
		std::string picaddress();

	private:

};



#endif // CARD_H_INCLUDED

