#ifndef BID_H_INCLUDED
#define BID_H_INCLUDED

#include"card.h"
#include"player.h"

void after(player [],const int,int &,bool [],int,int,card*);
bool judge(const bool [],player [],int,int);
void singlegaming(player p[],int bankcounter, int N, card* general , bool* broke); // 玩家狀態 、莊家換誰、玩家數、公牌

void printmoneypotInPic(int);
void player_preoperation(player*, int, int, int);
void show_generalcardInPic( card* , int);
void personal_reset(player p[], const int bank, int &moneypot, int N, int stage, card* general, int turn, int highest);


#endif // BID_H_INCLUDED
