#ifndef BASIC_H
#define BASIC_H

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};


//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y , int, int);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	protected:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};


//The mouse button
class LButton : public LTexture
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );
		int getposx(){return mPosition.x;}
		int getposy(){return mPosition.y;}
		
		//Handles mouse event
		void handleEvent( SDL_Event* e );
		bool click( SDL_Event* e );
		bool numclick( SDL_Event* e );
		//Shows button sprite
		//void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};




//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void clear_screan();
std::string numtoaddress(int num);
int type_in();
void ok();
void wrong_input();
void print_2number(int, int, int);

#endif





