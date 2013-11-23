#include <iostream>

void logError(std::ostream &os, const std::string &msg);

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);

void tileImage(SDL_Texture* tex, SDL_Renderer* ren);
bool setup(SDL_Window *&win, SDL_Renderer *&ren);
bool initialize();
