
#ifndef BID_H_INCLUDED
#define BID_H_INCLUDED

#include"card.h"
#include"player.h"


void after(player [],const int,int &,bool [],int,int,card*);
bool judge(const bool [],player [],int,int);
void singlegaming(player [],int , int , card*  , bool* ); 

void printmoneypotInPic(int);
void player_preoperation(player*, int, int, int);
void show_generalcardInPic( card* , int);
void personal_reset(player [], const int , int &, int , int , card* , int , int );



#endif // BID_H_INCLUDED
