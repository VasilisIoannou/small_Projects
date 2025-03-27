#include<SDL3/SDL.h>
#include<cmath>

const float pi = 3.1415;

struct str_screen{
	const int Width = 640;
	const int Height = 480;
}screen;

class object{
private:
	float mass;
	float x,y;
	float velocity,acceleration;
	float phase;
public:
	object(float setMass,float setX,float setY,float setV = 0,float setA = 0,float setPhase = 0){
		mass = setMass;
		x = setX;
		y = setY;
		velocity = setV;
		acceleration = setA;
		phase = setPhase;
	}

	void draw(SDL_Renderer* renderer){
		//Draw a Circle
		const float r = 5;
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		for(int j=0;j<r;j++){
			for(int f=0;f<2*pi;f+=pi/10){
				SDL_RenderPoint(renderer,x + j*cos(f), y + j*sin(f));			
			}
		}
	}
};

int main(){
	
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("SDL3 gravity", screen.Width, screen.Height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr) ;
	
	//Declare objects
	object M1(10,200,200);

	bool running = true;
		
	while(running){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT) { 
				running = false;  
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//Black
		SDL_RenderClear(renderer);
		
		M1.draw(renderer);
		SDL_RenderPresent(renderer);
	
		SDL_Delay(1000);		
	}	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
