
#include "basic.h"

//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;

extern const int NUM_BUT_L;

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

LButton numberbottum[10];

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, int width, int height)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_Rect clip={0,0,mWidth,mHeight};
	SDL_RenderCopy( gRenderer, mTexture, &clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

// edit by arthur
bool LButton::click( SDL_Event* e ){
	if(e->type == SDL_MOUSEBUTTONDOWN){
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}
		
		if(!inside){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 0;
	}
}
bool LButton::numclick( SDL_Event* e ){
	if(e->type == SDL_MOUSEBUTTONDOWN){
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + NUM_BUT_L )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + NUM_BUT_L )
		{
			inside = false;
		}
		
		if(!inside){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 0;
	}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	//Load background texture
	if( !gBackgroundTexture.loadFromFile( "pictures/texaspokerdesk.png" ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}
	if( !gbottum[0].loadFromFile( "pictures/seehandcard.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	if( !gbottum[1].loadFromFile( "pictures/call.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	if( !gbottum[2].loadFromFile( "pictures/raise.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	if( !gbottum[3].loadFromFile( "pictures/fold.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	if( !gbottum[4].loadFromFile( "pictures/pass.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	for(int i=0;i<5;i++){
		gbottum[i].setPosition(i*BUTTON_WIDTH, SCREEN_HEIGHT-BUTTON_HEIGHT);
	}
	for(int i =0;i<10;i++){
		numberbottum[i].loadFromFile(numtoaddress(i));
		if(i<5){
			numberbottum[i].setPosition(SCREEN_WIDTH/4 + i*NUM_BUT_L, SCREEN_HEIGHT*2/4);
		}
		else{
			numberbottum[i].setPosition(SCREEN_WIDTH/4 + (i-5)*NUM_BUT_L, SCREEN_HEIGHT*2/4 + NUM_BUT_L);
		}
	}
	if( !exit_bottum.loadFromFile( "pictures/EXIT.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	exit_bottum.setPosition(SCREEN_WIDTH-BUTTON_WIDTH, 0);
	
	if( !resume_bottum.loadFromFile( "pictures/resume.png" ) )
	{
		printf( "Failed to load gbottum texture image!\n" );
		success = false;
	}
	resume_bottum.setPosition(SCREEN_WIDTH- 2*BUTTON_WIDTH, 0);
	
	return success;
}

void close()
{
	//Free loaded images
	hand1texture.free();
	hand2texture.free();
	gBackgroundTexture.free();
	exit_bottum.free();
	resume_bottum.free();
	for(int i=0;i<5;i++){
		gbottum[i].free();
	}

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

//started here
void clear_screan(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	//Render background texture to screen
	gBackgroundTexture.render( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_RenderPresent( gRenderer );
}

std::string numtoaddress(int num){
	std::string address = "pictures/";
	address +=  ('0' + num);
	address += ".png";
	return address;
}

int type_in(){
	
	int count_input = 0;
	for(int i =0;i<10;i++){
		numberbottum[i].render(numberbottum[i].getposx(), numberbottum[i].getposy(), NUM_BUT_L, NUM_BUT_L);
	}
	
	gbottum[4].loadFromFile("pictures/ok.png");
	gbottum[4].render(gbottum[4].getposx(), gbottum[4].getposy(), BUTTON_WIDTH, BUTTON_HEIGHT);	
	SDL_RenderPresent( gRenderer );
	
	SDL_Event e;
	while(true){
		bool flag = false;
		while(SDL_PollEvent( &e ) != 0){
			for(int i=0;i<10;i++){
				if(numberbottum[i].numclick( &e )){
					count_input *= 10;
					count_input += i;
					break;
				}
			}
			if(gbottum[4].click( &e )){
				flag = true;
				break;
			}
		}
		if(flag){
			break;
		}
	}
	return count_input;
}

void ok(){
	gbottum[4].loadFromFile("pictures/ok.png");
	gbottum[4].render(gbottum[4].getposx(), gbottum[4].getposy(), BUTTON_WIDTH, BUTTON_HEIGHT);
		
	SDL_RenderPresent( gRenderer );
		
	
	SDL_Event e;
	while(true){
		bool flag = false;
		while(SDL_PollEvent( &e ) != 0){
			if(gbottum[4].click( &e )){
				flag = true;
				break;
			}
		}
		if(flag){
			break;
		}
	}
}
void wrong_input(){
	clear_screan();
	hand1texture.loadFromFile("pictures/Wrong Input!!.png");//number
	hand1texture.render( SCREEN_WIDTH/4 , SCREEN_HEIGHT/4 , SCREEN_WIDTH/2, SCREEN_HEIGHT/5 );
	SDL_RenderPresent( gRenderer );
	ok();
}
void print_2number(int num, int posx, int posy){
	
	int length = SCREEN_WIDTH/20;
	int height = SCREEN_HEIGHT/6;
	int mynum = num;
	
	if(mynum<10){
		hand1texture.loadFromFile(numtoaddress(mynum));
		hand1texture.render( posx , posy , length, height );
	}
	else{
		int digit = 0;
		while(mynum > 0){
			mynum /= 10;
			digit += 1;
		}
		mynum = num;
		for(int i =digit-1;i>=0;i--){
			hand1texture.loadFromFile(numtoaddress(mynum%10));
			hand1texture.render( posx + i* (length*2/digit) , posy , (length*2/digit), height );
			mynum /= 10;
		}
	}
	SDL_RenderPresent( gRenderer );
}

