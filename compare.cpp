
#include "compare.h"


#include"card.h"
#include<algorithm>
#include<cstdlib>
#include"player.h"

card* arrange(card* hand, card* general){

    card* total = new card[7];
    card* random = new card [5];
    card* best = new card[5];
    int max =0;

    for(int i=0;i<7;i++){
        if(i<=4)
            total[i] = general[i];
        else
            total[i] = hand[i-5];
    }

    for(int i=0;i<7;i++){
        for(int j=i+1;j<7;j++){
            int tmp =0;

            for(int k=0;k<5;k++){
                if(tmp == i) tmp++;
                if(tmp == j) tmp++;

                random[k] = total[tmp];
                tmp++;
            }
                int t = countvalue(random);
                if(t > max){
                    for(int i=0;i<5;i++){
                        best[i].number = random[i].number;
                        best[i].color = random[i].color;}
                    max = t;
                }

          }
    }
    return best;
 }

 int countvalue(card* combine)
 {
    int color[4]={0};
    for(int i=0;i<5;i++){
        color[combine[i].color]++;}

    int number[14]={0};
    for(int i=0;i<5;i++){
        number[combine[i].number]++;}

    bool fivecolors = 0;

    for(int i=0;i<5;i++){
        if(color[i]==5)
            fivecolors = 1;
    }
    bool fourkinds=0;
    bool threekinds =0;
    int pairs=0;

    for(int i=0;i<14;i++){
        if(number[i]==4)
            fourkinds = 1;
        if(number[i]==3)
            threekinds = 1;
        if(number[i]==2)
            pairs++;
    }

    card* temp= combine;
    sort(temp,temp+5,compare); 


    if(fivecolors == 1){

        if((pairs==0 && temp[0].number - 4 == temp[4].number ) || (pairs==0 && temp[0].number ==1 && temp[4].number==10))
        {
            return 5000 + temp[4].number; 
        }
        else{
            if(temp[0].number==1)
                return 3500 + 14;
            else
                return 3500 + temp[0].number;

        }
    }

    else if(fourkinds){ 
        int x;
        for(int i=0;i<14;i++)
            if(number[i]==4)
                x = number[i];

            if(temp[0].number==1)
                return 4500 + 14;
            else
               return  4500 + x;
    }

    else if(threekinds){ 
        if(pairs==0)
        {
            if(temp[0].number==1)
                return 3000 + 14;
            else
                return 3000 + temp[2].number; 
        }

        else{
            if(temp[0].number==1)
                return 4000 + 14;
            else
                return 4000 + temp[2].number;
        }
    }

    else if(pairs == 2) //2 pairs
    {
        if(temp[1].number==temp[0].number && temp[3].number==temp[4].number){
           if(temp[0].number == 1)
                return 350 + 13*13*14 + 13* temp[3].number + temp[2].number;
            else
                return 350 + 13*13*temp[1].number + 13* temp[3].number + temp[2].number;
        }
 

        else if(temp[2].number==temp[3].number){
            if(temp[0].number == 1)
                return 350 + 13*13*14 + 13* temp[3].number + temp[4].number;
            else
                return 350 + 13*13*temp[1].number + 13* temp[3].number + temp[4].number;
        }
        else if(temp[1].number==temp[2].number)
        {   if(temp[0].number == 1)
                return 350 + 13*13*temp[1].number + 13* temp[3].number + 14;
            else
                return 350 + 13*13*temp[1].number + 13* temp[3].number + temp[0].number;
        }
    }

    else if(pairs==1) //pair
    {
        if(temp[0].number==temp[1].number)
        {   if(temp[0].number==1)
                return 20 + 13*14 +temp[2].number;
            else
                return 20 + 13*temp[0].number + temp[2].number;
        }
        else{
            int x;
            for(int i=0;i<14;i++)
                if(number[i]==2)
                    x = i;

            if(temp[0].number==1)
                return 20 + 14 + x*13;

            else
            {
                return 20 + temp[0].number + x*13;
            }
        }

    }

    else if(pairs==0)
    {
        if((pairs==0 && temp[0].number - 4 == temp[4].number ) || (pairs==0 && temp[0].number ==1 && temp[4].number==10)){
            return 3200 + temp[4].number;
        }
        else
        {
            if(temp[0].number == 1)
                return 14;
            else
                return temp[0].number;
        }

    }
 }

 bool compare(card a, card b){

    if(a.number == 1)
        return 1;
    else if(b.number ==1)
        return false;

	return a.number > b.number;
}




