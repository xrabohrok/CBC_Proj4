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

const float wheelRad = .03f; //meters
const float axelLength = .15;

const float cellSize = .3f; //meters

const int ticksPerRev = 1100;

#include <stdio.h>


float posX; //meters
float posY; //meters

int main() 
{
	posX = 0;
	posY = 0;
	
	printf("Hello, World!\n");
	return 0;
}

//determines location and cell from that info
void accumulateMoveData()
{
	//lord help me if this math is wrong
	int ticksR; 
	int ticksL;
	float distL;
	float distR;
	float theta;
	float xDiff
	float r;
	float xDisp;
	float xR;
	
	ticksR = get_motor_position_counter(1);
	ticksL = get_motor_position_counter(0);
	
	distL = ticksR/ticksPerRev * wheelRad;
	distR = ticksL/ticksPerRev * wheelRad;
	
	theta = (distL - distR)/axelLength;
	
	r = distR/theta;
	
	xdisp = (posX - r - axelLength/2);
	xR = posX - xdisp;
	posX = xR * cos(theta)  + xdisp; //- 0 * sin(theta);
	posY = xR * sin(theta) + posY; //- 0 * sin(theta);
	
	
	
	
	//clear_motor_position_counter(motor)
	//get_motor_position_counter(motor)
}

//rotates to face cell provided
void rotateTo(int x, int y)
{
}

void evasiveManuver(int ticks)
{
	static int ticksSoFar;
}

