//Name:Mutharib Ayub
//Roll Number: 20i-0476
//PF Semester Project




#define CENTIPEDE_CPP_
//#include "Board.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;




/*-----------------------------------------------------------------------------------------------------------------------------
---------------------------Global Variables have only been used where absolutely necessary-------------------------------------
------------------------------------------------------------------------------------------------------------------------------*/



/*--Variables for Making grids to make displaying things easier-----------------------------*/
int gridcoord1[69][57][2]; 					//gridcoord for screen2//
int ludocoord[2]={30,30};					//stores distance of board from bottom left corner//
int ludogridcoord[15][15][2];					//stores coordinates of boxes in ludo board//
float mid[2]={ludocoord[0]+(675/2),ludocoord[1]+(675/2)};	//stores coords of midpoint of ludoboard//
int gboxcoord[2]={405+ludocoord[0],ludocoord[1]};		//stores coords of big greenbox in ludoboard//
int yboxcoord[2]={405+ludocoord[0],405+ludocoord[1]};	//stores coords of big yellowbox in ludoboard//
int bboxcoord[2]={ludocoord[0],405+ludocoord[1]};		//stores coords of big bluebox in ludoboard//
int rboxcoord[2]={ludocoord[0],ludocoord[1]};			//stores coords of big redbox in ludoboard//
/*------------------------------------------------------------------------------------------*/





/*--Variables for screen 1----------------------------------------------------------------------------------------------*/
int playercount=4;							//Number of Players//
int screen=1;								//Screen 1 is menu, screen 2 is game, screen 3 is leaderboard//
string playername[4]={"        ","        ","        ","        "}; 	//for each player//
int scr1data[4][3];							//{{playernumber},{name_edit_state,colourselect,namelength}//
bool teams;
/*-----------------------------------------------------------------------------------------------------------------------*/





/*--Variables for screen 2-----------------------------------------------------------------------------------*/
int scr2data[4][6][4],			//[4colors][4goti+highscore+{killstatus,movepossible status}][color relative steps of goti,xcoord,ycoord,direction]//
turn=0,				//tells player turn according to player number//
diceroll[3],				//dice roll number save//
block[4][4];				//{{playernumber},{block1 placement in terms of gsteps,block2 placement in terms of gsteps}}//
string event1="Game has not started yet.",event2="Throw dice to decide",event3="who gets first turn.",event4="press space to roll.",event5="press space to roll."; //global variable for storing events//
int turnscore[4];	//the player with highest turn score gets the first turn {p1score,p2score,p3score,p4score,TurnDecidedOrNot}//
int turnchangecounter; 	//records how many times turns have changed since first roll (before game start)//
int task=0;			//task=0 means checking first turn, task=1 means rolling dice for goti moving, task=2 means selecting dice and goti, task=3 means no possible moves and turn should be skipped//
int diceselect=-1,gotiselect=-1;	//Variable to save which dice and goti have been selected//
int won[4]; 				//saves 1 for the player number if that player has won//
int gameEnd=0;				//Goes to 1 when gameEnded.Goes to 2 when leaderboard updated//
int teammate[4];			//for each index saves the number of the player who is the teammate of that player//
	
	/*--for color,1 means red, 2 means blue,3 means yellow,4 means green
	
	for direction, 1 means up, 2 means right, 3 means down, 4 means left 
	
	gsteps are steps of any color relative to red player starting box--*/
	
/*-------------------------------------------------------------------------------------------------------------*/





/*--My Function Prototypes-----------------------------------------------------------*/
void LudoTitle();					//Function to Display the Title Bar at Top on all screens//
void screen1();					//Function to Display and Control the first screen//
void screen2();					//Function To Display and Control the Second Screen//
void screen3();					//Function to Display and Control the third screen//
void ludoboard();					//Function to Display LudoBoard on screen2//
void nameandturn();					//Function to Display Player Names and Turn on screen2//
void scoreboard();					//Function to Display Scoreboard on screen2//
void dice_display();					//Function to Display Dice throw on screen2//
void events_display();					//Function to Display Event Box on screen2//
void goti_display();					//Function to Display Goti and their Numbers on screen2//
void blocks_display();					//Function to Display the blocks on Ludo Board on screen2//

bool rollcomplete();					//Function to calculate if a dice roll is complete//
bool move_possible(int location,int movement);	//Function to Check if A single goti can move a certain steps//
bool block_stop(int loc,int move);			//Function to check if a block is stopping movement of goti of current turn//
bool turn_skip();					//Function to check if turn should be skipped (there might be no possible moves)//
void goti_mover();					//Function to move a goti according to the dice selected and goti selected//
void goti_kill();					//Function to kill a goti if another goti steps on it//
void update_blocks(int playernum);			//Function to create or remove any blocks on the board (of a certain player) according to new goti positions//
void goti_coord_setter(int player,int gotinumber);	//sets display location of goti on ludo board according to its steps value//
void all_goti_coord_setter();				//function to set coords of all the gotis on board//
void multi_goti_setter();				//Function to set goti coords if there are multiple gotis on single box//
void event_controller(string next_event);		//Function to Control and Add new events to event box on screen2//
void turnchanger();					//Function to change turn clockwise one time//
void teammates();					//Function to Check which player is the teammate of which player//
int  steps_to_gsteps(int colo,int steps);		//Function to convert steps of any color to gsteps//
/*-----------------------------------------------------------------------------------*/






void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); 	//set the screen size to given width and height//
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}



	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	----------------------------------------------GameDisplay FUNCTION-----------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/
void GameDisplay(){
	
	glClearColor(0.411764705882353/*Red Component*/, 0.411764705882353,	//148.0/255/*Green Component*/,
			0.411764705882353/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	
	LudoTitle();
	if (screen==2)screen2();
	if (screen==3)screen3();
	if(screen==1)screen1();
	glutSwapBuffers(); // do not modify this line..

}
	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/








/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}
	glutPostRedisplay();
}






	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	----------------------------------------------PRINTABLE KEYS FUNCTION--------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/
