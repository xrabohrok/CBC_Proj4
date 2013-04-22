//Westin Breger
// Created on Tue Apr 9 2013
//updated for part 2 on april 20

//1 orange
//2 green

//define map here
int map[11][11] = 
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

const float wheelRad = .033f; //meters
const float axelLength = .153f;
const float PI = 3.1415926f;

const float cellSize = .3048f; //meters

const int ticksPerRev = 1100;

const int SPEED = 300;
const int MIN_OBJECT_SIZE  = 10;
const int MIN_OBSTACLE_SIZE = 30;
const int SCREEN_WIDTH = 159;

const int TICK_SHAVE = 0; //subtracts these ticks from rotation, because 

const float ROT_THRESH = .2f;


#include <stdio.h>

void selectRooms();
void moveTowardFoundObject();
void missingObjects();
void Rotate90();
void moveSquare();
void accumulateMoveData();
void initializeNodes();
void checkForObstacle();


int targetColor = 1;
int obstacleColor = 2;
int currentRoom = 1;
int targetRoom  = 1;
int direction = 1; //1 = forward, -1 = backward
int facing = 0; //where 0 is facing down the hallway(negative) and advances ccw to 3

int color = 0;
int objectFound = 0;


int cellX;
int cellY;
float posX; //meters
float posY; //meters
float rot;  //rad



struct room
{
	int designation;
	int alleys[2][2]; //[node][coord]
	struct room *neighbor[4];
};

struct room allRooms[5];

int main() 
{
	
	int goal = 0; //to 2
	int goalDir = 0;
	int link = 0; //of nodes of chain
	
	
	int triggered = 0;
	
	
	initializeNodes();
	
	
	
	posX = 0;
	posY = 0;
	rot = 1.5f * PI;
	cellX = 5;
	cellY = 9;
	
	set_each_analog_state(1,1,1,0,0,0,0,0);
	selectRooms();
    //main loop
    while(1)    
	{ 
		//DO I SEE ANYTHING
		//look at goal, am I there	
		//yes
		track_update();
		int size = track_size(targetColor,0);
		if (track_count(targetColor) > 0 && size >= MIN_OBJECT_SIZE)
		{
			triggered = 1;
		}
		if(triggered == 0)
		{
			if(branch[goal][link][0] == cellY && branch[goal][link][1] == cellX )
			{
				//advance goal chain
				link += direction;	
				//overlimit	
				if(link >= 5 || link < 0)
				{
					if (link < 0)	
					{	
						//got to beginning, send us on a new adventure
						goal++;	
						if (goal > 2)	
							goal = 0;
					}
					direction *= -1;	
					link += 2 * direction;
				}
				else if(branch[goal][link][0] == -1)	
				{
					direction *= -1;
					link += 2 * direction;
				}
				printf("new Destination: %d, %d\n", branch[goal][link][0], branch[goal][link][1]);
			}
			//am I facing goal?	
			//needs to be...(x resolved first, unless map says no)
			printf("\t\t%d, %d\n", cellY, cellX);
			checkForObstacle();
			goalDir = -1;
			int horiz = 0;
			int vert = 0;
			while (goalDir ==-1)
			{
				if((vert == 1 || cellX > branch[goal][link][1]) && map[cellY][cellX-1] == 0)
				{
					goalDir = 1;
					printf("west\n");
				}	
				else
				{
					if(cellX > branch[goal][link][1])
					{
						horiz = 1;
					}
				}
				
				if((vert == 1 || cellX < branch[goal][link][1]) && map[cellY][cellX+1] == 0)
				{
					goalDir = 3;	
					printf("east\n");	
				}
				else
				{
					if(cellX < branch[goal][link][1])
					{
						horiz = 1;
					}
				}
				
				if((horiz == 1 || cellY > branch[goal][link][0]) && map[cellY-1][cellX] == 0)
				{	
					goalDir = 0;
					printf("north\n");	
				}
				else
				{
					if(cellY > branch[goal][link][0])
					{
						vert = 1;
					}
				}
				
				if((horiz == 1 || cellY < branch[goal][link][0]) && map[cellY+1][cellX] == 0)
				{	
					goalDir = 2;	
					printf("south\n");
				}
				else
				{
					if(cellY < branch[goal][link][0])
					{
						vert = 1;
					}
				}
			}	
			//if not right facing, resolve now
			if(facing != goalDir){
				while(facing != goalDir)
				{	
					printf("%d vs %d", facing, goalDir);
					Rotate90();	
					
					
				}
			}
			else
			moveSquare();
			//move forward	
			        		//sleep(.5f);
		}
		else
		{
			moveTowardFoundObject();
		}
		
	}
	
	return 0;
}


