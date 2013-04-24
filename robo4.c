//Westin Breger
// Created on Tue Apr 9 2013
//updated for part 2 on april 20

//1 orange
//2 green

#include <math.h>

//define map here..
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
//branch-node-coords.
 
 //x,y
int dests[5][2] = {
	{-1,-1},
	{8,2},
	{8,8},
	{2,2},
	{2,8}};
 


const float WHEEL_RAD = .033f; //meters
const float AXEL_LENGTH = .153f;
const float PI = 3.1415926f;

const float CELL_SIZE = .3048f; //meters

const int TICKS_PER_REV = 1100;

const int SPEED = 300;
const int MIN_OBJECT_SIZE  = 10;
const int MIN_OBSTACLE_SIZE = 30;
const int SCREEN_WIDTH = 159;
const int SCREEN_HEIGHT= 119;

const int TICK_SHAVE = 0; //subtracts these ticks from rotation, because 

const float ROT_THRESH = .2f;


#include <stdio.h>

void selectRooms();
void moveTowardFoundObject();
void missingObjects();
void Rotate90(int direction);
void moveSquare();
void accumulateMoveData();
void checkForObstacle();
void moveTowardTargetByGrid();


void setPath();
void pushStack();
void popStack();
void pushClosed();
void popClosed();



int targetColor = 1;
int obstacleColor = 2;
int currentRoom = 1;
int targetRoom  = 1;
int facing = 0; //where 0 is facing down the hallway(negative) and advances ccw to 3

int pathSize = 0;
int stackHead = 0;
int closedHead = 0;
int stack[100][2];
int closed[80][2];
int path[30][2];

int color = 0;
int objectFound = 0;


int cellX;
int cellY;
int targetX;
int targetY;
float posX; //meters
float posY; //meters
float rot;  //rad



struct cell{
	int x;
	int y;
	int cost;
	int currCost;
	int prevX;
	int prevY;
	
};



int main() 
{
	
	int goal = 0; //to 2
	int goalDir = 0;
	int link = 0; //of nodes of chain
	
	
	int triggered = 0;
	
	
	
	
	
	posX = 0;
	posY = 0;
	rot = 1.5f * PI;
	
	set_each_analog_state(1,1,1,0,0,0,0,0);
	selectRooms();
	setPath();
	link = pathSize -1;
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
			printf("Testing %d, %d, %d, %d\n",path[link][1],cellY,path[link][0],cellX);
			if(path[link][1] == cellY && path[link][0] == cellX )
			{
				//advance goal chain
				link--;	
				//overlimit	

				printf("\nnew Destination: %d, %d\n", path[link][0], path[link][1]);
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
				if((vert == 1 || cellX > path[link][0]) && map[cellY][cellX-1] == 0)
				{
					goalDir = 1;
					printf("west\n");
				}	
				else
				{
					if(cellX > path[link][0])
					{
						horiz = 1;
					}
				}
				
				if((vert == 1 || cellX < path[link][0]) && map[cellY][cellX+1] == 0)
				{
					goalDir = 3;	
					printf("east\n");	
				}
				else
				{
					if(cellX < path[link][0])
					{
						horiz = 1;
					}
				}
				
				if((horiz == 1 || cellY > path[link][1]) && map[cellY-1][cellX] == 0)
				{	
					goalDir = 0;
					printf("north\n");	
				}
				else
				{
					if(cellY > path[link][1])
					{
						vert = 1;
					}
				}
				
				if((horiz == 1 || cellY < path[link][1]) && map[cellY+1][cellX] == 0)
				{	
					goalDir = 2;	
					printf("south\n");
				}
				else
				{
					if(cellY < path[link][1])
					{
						vert = 1;
					}
				}
			}	
			//if not right facing, resolve now
			if(facing != goalDir){				
					printf("%d vs %d", facing, goalDir);
					if((facing + 1)%4 == goalDir%4)
					Rotate90(0);
					else if((facing - 1 + 4)%4 == goalDir%4)
					Rotate90(1);
					else
					Rotate90(0);	
			}
			else
			moveSquare();
			//move forward	
			        		//sleep(.5f);
		}
		else
		{
			moveTowardTargetByGrid();
		}
		
	}
	
	return 0;
}


