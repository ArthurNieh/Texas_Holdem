
#include <algorithm>
//#include "windows.h"
#include "card.h"
#include "player.h" 
#include "basic.h"
#include "gaming.h"
#include "compare.h"

using namespace std;


extern const int SCREEN_WIDTH = 1080;
extern const int SCREEN_HEIGHT = 720;

//Button constants
extern const int BUTTON_WIDTH = 200;
extern const int BUTTON_HEIGHT = 120;

extern const int NUM_BUT_L = SCREEN_WIDTH/16;
extern const int blind_size = 70;

LTexture hand1texture;
LTexture hand2texture;
LTexture generaltexture;
LTexture gBackgroundTexture;
LButton gbottum[5];
LButton exit_bottum;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

void welcome(int &n);


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		
	}
	//
	int N = 0;//player number
	welcome(N);
	
	player* p = new player[N];
	
	gaming(p, N);
	
	//Free resources and close SDL
	close();

	return 0;
}

void welcome(int &n){
		
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
		if(n<=1){
			wrong_input();
		}
	}while(n<=1);
	
}