void selectRooms(){
	printf("Press A button to cycle\n");
	printf("Press B button to select\n");
	printf("Starting Room %d\n",currentRoom);
	while(!b_button()){
		if(a_button()){
			currentRoom++;
			if(currentRoom > 4)
				currentRoom = 1;
			printf("Starting Room %d\n",currentRoom);
		}
		
	}
	printf("Press A button to cycle\n");
	printf("Press B button to select\n");
	printf("Target Room %d\n",targetRoom);
	while(!b_button()){
		if(a_button()){
			targetRoom++;
			if(targetRoom > 4)
				targetRoom = 1;
			printf("Target Room %d\n",targetRoom);
		}
		
	}
	printf("Hit A button to start search\n");
	while(!a_button());
	sleep(2.0);
	
	//now that we know our destination and start, we can figure out a path
	
}


//rotates to face cell provided
void Rotate90()
{
	//a 90 degree rotation is 1/4 of a turn of the robot, or the distance of one of the wheels traveling that distance
	float quarterTurn;
	float revs;
	//the number of ticks needed
	int ticksNeeded;
	

	
	quarterTurn = .25f * ( PI * axelLength);
	revs = quarterTurn / (2 * PI * wheelRad);
	ticksNeeded = ticksPerRev * revs;// -TICK_SHAVE;
	
	//printf("needtorotate %d", ticksNeeded);
	
	move_relative_position(0, SPEED, -ticksNeeded);
	move_relative_position(3, SPEED, ticksNeeded);
	
	while((get_motor_done(0)==0))
	{
		sleep(.5);
	}
	facing++;
	if(facing > 3)	
		facing = 0;	
	
	
}

void moveSquare()
{
	float revs;
	int ticksNeeded;
	
	revs = cellSize / (2 * PI * wheelRad);
	ticksNeeded = ticksPerRev * revs;
	
	move_relative_position(0, SPEED, ticksNeeded);
	move_relative_position(3, SPEED, ticksNeeded);
	
	//printf("done? %d", get_motor_done(0));
	while((get_motor_done(0)==0))
	{
		sleep(.5);
	}
	if(facing == 0)	
		cellY--;
	else if (facing == 2)
		cellY++;
	else if(facing == 1)
		cellX--;
	else if (facing == 3)	
		cellX++;
}

void evasiveManuver(int ticks)
{
	static int ticksSoFar;
}

void moveTowardFoundObject(){
	int x;
	int y;
	track_update();
	int size = track_size(targetColor,0);
	int ETsensorData = 0;
	if (track_count(targetColor) > 0 && size >= MIN_OBJECT_SIZE){
			if(!objectFound){
				objectFound = 1 - objectFound;
				beep();
			}
		x = track_x(targetColor,0); 
		y = track_y(targetColor,0);	
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
		missingObjects();
		ao();
		//printf("No Color match in Frame\n");
	}
}

void missingObjects()
{
    if(objectFound){
        objectFound = 1 - objectFound;
        beep();
        sleep(0.2);
        beep();
    }
}


