#ifndef GAMING_H_INCLUDED
#define GAMING_H_INCLUDED
#include"player.h"

#include"bid.h"
#include<ctime>
#include"basic.h"


void serve(player* , int, player&);
bool gaming(player p[], int N);

void setpos(player* p, int N);




#endif // GAMING_H_INCLUDED