void selectRooms(){
	int randomBool = 0;
	printf("Press A button to cycle\n");
	printf("Press B button to select\n");
	printf("Starting Room %d\n",currentRoom);
	while(!b_button()){
		if(a_button()){
			randomBool = 1;
		}
		else if(randomBool == 1){
			randomBool = 0;
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
			randomBool = 1;
		}
		else if(randomBool ==1)
		{
			randomBool  =0;
			targetRoom++;
			if(targetRoom > 4)
				targetRoom = 1;
			printf("Target Room %d\n",targetRoom);
		}
		
	}
	printf("Hit A button to start search\n");
	while(!a_button());
	sleep(2.0);
	
	cellX = dests[currentRoom][0];
	cellY = dests[currentRoom][1];
	
	printf("selected %d start to %d end\n", currentRoom, targetRoom);
	//now that we know our destination and start, we can figure out a path
	
}


//rotates to face cell provided
//0 for left 1 for right
void Rotate90(int direction)
{
	//a 90 degree rotation is 1/4 of a turn of the robot, or the distance of one of the wheels traveling that distance
	float quarterTurn;
	float revs;
	//the number of ticks needed
	int ticksNeeded;
	

	
	quarterTurn = .25f * ( PI * AXEL_LENGTH);
	revs = quarterTurn / (2 * PI * WHEEL_RAD);
	ticksNeeded = TICKS_PER_REV * revs;// -TICK_SHAVE;
	
	//printf("needtorotate %d", ticksNeeded);
	if( direction == 0)
	{
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
	else
	{
		move_relative_position(0, SPEED, ticksNeeded);
		move_relative_position(3, SPEED, -ticksNeeded);
		
		while((get_motor_done(0)==0))
		{
			sleep(.5);
		}
		facing--;
		if(facing < 0)	
			facing = 3;	
	}
		
}

void moveSquare()
{
	float revs;
	int ticksNeeded;
	
	revs = CELL_SIZE / (2 * PI * WHEEL_RAD);
	ticksNeeded = TICKS_PER_REV * revs;
	
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
			//go until reasonably in the middle
			while(x<= (SCREEN_WIDTH/2) - SCREEN_WIDTH/24){
				mav(0,-SPEED/3);
				mav(3,SPEED/3);
				track_update();
					x = track_x(targetColor,0); 
			}
			ao();
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
				mav(0,-SPEED/3);
				mav(3,-SPEED/3);
			}
		}
		else{
		//	printf("Biggest blob is right\n");
			while(x>= (SCREEN_WIDTH/2) + SCREEN_WIDTH/24){
				mav(0,SPEED/3);
				mav(3,-SPEED/3);
				track_update();
				x = track_x(targetColor,0); 
				}
				ao();
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

void setPath()
{
	//currentRoom
	//targetRoom
	

	//initizlize A* stuff
	struct cell map2[11][11];
	int count = 0;
	int i = 0;
	int j = 0;
	int currentX = 0;
	int currentY = 0;
	int lowCost = 1000000;
	int focus = 0;
	int found = 0;
	int salePrice = 0;
	int directions[4][2] = {
		{0,1},
		{0,-1},
		{1,0},
		{-1, 0}};
	
	
	int destX = 0;
	int destY = 0;
	
	
	for(i = 0; i < 11; i++)
	{
		for(j = 0; j < 11; j++)
		{
			map2[i][j].x = i;
			map2[i][j].y = j;
			map2[i][j].currCost = -1;
			map2[i][j].prevX = -1;
			map2[i][j].prevY = -1;
			
			if (map[i][j] > 0 )
			{
				map2[i][j].cost = 10000000;
			}
			else
			{
				map2[i][j].cost = 1;
			}
		}
	}	

	
	for(i = 0; i < 80; i++)
	{
		closed[i][0] = -1;
		closed[i][1] = -1;
	}
	
	for(i = 0; i < 100; i++)
	{
		stack[i][0] = -1;
		stack[i][1] = -1;
	}
	stackHead = 0;
	
	for(i = 0; i < 30; i++)
	{
		path[i][0] = -1;
		path[i][1] = -1;
	}
	pathSize = 0;
	
	currentX = cellX;
	currentY = cellY;
	
	destX = dests[targetRoom][0];
	destY = dests[targetRoom][1];
	
	//end initialization
	pushStack(currentX, currentY);
	
	map2[currentY][currentX].currCost = 1;
	
	printf("from %d, %d to %d, %d\n", currentX, currentY, destX, destY);
	//begin A*.
	while(currentX != destX || currentY != destY)
	{
		printf("\t proc %d, %d\n", currentX, currentY);
		//find cheapest node
		j = 0;
		while (stack[j][0] == -1 && j < stackHead)
			j++;
		lowCost = map2[stack[j][1]][stack[j][0]].currCost;
		focus = j;
		printf("j+1: %d, stackHead: %d\n",j+1,stackHead);
		for(i = 0; i < stackHead; i++)
		{
			if( map2[stack[i][1]][stack[i][0]].currCost < lowCost && stack[i][0] != -1)
			{
				printf("lowcost: %d, currCost: %d\n", lowCost, map2[stack[i][1]][stack[i][0]].currCost); 
				focus = i;
				lowCost = map2[stack[i][1]][stack[i][0]].currCost;
			}
		}
		
		currentX = stack[focus][0];
		currentY = stack[focus][1];
		
		//remove the node we are at from consideration
		popStack(currentX, currentY);
		pushClosed(currentX, currentY);
		
		//add all unadded neighbors to the set
		//left
		
		for(j = 0; j < 4; j++)
		{
			found = 0;
			//bounds check
			if(currentX + directions[j][0] >= 0 && currentX + directions[j][0] < 11 && currentY + directions[j][1] < 11 && currentY + directions[j][1] >= 0)
			{
				//cost calc
				salePrice = map2[currentY ][currentX ].currCost + map2[currentY + directions[j][1]][currentX + directions[j][0]].cost + abs(destX - (currentX + directions[j][0])) + abs(destY - (currentY + directions[j][1]));
				//is it in closed?
				
				printf(" cost: %d\n", salePrice);
				for(i = 0; i < closedHead; i++)
				{
					if((closed[i][0] == currentX + directions[j][0]) && (currentY + directions[j][1] == closed[i][1]))
						found = 1;
				}
				
				//is it open?
				if(found == 0)
				{
					for(i = 0; i < stackHead; i++)
					{
						if((stack[i][0] == currentX + directions[j][0]) && (currentY + directions[j][0] == stack[i][1]))
						{
							found = 1;
							//only update if it is an improvement
							if(map2[stack[i][1]][stack[i][0]].currCost > salePrice || map2[stack[i][1]][stack[i][0]].currCost == -1)
							{
								map2[currentY + directions[j][1]][currentX + directions[j][0]].currCost = salePrice;
								map2[currentY + directions[j][1]][currentX + directions[j][0]].prevX = currentX;
								map2[currentY + directions[j][1]][currentX + directions[j][0]].prevY = currentY;
							}
						}
					}
				}
				
				//if it is neither, add it to open
				if(found == 0)
				{
					map2[currentY + directions[j][1]][currentX + directions[j][0]].currCost = salePrice;
					map2[currentY + directions[j][1]][currentX + directions[j][0]].prevX = currentX;
					map2[currentY + directions[j][1]][currentX + directions[j][0]].prevY = currentY;
					if(map[currentY + directions[j][1]][currentX + directions[j][0]] == 0){
						pushStack(currentX + directions[j][0], currentY + directions[j][1]);
						printf("pushed\n");
					}
				}
			}
		}
		count++;
			
	}//end A* processing
	printf("A* done\n");
	
	//build path
	currentX = destX;
	currentY = destY;
	pathSize = 0;
	while (currentX != cellX || currentY != cellY && pathSize < 30)
	{
		
		printf("p(%d , %d)\n", currentX, currentY);
		path[pathSize][0] = currentX;
		path[pathSize][1] = currentY;
		pathSize++;
		
		i = currentX;
		j = currentY;
		
		currentX = map2[j][i].prevX;
		currentY = map2[j][i].prevY;
	}

	
	//...path should now be up to date
		
	
	
	

	
}

//put coords in first availble
void pushStack(int x, int y)
{
	int i = 0;
	for(i = 0; i < 100; i++)
	{
		if(stack[i][0] == -1 )
		{
			if( i >= stackHead)
				stackHead ++;
			stack[i][0] = x;
			stack[i][1] = y;
			break;
		}
	}
}

void popStack(int x, int y)
{
	int i = 0;
	for(i = 0; i < 100; i++)
	{
		if(stack[i][0] == x && stack[i][1] == y)
		{
			//stackHead--;
			stack[i][0] = -1;
			stack[i][1] = -1;
			break;
		}
	}
}

void pushClosed(int x, int y)
{
	int i = 0;
	for(i = 0; i < 80; i++)
	{
		if(closed[i][0] == -1)
		{
			if (i >= closedHead)
				closedHead ++;
			closed[i][0] = x;
			closed[i][1] = y;
			break;
		}
	}
}

void popClosed(int x, int y)
{
	int i = 0;
	for(i = 0; i < 80; i++)
	{
		if(closed[i][0] == x && closed[i][1] == y)
		{
			//closedHead--;
			closed[i][0] = -1;
			closed[i][1] = -1;
			break;
		}
	}
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
				if(ETsensorData < 550 && ETsensorData > 300 ){
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
					ao();
					setPath();
				}
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

void moveTowardTargetByGrid(){
	int x;
	int y;
	
	int size;
	//0 left, 1 center, 2 right;
	int turn = 1;
	while(1){
	track_update();
	size = track_size(targetColor,0);
	if (track_count(targetColor) > 0 && size >= MIN_OBJECT_SIZE){
			if(!objectFound){
				objectFound = 1 - objectFound;
				beep();
			}
		printf("Starting Loop\n");
		x = track_x(targetColor,0); 
		y = track_y(targetColor,0);	
		printf("x: %d, y: %d ",x,y);

		if(y >= SCREEN_HEIGHT /2){
			printf("Done\n");
			switch(facing){
				case 0:							
					targetY = cellY-1;
					targetX = cellX;				
					break;
				case 1:
					targetY = cellY;
					targetX = cellX-1;
					break;
				case 2:
					targetY = cellY+1;
					targetX = cellX;
					break;
				case 3:
					targetY = cellY;
					targetX = cellX+1;
					break;
			}
			ao();
			printf("Robot Position (world): %d, %d\n",cellX,cellY);
			printf("Target Position (robot): 1, 0\n");
			printf("Target Position (world): %d, %d\n",targetX,targetY);
			return;
		}
		else
			
		if(x <= (SCREEN_WIDTH/2) - SCREEN_WIDTH/6){
				turn = 0;
					printf("Left\n");
		}
		else if(x <= (SCREEN_WIDTH/2) + SCREEN_WIDTH/6){
			turn = 1;
			printf("Center\n");
		}
		else{
			turn = 2;
			printf("Right\n");
		}
		moveSquare();
		if( turn == 0){
			Rotate90(0);
			track_update();
			 size = track_size(targetColor,0);
			if (track_count(targetColor) > 0 && size >= MIN_OBJECT_SIZE)
				continue;
			else{
				moveSquare();
				Rotate90(1);
			}
		}
		else if( turn == 2){
			Rotate90(1);
			track_update();
			 size = track_size(targetColor,0);
			if (track_count(targetColor) > 0 && size >= MIN_OBJECT_SIZE)
				continue;
			else{
				moveSquare();
				Rotate90(0);
			}
			
		}
		printf("Ending Loop\n");
		
	}
	}
}