void initializeNodes()
{
	int i = 0;
	
	allRooms[0].designation = 0; //hall
	allRooms[0].alleys[0][0] = 5;
	allRooms[0].alleys[0][1] = 8;
	allRooms[0].alleys[1][0] = 3;
	allRooms[0].alleys[1][0] = 5;
	
	allRooms[1].designation = 1; //northeast
	allRooms[1].alleys[0][0] = 3;
	allRooms[1].alleys[0][1] = 5;
	allRooms[1].alleys[1][0] = -1;
	allRooms[1].alleys[1][1] = -1;
	
	allRooms[2].designation = 2; //southeast
	allRooms[2].alleys[0][0] = 6;
	allRooms[2].alleys[0][1] = 9;
	allRooms[2].alleys[1][0] = -1;
	allRooms[2].alleys[1][1] = -1;
	
	allRooms[3].designation = 3; //northwest
	allRooms[3].alleys[0][0] = 1;
	allRooms[3].alleys[0][1] = 5;
	allRooms[3].alleys[1][0] = -1;
	allRooms[3].alleys[1][1] = -1;
	
	allRooms[4].designation = 4; //southwest
	allRooms[4].alleys[0][0] = 1;
	allRooms[4].alleys[0][1] = 5;
	allRooms[4].alleys[1][0] = 4;
	allRooms[4].alleys[1][1] = 9;
	
	
	//declare neighbors for rooms
	allRooms[1].neighbor[0] = &(allRooms[0]);
	for (i = 1; i < 4; i++)
	{
		allRooms[1].neighbor[i] = 0;
	}
	
	allRooms[2].neighbor[0] = &(allRooms[0]);
	allRooms[2].neighbor[1] = &(allRooms[4]);
	for (i = 2; i < 4; i++)
	{
		allRooms[2].neighbor[i] = 0;
	}
	
	allRooms[3].neighbor[0] = &(allRooms[4]);
	for(i = 1; i < 4; i++)
	{
		allRooms[3].neighbor[i] = 0;
	}
	
	allRooms[4].neighbor[0] = &(allRooms[3]);
	allRooms[4].neighbor[1] = &(allRooms[0]);
	allRooms[4].neighbor[2] = &(allRooms[2]);
	allRooms[4].neighbor[3] = 0;
}

void checkForObstacle(){
	track_update();
	int size[10];
	int i;
	int ETsensorData = 0;
	int x;
	for(i = 0; i < 10; i++)
		size[i] = track_size(obstacleColor,i);
	for( i = 0; i < 10; i++){
		if (track_count(obstacleColor) > i && size[i] >= MIN_OBSTACLE_SIZE)
		{
			x = track_x(obstacleColor,i); 
			if(x >= (SCREEN_WIDTH/2) - SCREEN_WIDTH/3 && x <= (SCREEN_WIDTH/2) + SCREEN_WIDTH/3){
				ETsensorData = analog10(2);
			//	printf("Biggest blob is center\n %d",ETsensorData);
				//if(ETsensorData < 550 && ETsensorData > 300 ){
					switch(facing){
						case 0:
							if(cellY > 0){
							map[cellY-1][cellX] = 1;
								printf("Obstacle Detected y:%d x:%d\n",cellY-1,cellX);
							}
							break;
						case 1:
							if(cellX > 0){
							map[cellY][cellX-1] = 1;
								printf("Obstacle Detected y:%d x:%d\n",cellY,cellX-1);
							}
							break;
						case 2:
							if(cellY < 10){
							map[cellY+1][cellX] = 1;
								printf("Obstacle Detected y:%d x:%d\n",cellY+1,cellX);
							}
							break;
						case 3:
							if(cellX < 10){
							map[cellY][cellX+1] = 1;
								printf("Obstacle Detected y:%d x:%d\n",cellY,cellX+1);
							}
							break;
					}
					
				//}
			}
			else{
				//printf("Not in middle\n");
			}
		}
		else{
			//printf("Too Small\n");
			break;
		}
		
	}
}

