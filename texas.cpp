
#include "texas.h"

#include <algorithm>
#include "card.h"
#include "player.h" 
#include "basic.h"
#include "gaming.h"
#include "compare.h"


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Button constants
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;

extern const int NUM_BUT_L;
extern const int blind_size;

extern LTexture hand1texture;
extern LTexture hand2texture;
extern LTexture generaltexture;
extern LTexture gBackgroundTexture;
extern LButton gbottum[5];
extern LButton exit_bottum;
extern LButton resume_bottum;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;


bool texas::holdem()
{
	//Start up SDL and create window
/*	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}*/
//	else
//	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		
//	}
	
	int N = 0;//player number
	welcome(N);
	//
	player* p = new player[N];
	
	//Free resources and close SDL
	//close();

	return gaming(p, N);
}

void texas::welcome(int &n){
		
	clear_screan();
	hand1texture.loadFromFile("pictures/rule1.png");//number
	hand1texture.render( 0 , 0 , SCREEN_WIDTH, SCREEN_HEIGHT );
	SDL_RenderPresent( gRenderer );
	ok();
	
	clear_screan();
	hand1texture.loadFromFile("pictures/rule2.png");//number
	hand1texture.render( SCREEN_WIDTH/4 , 0 , SCREEN_HEIGHT*5/8, SCREEN_HEIGHT );
	SDL_RenderPresent( gRenderer );
	ok();
	
	do{	
		clear_screan();
		hand1texture.loadFromFile("pictures/player_number.png");//number
		hand1texture.render( SCREEN_WIDTH/4 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/2, SCREEN_HEIGHT/5 );
		SDL_RenderPresent( gRenderer );
		n = type_in();
		if(n<=1 || n>23){
			wrong_input();
		}
	}while(n<=1 || n>23);
	
}
