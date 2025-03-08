#include<iostream>
#include<SDL3/SDL.h>
#include<cmath>
#include<vector>

const float pi = 3.142;

struct str_screen{
	const int Height = 480;
	const int Width = 640;

	const int fieldH = 48;
	const int fieldW = 64;
}screen;

class cl_vector{
private:
	float x0,y0,x1,y1;
	float magnitude;
	float phase;

	void calculateEndPoints(){
		x1 = magnitude * cos(phase) + x0;
		y1 = magnitude * sin(phase) + y0;
	}
public:
	cl_vector(float setX,float setY,float setM,float setPh){
		x0 = setX;
		y0 = setY;
		magnitude = setM;
		phase = setPh;

		x1 = setM * cos(setPh) + setX;
		y1 = setM * sin(setPh) + setY;
	}
	
	void update(){
		calculateEndPoints();
	}

	float getX0(){
		return x0;
	}
	float getY0(){
		return y0;
	}
	float getX1(){
		return x1;
	}
	float getY1(){
		return y1;
	}
	float getPhase(){
		return phase;
	}

	void setPhase(float newPhase){
		phase = newPhase;
		while(phase > 2* pi){
			phase -= 2*pi;
		}
		while(phase < -2*pi){
			phase += 2*pi;
		}
	}
};

class cl_field{
private:
	cl_vector* field[64][48];
	
	void initialiseVectors(){
		for(int w = 0;w<screen.Width;w++){
			for(int h = 0;h<screen.Height; h++){
				cl_vector initialVector(w*10,h*10,100,0);
				field[w][h] = &initialVector;
			}
		}
	}
public:
	cl_field(){
		initialiseVectors();
	}
	void draw(SDL_Renderer* renderer){
		//Draw a vector
		for(int w=0;w<screen.Width;w++){
			for(int h=0;h<screen.Height;h++){
				cl_vector* currentVector = field[w][h];

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				SDL_RenderLine(renderer,currentVector->getX0(),currentVector->getY0(),currentVector->getX1(),currentVector->getY1());
			}
		}
	}
}field;

int main(){
	
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("SDL3 vector", screen.Width, screen.Height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr) ;

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
		
		SDL_RenderPresent(renderer);

		field.draw(renderer);

		SDL_Delay(100);
		
		

	}	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
