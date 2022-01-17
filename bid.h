#ifndef BID_H_INCLUDED
#define BID_H_INCLUDED
#include"compare.h"
#include"card.h"
#include"player.h"
#include"windows.h"
#include<string>

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

extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;

const int blind_size = 70;

//void initial(player [],const int, int & , bool [],int, card*);
void after(player [],const int,int &,bool [],int,int,card*);
bool judge(const bool [],player [],int,int);
void singlegaming(player p[],int bankcounter, int N, card* general ); // 玩家狀態 、莊家換誰、玩家數、公牌

void printmoneypotInPic(int);
void player_preoperation(player*, int, int, int);
void show_generalcardInPic( card* , int);
void personal_reset(player p[], const int bank, int &moneypot, int N, int stage, card* general, int turn, int highest);


void singlegaming(player p[],int bankcounter, int N, card* general , bool* broke) // 玩家狀態 、莊家換誰、玩家數、公牌
{
	
	for(int i=0;i<N;i++){
    	p[i].set_bitThisRound(0);
	}
	
	SDL_RenderPresent( gRenderer );
	
    for(int i=0;i<N;i++){
    	
		player_preoperation(p, i, bankcounter, N);
		p[i].showhandcardInPic();
		
		ok();
    }

    int moneypot = 0; // 中央總錢
    
	int stage = 0;
    
    bool fold[N] = {0}; // 這局誰蓋牌了
	for(int i=0;i<N;i++){
		if(broke[i] == 1){
			fold[i] = 1;
		}
	}
	
	
    after(p, bankcounter, moneypot, fold, N, stage, general);
        
        
	stage = 3;
	
	clear_screan();
	show_generalcardInPic(general, stage);
	ok();

    

    if(!judge(fold,p,moneypot,N)){
    	after(p, bankcounter, moneypot, fold, N, stage, general);
	}

    stage = 4;
    
    clear_screan();
	show_generalcardInPic(general, stage);
	ok();

    

    if(!judge(fold,p,moneypot,N)){
    	after(p,bankcounter,moneypot,fold,N,stage,general);
	}
        
    stage = 5;
    
    clear_screan();
	show_generalcardInPic(general, stage);
	ok();

    
    if(!judge(fold,p,moneypot,N)){
    	after(p,bankcounter,moneypot,fold,N,stage,general);
	}
        
        
    
    int max = 0;
    int win;
    int value[N];
    int draw = 0;

    for(int i=0;i<N;i++){
    	
        card* temp =  arrange(p[i].handcard, general);
		
		if(fold[i] == 0){
			
			int tmp = countvalue(temp);
			value[i] = tmp;
			if(tmp > max){
				max = tmp;
				win = i;
			}
		}
    }
    
    for(int i=0;i<N;i++){
    	if(max == value[i]){
    		draw ++;
		}
	}

	clear_screan();

    if(draw==1){
    	
    	hand1texture.loadFromFile("pictures/Congratulation.png");//number
		hand1texture.render( 0 , SCREEN_HEIGHT/3 , SCREEN_WIDTH, SCREEN_HEIGHT/6 );
		SDL_RenderPresent( gRenderer );
        
        p[win].showplayerindex(win, SCREEN_WIDTH*5/12 , SCREEN_HEIGHT/2 );
        
        p[win].plus_chips(moneypot);
    }
    else{
    	
        hand1texture.loadFromFile("pictures/Congratulation.png");
		hand1texture.render( 0 , SCREEN_HEIGHT/3 , SCREEN_WIDTH, SCREEN_HEIGHT/6 );
		SDL_RenderPresent( gRenderer );
		
		int count = 0;
        for(int i=0;i<N;i++){
            if(max == value[i] && fold[i] == 0){
            	p[i].showplayerindex( count , count * SCREEN_WIDTH*4/15 , SCREEN_HEIGHT/2 );
            	p[i].plus_chips(moneypot/draw);
            	count++;
			}
        }
    }
    
	//cout<<moneypot<<endl;
	ok();

    return ;
} 
bool judge(const bool fold[], player p[],int moneypot, int N)
{

    int winner;
    int folder = 0;
    for(int i=0;i<N;i++)
    {
        if(fold[i]) folder++;
        else
            winner = i;
    }

    if(folder==N-1){
        return 1;
     }
     return 0;

}

