#include<iostream>
#include<SDL3/SDL.h>
#include<cmath>
#include<vector>

const float pi = 3.1415926535;
const float electronCharge = 1.60217663 * pow(10,-19);
const float CoulombsConst = 8.99 * pow(10,9);

struct str_screen{
	const int Height = 480;
	const int Width = 640;

	const int fieldH = 48;
	const int fieldW = 64;
}screen;

class cl_charge{
private:
	float chargeMagnitude;
	bool sign;
	float x,y;
public:
	cl_charge(float setCM,bool setS,float setX,float setY){
		chargeMagnitude = setCM;
		sign = setS;
		x = setX;
		y = setY;
	}
	float getChargeMagnitude() const { return chargeMagnitude; }
	bool getSign() const { return sign; }
	float getX() const { return x; }
	float getY() const { return y; }

	void draw(SDL_Renderer* renderer){
		const int r = 5;

		SDL_SetRenderDrawColor(renderer,0,0,255,255);
		for(float f=0;f<2*pi;f+=pi/10){
			SDL_RenderPoint(renderer,x+r*sin(f),y+r*cos(f));
		}
	}
}NegCharge(pow(10,-6),false,screen.Width/2,screen.Height/2);

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
	
	void update(float chargeX, float chargeY, float chargeMagnitude, bool chargeSign){
		float r = sqrt((x0 - chargeX) * (x0 - chargeX) + (y0 - chargeY)*(y0 - chargeY));
		if(r != 0){
			magnitude += (CoulombsConst * chargeMagnitude) / (r * r);
		} else magnitude = 0;
		if(x0 - chargeX !=0){
			phase = atan( (y0 - chargeY) / (x0 - chargeX) );
		} else if( chargeX > x0 ) {
			phase = pi/2;
		} else phase = -pi/2;
		if(!chargeSign){ //The electric field has a negative sign
			phase =  phase + pi;
		}

		calculateEndPoints();
	}

	float getX0() const{
		return x0;
	}
	float getY0() const{
		return y0;
	}
	float getX1() const{
		return x1;
	}
	float getY1() const{
		return y1;
	}
	float getPhase() const{
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
		calculateEndPoints();
	}
};

class cl_field{
private:
	std::vector<std::vector<cl_vector>> field;
        
	std::vector<cl_charge*> chargeList;

	void initialiseVectors(){
		field.resize(screen.fieldW, std::vector<cl_vector>(screen.fieldH,cl_vector(0,0,0,0)));
		for(int w = 0;w<screen.fieldW;w++){
			for(int h = 0;h<screen.fieldH; h++){
				field[w][h] = cl_vector(w*10,h*10,0,0);
			}
		}
	}
public:
	cl_field(){
		initialiseVectors();
	}
	void update(){
		for(auto currentChargeIt = chargeList.begin(); currentChargeIt != chargeList.end(); ++currentChargeIt){
			cl_charge* currentCharge = *currentChargeIt;
			float currentChargeMagnitude = currentCharge->getChargeMagnitude();
			float currentChargeX = currentCharge->getX();
			float currentChargeY = currentCharge->getY();
			bool currentChargeSign = currentCharge->getSign();
			for(int w=0;w<screen.fieldW;w++){
				for(int h=0;h<screen.fieldH;h++){
					field[w][h].update(currentChargeX,currentChargeY,currentChargeMagnitude,currentChargeSign);	
				}
			}
		}


	}
	
	void setCharge(cl_charge* newCharge){
		chargeList.push_back(newCharge);
	}

	void draw(SDL_Renderer* renderer){
		//Draw a vector
		for(int w=0;w<screen.fieldW;w++){
			for(int h=0;h<screen.fieldH;h++){
				cl_vector& currentVector = field[w][h];

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				SDL_RenderLine(renderer,currentVector.getX0(),currentVector.getY0(),currentVector.getX1(),currentVector.getY1());
			}
		}
	}
}field;


int main(){
	
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("SDL3 vector", screen.Width, screen.Height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr) ;

	field.setCharge(&NegCharge);
	field.update();

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
		
		field.draw(renderer);
		NegCharge.draw(renderer);
		SDL_RenderPresent(renderer);
		
		SDL_Delay(1000);
		
		

	}	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
