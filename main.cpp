
#include <iostream>
#include <algorithm>
#include "windows.h"
#include "card.h"
#include "player.h" 
#include "basic.h"
#include  "gaming.h"
using namespace std;


extern const int SCREEN_WIDTH = 1080;
extern const int SCREEN_HEIGHT = 720;

//Button constants
extern const int BUTTON_WIDTH = 200;
extern const int BUTTON_HEIGHT = 120;

extern const int NUM_BUT_L = SCREEN_WIDTH/16;


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
	
	int N;//player number
	welcome(N);
	
	player* p = new player[N];
	//cout<<"asshole"<<endl;
	
	gaming(p, N);
	
	//Start up SDL and create window
	

	//Free resources and close SDL
	close();

	return 0;
}

void welcome(int &n){
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