void PrintableKeys(unsigned char key, int x, int y) {
	
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	
		/*---------------------------------------------------------------------------------------------
		------------------------------------------FOR SCREEN 1-----------------------------------------
		---------------------------------------------------------------------------------------------*/
	if (screen==1){
	
		
	if ((key>=65 and key<=90) or (key>=97 and key<=122) or key==' ' or key==8) //Keys for typing player name
	{
	//Changing Player Name
	//player 1
	if(scr1data[0][0]==1 and key==8 and scr1data[0][2]!=0){playername[0][scr1data[0][2]-1]=' ';scr1data[0][2]--;}
	else if(scr1data[0][0]==1 and scr1data[0][2]!=8){playername[0][scr1data[0][2]]=key;scr1data[0][2]++;}
	else if(scr1data[0][0]==1 and scr1data[0][2]==8)playername[0][7]=key;	
	//player 2
	if(scr1data[1][0]==1 and key==8 and scr1data[1][2]!=0){playername[1][scr1data[1][2]-1]=' ';scr1data[1][2]--;}
	else if(scr1data[1][0]==1 and scr1data[1][2]!=8){playername[1][scr1data[1][2]]=key;scr1data[1][2]++;}
	else if(scr1data[1][0]==1 and scr1data[1][2]==8)playername[1][7]=key;
	//player 3
	if(scr1data[2][0]==1 and key==8 and scr1data[2][2]!=0){playername[2][scr1data[2][2]-1]=' ';scr1data[2][2]--;}
	else if(scr1data[2][0]==1 and scr1data[2][2]!=8){playername[2][scr1data[2][2]]=key;scr1data[2][2]++;}
	else if(scr1data[2][0]==1 and scr1data[2][2]==8)playername[2][7]=key;
	//player 4
	if(scr1data[3][0]==1 and key==8 and scr1data[3][2]!=0){playername[3][scr1data[3][2]-1]=' ';scr1data[3][2]--;}
	else if(scr1data[3][0]==1 and scr1data[3][2]!=8){playername[3][scr1data[3][2]]=key;scr1data[3][2]++;}
	else if(scr1data[3][0]==1 and scr1data[3][2]==8)playername[3][7]=key;	
		}
	}
		/*---------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------
		---------------------------------------------------------------------------------------------*/
	
	
	
	
	
	
		/*---------------------------------------------------------------------------------------------
		------------------------------------------FOR SCREEN 2-----------------------------------------
		---------------------------------------------------------------------------------------------*/
	if(screen==2){
	if(gameEnd==0){
	if ((task==0 or task==1 )and key==32){
	
		//DICE ROLLING
	///////////////////////////////////////////////////////////////////////
	if (!rollcomplete() and diceroll[0]==0 and diceroll[1]==0 and diceroll[2]==0){
		diceroll[0]=static_cast<int>((rand()%6)+1);
		if(diceroll[0]==6 and won[turn]==1 and teams){won[turn]=2;event_controller("You can move your");event_controller("teammate's piece now");}
		if (task==0){turnscore[turn]+=diceroll[0];}
		}
	
	else if (!rollcomplete() and diceroll[0]==6 and diceroll[1]==0){
		diceroll[1]=static_cast<int>((rand()%6)+1);
		if (task==0){turnscore[turn]+=diceroll[1];}
		}
	
	else if (!rollcomplete() and diceroll[0]==6 and diceroll[1]==6 and diceroll[2]==0){
		diceroll[2]=static_cast<int>((rand()%6)+1);
		if (task==0){turnscore[turn]+=diceroll[2];}
		}
	else if (rollcomplete() and task==0){turnchanger();
	event_controller("");
	event_controller("Go "+playername[turn]);
	event_controller("press space to roll");}
	/////////////////////////////////////////////////////////////////////////
	
	//END OF THROWS(FIRST TURN SELECT)
	////////////////////////////////////////////////////////////////////////
	if(rollcomplete() and task==1){
	task=2;
	if(!turn_skip() and !(diceroll[0]==6 and diceroll[1]==6 and diceroll[2]==6)){
	event_controller("Select Goti# (in base) and");
	event_controller("dice(with cursor) and then");
	event_controller("press ENTER to move");
	}
	}
	//Determining Who will have first turn based on turnscore
	if(turnchangecounter==playercount and task==0){
	task=1;
	for (int i=0;i<playercount;i++){
		if (turnscore[turn]<turnscore[i])turn=i;
	}
	event_controller(">>Game Started<<");
	event_controller(playername[turn]+"'s turn");
	event_controller("press space to roll");
	/////////////////////////////////////////////////////////////////////////
	}}
	//Applying the selected moves to goti
	if(key==13 and task==2){
		if(gotiselect!=-1 and diceselect!=-1 and (move_possible(scr2data[turn][gotiselect][0],diceroll[diceselect]) or(teams and move_possible(scr2data[teammate[turn]][gotiselect][0],diceroll[diceselect]) and won[turn]==2))){goti_mover();}
		else{event_controller(">move not possible<");gotiselect=-1;}
	}
	if(key==13 and task==3){turnchanger();task=1;}
	}
	
	//Shifting to Screen3 when ENTER pressed after game ended
	else{
	if(key==13){screen=3;}
	}
	}
		/*---------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------
		---------------------------------------------------------------------------------------------*/	
	glutPostRedisplay();
}
	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/



	

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(1000.0, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {

	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */





 
 
 
	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	----------------------------------------------MOUSE CLICKED FUNCTION---------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/ 
void MouseClicked(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON and state==0) // dealing only with left button
			{	
	int gridcoord[51][42][2];
	for(int i=0;i<51;i++)
		for(int j=0;j<42;j++){
			gridcoord[i][j][0]=(20*i);
			gridcoord[i][j][1]=(20*j);}		
	
	
		/*---------------------------------------------------------------------------------------------
		------------------------------------------FOR SCREEN 1-----------------------------------------
		---------------------------------------------------------------------------------------------*/
	if (screen==1){
	if (playercount<4){scr1data[3][1]=0;playername[3]="        ";}
	if (playercount<3){scr1data[2][1]=0;playername[2]="        ";}	
	
	//selecting number of players		
	if(x>gridcoord[9][27][0] and y>171 and x<gridcoord[14][32][0] and y<252)
		playercount=2;
	if(x>gridcoord[16][27][0] and y>171 and x<gridcoord[21][32][0] and y<252)
		playercount=3;
	if(x>gridcoord[23][27][0] and y>171 and x<gridcoord[28][32][0] and y<252)
		playercount=4;	
	
	//selecting colour
	//player1
	if(x>gridcoord[30][23][0] and y>305 and x<gridcoord[31][24][0] and y<319 and scr1data[1][1]!=1 and scr1data[2][1]!=1 and scr1data[3][1]!=1)scr1data[0][1]=1;
	if(x>gridcoord[30][24][0] and y>288 and x<gridcoord[31][25][0] and y<305 and scr1data[1][1]!=2 and scr1data[2][1]!=2 and scr1data[3][1]!=2)scr1data[0][1]=2;	
	if(x>gridcoord[31][24][0] and y>288 and x<gridcoord[32][25][0] and y<305 and scr1data[1][1]!=3 and scr1data[2][1]!=3 and scr1data[3][1]!=3)scr1data[0][1]=3;	
	if(x>gridcoord[31][23][0] and y>305 and x<gridcoord[32][24][0] and y<319 and scr1data[1][1]!=4 and scr1data[2][1]!=4 and scr1data[3][1]!=4)scr1data[0][1]=4;	
	//player2
	if(x>gridcoord[30][23][0] and y>389 and x<gridcoord[31][24][0] and y<403 and scr1data[0][1]!=1 and scr1data[2][1]!=1 and scr1data[3][1]!=1)scr1data[1][1]=1;
	if(x>gridcoord[30][24][0] and y>372 and x<gridcoord[31][25][0] and y<389 and scr1data[0][1]!=2 and scr1data[2][1]!=2 and scr1data[3][2]!=1)scr1data[1][1]=2;	
	if(x>gridcoord[31][24][0] and y>372 and x<gridcoord[32][25][0] and y<389 and scr1data[0][1]!=3 and scr1data[2][1]!=3 and scr1data[3][1]!=3)scr1data[1][1]=3;	
	if(x>gridcoord[31][23][0] and y>389 and x<gridcoord[32][24][0] and y<403 and scr1data[0][1]!=4 and scr1data[2][1]!=4 and scr1data[3][1]!=4)scr1data[1][1]=4;	
	//player3
	if(playercount>2 and x>gridcoord[30][23][0] and y>473 and x<gridcoord[31][24][0] and y<487 and scr1data[0][1]!=1 and scr1data[1][1]!=1 and scr1data[3][1]!=1)scr1data[2][1]=1;
	if(playercount>2 and x>gridcoord[30][24][0] and y>456 and x<gridcoord[31][25][0] and y<473 and scr1data[0][1]!=2 and scr1data[1][1]!=2 and scr1data[3][1]!=2)scr1data[2][1]=2;	
	if(playercount>2 and x>gridcoord[31][24][0] and y>456 and x<gridcoord[32][25][0] and y<473 and scr1data[0][1]!=3 and scr1data[1][1]!=3 and scr1data[3][1]!=3)scr1data[2][1]=3;	
	if(playercount>2 and x>gridcoord[31][23][0] and y>473 and x<gridcoord[32][24][0] and y<487 and scr1data[0][1]!=4 and scr1data[1][1]!=4 and scr1data[3][1]!=4)scr1data[2][1]=4;	
	//player4
	if(playercount>3 and x>gridcoord[30][23][0] and y>557 and x<gridcoord[31][24][0] and y<571 and scr1data[0][1]!=1 and scr1data[1][1]!=1 and scr1data[2][1]!=1)scr1data[3][1]=1;
	if(playercount>3 and x>gridcoord[30][24][0] and y>540 and x<gridcoord[31][25][0] and y<557 and scr1data[0][1]!=2 and scr1data[1][1]!=2 and scr1data[2][1]!=2)scr1data[3][1]=2;	
	if(playercount>3 and x>gridcoord[31][24][0] and y>540 and x<gridcoord[32][25][0] and y<557 and scr1data[0][1]!=3 and scr1data[1][1]!=3 and scr1data[2][1]!=3)scr1data[3][1]=3;	
	if(playercount>3 and x>gridcoord[31][23][0] and y>557 and x<gridcoord[32][24][0] and y<571 and scr1data[0][1]!=4 and scr1data[1][1]!=4 and scr1data[2][1]!=4)scr1data[3][1]=4;	
	
	//Selecting Name Input Box
	//Player 1
	if(x>gridcoord[9][22][0] and y>288 and x<gridcoord[29][25][0] and y<337){scr1data[0][0]=1;}
	else {scr1data[0][0]=0;}
	//player 2
	if(x>gridcoord[9][22][0] and y>372 and x<gridcoord[29][25][0] and y<421){scr1data[1][0]=1;}
	else {scr1data[1][0]=0;}
	//player 3
	if(x>gridcoord[9][22][0] and y>456 and x<gridcoord[29][25][0] and y<505){scr1data[2][0]=1;}
	else {scr1data[2][0]=0;}
	//player 4
	if(x>gridcoord[9][22][0] and y>540 and x<gridcoord[29][25][0] and y<589){scr1data[3][0]=1;}
	else {scr1data[3][0]=0;}
	
	//Teams button
	if(x>gridcoord[5][3][0] and y>622 and x<(gridcoord[16][5][0]+5) and y<656 and playercount==4){teams=!teams;} 
	
	//LEADERBOARD button
	if(x>gridcoord[37][39][0] and y>20 and x<gridcoord[47][39][0] and y<50)screen=3;
	
	//Proceeding to Screen2
	if(x>gridcoord[25][3][0] and y>622 and x<(gridcoord[31][5][0]+10) and y<656){
		screen=2;
		if (playercount<4)teams=0;
		//Setting a Player Name if none was selected
		if (playername[0]=="        ")playername[0]="player1";
		if (playername[1]=="        ")playername[1]="player2";
		if(playercount>2)if (playername[2]=="        ")playername[2]="player3";
		if(playercount>3)if (playername[3]=="        ")playername[3]="player4";
		//setting a colour if none was selected
		if(scr1data[0][1]==0){
			do{scr1data[0][1]=(rand()%4)+1;}
			while(scr1data[0][1]==scr1data[1][1] or scr1data[0][1]==scr1data[2][1] or scr1data[0][1]==scr1data[3][1]);}
		if(scr1data[1][1]==0){
			do{scr1data[1][1]=(rand()%4)+1;}
			while(scr1data[1][1]==scr1data[0][1] or scr1data[1][1]==scr1data[2][1] or scr1data[1][1]==scr1data[3][1]);}
		if(playercount>2)if(scr1data[2][1]==0){
			do{scr1data[2][1]=(rand()%4)+1;}
			while(scr1data[2][1]==scr1data[0][1] or scr1data[2][1]==scr1data[1][1] or scr1data[2][1]==scr1data[3][1]);}
		if(playercount>3)if(scr1data[3][1]==0){
			do{scr1data[3][1]=(rand()%4)+1;}
			while(scr1data[3][1]==scr1data[0][1] or scr1data[3][1]==scr1data[1][1] or scr1data[3][1]==scr1data[2][1]);}
		event4="Go "+playername[turn];
		if (teams)teammates();
	}}
		/*---------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------
		---------------------------------------------------------------------------------------------*/
	
	
	
	
	
	
		/*---------------------------------------------------------------------------------------------
		------------------------------------------FOR SCREEN 2-----------------------------------------
		---------------------------------------------------------------------------------------------*/
	if (screen==2){
	if (gameEnd==0){
	///SELECTING GOTI AND DICE FOR MOVEMENT
	if (task==2){
	int y1=680,y2=342; //y coords using mouse click are different so have to customly apply them
	int goti;
	if(teams and won[turn]==2)goti=scr1data[teammate[turn]][1]-1;else goti=scr1data[turn][1]-1; 
	int * ptr[8] = {&rboxcoord[0],&y1,&bboxcoord[0],&y2,&yboxcoord[0],&y2,&gboxcoord[0],&y1};
		//dice select
		if(x>gridcoord1[49][2][0] and y>630 and x<gridcoord1[53][6][0] and y<680 and diceroll[0]>0)diceselect=0;
		if(x>gridcoord1[55][2][0] and y>630 and x<gridcoord1[59][6][0] and y<680 and diceroll[1]>0)diceselect=1;
		if(x>gridcoord1[61][2][0] and y>630 and x<gridcoord1[65][6][0] and y<680 and diceroll[2]>0)diceselect=2;
		//goti select
		if(x>*ptr[2*goti]+75 and y>*ptr[(2*goti)+1]-160 and x<*ptr[2*goti]+120 and y<*ptr[(2*goti)+1]-125){gotiselect=0;}
		if(x>*ptr[2*goti]+150 and y>*ptr[(2*goti)+1]-160 and x<*ptr[2*goti]+195 and y<*ptr[(2*goti)+1]-125){gotiselect=1;}
		if(x>*ptr[2*goti]+75 and y>*ptr[(2*goti)+1]-100 and x<*ptr[2*goti]+120 and y<*ptr[(2*goti)+1]-60){gotiselect=2;}
		if(x>*ptr[2*goti]+150 and y>*ptr[(2*goti)+1]-100 and x<*ptr[2*goti]+195 and y<*ptr[(2*goti)+1]-60){gotiselect=3;}
	}
	}
	}	
		/*---------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------
		---------------------------------------------------------------------------------------------*/
	
	
	
	
	
		/*---------------------------------------------------------------------------------------------
		------------------------------------------FOR SCREEN 3-----------------------------------------
		---------------------------------------------------------------------------------------------*/
	if (screen==3 and gameEnd==0){
	///Selecting go back button
	if(x>gridcoord[2][39][0] and y>20 and x<gridcoord[7][41][0] and y<50){screen=1;}
	
	}	
		/*---------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------
		---------------------------------------------------------------------------------------------*/




	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{

	}
	glutPostRedisplay();
}
	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/











	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	----------------------------------------------MAIN FUNCTION------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/
int main(int argc, char*argv[]) {
	
	for (int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			scr2data[i][j][0]=-1;}
		

	int width = 1020, height = 840; // i have set my window size to be 800 x 600
	//b->InitalizeBoard(width, height);
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("PF Project"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
	/*---------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------
	---------------------------------------------------------------------------------------------------------------*/















/*---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------MY FUNCTION DEFINITIONS---------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------*/




		/*--Function to Check who the teammate of each player according to color (opp. colors form teams)--*/
void teammates(){
	int colors[4];
	for (int playernum=0;playernum<4;playernum++){
		colors[playernum]=scr1data[playernum][1];}
	
	for (int playernum1=0;playernum1<4;playernum1++){
		for (int playernum2=0;playernum2<4;playernum2++){
			if (playernum1!=playernum2){
				if ((colors[playernum1]+2==colors[playernum2]) or (colors[playernum2]+2==colors[playernum1])) teammate[playernum1]=playernum2;
			}
		}
	}
}




			/*--Function to convert steps of any color to gsteps (gsteps are the same for each color)--*/
int steps_to_gsteps(int colo,int steps){
	int gsteps;
	if (steps<0 or steps>50)return -1;
	if(colo==1){
		return steps;}
	else if(colo==2)gsteps=13+steps;
	else if(colo==3)gsteps=26+steps;
	else if(colo==4)gsteps=39+steps;
	if (gsteps>51)gsteps-=52;
	return gsteps;
}




					/*--Function to Display the Title Bar at Top--*/
void LudoTitle(){
	//LUDO TITLE
	int x_coord=450,y_coord=780,length=9;
	string printstring="LUDO GAME";
	DrawRoundRect(0,y_coord-40,1020,100,colors[LIGHT_YELLOW],1);
	DrawRoundRect(x_coord-20,y_coord-20,(length*16)+40,62.5,colors[LINEN],10);	
	DrawString(x_coord, y_coord, printstring, colors[BLACK]);	

}




				/*--Function to Display and Control the first screen--*/
void screen1(){
	int gridcoord[51][42][2];	//grid coord for screen1
	for(int i=0;i<51;i++)
		for(int j=0;j<42;j++){
			gridcoord[i][j][0]=(20*i);
			gridcoord[i][j][1]=(20*j);}
	
	//Select number of players
	DrawRoundRect(gridcoord[1][2][0],gridcoord[1][2][1],640,680,colors[IVORY],10);
	DrawString(gridcoord[2][34][0], gridcoord[2][34][1], "SELECT NUMBER OF PLAYERS:", colors[BLACK]);
	if(playercount==2)DrawSquare(gridcoord[9][27][0]-3,gridcoord[9][27][1]-3,106,colors[BLACK]);
	DrawSquare(gridcoord[9][27][0],gridcoord[9][27][1],100,colors[TAN]);
	DrawString(gridcoord[11][29][0]+5,gridcoord[11][29][1], "2", colors[BLACK]);	
	if(playercount==3)DrawSquare(gridcoord[16][27][0]-3,gridcoord[16][27][1]-3,106,colors[BLACK]);
	DrawSquare(gridcoord[16][27][0],gridcoord[16][27][1],100,colors[TAN]);
	DrawString(gridcoord[18][29][0]+5,gridcoord[18][29][1], "3", colors[BLACK]);
	if(playercount==4)DrawSquare(gridcoord[23][27][0]-3,gridcoord[23][27][1]-3,106,colors[BLACK]);	
	DrawSquare(gridcoord[23][27][0],gridcoord[23][27][1],100,colors[TAN]);
	DrawString(gridcoord[25][29][0]+5,gridcoord[25][29][1], "4", colors[BLACK]);	
	
	//Player 1 Name and Color
	DrawString(gridcoord[2][23][0]+5,gridcoord[2][23][1], "PLAYER 1:", colors[BLACK]);
	if(scr1data[0][0]==1)DrawRoundRect(gridcoord[9][22][0]-3,gridcoord[9][22][1]-3,406,66,colors[BLACK],5);
	DrawRoundRect(gridcoord[9][22][0],gridcoord[9][22][1],400,60,colors[BURLY_WOOD],5);
	DrawString(gridcoord[9][22][0]+20,gridcoord[9][22][1]+20, playername[0], colors[BLACK]);
	
	DrawSquare(gridcoord[30][23][0],gridcoord[30][23][1],20,colors[RED]);		//red box
	if(scr1data[0][1]==1)DrawCircle(gridcoord[30][23][0]+10,gridcoord[30][23][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[30][24][0],gridcoord[30][24][1],20,colors[DARK_BLUE]);		//blue box
	if(scr1data[0][1]==2)DrawCircle(gridcoord[30][24][0]+10,gridcoord[30][24][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][24][0],gridcoord[31][24][1],20,colors[OLIVE]);		//yellow box
	if(scr1data[0][1]==3)DrawCircle(gridcoord[31][24][0]+10,gridcoord[31][24][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][23][0],gridcoord[31][23][1],20,colors[GREEN]);		//green box
	if(scr1data[0][1]==4)DrawCircle(gridcoord[31][23][0]+10,gridcoord[31][23][1]+10,5,colors[BLACK]);
	
	
	//Player 2 Name and Color
	if (playercount>=2){
	DrawString(gridcoord[2][18][0]+5,gridcoord[2][18][1], "PLAYER 2:", colors[BLACK]);
	if(scr1data[1][0]==1)DrawRoundRect(gridcoord[9][17][0]-3,gridcoord[9][17][1]-3,406,66,colors[BLACK],5);
	DrawRoundRect(gridcoord[9][17][0],gridcoord[9][17][1],400,60,colors[BURLY_WOOD],5);
	DrawString(gridcoord[9][17][0]+20,gridcoord[9][17][1]+20, playername[1], colors[BLACK]);
	
	DrawSquare(gridcoord[30][18][0],gridcoord[30][18][1],20,colors[RED]);		//red box
	if(scr1data[1][1]==1)DrawCircle(gridcoord[30][18][0]+10,gridcoord[30][18][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[30][19][0],gridcoord[30][19][1],20,colors[DARK_BLUE]);		//blue box
	if(scr1data[1][1]==2)DrawCircle(gridcoord[30][19][0]+10,gridcoord[30][19][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][19][0],gridcoord[31][19][1],20,colors[OLIVE]);		//yellow box
	if(scr1data[1][1]==3)DrawCircle(gridcoord[31][19][0]+10,gridcoord[31][19][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][18][0],gridcoord[31][18][1],20,colors[GREEN]);		//green box
	if(scr1data[1][1]==4)DrawCircle(gridcoord[31][18][0]+10,gridcoord[31][18][1]+10,5,colors[BLACK]);
	}
	
	//Player 3 Name and Color
	if (playercount>=3){
	DrawString(gridcoord[2][13][0]+5,gridcoord[2][13][1], "PLAYER 3:", colors[BLACK]);
	if(scr1data[2][0]==1)DrawRoundRect(gridcoord[9][12][0]-3,gridcoord[9][12][1]-3,406,66,colors[BLACK],5);
	DrawRoundRect(gridcoord[9][12][0],gridcoord[9][12][1],400,60,colors[BURLY_WOOD],5);
	DrawString(gridcoord[9][12][0]+20,gridcoord[9][12][1]+20, playername[2], colors[BLACK]);
	
	DrawSquare(gridcoord[30][13][0],gridcoord[30][13][1],20,colors[RED]);		//red box
	if(scr1data[2][1]==1)DrawCircle(gridcoord[30][13][0]+10,gridcoord[30][13][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[30][14][0],gridcoord[30][14][1],20,colors[DARK_BLUE]);		//blue box
	if(scr1data[2][1]==2)DrawCircle(gridcoord[30][14][0]+10,gridcoord[30][14][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][14][0],gridcoord[31][14][1],20,colors[OLIVE]);		//yellow box
	if(scr1data[2][1]==3)DrawCircle(gridcoord[31][14][0]+10,gridcoord[31][14][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][13][0],gridcoord[31][13][1],20,colors[GREEN]);		//green box
	if(scr1data[2][1]==4)DrawCircle(gridcoord[31][13][0]+10,gridcoord[31][13][1]+10,5,colors[BLACK]);
	}
		
	//Player 4 Name and Color
	if (playercount==4){
	DrawString(gridcoord[2][8][0]+5,gridcoord[2][8][1], "PLAYER 4:", colors[BLACK]);
	if(scr1data[3][0]==1)DrawRoundRect(gridcoord[9][7][0]-3,gridcoord[9][7][1]-3,406,66,colors[BLACK],5);
	DrawRoundRect(gridcoord[9][22][0],gridcoord[9][7][1],400,60,colors[BURLY_WOOD],5);
	DrawString(gridcoord[9][22][0]+20,gridcoord[9][7][1]+20, playername[3], colors[BLACK]);
	
	DrawSquare(gridcoord[30][8][0],gridcoord[30][8][1],20,colors[RED]);			//red box
	if(scr1data[3][1]==1)DrawCircle(gridcoord[30][8][0]+10,gridcoord[30][8][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[30][9][0],gridcoord[30][9][1],20,colors[DARK_BLUE]);		//blue box
	if(scr1data[3][1]==2)DrawCircle(gridcoord[30][9][0]+10,gridcoord[30][9][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][9][0],gridcoord[31][9][1],20,colors[OLIVE]);		//yellow box
	if(scr1data[3][1]==3)DrawCircle(gridcoord[31][9][0]+10,gridcoord[31][9][1]+10,5,colors[BLACK]);
	DrawSquare(gridcoord[31][8][0],gridcoord[31][8][1],20,colors[GREEN]);		//green box
	if(scr1data[3][1]==4)DrawCircle(gridcoord[31][8][0]+10,gridcoord[31][8][1]+10,5,colors[BLACK]);
	}
	
	//Proceed button
	DrawRoundRect(gridcoord[25][3][0]-4,gridcoord[25][3][1]-4,135,45,colors[BLACK],5);
	DrawRoundRect(gridcoord[25][3][0],gridcoord[25][3][1],130,40,colors[WHITE],5);
	DrawString(gridcoord[25][3][0]+10,gridcoord[25][3][1]+10, "PROCEED", colors[BLACK]);
	
	//Instructions
	DrawRoundRect(gridcoord[34][2][0],gridcoord[34][2][1],320,680,colors[IVORY],10);
	DrawString(gridcoord[37][34][0]+10, gridcoord[37][34][1], "INSTRUCTIONS", colors[BLACK]);
	DrawString(gridcoord[35][32][0], gridcoord[35][32][1], "1. Use the mouse to select", colors[BLACK]);
	DrawString(gridcoord[36][30][0], gridcoord[36][30][1], " number of players, color", colors[BLACK]);
	DrawString(gridcoord[36][28][0], gridcoord[36][28][1], " of players, team mode", colors[BLACK]);
	DrawString(gridcoord[36][26][0], gridcoord[36][26][1], " toggle, player name", colors[BLACK]);
	DrawString(gridcoord[36][24][0], gridcoord[36][24][1], " textbox, and leaderboard.", colors[BLACK]);
	DrawString(gridcoord[35][22][0], gridcoord[35][22][1], "2. Max length of name is 8", colors[BLACK]);
	DrawString(gridcoord[36][20][0], gridcoord[36][20][1], " and only English Alphabets", colors[BLACK]);
	DrawString(gridcoord[36][18][0], gridcoord[36][18][1], " and space can be used.", colors[BLACK]);
	DrawString(gridcoord[35][16][0], gridcoord[35][16][1], "3. Selecting no colour will", colors[BLACK]);
	DrawString(gridcoord[36][14][0], gridcoord[36][14][1], " make it random.", colors[BLACK]);
	DrawString(gridcoord[35][12][0], gridcoord[35][12][1], "4. Not naming a player ", colors[BLACK]);
	DrawString(gridcoord[36][10][0], gridcoord[36][10][1], " will cause them to be", colors[BLACK]);
	DrawString(gridcoord[36][8][0], gridcoord[36][8][1], " named by default.", colors[BLACK]);
	DrawString(gridcoord[35][6][0], gridcoord[35][6][1], "5. Click on proceed to", colors[BLACK]);
	DrawString(gridcoord[36][4][0], gridcoord[36][4][1], " start the game.", colors[BLACK]);
	
	//Teams Button	
	if (playercount==4){
	string state[2]={"OFF","ON"};
	int butncolor[2]={RED,FOREST_GREEN};
	DrawRoundRect(gridcoord[5][3][0]-4,gridcoord[5][3][1]-4,230,45,colors[BLACK],5);
	DrawRoundRect(gridcoord[5][3][0],gridcoord[5][3][1],225,40,colors[butncolor[teams]],5);
	DrawString(gridcoord[5][3][0]+10,gridcoord[5][3][1]+10, "TEAM MODE: "+state[teams], colors[BLACK]);}
	
	//LEADERBOARD BUTTON
	DrawRoundRect(gridcoord[37][39][0]-4,gridcoord[37][39][1]-4,205,45,colors[BLACK],5);
	DrawRoundRect(gridcoord[37][39][0],gridcoord[37][39][1],200,40,colors[WHITE],5);
	DrawString(gridcoord[37][39][0]+10,gridcoord[37][39][1]+10, "LEADERBOARD", colors[BLACK]);
}




					/*--Function to Display and Control the Third Screen--*/
void screen3(){
	int count=0;
	string line,playernames[10],highscores[10];
	
	ifstream reader;
	ofstream writer;
	
	/*--Checking If a highscores.txt exists and has 20 lines (for each score)--*/ 
	reader.open("highscores.txt");
	if (reader.is_open()){
		while (!reader.eof()){
		getline(reader, line);
		count++;
		}}
        reader.close();
	/*--If highscores.txt does not have proper initialisation then initialise it with default data--*/       
	if (count<21)
	{
		writer.open("highscores.txt");
		for(int linenum=1;linenum<21;linenum++){
		if(((linenum+1)%2)==0)
			{
				writer<<"noPlayerEver"<<endl;
			}		
		else
			{
				writer<<"0"<<endl;
			}
		}
		writer.close();
	}
	
	
	/*--Getting Existing File from Data and saving it in arrays--*/
	reader.open("highscores.txt");
	for (int linenum=1;linenum<21;linenum++){
		if(linenum==0){getline(reader, line);}
		else{
			if(((linenum+1)%2)==0)
			{
				getline(reader,playernames[(linenum/2)]);
			}
			else
			{	
				getline(reader,highscores[(linenum/2)-1]);
			}
		}
	}
	reader.close();
	
	/*--Updating Arrays and File if Game has ended--*/
	if(gameEnd==1)
	{	
		gameEnd=2;
		int scores[10];
		for(int num=0;num<10;num++)scores[num]=stoi(highscores[num]);	/*--Converting String Highscores to Integer--*/
		for (int player=0;player<playercount;player++)		/*--Updating HighScores and Playernames in arrays--*/
		{
			for(int prevscores=0;prevscores<10;prevscores++)
			{
				if(scr2data[player][4][0]>scores[prevscores])
				{
					for(int i=9;i!=prevscores;i--)
					{
						scores[i]=scores[i-1];
						playernames[i]=playernames[i-1];
					}
					scores[prevscores]=scr2data[player][4][0];
					playernames[prevscores]=playername[player];
					break;
				}
			}
		}
		for(int num=0;num<10;num++)highscores[num]=to_string(scores[num]);
		writer.open("highscores.txt");
		for(int linenum=1;linenum<21;linenum++){
		if(((linenum+1)%2)==0)
			{
				writer<<playernames[(linenum/2)]<<endl;
			}		
		else
			{
				writer<<highscores[(linenum/2)-1]<<endl;
			}
		}
		writer.close();	
	}
	
	
	
	
	int gridcoord[51][42][2];	//grid coord for screen3
	for(int i=0;i<51;i++)
		for(int j=0;j<42;j++){
			gridcoord[i][j][0]=(20*i);
			gridcoord[i][j][1]=(20*j);}
	
	
	//GO back button
	if(gameEnd==0){
	DrawRoundRect(gridcoord[2][39][0]-4,gridcoord[2][39][1]-4,115,45,colors[BLACK],5);
	DrawRoundRect(gridcoord[2][39][0],gridcoord[2][39][1],110,40,colors[WHITE],5);
	DrawString(gridcoord[2][39][0]+10,gridcoord[2][39][1]+10, "Go Back", colors[BLACK]);}
	
	//"Leaderboard" title
	DrawRoundRect(gridcoord[13][7][0],gridcoord[13][7][1],500,570,colors[BLACK],5);
	DrawString(gridcoord[21][32][0]+10,gridcoord[21][32][1]+10, "LEADERBOARD", colors[IVORY]);

	//Headings
	DrawString(gridcoord[14][29][0]+10,gridcoord[14][29][1]+10, "RANK", colors[IVORY]);
	DrawString(gridcoord[20][29][0]+10,gridcoord[20][29][1]+10, "PLAYER NAME", colors[IVORY]);
	DrawString(gridcoord[32][29][0]+10,gridcoord[32][29][1]+10, "SCORE", colors[IVORY]);
	
	//Player info
	for(int rank=0;rank<10;rank++)
	{
	DrawString(gridcoord[15][27-(rank*2)][0]+10,gridcoord[15][27-(rank*2)][1]+10, to_string(rank+1), colors[IVORY]);
	DrawString(gridcoord[21][27-(rank*2)][0]+10,gridcoord[21][27-(rank*2)][1]+10, playernames[rank], colors[IVORY]);
	DrawString(gridcoord[32][27-(rank*2)][0]+10,gridcoord[32][27-(rank*2)][1]+10, "   "+highscores[rank], colors[IVORY]);
	}
}




						/*--Function to Display Ludo Board--*/
void ludoboard(){
	//Background white box for board
	DrawSquare( ludocoord[0] , ludocoord[1] ,675,colors[WHITE]);
		
	//Small Coloured Boxes on grid
	for (int xboxnum=9;xboxnum<14;xboxnum++)
		DrawSquare( ludogridcoord[xboxnum][7][0] , ludogridcoord[xboxnum][7][1] ,46,colors[GREEN]);
	for (int xboxnum=1;xboxnum<6;xboxnum++)
		DrawSquare( ludogridcoord[xboxnum][7][0] , ludogridcoord[xboxnum][7][1] ,46,colors[DARK_BLUE]);
	for (int yboxnum=9;yboxnum<14;yboxnum++)
		DrawSquare( ludogridcoord[7][yboxnum][0] , ludogridcoord[7][yboxnum][1] ,46,colors[OLIVE]);
	for (int yboxnum=1;yboxnum<6;yboxnum++)
		DrawSquare( ludogridcoord[7][yboxnum][0] , ludogridcoord[7][yboxnum][1] ,46,colors[RED]);
	DrawSquare( ludogridcoord[1][8][0] , ludogridcoord[1][8][1] ,46,colors[DARK_BLUE]);
	DrawSquare( ludogridcoord[6][1][0] , ludogridcoord[6][1][1] ,46,colors[RED]);
	DrawSquare( ludogridcoord[8][13][0] , ludogridcoord[8][13][1] ,46,colors[OLIVE]);
	DrawSquare( ludogridcoord[13][6][0] , ludogridcoord[13][6][1] ,46,colors[GREEN]);
	DrawSquare( ludogridcoord[2][6][0],ludogridcoord[2][6][1],46,colors[DIM_GRAY]);
	DrawSquare( ludogridcoord[6][12][0],ludogridcoord[6][12][1],46,colors[DIM_GRAY]);
	DrawSquare( ludogridcoord[12][8][0],ludogridcoord[12][8][1],46,colors[DIM_GRAY]);
	DrawSquare( ludogridcoord[8][2][0],ludogridcoord[8][2][1],46,colors[DIM_GRAY]);
	
	
	//Drawing grid lines in Board
	for(int linenumber=0;linenumber<14;linenumber++)
		DrawLine( (ludocoord[0]+45)+(45*linenumber) , ludocoord[1] ,  (ludocoord[0]+45)+(45*linenumber) , (ludocoord[1]+675) , 1 , colors[BLACK]);
	for(int linenumber=0;linenumber<14;linenumber++)
	DrawLine( ludocoord[0] , (ludocoord[1]+45)+(45*linenumber) ,  (675+ludocoord[0]) , (ludocoord[1]+45)+(45*linenumber) , 1 , colors[BLACK] );
	
	
	//Drawing home bases of each player
	////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	int * ptr[8] = {&rboxcoord[0],&rboxcoord[1],&bboxcoord[0],&bboxcoord[1],&yboxcoord[0],&yboxcoord[1],&gboxcoord[0],&gboxcoord[1]};
	int boxcolor[4]={RED,DARK_BLUE,OLIVE,GREEN};
	for(int boxnum=0;boxnum<4;boxnum++){
		DrawSquare( *ptr[2*boxnum] , *ptr[(2*boxnum)+1] ,270,colors[boxcolor[boxnum]]);
		DrawRoundRect(*ptr[2*boxnum]+45,*ptr[(2*boxnum)+1]+45,180,180,colors[WHITE],20); 
		
		//Goti Home Resting Spots+Displaying if a box is selected
		DrawRoundRect(*ptr[2*boxnum]+75-4,*ptr[(2*boxnum)+1]+150-4,49,49,colors[BLACK],10); 
		if(gotiselect==0 and ((scr1data[turn][1]==boxnum+1 and won[turn]!=2) or (scr1data[teammate[turn]][1]==boxnum+1 and won[turn]==2))) DrawRoundRect(*ptr[2*boxnum]+72-1,*ptr[(2*boxnum)+1]+147-1,53,53,colors[WHITE],10); 
		DrawRoundRect(*ptr[2*boxnum]+75,*ptr[(2*boxnum)+1]+150,45,45,colors[boxcolor[boxnum]],10); 
		
		DrawRoundRect(*ptr[2*boxnum]+150-4,*ptr[(2*boxnum)+1]+150-4,49,49,colors[BLACK],10);
		if(gotiselect==1 and ((scr1data[turn][1]==boxnum+1 and won[turn]!=2) or (scr1data[teammate[turn]][1]==boxnum+1 and won[turn]==2)))DrawRoundRect(*ptr[2*boxnum]+147-1,*ptr[(2*boxnum)+1]+147-1,53,53,colors[WHITE],10); 
		DrawRoundRect(*ptr[2*boxnum]+150,*ptr[(2*boxnum)+1]+150,45,45,colors[boxcolor[boxnum]],10); 
		
		DrawRoundRect(*ptr[2*boxnum]+75-4,*ptr[(2*boxnum)+1]+75-4,49,49,colors[BLACK],10);
		if(gotiselect==2 and ((scr1data[turn][1]==boxnum+1 and won[turn]!=2) or (scr1data[teammate[turn]][1]==boxnum+1 and won[turn]==2)))DrawRoundRect(*ptr[2*boxnum]+72-1,*ptr[(2*boxnum)+1]+72-1,53,53,colors[WHITE],10);
		DrawRoundRect(*ptr[2*boxnum]+75,*ptr[(2*boxnum)+1]+75,45,45,colors[boxcolor[boxnum]],10);  
		
		DrawRoundRect(*ptr[2*boxnum]+150-4,*ptr[(2*boxnum)+1]+75-4,49,49,colors[BLACK],10);
		if(gotiselect==3 and ((scr1data[turn][1]==boxnum+1 and won[turn]!=2) or (scr1data[teammate[turn]][1]==boxnum+1 and won[turn]==2)))DrawRoundRect(*ptr[2*boxnum]+147-1,*ptr[(2*boxnum)+1]+72-1,53,53,colors[WHITE],10);
		DrawRoundRect(*ptr[2*boxnum]+150,*ptr[(2*boxnum)+1]+75,45,45,colors[boxcolor[boxnum]],10);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Drawing Boundary of Board
	DrawLine( ludocoord[0] , ludocoord[1] ,  ludocoord[0] , ludocoord[1]+675 , 2 , colors[BLACK] );	
	DrawLine( ludocoord[0] , ludocoord[1]+675 ,  ludocoord[0]+675 , ludocoord[1]+675 , 2 , colors[BLACK] );
	DrawLine( ludocoord[0] , ludocoord[1] ,  ludocoord[0]+675 , ludocoord[1] , 2 , colors[BLACK] );
	DrawLine( ludocoord[0]+675 , ludocoord[1] ,  ludocoord[0]+675 , ludocoord[1]+675 , 2 , colors[BLACK] );
	
	
	//Drawing Middle Box
	DrawSquare(ludogridcoord[6][6][0]-2,ludogridcoord[6][6][1]-2,140,colors[BLACK]);
	DrawTriangle( 405+ludocoord[0], 270+ludocoord[1] , 405+ludocoord[0], 405+ludocoord[1] , mid[0] , mid[1], colors[GREEN] );
	DrawTriangle( 405+ludocoord[0], 405+ludocoord[1] , 270+ludocoord[0], 405+ludocoord[1] , mid[0] , mid[1], colors[OLIVE] );
	DrawTriangle( 270+ludocoord[0], 405+ludocoord[1] , 270+ludocoord[0], 270+ludocoord[1] , mid[0] , mid[1], colors[DARK_BLUE] );
	DrawTriangle( 270+ludocoord[0], 270+ludocoord[1] , 405+ludocoord[0], 270+ludocoord[1] , mid[0] , mid[1], colors[RED] );
	DrawLine(ludogridcoord[6][6][0],ludogridcoord[6][6][1],ludogridcoord[9][9][0],ludogridcoord[9][9][1],2,colors[BLACK]);
	DrawLine(ludogridcoord[6][9][0],ludogridcoord[6][9][1],ludogridcoord[9][6][0],ludogridcoord[9][6][1],2,colors[BLACK]);	
}




					/*--Function to Display Player Names and Turn--*/
void nameandturn(){
	//Player names
	string windisplay;
	for(int i=0,color;i<playercount;i++){
	color=scr1data[i][1];
	if((won[i]==1 and !teams) or (teams and won[i]==1 and won[teammate[i]]==1))windisplay=" has WON";else windisplay=" ";
	if(color==1)DrawString(gridcoord1[3][18][0], gridcoord1[3][18][1], playername[i]+windisplay, colors[BLACK]);
	if(color==2)DrawString(gridcoord1[3][45][0], gridcoord1[3][45][1], playername[i]+windisplay, colors[BLACK]);
	if(color==3)DrawString(gridcoord1[30][45][0], gridcoord1[30][45][1], playername[i]+windisplay, colors[BLACK]);
	if(color==4)DrawString(gridcoord1[30][18][0], gridcoord1[30][18][1], playername[i]+windisplay, colors[BLACK]);
	if(teams){
	if(color==1)DrawString(gridcoord1[30][45][0], gridcoord1[30][45][1], playername[teammate[i]]+windisplay, colors[BLACK]);
	if(color==2)DrawString(gridcoord1[30][18][0], gridcoord1[30][18][1], playername[teammate[i]]+windisplay, colors[BLACK]);
	if(color==3)DrawString(gridcoord1[3][18][0], gridcoord1[3][18][1], playername[teammate[i]]+windisplay, colors[BLACK]);
	if(color==4)DrawString(gridcoord1[3][45][0], gridcoord1[3][45][1], playername[teammate[i]]+windisplay, colors[BLACK]);
	}
	
	//Player turn
	if(turn==i and color==1)DrawString(gridcoord1[5][3][0], gridcoord1[5][3][1], "YOUR TURN", colors[BLACK]);
	if(turn==i and color==2)DrawString(gridcoord1[5][30][0], gridcoord1[5][30][1], "YOUR TURN", colors[BLACK]);
	if(turn==i and color==3)DrawString(gridcoord1[32][30][0], gridcoord1[32][30][1], "YOUR TURN", colors[BLACK]);
	if(turn==i and color==4)DrawString(gridcoord1[32][3][0], gridcoord1[32][3][1], "YOUR TURN", colors[BLACK]);}
	
	if (teams==1){
	DrawString(gridcoord1[6][15][0], gridcoord1[5][15][1]+5, "TEAM 2", colors[BLACK]);
	DrawString(gridcoord1[6][42][0], gridcoord1[5][42][1]+5, "TEAM 1", colors[BLACK]);
	DrawString(gridcoord1[33][42][0], gridcoord1[3][42][1]+5, "TEAM 2", colors[BLACK]);
	DrawString(gridcoord1[33][15][0], gridcoord1[32][15][1]+5, "TEAM 1", colors[BLACK]);
	}
	
}




					/*--Function to Display Scoreboard--*/
void scoreboard(){
	DrawString(gridcoord1[52][45][0], gridcoord1[52][45][1], "SCOREBOARD", colors[BLACK]);	//title
	//Player 1 score
	DrawString(gridcoord1[50][41][0], gridcoord1[50][41][1], playername[0], colors[BLACK]);
	DrawString(gridcoord1[61][41][0], gridcoord1[61][41][1], ":  "+to_string(scr2data[0][4][0]), colors[BLACK]);
	//Player 2 score
	DrawString(gridcoord1[50][38][0], gridcoord1[50][38][1], playername[1], colors[BLACK]);
	DrawString(gridcoord1[61][38][0], gridcoord1[61][38][1], ":  "+to_string(scr2data[1][4][0]), colors[BLACK]);
	//Player 3 score
	if (playercount>2){DrawString(gridcoord1[50][35][0], gridcoord1[50][35][1], playername[2], colors[BLACK]);
	DrawString(gridcoord1[61][35][0], gridcoord1[61][35][1], ":  "+to_string(scr2data[2][4][0]), colors[BLACK]);}
	//Player 4 score
	if (playercount>3){DrawString(gridcoord1[50][32][0], gridcoord1[50][32][1], playername[3], colors[BLACK]);
	DrawString(gridcoord1[61][32][0], gridcoord1[61][32][1], ":  "+to_string(scr2data[3][4][0]), colors[BLACK]);}
}




					/*--Function to Display Dice throw--*/
void dice_display(){
	//indicate selection
	if (diceselect==0){DrawRoundRect(gridcoord1[49][2][0]-3,gridcoord1[49][2][1]-3,66,66,colors[BLACK],10);};//dice1
	if (diceselect==1){DrawRoundRect(gridcoord1[55][2][0]-3,gridcoord1[55][2][1]-3,66,66,colors[BLACK],10);};//dice1
	if (diceselect==2){DrawRoundRect(gridcoord1[61][2][0]-3,gridcoord1[61][2][1]-3,66,66,colors[BLACK],10);};//dice1	

	
	//title
	DrawString(gridcoord1[49][7][0], gridcoord1[49][7][1], "DICE", colors[BLACK]);
	//first dice
	if(diceroll[0]>0){
	DrawRoundRect(gridcoord1[49][2][0],gridcoord1[49][2][1],60,60,colors[WHITE],10);
	DrawString(gridcoord1[50][3][0]+10, gridcoord1[50][3][1]+4, to_string(diceroll[0]), colors[RED]);}
	//second dice
	if(diceroll[1]>0){DrawRoundRect(gridcoord1[55][2][0],gridcoord1[55][2][1],60,60,colors[WHITE],10);
	DrawString(gridcoord1[56][3][0]+10, gridcoord1[56][3][1]+4, to_string(diceroll[1]), colors[RED]);}
	//third dice
	if(diceroll[2]>0){DrawRoundRect(gridcoord1[61][2][0],gridcoord1[61][2][1],60,60,colors[WHITE],10);
	DrawString(gridcoord1[62][3][0]+10, gridcoord1[62][3][1]+4, to_string(diceroll[2]), colors[RED]);}
	
}




					/*--Function to Display Event Box--*/
void events_display(){
	DrawRoundRect(gridcoord1[49][11][0],gridcoord1[49][11][1],270,285,colors[WHITE],10);
	DrawString(gridcoord1[50][27][0], gridcoord1[50][27][1], "EVENTS:", colors[BLACK]);	
	DrawString(gridcoord1[50][24][0], gridcoord1[50][24][1], ""+event1, colors[BLACK]);
	DrawString(gridcoord1[50][21][0], gridcoord1[50][21][1], ""+event2, colors[BLACK]);
	DrawString(gridcoord1[50][18][0], gridcoord1[50][18][1], ""+event3, colors[BLACK]);
	DrawString(gridcoord1[50][15][0], gridcoord1[50][15][1], ""+event4, colors[BLACK]);
	DrawString(gridcoord1[50][12][0], gridcoord1[50][12][1], ""+event5, colors[BLACK]);
}




					/*--Function to Display Goti and their Numbers--*/
void goti_display(){

	/*
		PRINTING GOTIs
	*/
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int goticolor[4]={CRIMSON,BLUE,ORANGE,FOREST_GREEN};
	for(int playernum=0;playernum<4;playernum++){
		int goti=scr1data[playernum][1];
		for(int gotinum=0;gotinum<4;gotinum++){
		DrawCircle(scr2data[playernum][gotinum][1],scr2data[playernum][gotinum][2],17,colors[BLACK]);
		DrawCircle(scr2data[playernum][gotinum][1],scr2data[playernum][gotinum][2],15,colors[goticolor[goti-1]]);
		DrawString(scr2data[playernum][gotinum][1]-5,scr2data[playernum][gotinum][2]-9, to_string(gotinum+1), colors[BLACK]);
		}		
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	/*
		PRINTING NUMBERS IN BASE PERMANENTLY
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int * ptr[8] = {&rboxcoord[0],&rboxcoord[1],&bboxcoord[0],&bboxcoord[1],&yboxcoord[0],&yboxcoord[1],&gboxcoord[0],&gboxcoord[1]};
	
	for (int a=0;a<4;a++){
	DrawString(*ptr[2*a]+75+22.5-5,*ptr[(2*a)+1]+150+22.5-9, "1 " , colors[BLACK]);
	DrawString(*ptr[2*a]+150+22.5-5,*ptr[(2*a)+1]+150+22.5-9, "2 " , colors[BLACK]);
	DrawString(*ptr[2*a]+75+22.5-5,*ptr[(2*a)+1]+75+22.5-9, "3 " , colors[BLACK]);
	DrawString(*ptr[2*a]+150+22.5-5,*ptr[(2*a)+1]+75+22.5-9, "4 " , colors[BLACK]);}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
}




					/*--Function to Display the blocks on Ludo Board--*/
void blocks_display(){
	int block_count;
	if (teams)block_count=4;else block_count=2;
	for(int i=0;i<4;i++)
		for(int j=0,xcoordinate,ycoordinate;j<block_count;j++){
			if (block[i][j]!=-1){
				for(int k=0;k<4;k++){
					if(block[i][j]==steps_to_gsteps(scr1data[i][1],scr2data[i][k][0])){
						xcoordinate=scr2data[i][k][1]-22.5;ycoordinate=scr2data[i][k][2]-22.5;
						DrawSquare(xcoordinate,ycoordinate,47,colors[BLACK]);
					}
				}
			}
		}
}




					/*--Function to Control and Add new events to event box--*/
void event_controller(string next_event){
	event1=event2;
	event2=event3;
	event3=event4;
	event4=event5;
	event5=next_event;
	glutPostRedisplay();
}




						/*--Function to move turn clockwise once--*/
void turnchanger(){
	int colornewturn,check=0;
	for(int j=1;check!=1;j++){
	colornewturn=(scr1data[turn][1])+j;
	if (colornewturn>4)colornewturn-=4;
	for (int i=0;i<playercount;i++){
	if(scr1data[i][1]==colornewturn and (won[i]!=1 or teams)){turn=i;check=1;}
	}
	}
	diceroll[0]=0;
	diceroll[1]=0;
	diceroll[2]=0;
	if (task!=0){
	event_controller(" ");
	event_controller(playername[turn]+"'s turn");
	event_controller("press space to roll");}
	turnchangecounter++;
	glutPostRedisplay();
}




						/*--Function to check if dice roll has finished --*/
bool rollcomplete(){
	if(diceroll[0]!=6 and diceroll[0]!=0){return true;}
	else if(diceroll[0]==6 and diceroll[1]!=6 and diceroll[1]!=0){return true;}
	else if(diceroll[0]==6 and diceroll[1]==6 and diceroll[2]!=0){return true;}
	else return false;
}




				/*--Function to set location of goti on board (works for steps range -1 to 55)--*/
void goti_coord_setter(int player,int gotinumber){
	int goti=scr1data[player][1];
	
	//setting location on a box in path
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(scr2data[player][gotinumber][0]>-1){	
	if (scr1data[player][1]==1){	//If red goti
	scr2data[player][gotinumber][1]=ludogridcoord[6][1][0]+22.5;scr2data[player][gotinumber][2]=ludogridcoord[6][1][1]+22.5;
	scr2data[player][gotinumber][3]=1;}
	if (scr1data[player][1]==2){	//If blue goti
	scr2data[player][gotinumber][1]=ludogridcoord[1][8][0]+22.5;scr2data[player][gotinumber][2]=ludogridcoord[1][8][1]+22.5;
	scr2data[player][gotinumber][3]=2;}
	if (scr1data[player][1]==3){	//If yellow goti
	scr2data[player][gotinumber][1]=ludogridcoord[8][13][0]+22.5;scr2data[player][gotinumber][2]=ludogridcoord[8][13][1]+22.5;
	scr2data[player][gotinumber][3]=3;}
	if (scr1data[player][1]==4){	//If green goti
	scr2data[player][gotinumber][1]=ludogridcoord[13][6][0]+22.5;scr2data[player][gotinumber][2]=ludogridcoord[13][6][1]+22.5;
	scr2data[player][gotinumber][3]=4;}
	//Moving Goti one place at a time
	////////////////////////////////
	for(int steps=0;steps<(scr2data[player][gotinumber][0]);steps++){
	//dealing with turning left, diagonally close to middle box
	if((steps==4 and goti==1) or (steps==17 and goti==4) or (steps==30 and goti==3)or (steps==43 and goti==2)){
	scr2data[player][gotinumber][1]-=45;scr2data[player][gotinumber][2]+=45;scr2data[player][gotinumber][3]=4;}
	else if((steps==4 and goti==2) or (steps==17 and goti==1) or (steps==30 and goti==4)or (steps==43 and goti==3)){
	scr2data[player][gotinumber][1]+=45;scr2data[player][gotinumber][2]+=45;scr2data[player][gotinumber][3]=1;}
	else if((steps==4 and goti==3) or (steps==17 and goti==2) or (steps==30 and goti==1)or (steps==43 and goti==4)){
	scr2data[player][gotinumber][1]+=45;scr2data[player][gotinumber][2]-=45;scr2data[player][gotinumber][3]=2;}
	else if((steps==4 and goti==4) or (steps==17 and goti==3) or (steps==30 and goti==2)or (steps==43 and goti==1)){
	scr2data[player][gotinumber][1]-=45;scr2data[player][gotinumber][2]-=45;scr2data[player][gotinumber][3]=3;}
	//dealing with turning right before win column
	else if((steps==10 and goti==1) or (steps==23 and goti==4) or (steps==36 and goti==3)or (steps==49 and goti==2)){
	scr2data[player][gotinumber][2]+=45;scr2data[player][gotinumber][3]=1;}
	else if((steps==10 and goti==2) or (steps==23 and goti==1) or (steps==36 and goti==4)or (steps==49 and goti==3)){
	scr2data[player][gotinumber][1]+=45;scr2data[player][gotinumber][3]=2;}
	else if((steps==10 and goti==3) or (steps==23 and goti==2) or (steps==36 and goti==1)or (steps==49 and goti==4)){
	scr2data[player][gotinumber][2]-=45;scr2data[player][gotinumber][3]=3;}
	else if((steps==10 and goti==4) or (steps==23 and goti==3) or (steps==36 and goti==2)or (steps==49 and goti==1)){
	scr2data[player][gotinumber][1]-=45;scr2data[player][gotinumber][3]=4;}
	//dealing with going into win column
	else if(steps==50){
		if(goti==1){scr2data[player][gotinumber][2]+=45;scr2data[player][gotinumber][3]=1;}
		if(goti==2){scr2data[player][gotinumber][1]+=45;scr2data[player][gotinumber][3]=2;}
		if(goti==3){scr2data[player][gotinumber][2]-=45;scr2data[player][gotinumber][3]=3;}
		if(goti==4){scr2data[player][gotinumber][1]-=45;scr2data[player][gotinumber][3]=4;}}
	//dealing with turning right after win column
	else if((steps==12 and goti==1) or (steps==25 and goti==4) or (steps==38 and goti==3)){
	scr2data[player][gotinumber][1]+=45;scr2data[player][gotinumber][3]=2;}
	else if((steps==12 and goti==2) or (steps==25 and goti==1) or (steps==38 and goti==4)){
	scr2data[player][gotinumber][2]-=45;scr2data[player][gotinumber][3]=3;}
	else if((steps==12 and goti==3) or (steps==25 and goti==2) or (steps==38 and goti==1)){
	scr2data[player][gotinumber][1]-=45;scr2data[player][gotinumber][3]=4;}
	else if((steps==12 and goti==4) or (steps==25 and goti==3) or (steps==38 and goti==2)){
	scr2data[player][gotinumber][2]+=45;scr2data[player][gotinumber][3]=1;}
	//dealing with straight movement
	else if(scr2data[player][gotinumber][3]==1){scr2data[player][gotinumber][2]+=45;}
	else if(scr2data[player][gotinumber][3]==2){scr2data[player][gotinumber][1]+=45;}
	else if(scr2data[player][gotinumber][3]==3){scr2data[player][gotinumber][2]-=45;}
	else if(scr2data[player][gotinumber][3]==4){scr2data[player][gotinumber][1]-=45;}
	//setting location if goti has won
	if (steps==55){
	if (goti==1 or goti==3){scr2data[player][gotinumber][1]-=15-(gotinumber*10);}
	else if(goti==2 or goti==4){scr2data[player][gotinumber][2]+=15-(gotinumber*10);}
	}
	}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//setting location in base 
	int * ptr[8] = {&rboxcoord[0],&rboxcoord[1],&bboxcoord[0],&bboxcoord[1],&yboxcoord[0],&yboxcoord[1],&gboxcoord[0],&gboxcoord[1]};
	if (scr2data[player][gotinumber][0]==-1){
	for (int a=0;a<4;a++)
	if(goti==a+1){
	{if(gotinumber==0){scr2data[player][gotinumber][1]=*ptr[2*a]+75+22.5;scr2data[player][gotinumber][2]=*ptr[(2*a)+1]+150+22.5;}
	if(gotinumber==1){scr2data[player][gotinumber][1]=*ptr[2*a]+150+22.5;scr2data[player][gotinumber][2]=*ptr[(2*a)+1]+150+22.5;}
	if(gotinumber==2){scr2data[player][gotinumber][1]=*ptr[2*a]+75+22.5;scr2data[player][gotinumber][2]=*ptr[(2*a)+1]+75+22.5;}
	if(gotinumber==3){scr2data[player][gotinumber][1]=*ptr[2*a]+150+22.5;scr2data[player][gotinumber][2]=*ptr[(2*a)+1]+75+22.5;}
	}}
	
	
	}	
}




		/*--Function to create or remove any blocks on the board (of a certain player) according to new goti positions--*/
void update_blocks(int playernum){
	int block_count=0;
	for(int i=0;i<4;i++)if (block[playernum][i]>-1)block_count++;
	block[playernum][0]=-2;
	block[playernum][1]=-2;
	block[playernum][2]=-2;
	block[playernum][3]=-2;
	int gstep[4];
	for (int gotinum=0;gotinum<4;gotinum++)gstep[gotinum]=steps_to_gsteps(scr1data[playernum][1],scr2data[playernum][gotinum][0]);
	for (int goti1=0;goti1<4;goti1++){
		for(int goti2=0;goti2<4;goti2++){
			if (goti1!=goti2 and gstep[goti1]==gstep[goti2] and gstep[goti1]!=-1 and (gstep[goti1]%13)!=0 and (gstep[goti1]+5)%13!=0){
				if(block[playernum][0]==-2)block[playernum][0]=gstep[goti1];
				else block[playernum][1]=gstep[goti1];
				if (block[playernum][0]==block[playernum][1])block[playernum][1]=-2;	
			}
		}
	}
	if (teams){
	int gsteps[8];
	for (int gotinum=0;gotinum<4;gotinum++){gsteps[gotinum]=steps_to_gsteps(scr1data[playernum][1],scr2data[playernum][gotinum][0]);}
	for (int gotinum=4;gotinum<8;gotinum++){gsteps[gotinum]=steps_to_gsteps(scr1data[teammate[playernum]][1], scr2data[teammate[playernum]][gotinum-4][0]);}
						
	for (int goti1=0;goti1<4;goti1++){
		for(int goti2=4;goti2<8;goti2++){
			if (goti1!=goti2 and gsteps[goti1]==gsteps[goti2] and gsteps[goti1]!=-1 and (gsteps[goti1]%13)!=0 and (gsteps[goti1]+5)%13!=0){
				if(block[playernum][0]==-2)block[playernum][0]=gsteps[goti1];
				else if(block[playernum][1]==-2)block[playernum][1]=gsteps[goti1];
				else if(block[playernum][2]==-2)block[playernum][2]=gsteps[goti1];				
				else block[playernum][3]=gstep[goti1];
				for(int blocknum1=3;blocknum1>=0;blocknum1--)
					for(int blocknum2=0;blocknum2<blocknum1;blocknum2++)
						if (block[playernum][blocknum1]==block[playernum][blocknum2])block[playernum][blocknum1]=-2;	
			}
		}
	}
	}
	for(int i=0;i<4;i++)if (block[playernum][i]>-1)block_count--;
	if(playernum==turn){
	if (block_count==-1)scr2data[playernum][4][0]+=2;
	if (block_count==-2)scr2data[playernum][4][0]+=4;
	if (block_count==-3)scr2data[playernum][4][0]+=6;
	if (block_count==-4)scr2data[playernum][4][0]+=8;	
	}	
}




					/*--Function to set coords of all the gotis on board--*/
void all_goti_coord_setter(){
	for(int playernum=0;playernum<playercount;playernum++)
		for(int gotinum=0;gotinum<4;gotinum++)
		{
			goti_coord_setter(playernum,gotinum);
		}
}




					/*--Function to kill a goti if another goti steps on it--*/
void goti_kill(){
	int gsteps=steps_to_gsteps(scr1data[turn][1],scr2data[turn][gotiselect][0]);
	for(int player=0;player<playercount;player++)
	if (turn!=player and (!teams or player!=teammate[turn]))	
		for(int goti=0;goti<4;goti++)
			if(gsteps==steps_to_gsteps(scr1data[player][1],scr2data[player][goti][0]) and gsteps!=-1 and gsteps%13!=0 and ((gsteps+5)%13)!=0){
			event_controller(playername[player]+"'s goti killed");
			scr2data[player][goti][0]=-1;
			scr2data[turn][5][0]=1;
			if (teams)scr2data[teammate[turn]][5][0]=1;
			scr2data[turn][4][0]+=10;		
			}
}




				/*--Function to move a goti according to the dice selected and goti selected--*/
void goti_mover(){
	int loc;
	if (teams and won[turn]==2){
	loc=scr2data[teammate[turn]][gotiselect][0];
	if (scr2data[teammate[turn]][gotiselect][0]!=-1){
	scr2data[teammate[turn]][gotiselect][0]+=diceroll[diceselect];scr2data[turn][4][0]+=(1*diceroll[diceselect]);}
	else {scr2data[teammate[turn]][gotiselect][0]=0;scr2data[turn][4][0]+=(1*diceroll[diceselect]);}
	if (loc<51 and scr2data[teammate[turn]][gotiselect][0]>=51){scr2data[turn][4][0]+=15;}
	}
	else{
	loc=scr2data[turn][gotiselect][0];
	if (scr2data[turn][gotiselect][0]!=-1){
	scr2data[turn][gotiselect][0]+=diceroll[diceselect];scr2data[turn][4][0]+=(1*diceroll[diceselect]);}
	else {scr2data[turn][gotiselect][0]=0;scr2data[turn][4][0]+=(1*diceroll[diceselect]);}
	if (loc<51 and scr2data[turn][gotiselect][0]>=51){scr2data[turn][4][0]+=15;}}
	goti_kill();
	all_goti_coord_setter();
	diceroll[diceselect]=0;
	diceselect=-1;
	gotiselect=-1;
	glutPostRedisplay();

}




				/*--Function to check if a block is stopping movement of goti of current turn--*/
bool block_stop(int loc,int move){
	int color=scr1data[turn][1],landingspot;
	landingspot=loc+move;
	for(;landingspot!=loc;landingspot--){
		for(int playernum=0;playernum<playercount;playernum++){
		  for(int blocknum=0;blocknum<2;blocknum++){ 	
		      if (steps_to_gsteps(color,landingspot)==block[playernum][blocknum] and block[playernum][blocknum]!=-1 and playernum!=turn and playernum!=teammate[turn]){
		      	return true;}
		    }	
	}}
	return false;
}




				/*--Function to Check if A single goti can move a certain steps--*/
bool move_possible(int location,int movement){
	if (location==-1 and movement==6)return true;	
	else if (block_stop(location,movement)){if(scr2data[turn][5][1]==0)event_controller(">block is stopping<");return false;}	//block is stopping movement
	else if ((location+movement)>50 and scr2data[turn][5][0]==0){if(scr2data[turn][5][1]==0)event_controller(">you haven't killed yet<");return false;} //hasn't killed yet
	else if (location<=(56-movement) and location!=-1)return true;	//has killed 
	else if(location==-1){if(scr2data[turn][5][1]==0)event_controller(">you need a six<");return false;} //still inside base and no six in dice
	else {if(scr2data[turn][5][1]==0)event_controller(">too high number<");return false;}	//going out of boundary  
	
}




			/*--Function to check if turn should be skipped (as there might be no possible moves)--*/
bool turn_skip(){
	scr2data[turn][5][1]=1;
	if(teams and won[turn]==2){
	int goti_pos[4]={scr2data[teammate[turn]][0][0],scr2data[teammate[turn]][1][0],scr2data[teammate[turn]][2][0],scr2data[teammate[turn]][3][0]};
	for(int goti1=0;goti1<4;goti1++){
		if (move_possible(goti_pos[goti1],diceroll[0]) and diceroll[1]==0){scr2data[turn][5][1]=0;return false;}
		else if(move_possible(goti_pos[goti1],diceroll[0])){
			if(goti_pos[goti1]!=-1)goti_pos[goti1]+=diceroll[0];
			else goti_pos[goti1]=0;
			for(int goti2=0;goti2<4;goti2++){
				if(move_possible(goti_pos[goti2],diceroll[1]) and diceroll[2]==0){scr2data[turn][5][1]=0;return false;}
				else if(move_possible(goti_pos[goti2],diceroll[1])){
					if(goti_pos[goti2]!=-1)goti_pos[goti2]+=diceroll[1];
					else goti_pos[goti2]=0;
					for(int goti3=0;goti3<4;goti3++){
						if(move_possible(goti_pos[goti3],diceroll[2])){scr2data[turn][5][1]=0;return false;}
					}
				}
			}
		}
	}	
	}
	else{
	int goti_pos[4]={scr2data[turn][0][0],scr2data[turn][1][0],scr2data[turn][2][0],scr2data[turn][3][0]};
	for(int goti1=0;goti1<4;goti1++){
		if (move_possible(goti_pos[goti1],diceroll[0]) and diceroll[1]==0){scr2data[turn][5][1]=0;return false;}
		else if(move_possible(goti_pos[goti1],diceroll[0])){
			if(goti_pos[goti1]!=-1)goti_pos[goti1]+=diceroll[0];
			else goti_pos[goti1]=0;
			for(int goti2=0;goti2<4;goti2++){
				if(move_possible(goti_pos[goti2],diceroll[1]) and diceroll[2]==0){scr2data[turn][5][1]=0;return false;}
				else if(move_possible(goti_pos[goti2],diceroll[1])){
					if(goti_pos[goti2]!=-1)goti_pos[goti2]+=diceroll[1];
					else goti_pos[goti2]=0;
					for(int goti3=0;goti3<4;goti3++){
						if(move_possible(goti_pos[goti3],diceroll[2])){scr2data[turn][5][1]=0;return false;}
					}
				}
			}
		}
	}
	}
	scr2data[turn][5][1]=0;
	return true;
}




				/*--Function to set goti coords if there are multiple gotis on single box--*/
void multi_goti_setter(){
	for (int box=0,count=0,subtractx=0,subtracty=0;box<52;box++,count=0)
	{
		for(int player=0;player<playercount;player++){
			for(int goti=0;goti<4;goti++)
				if(steps_to_gsteps(scr1data[player][1],scr2data[player][goti][0])==box)count++;}
		if (count==1)continue;
		else if(count>1 and count<4)subtractx=-5;
		else if(count>3)subtractx=-10;
		if(count>4)subtracty=-7;
		for(int player=0,count1=0;count1<count;player++)
			for(int goti=0;goti<4;goti++)
				if(steps_to_gsteps(scr1data[player][1],scr2data[player][goti][0])==box){
					if(count<5){scr2data[player][goti][1]+=subtractx+(5*count1);count1++;}
					else if(count>4){
						if(count1<4){
						scr2data[player][goti][1]+=subtractx+(5*count1);count1++;
						scr2data[player][goti][2]+=subtracty;
						}else{
						scr2data[player][goti][1]+=subtractx+(5*(count1-4));count1++;
						scr2data[player][goti][2]-=subtracty;
						}
					}
				}
	}
	for(int box=51;box<56;box++){
		for (int player=0,count=0,subtractx=0;player<playercount;player++,count=0){
			for(int goti=0;goti<4;goti++){if(scr2data[player][goti][0]==box)count++;}
			if (count<2)continue;
			else if(count>1 and count<4)subtractx=-5;
			else if(count>3)subtractx=-10; 
			for(int goti=0;goti<4;goti++){	
				if(scr2data[player][goti][0]==box){scr2data[player][goti][1]+=subtractx+(count*5);count--;}
			}
		}
	}
}




					/*--Function To Display the Second Screen of Game--*/
void screen2(){
	
	
	//Setting coordinate values for gridboxes 
	
	for(int i=0;i<69;i++)
		for(int j=0;j<57;j++){
			gridcoord1[i][j][0]=(15*i);
			gridcoord1[i][j][1]=(15*j);}
	
	
	//Setting coordinate values for ludogridboxes 
	for(int i=0;i<15;i++)
		for(int j=0;j<15;j++)
		{
			ludogridcoord[i][j][0]=ludocoord[0]+(45*i);
			ludogridcoord[i][j][1]=ludocoord[1]+(45*j);
		}
		
	//Skipping turns	
	if(task==2 and diceroll[0]<=0 and diceroll[1]<=0 and diceroll[2]<=0){turnchanger();task=1;}
	if(task==2 and turn_skip()){event_controller(">no possible moves<");event_controller("press ENTER");task=3;}//skipping turn when no possible move
	if(task==2 and diceroll[0]==6 and diceroll[1]==6 and diceroll[2]==6){event_controller("three 6s, turn skipped");event_controller("press ENTER");task=3;} //skipping turn when three 6s}
	
	//Skipping turn if player won
	for (int player=0;player<playercount;player++){
		if (won[player]<1){
		won[player]=1;
		for(int gotinum=0;gotinum<4;gotinum++)
		if(scr2data[player][gotinum][0]!=56){won[player]=0;break;}
	}}
	
	//End game if only one player/team left
	if (gameEnd==0){
	if (!teams){
		for(int player=0,count=0;player<playercount;player++){
			if(won[player]==1){count++;}
			if((count+1)==playercount){gameEnd=1;}
		}
	}
	else{
		for(int player=0;player<playercount;player++){
			if(won[player]>=1 and won[teammate[player]]>=1){gameEnd=1;}
		}
	}
		if (gameEnd==1){event_controller(">Game Ended<");event_controller("Press ENTER to continue");}
	}
	
	//display ludoboard	
	ludoboard();
	nameandturn();
	scoreboard();
	dice_display();
	events_display();
	all_goti_coord_setter();
	update_blocks(0);
	update_blocks(1);
	update_blocks(2);
	update_blocks(3);
	blocks_display();
	multi_goti_setter();
	goti_display();
}
/*----------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------*/