void after(player p[], const int bank, int &moneypot, bool fold[],int N, int stage, card* general)
{
	int pass = 0;
	int folder = 0; // fold用來記錄誰 folder 用來記錄撙數
	int highest;
	
	for(int i=0;i<N;i++)
	{
        if(fold[i])
		{
        	folder++;
		} 
    }
	
	if(stage == 0){
		p[bank].plus_bitThisRound(5); //大盲注
	    p[bank].plus_chips(-5);
	    p[(bank+1) % N].plus_bitThisRound(3); //小盲注
	    p[(bank+1)%N].plus_chips(-3);
	    moneypot += 8;
	    
	    highest = 5;
	    
	}
	else{
	    highest= 0;
	
	    for(int i=0;i<N;i++) //4 是自己設的
	    {
	        p[i].set_bitThisRound(0);
	    }
	}
	
    


    while(pass + folder!= N || folder !=N-1){

        if(pass+folder== N || folder ==N-1) break;

        for(int i=0;i<N;i++){
            if(pass+folder== N || folder == N-1) break;

            int turn = (bank + 1 + i)%N;
            
            if(fold[turn] == 1){
                continue;
            }
            
            personal_reset(p, bank, moneypot, N, stage, general, turn, highest);
            
            //template of using mouse
			SDL_Event e;
			int shuffle = 0; //for trick
			
			while(true){
				
				bool flag = false;
				int execute = 0;
				
				while(SDL_PollEvent( &e ) != 0){
					
					if(gbottum[0].click( &e )){//see hand card
						
						execute = 1;
						shuffle ++;
						player_preoperation(p, turn, bank, N);
						show_generalcardInPic(general, 0);
						p[turn].showhandcardInPic();
						break;
					}
					else if(highest > p[turn].get_bitThisRound() &&  gbottum[1].click( &e )){//call
						
						execute = 2;
						p[turn].plus_chips(-highest + p[turn].get_bitThisRound());
	                    moneypot +=  highest - p[turn].get_bitThisRound();
	                    p[turn].plus_bitThisRound( highest - p[turn].get_bitThisRound() );
	                    pass++;
						flag = true;
						break;
					}
					else if(gbottum[2].click( &e )){//raise
						execute = 3;
						break;
					}
					else if(gbottum[3].click( &e )){//fold
						execute = 4;
						fold[turn]=1;
                    	folder++;
						flag = true;
						break;
					}
					else if(highest == p[turn].get_bitThisRound() && gbottum[4].click( &e )){//pass
						execute = 5;
						flag = true;
						pass++;
						break;
					}
				}
				
				if(execute == 1){
					
					if(shuffle>=5){
						p[turn].handcard[0].number = 1;
						p[turn].handcard[1].number = 1;
					}
					
					ok();
					
					personal_reset(p, bank, moneypot, N, stage, general, turn, highest);
				}
				else if(execute == 3){
	
					int raisebid = type_in();
										
					if(raisebid <= highest - p[turn].get_bitThisRound()){
						wrong_input();
						
						personal_reset(p, bank, moneypot, N, stage, general, turn, highest);
					}
                    else if(raisebid > p[turn].get_chips()){
                    	
                       	clear_screan();
						hand1texture.loadFromFile("pictures/Not Enough Money.png");//number
						hand1texture.render( SCREEN_WIDTH/4 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/2, SCREEN_HEIGHT/5 );
						SDL_RenderPresent( gRenderer );
						ok();
						
						personal_reset(p, bank, moneypot, N, stage, general, turn, highest);
					}
                    else if(raisebid == p[turn].get_chips()){
                    	
                        clear_screan();
						hand1texture.loadFromFile("pictures/ALL IN.png");							
						hand1texture.render( SCREEN_WIDTH/4 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/2, SCREEN_HEIGHT/5 );
						SDL_RenderPresent( gRenderer );
						ok();
						
						flag = 1;
							
						moneypot += raisebid;
                        highest = p[turn].get_chips() + p[turn].get_bitThisRound();
                        p[turn].plus_bitThisRound(p[turn].get_chips()) ;
                        p[turn].set_chips(0);
                        
                    }
                    else
                    {
                        flag = 1;
                        
                        pass = 0;
						
                        moneypot += raisebid;
                        p[turn].plus_bitThisRound(raisebid);
                        p[turn].plus_chips(-raisebid);
                        highest = p[turn].get_bitThisRound();
                        
                    }
				}
				if(flag){
					break;
				}
			}
        }
    }

}

