//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include <stdio.h>

CP_Image Background;
CP_Image Fox;
CP_Image Dice;
CP_Font Text;

#define MAX_DICE 10

float foxPosX = 960;
float foxPosY = 900;
float foxWidth = 105;
float foxHeight = 93;

float speed = 5;
float score = 0;

float timer = 0.0f;
int activeDice = 0;

float dicePosX[MAX_DICE] = {0};
float dicePosY[MAX_DICE] = {0};
float diceLeft[MAX_DICE] = { 0 };

float diceWidth = 71;
float diceHeight = 94;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void) {
	// initialize variables and CProcessing settings for this gamestate
	CP_System_SetWindowSize(1920, 1080);
	Text = CP_Font_Load("./Assets/LuckiestGuy-Regular.ttf");
	Background = CP_Image_Load("./Assets/Background.png");
	Fox = CP_Image_Load("./Assets/Fox.png");
	Dice = CP_Image_Load("./Assets/Dice.png");
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void) {
	// check input, update simulation, render etc.
	CP_Image_Draw(Background, 960, 540, 1920, 1080, 255);

	char buffer[50];
	sprintf_s(buffer, 50, "Score: %d", (int) score);

	CP_Font_Set(Text);
	CP_Settings_TextSize(100);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	
	CP_Font_DrawText(buffer, 100, 150);
	
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		if (foxPosX <= 1820)
		foxPosX += speed;
	}
	if (CP_Input_KeyDown(KEY_LEFT)) {
		if (foxPosX >= 100)
		foxPosX -= speed;
	}
	CP_Image_Draw(Fox, foxPosX, 900, 105, 93, 255);
	
	int spawnIndex = 0;
	for (int i = 0; i < MAX_DICE; i++) {
		if (!diceLeft[i]) {
			spawnIndex = i;
		}
	}

	timer -= CP_System_GetDt();
	if (timer <= 0 && activeDice < MAX_DICE) {
		dicePosX[activeDice] = CP_Random_RangeFloat(100, 1820);
		dicePosY[activeDice] = 0;
		diceLeft[activeDice] = 1;
		activeDice++;
		timer = 2.5f;
	}

	for (int i = 0; i < activeDice; i++) {
		if (diceLeft[i]) {
			CP_Image_Draw(Dice, dicePosX[i], dicePosY[i], 71, 94, 255);
			if (dicePosY[i] <= 900)
				dicePosY[i] += (float) speed * CP_System_GetDt() * 25;
			if (dicePosX[i] < foxPosX + foxWidth &&
				dicePosX[i] + diceWidth > foxPosX &&
				dicePosY[i] < 900 + foxHeight &&
				dicePosY[i] + diceHeight > 900) {
				score++;
				diceLeft[i] = 0;
			}
			if (dicePosY[i] >= 900) {
				diceLeft[i] = 0;
			}
		}
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void) {
	// shut down the gamestate and cleanup any dynamic memory
}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void) {
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
