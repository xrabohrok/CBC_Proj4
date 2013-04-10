//Westin Breger
// Created on Tue Apr 9 2013

//define map here
const int map[22][22] = 
{{2,2,2,2,2,2,2,2,2,2,2}, //0
 {2,0,0,0,2,0,0,0,0,0,2}, //1
 {2,0,0,0,2,0,0,0,0,0,2}, //2
 {2,0,0,0,2,0,0,0,0,0,2}, //3
 {2,0,0,0,2,0,2,0,0,0,2}, //4
 {2,0,2,2,2,0,2,2,2,2,2}, //5
 {2,0,0,0,2,0,2,0,0,0,2}, //6
 {2,0,0,0,2,0,2,0,0,0,2}, //7
 {2,0,0,0,2,0,2,0,0,0,2}, //8
 {2,0,0,0,0,0,0,0,0,0,2}, //9
 {2,2,2,2,2,2,2,2,2,2,2}}; //10
//0,1,2,3,4,5,6,7,8,9,10
//define goal nodes here
//branch-node-coords
const int branch[3][5][2] = 
{
{{9,5},{9,8},{7,8},{-1,-1},{-1,-1}},
{{9,5},{5,5},{2,8},{2,8},{3,8}},
{{9,2},{7,1},{3,1},{3,2},{-1,-1}}
};
const int SPEED = 300;
const int MIN_OBJECT_SIZE  = 10;
const int SCREEN_WIDTH = 159;
#include <stdio.h>
void selectColor();
void moveTowardFoundObject();
int color = 0;
int objectFound = 0;

int main() 
{
	set_each_analog_state(1,1,1,0,0,0,0,0);
	selectColor();
	
	return 0;
}
void selectColor(){
	printf("Orange\n");
	while(!b_button()){
		if(a_button()){
			color++;
			if(color >= 4)
				color = 0;
			switch(color){
				case 0:
				 printf("Orange\n");
				break;
				case 1:
				 printf("Pink\n");	
				break;
				case 2:
			      printf("Green\n");		 
				break;
				case 3:
				  printf("Blue\n");	
				break;
			}
		}
		
	}
	printf("Hit A button to start search\n");
	while(!a_button());
	sleep(2.0);
	
}
//determines location and cell from that info
void accumulateMoveData(int ticks)
{
}

//rotates to face cell provided
void rotateTo(int x, int y)
{
}

void evasiveManuver(int ticks)
{
	static int ticksSoFar;
}

void moveTowardFoundObject(){
	int x;
	int y;
	track_update();
	int size = track_size(color,0);
	int ETsensorData = 0;
	if (track_count(color) > 0 && size >= MIN_OBJECT_SIZE){
			if(!objectFound){
				objectFound = 1 - objectFound;
				beep();
			}
		x = track_x(color,0); 
		y = track_y(color,0);	
		if(x <= (SCREEN_WIDTH/2) - SCREEN_WIDTH/12){
				//printf("Biggest blob is left\n");
				mav(0,-SPEED/3);
				mav(3,SPEED/3);
		}
		else if(x <= (SCREEN_WIDTH/2) + SCREEN_WIDTH/12){
			ETsensorData = analog10(2);
		//	printf("Biggest blob is center\n %d",ETsensorData);
			if(ETsensorData < 340 ){
				if(ETsensorData > 300)
					ao();
				else{
					mav(0,SPEED);
					mav(3,SPEED);
				}
			}
			else{
				mav(0,-SPEED);
				mav(3,-SPEED);
			}
		}
		else{
		//	printf("Biggest blob is right\n");
			mav(0,SPEED/3);
			mav(3,-SPEED/3);
		}
	}
	else{
		if(objectFound){
			objectFound = 1 - objectFound;
			beep();
			sleep(0.2);
			beep();
		}
		ao();
		//printf("No Color match in Frame\n");
	}
}