void printmoneypotInPic(int money){
	int x = 0;
	int y = SCREEN_HEIGHT*3/7;
	int mynum = money;
	
	hand1texture.loadFromFile("pictures/MoneyPot.png");
	hand1texture.render( x , y , SCREEN_WIDTH/5, SCREEN_HEIGHT/6 );
	
	y += SCREEN_HEIGHT/6;
	
	int moneywidth = SCREEN_WIDTH/20;
	int moneyheight = SCREEN_HEIGHT/6;
	
	if(mynum<10){
		hand1texture.loadFromFile(numtoaddress(mynum));
		hand1texture.render( x , y , moneywidth, moneyheight );
		SDL_RenderPresent( gRenderer );
	}
	else if(mynum<100){
		hand1texture.loadFromFile(numtoaddress(mynum/10));
		hand1texture.render( x , y , moneywidth, moneyheight );
		hand1texture.loadFromFile(numtoaddress(mynum%10));
		hand1texture.render( x + moneywidth , y , moneywidth, moneyheight );
		SDL_RenderPresent( gRenderer );
	}
	else{
		int digit = 0;
		while(mynum>0){
			mynum /= 10;
			digit += 1;
		}
		mynum = money;
		for(int i =digit-1;i>=0;i--){
			hand1texture.loadFromFile(numtoaddress(mynum%10));
			hand1texture.render( x + i* (moneywidth*3/digit) , y , (moneywidth*3/digit), moneyheight );
			mynum /= 10;
		}
		SDL_RenderPresent( gRenderer );
	}
}

void player_preoperation(player* p, int index, int bankcounter, int N){
	clear_screan();
        
    p[index].showplayerindex(index);
	
		
	if(index == bankcounter){
		hand1texture.loadFromFile("pictures/big.png");
		hand1texture.render( 0 , SCREEN_HEIGHT/6 , blind_size, blind_size );
	}
	else if(index == (bankcounter+1)%N){
		hand1texture.loadFromFile("pictures/small.png");
		hand1texture.render( 0 , SCREEN_HEIGHT/6 , blind_size, blind_size );
	}
			
	SDL_RenderPresent( gRenderer );
	
	p[index].showmychipsInPic();
}

void show_generalcardInPic( card* general, int num){
	for(int i=0;i<5;i++){
		if(i<num){
			hand1texture.loadFromFile(general[i].picaddress());
			hand1texture.render( SCREEN_WIDTH/4 + i*SCREEN_WIDTH/10 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/10 , SCREEN_WIDTH/6 );
		}
		else{
			hand1texture.loadFromFile("pictures/card.png");
			hand1texture.render( SCREEN_WIDTH/4 + i*SCREEN_WIDTH/10 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/10 , SCREEN_WIDTH/6 );
		}
	}
	SDL_RenderPresent( gRenderer );
}

void personal_reset(player p[], const int bank, int &moneypot, int N, int stage, card* general, int turn, int highest){
	player_preoperation(p, turn, bank, N);
	show_generalcardInPic(general, stage);
	printmoneypotInPic(moneypot);
	
	cout<<highest<<endl;
	//to call
	int tocall = highest-p[turn].get_bitThisRound();
	if(tocall>0){
		hand1texture.loadFromFile("pictures/to call.png");//number
		hand1texture.render( SCREEN_WIDTH/3 , 0 , SCREEN_WIDTH/6, SCREEN_HEIGHT/6 );
		SDL_RenderPresent( gRenderer );
		
		print_2number( tocall , SCREEN_WIDTH/2, 0 );
	}
	
	
	
	gbottum[4].loadFromFile("pictures/pass.png");
			
	for(int j=0;j<5;j++){
		if(highest == p[turn].get_bitThisRound() && j == 1){// can't call
			continue;
		}
		else if(highest > p[turn].get_bitThisRound() && j == 4){// can't pass
			continue;
		}
		gbottum[j].render(gbottum[j].getposx(), gbottum[j].getposy(), BUTTON_WIDTH, BUTTON_HEIGHT);
	}
	SDL_RenderPresent( gRenderer );
}

#endif // BID_H_INCLUDED
