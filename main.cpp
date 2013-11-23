#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "main.h"

const int IMG_INIT_FLAGS = IMG_INIT_PNG;
const int SCRN_WIDTH = 512;
const int SCRN_HEIGHT = 512;

const int FRAME_RATE = 30;
const int TICK_INTERVAL = 1000/FRAME_RATE;
const int Magn = 5;

SDL_Texture* loadTexture(SDL_Renderer* ren, const std::string &file);

SDL_Texture* loadTexture(SDL_Renderer* ren, const std::string &file){
	return IMG_LoadTexture(ren, file.c_str());	
}

int main(int argc, char** argv){
	
	//loads our modules/subsystems
	if(!initialize()){
		logError(std::cout, "Main - Initialize");	
		return 1;
	}

	SDL_Window* win;			//Window
	SDL_Renderer* ren;		//Renderer

	//Creates our window, renderer and textures
	//We're passing pointers by pointers
	if(!setup(win, ren)){
		logError(std::cout, "Main - Setup");
		return 2;
	}
	
	//SDL_Texture* background = loadTexture(ren, "Images/black.png");	//Textures
	SDL_Texture* ball			= loadTexture(ren, "Images/ball.png");
	if(/*background == nullptr || */ball == nullptr){
		logError(std::cout, "Main - Texture loading");
		return 3;
	}

	//Our starting image (constructor of sorts)
	int x = 256;
	int y = 256;
	//tileImage(background, ren);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); //set the default render color (our background)
	SDL_RenderClear(ren);	//clear the background 'canvas' to the renderer color (not visible on screen!)
	renderTexture(ball, ren, x, y); //render our ball onto the canvas
	SDL_RenderPresent(ren);	//'print' our canvas to the screen

	//setup controls to move ball around with keys
	SDL_Event e;
	bool quit = false;
	bool update = false;

	int moveX = 5;
	int moveY = 0;

	const Uint8 *keyState = SDL_GetKeyboardState(NULL);

	Uint32 nextTick = 0;
	Uint32 tickDelay = 0;
	Uint32 curTick  = SDL_GetTicks();

	while(!quit){
		/*if(x < 100 )
			moveX = 5;
		if(x > 500 )
			moveX = -5;	
		*/
		while(SDL_PollEvent(&e)){ 		//PollEvent returns 0 if no events are available

			if(keyState[SDL_GetScancodeFromKey(SDLK_ESCAPE)]){ 	//using a state allows to detect multiple keypresses
				quit = true;													//When a key is pressed, we mark a state as being depressed
			}																		//It exits that state when it is unpressed.
																					//Doing this allows us to free up the keyboard to look for 
			if(keyState[SDL_GetScancodeFromKey(SDLK_DOWN)]){		//other keypresses
				moveY = Magn;
				update = true;
			}
		
			else if(keyState[SDL_GetScancodeFromKey(SDLK_UP)]){
				moveY = -Magn;
				update = true;
			}
			else { moveY = 0; }
	
			if(keyState[SDL_GetScancodeFromKey(SDLK_RIGHT)]){	
				moveX = Magn;
				update = true;
			}

			else if(keyState[SDL_GetScancodeFromKey(SDLK_LEFT)]){
				moveX = -Magn;
				update = true;
			}
			else { moveX = 0; }

			if(update){ //only update image if we made a change.
				x+=moveX; y+=moveY; //x, y were defined up above when we first rendered
				SDL_RenderClear(ren);
				renderTexture(ball, ren, x, y); 
				SDL_RenderPresent(ren);
				update = false;
				curTick = SDL_GetTicks();

				if(nextTick <= curTick){
					nextTick = curTick + TICK_INTERVAL;
					tickDelay = 0;
				}
				else{
					tickDelay = nextTick - curTick;
				}
		SDL_Delay(tickDelay);
			}
		}
	}
	
	//repaint foreground

	SDL_DestroyWindow(win);
	//SDL_DestroyTexture(background);
	SDL_DestroyTexture(ball);
	SDL_DestroyRenderer(ren);

	IMG_Quit();
	SDL_Quit();
	return 0;
}

void tileImage(SDL_Texture* tex, SDL_Renderer* ren){
	int width, height;
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);	

	for(int y=0; y<SCRN_HEIGHT; y+=height){
		for(int x=0; x<SCRN_WIDTH; x+=width){
			renderTexture(tex, ren, x, y);
		}
	}
}

/*
	Preserves texture scaling
	i.e. No stretching
*/
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}
/*
	Render texture with desired scaling
*/
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h){
	SDL_Rect dest;	
	dest.x = x;	dest.w = w;
	dest.y = y;	dest.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dest);
}

//Output Error message to desired output stream
void logError(std::ostream &os,  const std::string &msg){
	os << msg << " Error: " << SDL_GetError() << std::endl;
}

//Creates a window and renderer. We pass a pointer BY reference.
//Another option is passing a pointer BY pointer. If you just pass
//a pointer (*win, *ren) then you're passing the object it points to.
//By passing a pointer by pointer, you modify the pointer instead of what it points to.
//Tl;Dr - We want to adjust what the pointer points to, so we pass in a pointer by Reference/pointer
bool setup(SDL_Window*& win, SDL_Renderer*& ren){
	bool successful = true;
	win = SDL_CreateWindow("Moving Ball", 0, 0, SCRN_WIDTH, SCRN_HEIGHT, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/);
	if(win == nullptr){
		logError(std::cout, "setup - SDL_Window");
		successful = false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr){
		logError(std::cout, "setup - SDL_Renderer");
		successful = false;
	}

	return successful;
}

//Initialize whatever modules you want
bool initialize(){
	bool successful = true;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logError(std::cout, "Initialize - SDL_Init");
		successful = false;
	}

	//IMG_Init returns bitmask of all loaded modules
	int init_result = IMG_Init(IMG_INIT_FLAGS);
	if( (init_result&IMG_INIT_FLAGS) != IMG_INIT_FLAGS){
		logError(std::cout, "Initialize - IMG_Init");
		/*
			result ANDed with flags should == flags
			unless a flag we ask for wasn't initialzed
			Side note: We can't just ask if the 
			result == IMG_INIT_FLAGS because result
			tells us all modules that are loaded
			Meaning, more could be loaded than what
			we just tried to initialized.
			ANDing them together says
			"Are the modules we asked to load, loaded?"
		*/
		successful = false;
	}

	return successful;
}
