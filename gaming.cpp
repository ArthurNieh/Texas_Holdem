
#include "gaming.h"


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Scene textures
extern LTexture hand1texture;
extern LTexture hand2texture;
extern LTexture gBackgroundTexture;
extern LButton gbottum[5];
extern LButton exit_bottum;
extern LButton resume_bottum;

bool gaming(player p[], int N)
{
	
    int bankcounter=0;
    while(1)
    {

        int broken = 0;
        bool broke[N] ={0};
        int winner;

        for(int i=0;i<N;i++)
        {
            if(p[i].get_chips() == 0){
                broke[i] = 1;
                broken++;
            }
            else
                winner = i;

        }
		//winner ending prsentation
        if(broken == N-1){
            
            clear_screan();
            
            hand1texture.loadFromFile("pictures/cute.png");
			hand1texture.render( 0 , 0 , SCREEN_HEIGHT*6/7, SCREEN_HEIGHT);
			hand1texture.loadFromFile("pictures/trophy.png");
			hand1texture.render( SCREEN_HEIGHT*6/7, BUTTON_HEIGHT , SCREEN_WIDTH-SCREEN_HEIGHT*6/7 , SCREEN_HEIGHT-BUTTON_HEIGHT);
			
			exit_bottum.render( SCREEN_WIDTH-BUTTON_WIDTH , 0 , BUTTON_WIDTH, BUTTON_HEIGHT);
			resume_bottum.render(SCREEN_WIDTH-BUTTON_WIDTH , BUTTON_HEIGHT , BUTTON_WIDTH, BUTTON_HEIGHT);
			
			SDL_RenderPresent( gRenderer );
			
			p[winner].showplayerindex(winner, SCREEN_WIDTH/4, 0);
			hand1texture.loadFromFile("pictures/WINNER.png");
			hand1texture.render( 0 , 0 , SCREEN_WIDTH/4, SCREEN_HEIGHT/6);
			SDL_RenderPresent( gRenderer );
            
            SDL_Event e;
			while(true){
				while(SDL_PollEvent( &e ) != 0){
					if(exit_bottum.click( &e )){
						return 0;
					}
					if(resume_bottum.click( &e )){
						return 1;
					}
				}
			}
			
            break;
        }

        for(int i=0;i<N;i++)
        	p[i].cardNumber = 2;
		
		player publics;
		publics.cardNumber = 5;
		
        serve(p, N, publics);
        setpos(p,N);
        
        
		clear_screan();
		
		
        for(int i=0;i<N;i++){
        	p[i].showcardontable();
		}
		//for general cards
		hand1texture.loadFromFile("pictures/card.png");
		for(int i=0;i<5;i++){
			hand1texture.render(  SCREEN_WIDTH*13/32+17+43*i,  SCREEN_HEIGHT*4/9, 30, 50 );
		}
		
		//for round number
		hand1texture.loadFromFile("pictures/Round.png");
		hand1texture.render( 0 , 0 , SCREEN_WIDTH/6, SCREEN_HEIGHT/6 );
		SDL_RenderPresent( gRenderer );
		
		//print round number
		print_2number(bankcounter + 1, SCREEN_WIDTH/6 , 0 );
		
		//for rabbit girl
		hand1texture.loadFromFile("pictures/iu_rabbit_girl.png");
		hand1texture.render( 0 , SCREEN_HEIGHT/6 , SCREEN_WIDTH/6, SCREEN_HEIGHT/2);
		SDL_RenderPresent( gRenderer );
				
        //ready bottum
        gbottum[4].loadFromFile("pictures/Ready.png");
		gbottum[4].render(gbottum[4].getposx(), gbottum[4].getposy(), BUTTON_WIDTH, BUTTON_HEIGHT);
		
		//exit bottum
		exit_bottum.render( SCREEN_WIDTH-BUTTON_WIDTH , 0 , BUTTON_WIDTH, BUTTON_HEIGHT);
		
		SDL_RenderPresent( gRenderer );
		
		//template of using mouse
		SDL_Event e;
		while(true){
			bool flag = false;
			while(SDL_PollEvent( &e ) != 0){
				if(gbottum[4].click( &e )){
					flag = true;
					break;
				}
				if(exit_bottum.click( &e )){
					return 0;
				}
			}
			if(flag){
				break;
			}
		}
		
		
        singlegaming(p, bankcounter%N, N, publics.handcard, broke);
        bankcounter++;


    }
}

void serve(player* p, int numberOfPeople, player &pc){
	
	bool allcards[52] = {0};
	srand(time(0));

	for(int i=0;i<numberOfPeople;i++){
		int* mycard = new int[p[i].cardNumber];

		for(int j=0;j<p[i].cardNumber;j++){
			int take;
			do{
				take = rand()%52;
			}while(allcards[take] == 1);
			mycard[j] = take;
			allcards[take] = 1;
		}
		p[i].sethandcard(mycard, p[i].cardNumber);
	}
	int* pccard = new int[pc.cardNumber];
	
	for(int j=0;j<pc.cardNumber;j++){
		int take;
		do{
			take = rand()%52;
		}while(allcards[take] == 1);
		pccard[j] = take;
		allcards[take] = 1;
	}
	pc.sethandcard(pccard, pc.cardNumber);
}

void setpos(player* p, int N){
	if(N%2 == 0){
		int half = N/2;
		int gap = SCREEN_WIDTH / (N/2+1);
		for(int i=0;i<N;i++){
			if(i/half == 0){
				p[i].setposition((i+1)*gap, SCREEN_HEIGHT*2/3);
			}
			else{
				p[i].setposition((i-half+1)*gap, SCREEN_HEIGHT*3/14);
			}
		}
	}
	else{
		int half = (N+1)/2;
		int gap = SCREEN_WIDTH / ((N+1)/2+1);
		for(int i=0;i<N;i++){
			if(i/half == 0){
				p[i].setposition((i+1)*gap, SCREEN_HEIGHT*2/3);
			}
			else{
				p[i].setposition((i-half+1)*SCREEN_WIDTH/(N/2+1), SCREEN_HEIGHT*3/14);
			}
		}
	}
} 
