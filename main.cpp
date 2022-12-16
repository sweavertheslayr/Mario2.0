#include "raylib.h"
#include <iostream>
#include <math.h>

struct Window {
	float width = 0;
	float height = 0;
	int mobCount = 0;

	int blocksWide = 0;
	float blockHeight = 0;
	float blockPadding = 8;
	int tempRenderX = 0;
	int renderPosX = 0;
	int renderPosY = 0;

	int currentLevel = 1;

	Font font;

	float dT = 0;
	float scale = 0;

	int renderPosDistance = 32;

	bool pause = false;
	bool levelSelect = false;
	bool exit = false;

	const char* title = "Super Mario Bros";
}window;

struct Sounds {
	Music runningAbout = LoadMusicStream("DevAssets/sounds/runningAbout.mp3");
	Music underground = LoadMusicStream("DevAssets/sounds/underground.mp3");
	Sound smallJump = LoadSound("DevAssets/sounds/smallJump.wav");
	Sound bigJump = LoadSound("DevAssets/sounds/bigJump.wav");
	Sound die = LoadSound("DevAssets/sounds/die.mp3");
}sound;

struct BadGuy {
	float mob = 0;
	float type = 0;

	float width = 16;
	float height = 16;

	int length = 0;

	bool flip = false;
	bool outShell = false;
	bool moving = false;

	float posX = 0;
	float posY = 2 * window.blockHeight;

	int iPosX = 0;
	int iPosXD = 0;
	int iPosY = 0;
	int scoreHit = 0;

	float velocity = 0;
	float speed = 0;
	float maxSpeed = 80;

	float startY = 0;

	Texture2D texture;

	float runningTime = 0;
	float updateTime = 1.0 / 6.0;

	float frame = 0;

	bool collideD = false;
	bool collideR = false;
	bool collideL = false;
	bool hostile = false;

	bool stationary = false;
	bool upDown = false;

	bool hit = false;

	bool direction = true;
	bool directionUp = true;

	bool isPlatform = false;

	bool gravity = false;
	bool blockCollide = false;

	bool loaded = true;
}reset, mob[30];

struct Block {
	Texture2D texture;
	Texture2D texture2;
	float selectedX = -1;
	float selectedY = -1;
	float shift = 0;
	float velocity = 0;
	float shiftHeight = window.blockHeight;
	float runningTime = 0;
	float updateTime = 1.0/4.0;
	float updateTime2 = 1.0/8.0;
	float frame = 6;
}block;

struct Player {
	//cords
	float posX = 3*window.blockHeight;
	float posY = 0;

	int tempPosX = 0;
	int tempPosY = 0;

	int platform = 0;

	int iPosX = 0;
	int iPosXL = 0;
	int iPosXD = 0;
	int iPosXLD = 0;
	int iPosXC = 0;
	int iPosY = 0;


	//stuff
	int score = 0;
	int lives = 3;
	int world = 1;
	int level = 1;
	int time = 400;
	int coins = 0;
	int streak = 1;
	int shellStreak = 1;

	//jump
	int tempVelocity = 0;
	int velocity = 0;
	int jumpVelocity = 1100;
	int sidewaysVelocity = 0;
	int runVelocity = 10;
	int maxSidewaysVelocity = 500;
	int slowVelocity = 5;
	bool moveRight = false;
	bool moveLeft = false;

	//collision
	bool collideR = false;
	bool collideL = false;
	bool collideU = false;
	bool collideD = true;
	bool collision = false;
	bool collidePlatform = false;
	bool bufferCollide = false;

	//animation
	float width = 32;
	float height = 64;
	int spriteHeight = 2;
	float currentSprite = 7;
	Texture2D texture;
	int frame = 7;
	float runningTime = 0;
	float updateTime = 1.0 / 6.0;
	bool direction = true;
	bool isRunning = false;
	bool isGrounded = true;
	bool justJumped = false;
	bool isTurning = false;
	bool isDucking = false;
	bool isHit = false;
	float tall = 0;
}player;

struct Scenery
{
	Texture2D texture;
	Texture2D texture2;
}scenery;

struct Levels {
	std::string current[32];
	std::string currentScene[32];

	int size = 18;

	int currentSize = 0;

	float type = 0;

	std::string oneA[30] = {
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                       o                                                          ________   ___o              o           ___    _oo_                                                        OO                                                                                               ",
	"                                                                                                                                                                                             OOO                                                                                               ",
	"                                                                                                                                                                                            OOOO                                                                                               ",
	"                                                                                                                                                                                           OOOOO                                                                                               ",
	"                 o   _o_o_                     tk         tk                   _o_              _     __    o  o  o     _          __      O  O          OO  O            __o_            OOOOOO                                                                                               ",
	"                                       tk      |h         |h                                                                              OO  OO        OOO  OO                          OOOOOOO                                                                                               ",
	"                             tk        |h      |h         |h                                                                             OOO  OOO      OOOO  OOO     tk              tk OOOOOOOO                       tk                                                                      ",
	"                             |h        |h      |h         |h                                                                            OOOO  OOOO    OOOOO  OOOO    |h              |hOOOOOOOOO        O              |h                                                                      ",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
	};

	std::string oneSceneA[30] = {
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                    7               0                               7                 0                              7                0                              7            0                                                                                                            ",
	"                             3                           7                   3                            7                  3                            7                  3                          j 7                                                                                    ",
	"                                                                                 G                                                                                                                     ki                                                                                      ",
	"                                                                                                               w                                                                                        o                                                                                      ",
	"                                                                                                                                                                                                        o                                                                                      ",
	"                                                                                                                                                                                                        o                                                                                      ",
	"                                                                                                                                                                                                        o    ,.m                                                                               ",
	"                      w                                                         w                                                                                                                       o                                                                                      ",
	"                                                                                                                                                                                                        o                                                                                      ",
	"                                                                                                                                                                                                        o   xcvcx                                                                              ",
	" 1          2    5     G6                G 9      1  G G     2    5       6                 9     1G G      K2    5 G G  6   G G  G G       9      1            6  5      6     G G                1     O                                                                                     ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               "
	};

	std::string twoA[30] = {
	"-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------",
	"-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	" #     #####################################################################################################################################                       ##########                                                                                                                  ",
	" #                                                                                                                                                                                                                                                                                             ",
	" #                                                                                                                                                                                                                                                                                             ",
	" #                                                                                                                                                                                                                                                                                             ",
	" #     #####################################################################################################################################                       #######|h#                                                                                                                  ",
	" #                                                      ##  ######  ####      ####                                                                                        |h#                                                                                                                  ",
	" #                                                      ##  ######  ####      ####                                                                                        |h#                                                                                                                  ",
	" #                                                    ##        ##   #    ##                                                                                              |h#                                                                                                                  ",
	" #                                                    ##        ##   #    ##                                                                                              |h#                                                                                                                  ",
	" #                                       # #### #     ##        ##   #    ##          ######                                                                              |h#                                                                                                                  ",
	" #                             #         # #  # #     ##        ##   # #  ##          ######                                                       ######               yedh#                                                                                                                  ",
	" #          ooooo        O O             ###  ###     ####  ######   ###  ##  ####                             tk                         OO                            sbwh#                                                                                                                  ",
	" #                     O O O O   O                      ##                                               tk    |h           ##           OOO                      ###########                                                                                                                  ",
	" #                   O O O O O   O O                    ##                                               |h    |h    tk     ##          OOOO                      ###########                                                                                                                  ",
	" #                 O O O O O O   O O                                                                     |h    |h    |h     ##         OOOOO                      ###########                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%  %%%%%%%%%%%%       %%%%%%%%       %%%%%%%%%%%                                                                                                                  ",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--%%--%%%%%%%%%%%%-------%%%%%%%%-------%%%%%%%%%%%------------------------------------------------------------------------------------------------------------------",
	" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%---%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--%%--%%%%%%%%%%%%-------%%%%%%%%-------%%%%%%%%%%%------------------------------------------------------------------------------------------------------------------"
	};
	std::string twoSceneA[30] = {
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                              Lqd            Lqu                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                          G                                                                   Lqd            Lqu                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                                ",
	"                                                                                                                                        G G                                                                                                                                                    ",
	"                                                                       w      G G                             P                                         w                                                                                                                                      ",
	"            w                                                                                           P                                                                                                                                                                                      ",
	"                                                                                                                    P                                                                                                                                                                          ",
	"                  G                                                                                                                                                                                                                                                                            ",
	"                G              G              K K            K  G G                                G G G           G                          Lqd   K        Lqu                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               ",
	"                                                                                                                                                                                                                                                                                               "
	};

	std::string threeA[30] = {
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                         r                    ",
	"                                         xaaaaav                                                                                                        nf                    ",
	"                           xaaav                             xaav                                                                             OO         l                    ",
	"                                                        ooo                  xaaav                                                ooo         OO         l                    ",
	"                                    xaaav                                                              xaaaaaav                             OOOO         l                    ",
	"                                                                                      ooo                                                   OOOO         l                    ",
	"                         xaaaaaav                                      xav                   ooo                   xaav  xaav             OOOOOO         l                    ",
	"                                                            o                                                                             OOOOOO         l                    ",
	"                                                                                                 xaav                                     OOOOOO         l                    ",
	"                   xaav          xav               xaav                                                                                   OOOOOO         O                    ",
	"%%%%%%%%%%%%%%%%%                                           xaaav xaaav                                         xav             %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%                                                                                                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%                                                                                                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%                                                                                                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%                                                                                                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%                                                                                                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%---------------------------------------------------------------------------------------------------------------%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"%%%%%%%%%%%%%%%%%---------------------------------------------------------------------------------------------------------------%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
	};

	std::string threeSceneA[30] = {
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                                                                                                                                                                              ",
	"                   0                                               0                                             0                                             ,.m            ",
	"    0                                               0                                             0                                                3                          ",
	"                                                                                                                                                                              ",
	"                                          ppppp                                                                                               OO              xcvcx           ",
	"                            ppp        7  ppppp               pp                                                                              OO                              ",
	"           7                ppp     7     ppppp               pp              ppp     7                                              7      OOOO                              ",
	"  ,.m                       ppp      ppp  ppppp           7   pp              ppp  7                    pppppp                    7         OOOO            cccvcvccccccccccccccc",
	"              G                      ppp  ppppp               pp              ppp                       pppppp                            OOOOOO                              ",
	"                          pppppp     ppp  ppppp               pp        p     ppp                       pppppp      pp    pp              OOOOOO                              ",
	" xcvcx                    pppppp     ppp  ppppp7              pp        p  7  ppp                       pppppp      pp    pp              OOOOOO            nnbnbnbnnnnnnnnnnnnnn",
	"                          pppppp     ppp  ppppp               pp        p     ppp            7    pp    pppppp      pp    pp7             OOOO7O         O                    ",
	"                    pp    pppppp  p  ppp  ppppp     pp                  p     ppp                 pp    pppppp      pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# ",
	"                    pp    pppppp  p  ppp  ppppp     pp       ppp   ppp  p     ppp                 pp    pppppp   p  pp    pp    ############################################# "
	};

	std::string fourA[30]  = {
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"################################################################################################################################################################################",
		"##############################             ##############################################         #       #        #########                   #######              ##       ###",
		"##############################             ##############################################         c       c        #########                   #######              ##       ###",
		"#                            #             ##############################################                                                                           ##       ###",
		"#                            c      o      c               c              c        c                                                                           --            ###",
		"#####                                                                                                                                                                        ###",
		"######                                                                                                                                                            <>         ###",
		"#######                                  ################################################     c       c       c                                                   /###       ###",
		"################  ##############   #c#   ###################################################################################           #####   #######OOOOOOOOOOOOO###       ###",
		"################  ##############   ###   ###################################################################################           #####   #######             ###       ###",
		"################  ##############   ###   ###################################################################################           #####   #######             ###       ###",
		"################  ##############   ###   #############################################################################################################             #############",
		"################  ##############   ###   #############################################################################################################             #############",
		"################  ##############   ###   #######################################################################################################################################",
		"################  ##############   ###   #######################################################################################################################################",
		"################  ##############   ###   #######################################################################################################################################",
		"################  ##############   ###   #######################################################################################################################################",
		"################  ##############   ###   #######################################################################################################################################",
		"################--##############---###---#######################################################################################################################################",
		"################--##############---###---#######################################################################################################################################"
	}; 
	
	std::string fourSceneA[30] = {
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                ",
		"                                                                                                                                                                                "
	};

	std::string pause[18] = {
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"##############################################",
	"##############################################",
	"##############################################",
	"##################OOOOOOOOOO##################",
	"##################O        O##################",
	"##################O q      O##################",
	"##################O        O##################",
	"##################O q      O##################",
	"##################O        O##################",
	"##################O q      O##################",
	"##################O        O##################",
	"##################O        O##################",
	"##################OOOOOOOOOO##################",
	"##############################################",
	"##############################################",
	"##############################################",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"
	};

}level;

void emptyArray(std::string arr[32])
{
	for (int i = 0; i < 32; i++)
	{
		arr[i] = "0";
	}
}

void setArray(int currentLevel)
{
	if (currentLevel == 1) { for (int i = 0; i < 30; i++) { level.current[i] = level.oneA[i]; level.currentScene[i] = level.oneSceneA[i]; } level.type = 0; player.world = 1; player.level = 1; }
	if (currentLevel == 2) { for (int i = 0; i < 30; i++) { level.current[i] = level.twoA[i];  level.currentScene[i] = level.twoSceneA[i]; } level.type = 1; player.world = 1; player.level = 2; }
	if (currentLevel == 3) { for (int i = 0; i < 30; i++) { level.current[i] = level.threeA[i];  level.currentScene[i] = level.threeSceneA[i]; } level.type = 0; player.world = 1; player.level = 3; }
	if (currentLevel == 4) { for (int i = 0; i < 30; i++) { level.current[i] = level.fourA[i];  level.currentScene[i] = level.fourSceneA[i]; } level.type = 2; player.world = 1; player.level = 4; }
}

void findSize(std::string arr[32])
{
	for (int i = 0; i < 32; i++)
	{
		if (arr[i] == "0")
		{
			level.currentSize = i - 3;
		}
	}
}

void outputPause()
{
	float a = 0;
	float b = 0;

	for (int i = 0; i < level.size; i++)
	{
		for (int j = 0; j < level.pause[0].length(); j++)
		{
			if (level.pause[i][j] == '#')
			{
				a = 0;
			}
			else if (level.pause[i][j] == 'q')
			{
				a = 2;
			}
			else if (level.pause[i][j] == 'O')
			{
				a = 5;
			}
			else if (level.pause[i][j] == 'o')
			{
				a = block.frame;
			}
			else
			{
				a = 17;
			}


			if (level.pause[i][j] == 'o' && block.runningTime >= block.updateTime)
			{
				block.frame += 1;
				if (block.frame > 8)
				{
					block.frame = 6;
				}

				a = block.frame;

				block.runningTime = 0;
			}

			if (a < 17)
			{
				DrawTexturePro(
					block.texture,
					Rectangle{ (level.type) * 16, ((16 * a) + 1), 16, 16 },
					Rectangle{ (j)*window.blockHeight - window.renderPosX, (i)*window.blockHeight - b, (32 * window.scale * 19 / 20), (32 * window.scale * 19 / 20) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
		}
	}
}

void levelSelection()
{
	float a = 0;
	float b = 0;

	for (int i = 0; i < level.size; i++)
	{
		for (int j = 0; j < level.pause[0].length(); j++)
		{
			if (level.pause[i][j] == '#')
			{
				a = 0;
			}
			else if (level.pause[i][j] == 'q')
			{
				a = 2;
			}
			else if (level.pause[i][j] == 'O')
			{
				a = 5;
			}
			else if (level.pause[i][j] == 'o')
			{
				a = block.frame;
			}
			else
			{
				a = 17;
			}


			if (level.pause[i][j] == 'o' && block.runningTime >= block.updateTime)
			{
				block.frame += 1;
				if (block.frame > 8)
				{
					block.frame = 6;
				}

				a = block.frame;

				block.runningTime = 0;
			}

			if (a < 17)
			{
				DrawTexturePro(
					block.texture,
					Rectangle{ (level.type) * 16, ((16 * a) + 1), 16, 16 },
					Rectangle{ (j)*window.blockHeight - window.renderPosX, (i)*window.blockHeight - b, (32 * window.scale * 19 / 20), (32 * window.scale * 19 / 20) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
		}
	}
}

void outputLevel()
{
	float a = 0;
	float b = 0;
	float c = 0;
	for (int i = 0; i < level.currentSize; i++)
	{
		for (int j = 1; j < level.current[0].length(); j++)
		{
			c = 0;
			if (level.currentScene[i][j] == '1')
			{
				a = 0;
				b = 0;
				c = 0;
			}
			else if (level.currentScene[i][j] == '2')
			{
				a = 1;
				b = 0;
				c = 0;
			}
			else if (level.currentScene[i][j] == '3')
			{
				a = 2;
				b = 0;
				c = 0;
			}
			else if (level.currentScene[i][j] == '4')
			{
				a = 3;
				b = 0;
				c = 0;
			}
			else if (level.currentScene[i][j] == '5')
			{
				a = 0;
				b = 1;
				c = 0;
			}
			else if (level.currentScene[i][j] == '6')
			{
				a = 1;
				b = 1;
				c = 0;
			}
			else if (level.currentScene[i][j] == '7')
			{
				a = 2;
				b = 1;
				c = 0;
			}
			else if (level.currentScene[i][j] == '8')
			{
				a = 3;
				b = 1;
				c = 0;
			}
			else if (level.currentScene[i][j] == '9')
			{
				a = 1;
				b = 2;
				c = 0;
			}
			else if (level.currentScene[i][j] == '0')
			{
				a = 2;
				b = 2;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'p')
			{
				a = 0;
				b = 2;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'i')
			{
				a = 0;
				b = 3;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'o')
			{
				a = 1;
				b = 3;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'j')
			{
				a = 2;
				b = 3;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'k')
			{
				a = 3;
				b = 3;
				c = 0;
			}
			else if (level.currentScene[i][j] == 'x')
			{
				a = 0;
				c = 1;
			}
			else if (level.currentScene[i][j] == 'c')
			{
				a = 1;
				c = 1;
			}
			else if (level.currentScene[i][j] == 'v')
			{
				a = 2;
				c = 1;
			}
			else if (level.currentScene[i][j] == 'b')
			{
				a = 3;
				c = 1;
			}
			else if (level.currentScene[i][j] == 'n')
			{
				a = 4;
				c = 1;
			}
			else if (level.currentScene[i][j] == 'm')
			{
				a = 5;
				c = 1;
			}
			else if (level.currentScene[i][j] == ',')
			{
				a = 6;
				c = 1;
			}
			else if (level.currentScene[i][j] == '.')
			{
				a = 7;
				c = 1;
			}
			else
			{
				a = 8;
			}
			if (c == 0 && a < 7)
			{
				DrawTexturePro(
					scenery.texture,
					Rectangle{ (80 * a), b * 35, 80, 35 },
					Rectangle{ (j)*window.blockHeight - (window.blockHeight * 1) - window.renderPosX, (i - 1)*window.blockHeight - (8 * window.scale) - ((level.currentSize - (level.currentSize - 7)) * window.blockHeight), (160 * window.scale), (70 * window.scale) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
			else if (a < 8)
			{
				DrawTexturePro(
					scenery.texture2,
					Rectangle{ (16 * a), 0, 17, 48 },
					Rectangle{ (j)*window.blockHeight - (window.blockHeight * 1) - window.renderPosX, (i - 1) * window.blockHeight - ((level.currentSize - (level.currentSize - 7)) * window.blockHeight), (32 * window.scale), (96 * window.scale) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
		}
	}

	block.shift += block.velocity * window.dT;

	if (block.shift > 0)
	{
		block.velocity -= 320 * window.dT;
	}
	else
	{
		block.velocity = 0;
	}

	if (block.velocity == 0)
	{
		block.selectedX = -1;
		block.selectedY = -1;
	}

	for (int i = 0; i < level.currentSize; i++)
	{
		for (int j = 1; j < level.current[0].length(); j++)
		{
			if (level.current[i][j] == '#')
			{
				a = 0;
			}
			else if (level.current[i][j] == '_')
			{
				a = 1;
			}
			else if (level.current[i][j] == 'q')
			{
				a = 2;
			}
			else if (level.current[i][j] == 'c')
			{
				a = 3;
			}
			else if (level.current[i][j] == '%')
			{
				a = 4;
			}
			else if (level.current[i][j] == 'O')
			{
				a = 5;
			}
			else if (level.current[i][j] == 'o')
			{
				a = block.frame;
			}
			else if (level.current[i][j] == 't')
			{
				a = 9;
			}
			else if (level.current[i][j] == 'k')
			{
				a = 10;
			}
			else if (level.current[i][j] == '|')
			{
				a = 11;
			}
			else if (level.current[i][j] == 'h')
			{
				a = 12;
			}
			else if (level.current[i][j] == 'y')
			{
				a = 13;
			}
			else if (level.current[i][j] == 's')
			{
				a = 14;
			}
			else if (level.current[i][j] == 'd')
			{
				a = 15;
			}
			else if (level.current[i][j] == 'w')
			{
				a = 16;
			}
			else if (level.current[i][j] == 'e')
			{
				a = 17;
			}
			else if (level.current[i][j] == 'b')
			{
				a = 18;
			}
			else if (level.current[i][j] == 'l')
			{
				a = 19;
			}
			else if (level.current[i][j] == 'f')
			{
				a = 20;
			}
			else if (level.current[i][j] == 'n')
			{
				a = 21;
			}
			else if (level.current[i][j] == 'r')
			{
				a = 22;
			}
			else if (level.current[i][j] == 'x')
			{
				a = 23;
			}
			else if (level.current[i][j] == 'a')
			{
				a = 24;
			}
			else if (level.current[i][j] == 'v')
			{
				a = 25;
			}
			else
			{
				a = 26;
			}


			if (level.current[i][j] == 'o' && block.runningTime >= block.updateTime)
			{
				block.frame += 1;
				if (block.frame > 8)
				{
					block.frame = 6;
				}

				a = block.frame;
			
				block.runningTime = 0;
			}

			if (i == block.selectedY && j == block.selectedX)
			{
				b = block.shift;
			}
			else
			{
				b = 0;
			}

			if (a < 26)
			{
				DrawTexturePro(
					block.texture,
					Rectangle{ (level.type) * 16, ((16 * a) + 1), 16, 16 },
					Rectangle{ (j) * window.blockHeight - (window.blockHeight * 1) - window.renderPosX, ((i) * window.blockHeight - b - ((level.currentSize - (level.currentSize - 7)) * window.blockHeight)), (32 * window.scale * 19/20), (32 * window.scale * 19/20) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
		}
	}
}

void outputPipes()
{
	float a = 0;


	for (int i = 0; i < level.currentSize; i++)
	{
		for (int j = 1; j < level.current[0].length(); j++)
		{
			if (level.current[i][j] == 't')
			{
				a = 9;
			}
			else if (level.current[i][j] == 'k')
			{
				a = 10;
			}
			else if (level.current[i][j] == '|')
			{
				a = 11;
			}
			else if (level.current[i][j] == 'h')
			{
				a = 12;
			}
			else
			{
				a = 26;
			}

			if (a < 26)
			{
				DrawTexturePro(
					block.texture,
					Rectangle{ (level.type) * 16, ((16 * a) + 1), 16, 16 },
					Rectangle{ (j)*window.blockHeight - (window.blockHeight * 1) - window.renderPosX, ((i)*window.blockHeight - ((level.currentSize - (level.currentSize - 7)) * window.blockHeight)), (32 * window.scale * 19 / 20), (32 * window.scale * 19 / 20) },
					Vector2{ 0, 0 },
					0,
					WHITE);
			}
		}
	}
}

void restartLevel()
{
	emptyArray(level.current);
	emptyArray(level.currentScene);
	setArray(window.currentLevel);
	findSize(level.current);
	window.renderPosX = 0;

	for (int i = 0; i < window.mobCount; i++)
	{
		mob[i] = reset;
	}

	window.mobCount = 0;

	player.posX = window.blockHeight * 4;

	player.posY = window.blockHeight * 7;

	player.collision = false;
}

void outputPlatform(float type, int i, int length)
{
	for (int j = 0; j < length; j++)
	{
		DrawTexturePro(
			mob[i].texture,
			Rectangle{ 0, type * 8, 16, 8 },
			Rectangle{ mob[i].posX - window.renderPosX - ((1 + j) * window.blockHeight), mob[i].posY - (7 * window.blockHeight), 32 * window.scale, 16 * window.scale },
			Vector2{ 0, 0 },
			0,
			WHITE);
	}
}

int main()
{
	InitWindow(window.width, window.height, window.title);
	ToggleFullscreen();
	int a = 1;
	int pauseMenu = 0;
	InitAudioDevice();

	window.font = LoadFont("DevAssets/super-mario-bros-nes.ttf");
	player.texture = LoadTexture("DevAssets/marioLargeRunSheet.png");
	block.texture = LoadTexture("DevAssets/blockSheet.png");
	scenery.texture = LoadTexture("DevAssets/sceneryOneSheet.png");
	scenery.texture2 = LoadTexture("DevAssets/castleSheet.png");
	SetSoundVolume(sound.bigJump, 0.25f);
	SetSoundVolume(sound.smallJump, 0.25f);
	SetSoundVolume(sound.die, 0.25f);

	window.width = GetScreenWidth();
	window.height = GetScreenHeight();

	emptyArray(level.current);
	emptyArray(level.currentScene);
	setArray(window.currentLevel);
	findSize(level.current);

	window.scale = (window.height / (508 + ((level.size - 16) * 16)));
	window.blockHeight = window.height / level.size;
	window.blocksWide = window.width / window.blockHeight;
	block.shiftHeight = window.blockHeight * 0.9;

	player.posX = window.blockHeight * 4;

	player.posY = window.blockHeight * 7;
	SetTargetFPS(60);
	const float gravity = 2200;
	SetExitKey(KEY_Y);

	float winTime = 400;
	while (!window.exit)
	{
		UpdateMusicStream(sound.runningAbout);
		window.dT = GetFrameTime();

		BeginDrawing();

		(level.type == 0) ? ClearBackground(Color{ 92, 148, 252, 255 }) : ClearBackground(Color{ BLACK });

		player.runningTime += window.dT;
		block.runningTime += window.dT;
		winTime -= 2 * window.dT;
		player.time = winTime;

		//MAKE MOBS
		if (!window.pause && !window.exit)
		{
			for (int i = 0; i < 30; i++)
			{
				for (int j = (window.renderPosDistance); j < window.blocksWide + window.renderPosX / window.blockHeight; j++)
				{

					//goombas
					if (level.currentScene[i][j] == 'G')
					{
						mob[window.mobCount].texture = LoadTexture("DevAssets/mobSheet.png");
						level.currentScene[i][j] = ' ';
						mob[window.mobCount].posX = (j + 2) * window.blockHeight;
						mob[window.mobCount].posY = (i)*window.blockHeight;
						mob[window.mobCount].mob = 0;
						mob[window.mobCount].type = level.type;
						mob[window.mobCount].hostile = true;
						mob[window.mobCount].startY = i * window.blockHeight;
						mob[window.mobCount].direction = true;
						mob[window.mobCount].stationary = false;
						mob[window.mobCount].upDown = false;
						mob[window.mobCount].isPlatform = false;
						mob[window.mobCount].flip = false;
						mob[window.mobCount].gravity = true;
						mob[window.mobCount].blockCollide = true;
						mob[window.mobCount].outShell = true;
						window.mobCount += 1;
					}
					//koopas
					else if (level.currentScene[i][j] == 'K')
					{
						mob[window.mobCount].texture = LoadTexture("DevAssets/mobSheet.png");
						level.currentScene[i][j] = ' ';
						mob[window.mobCount].posX = (j + 2) * window.blockHeight;
						mob[window.mobCount].posY = (i)*window.blockHeight;
						mob[window.mobCount].mob = 3;
						mob[window.mobCount].type = level.type;
						mob[window.mobCount].hostile = true;
						mob[window.mobCount].startY = i * window.blockHeight;
						mob[window.mobCount].direction = true;
						mob[window.mobCount].stationary = false;
						mob[window.mobCount].upDown = false;
						mob[window.mobCount].isPlatform = false;
						mob[window.mobCount].flip = false;
						mob[window.mobCount].gravity = true;
						mob[window.mobCount].blockCollide = true;
						mob[window.mobCount].outShell = true;
						window.mobCount += 1;
					}
					//tube thing 
					else if (level.currentScene[i][j] == 'P')
					{
						mob[window.mobCount].texture = LoadTexture("DevAssets/standEnemySheet.png");
						level.currentScene[i][j] = ' ';
						mob[window.mobCount].posX = (j + 2.48) * window.blockHeight;
						mob[window.mobCount].posY = (i)*window.blockHeight;
						mob[window.mobCount].mob = 0;
						mob[window.mobCount].velocity = 0;
						mob[window.mobCount].type = level.type;
						mob[window.mobCount].hostile = true;
						mob[window.mobCount].startY = i * window.blockHeight;
						mob[window.mobCount].direction = true;
						mob[window.mobCount].stationary = true;
						mob[window.mobCount].upDown = true;
						mob[window.mobCount].isPlatform = false;
						mob[window.mobCount].flip = false;
						mob[window.mobCount].gravity = false;
						mob[window.mobCount].blockCollide = false;
						mob[window.mobCount].outShell = true;
						window.mobCount += 1;
					}
					//mushroom
					else if (level.currentScene[i][j] == 'M')
					{
						mob[window.mobCount].texture = LoadTexture("DevAssets/mobSheet.png");
						level.currentScene[i][j] = ' ';
						mob[window.mobCount].posX = (j + 2) * window.blockHeight;
						mob[window.mobCount].posY = (i)*window.blockHeight;
						mob[window.mobCount].mob = 6;
						mob[window.mobCount].type = level.type;
						mob[window.mobCount].hostile = false;
						mob[window.mobCount].startY = i * window.blockHeight;
						mob[window.mobCount].direction = false;
						mob[window.mobCount].stationary = false;
						mob[window.mobCount].upDown = false;
						mob[window.mobCount].isPlatform = false;
						mob[window.mobCount].flip = false;
						mob[window.mobCount].gravity = true;
						mob[window.mobCount].blockCollide = true;
						mob[window.mobCount].outShell = true;
						window.mobCount += 1;
					}
					//platform
					else if (level.currentScene[i][j] == 'L')
					{
						mob[window.mobCount].texture = LoadTexture("DevAssets/platformSheet.png");
						level.currentScene[i][j] = ' ';
						mob[window.mobCount].posX = (j + 2) * window.blockHeight;
						mob[window.mobCount].posY = (i)*window.blockHeight;
						mob[window.mobCount].mob = 6;
						mob[window.mobCount].type = level.type;
						mob[window.mobCount].hostile = false;
						mob[window.mobCount].startY = i * window.blockHeight;
						mob[window.mobCount].direction = false;
						mob[window.mobCount].stationary = true;
						mob[window.mobCount].upDown = false;
						mob[window.mobCount].isPlatform = true;
						mob[window.mobCount].gravity = false;
						mob[window.mobCount].flip = false;
						mob[window.mobCount].blockCollide = false;
						mob[window.mobCount].outShell = true;


						if (level.currentScene[i][j + 2] == 'u')
						{
							mob[window.mobCount].directionUp = true;
						}
						else if (level.currentScene[i][j + 2] == 'd')
						{
							mob[window.mobCount].directionUp = false;
						}

						if (level.currentScene[i][j + 1] == 'q')
						{
							mob[window.mobCount].length = 3;
						}
						window.mobCount += 1;
					}
				}
			}
		}

		for (int i = 0; i < window.mobCount; i++)
		{
			if (mob[i].loaded && !window.pause)
			{
				//POSITION
				mob[i].posX += mob[i].speed * window.dT;
				mob[i].posY += mob[i].velocity * window.dT;
				mob[i].runningTime += window.dT;

				if (mob[i].stationary)
				{
					mob[i].maxSpeed = 0;
				}
				else if (mob[i].hostile && !mob[i].moving)
				{
					mob[i].maxSpeed = 80;
				}
				else if (mob[i].moving)
				{
					mob[i].maxSpeed = 800;
				}
				else
				{
					mob[i].maxSpeed = 160;
				}

				mob[i].iPosX = ((mob[i].posX) / window.blockHeight);
				mob[i].iPosXD = ((mob[i].posX) / window.blockHeight) - 0.5;

				mob[i].iPosY = (mob[i].posY / window.blockHeight);

				//COLLISION

				if (mob[i].blockCollide)
				{
					//down
					if (level.current[mob[i].iPosY + 1][mob[i].iPosXD] == ' ')
					{
						mob[i].collideD = false;
					}
					else
					{
						if (mob[i].iPosY + 1 == block.selectedY && mob[i].iPosXD == block.selectedX && mob[i].hostile && !mob[i].hit)
						{
							mob[i].flip = true;
							mob[i].hit = true;
							player.score += 100 * player.streak;
							mob[i].scoreHit = 100 * player.streak;
						}

						mob[i].collideD = true;
						mob[i].posY = mob[i].iPosY * window.blockHeight;
					}

					//right
					if (level.current[mob[i].iPosY][mob[i].iPosX] != ' ' || (level.current[mob[i].iPosY - 1][mob[i].iPosX] != ' ' && mob[i].mob == 3 && !mob[i].moving))
					{
						mob[i].direction = true;
					}

					//left
					if (mob[i].iPosX == 0)
					{
						mob[i].loaded = false;
						if (mob[i].moving)
						{
							player.shellStreak = 1;
						}
					}
					else
					{
						if (level.current[mob[i].iPosY][mob[i].iPosX - 1] != ' ' || (level.current[mob[i].iPosY - 1][mob[i].iPosX - 1] != ' ' && mob[i].mob == 3 && !mob[i].moving))
						{
							mob[i].direction = false;
						}
					}
				}


				//CHARACTER COLLISION
				if (mob[i].hostile)
				{
					//player
					if (!mob[i].hit || !mob[i].outShell)
					{
						Rectangle boxCollider{ mob[i].posX - (0.6 * window.blockHeight), mob[i].posY - ((4.0 - mob[i].stationary) * window.blockHeight) - 8, mob[i].width * window.scale * 0.4, mob[i].height * window.scale };
						Rectangle playerCollider{ player.posX + window.renderPosX, player.posY + (!player.tall * 32), player.width * window.scale, player.height * window.scale };

						if (CheckCollisionRecs(boxCollider, playerCollider))
						{
							if (!mob[i].outShell && !mob[i].moving && mob[i].mob == 3)
							{
								mob[i].moving = true;
								mob[i].maxSpeed = 800;
								mob[i].runningTime = 0;
								if (player.posX + 1.4 * window.blockHeight > mob[i].posX)
								{
									mob[i].direction = true;
								}
								else
								{
									mob[i].direction = false;
								}
							}
							else if (!mob[i].outShell)
							{
								mob[i].moving = false;
								mob[i].maxSpeed = 0;
								player.shellStreak = 1;
							}
							else if (!mob[i].hit)
							{
								mob[i].hit = true;
								player.score += 100 * player.streak;
								mob[i].scoreHit = 100 * player.streak;
								player.streak += 1;
								player.velocity = player.jumpVelocity;
								player.velocity = player.jumpVelocity;
							}
						}
						else if ((mob[i].outShell || (mob[i].moving && mob[i].runningTime > 2 * mob[i].updateTime)))
						{
							Rectangle boxCollider{ mob[i].posX - (0.8 * window.blockHeight), mob[i].posY - ((3.4 + mob[i].stationary) * window.blockHeight) - 8, mob[i].width * window.scale * 1.2, mob[i].height * window.scale * 0.1 };
							Rectangle playerCollider{ player.posX + window.renderPosX, player.posY + (!player.tall * 32), player.width * window.scale, player.height * window.scale };
						
							if (CheckCollisionRecs(boxCollider, playerCollider))
							{
								player.collision = true;
							}
						}
					}
				}
				else if (mob[i].isPlatform)
				{
					Rectangle boxCollider{ mob[i].posX - (2 * window.blockHeight), mob[i].posY - (5 * window.blockHeight) + 9, (32 * window.scale) + ((mob[i].length - 1) * window.blockHeight), 4 * window.scale };
					Rectangle playerCollider{ player.posX + window.renderPosX, player.posY, player.width * window.scale, player.height / 2 * window.scale };

					if (CheckCollisionRecs(boxCollider, playerCollider))
					{
						if (player.velocity > 0)
						{
							player.velocity /= -2;
						}
					}
					else
					{
						Rectangle playerCollider{ player.posX + window.renderPosX, player.posY + (32 * window.scale), player.width * window.scale, player.height / 2 * window.scale };

						if (CheckCollisionRecs(boxCollider, playerCollider) && (!player.bufferCollide))
						{
							player.platform = i;
							player.collidePlatform = true;


							if (mob[i].directionUp)
							{
								player.velocity = 80;
							}
							else
							{
								player.velocity = -80;
							}

							player.posY = mob[i].posY - (7 * window.blockHeight) + 2;
						}
						else if (i == player.platform)
						{
							player.collidePlatform = false;
							player.bufferCollide = false;
						}
					}
				}
				else if (!mob[i].hit)
				{
					Rectangle boxCollider{ mob[i].posX - (1 * window.blockHeight), mob[i].posY - (2.5 * window.blockHeight), mob[i].width * window.scale * 2, mob[i].height * window.scale };
					Rectangle playerCollider{ player.posX + window.renderPosX, player.posY + (!player.tall * 32) + (2 * window.blockHeight), player.width * window.scale, player.height * player.spriteHeight * window.scale };

					if (CheckCollisionRecs(boxCollider, playerCollider))
					{
						player.tall = 1;
						player.score += 1000;
						mob[i].scoreHit = 1000;
						mob[i].hit = true;
					}
				}

				//other mobs
				Rectangle boxCollider{ mob[i].posX - (1 * window.blockHeight), mob[i].posY - (4 * window.blockHeight) - 8, mob[i].width * window.scale * 2, mob[i].height * window.scale * 2 };
				for (int j = 0; j < window.mobCount; j++)
				{
					if (mob[j].hostile)
					{
						Rectangle boxCollider2{ mob[j].posX - (1 * window.blockHeight), mob[j].posY - (4 * window.blockHeight) - 8, mob[j].width * window.scale, mob[j].height * window.scale * 2 };
						if (CheckCollisionRecs(boxCollider, boxCollider2) && mob[j].loaded)
						{
							if (!mob[i].outShell && mob[i].speed != 0)
							{
								if (!mob[j].hit)
								{
									player.score += 100 * player.shellStreak;
									mob[j].scoreHit = 100 * player.shellStreak;
									player.shellStreak += 1;
								}

								mob[j].hit = true;
							}
							else if (!mob[j].outShell && mob[j].speed != 0)
							{
								if (!mob[i].hit)
								{
									player.score += 100 * player.shellStreak;
									mob[i].scoreHit = 100 * player.shellStreak;
									player.shellStreak += 1;
								}

								mob[i].hit = true;
							}
							else
							{
								if (mob[i].posX < mob[j].posX)
								{
									mob[i].direction = true;
									mob[j].direction = false;
								}
								if (mob[i].posX > mob[j].posX)
								{
									mob[i].direction = false;
									mob[j].direction = true;
								}
							}
						}
					}
				}

				//MOVEMENT

				if (mob[i].gravity)
				{
					//down
					if (!mob[i].collideD)
					{
						mob[i].velocity += gravity * window.dT;
					}
					else
					{
						mob[i].velocity = 0;
					}

					//right + left
					if (mob[i].direction)
					{
						mob[i].speed = -mob[i].maxSpeed;
					}
					else
					{
						mob[i].speed = mob[i].maxSpeed;
					}
				}
				//updown
				else if (mob[i].upDown)
				{
					if (mob[i].startY >= mob[i].posY)
					{
						mob[i].velocity = 40;
					}
					else if (mob[i].startY + (2 * window.blockHeight) <= mob[i].posY)
					{
						mob[i].velocity = -40;
					}
				}
				else if (mob[i].isPlatform)
				{
					if (mob[i].directionUp)
					{
						mob[i].velocity = -120;

						if (mob[i].iPosY < 4)
						{
							mob[i].posY = 28 * window.blockHeight;
						}
					}
					else
					{
						mob[i].velocity = 120;

						if (mob[i].iPosY > 28)
						{
							mob[i].posY = 4 * window.blockHeight;
						}
					}
				}

				//ANIMATE
				if (mob[i].runningTime >= mob[i].updateTime && !mob[i].hit)
				{
					if (mob[i].direction)
					{
						mob[i].frame++;

						if (mob[i].frame > 1)
						{
							mob[i].frame = 0;
						}
					}
					else
					{
						mob[i].frame++;

						if (mob[i].frame > 3)
						{
							mob[i].frame = 2;
						}
					}
					mob[i].runningTime = 0;
				}
				else if (mob[i].hit && mob[i].runningTime >= 4 * mob[i].updateTime && mob[i].mob != 3)
				{
					mob[i].loaded = false;
				}
				else if (mob[i].hit && mob[i].runningTime >= 2 * mob[i].updateTime)
				{
					mob[i].outShell = false;
				}
				else if (!mob[i].outShell && mob[i].runningTime >= 60 * mob[i].updateTime)
				{
					mob[i].hit = false;
					player.shellStreak = 1;
				}
				if (mob[i].hit && mob[i].loaded && !mob[i].moving)
				{
					mob[i].frame = 4;
					mob[i].speed = 0;
					mob[i].velocity = 0;
				}
			}
		}


		//PLAYER INFO
		player.posY -= player.velocity * window.dT;
		player.posX += player.sidewaysVelocity * window.dT;
		player.iPosX = (player.posX - (4 * window.scale)) / window.blockHeight + (window.renderPosX / window.blockHeight) + 1;
		player.iPosXD = (player.posX - (8 * window.scale)) / window.blockHeight + (window.renderPosX / window.blockHeight) + 1;
		player.iPosXL = (player.posX - (28 * window.scale)) / window.blockHeight + (window.renderPosX / window.blockHeight) + 1;
		player.iPosXLD = (player.posX - (24 * window.scale)) / window.blockHeight + (window.renderPosX / window.blockHeight) + 1;
		player.iPosXC = (player.posX - (16 * window.scale)) / window.blockHeight + (window.renderPosX / window.blockHeight) + 1;


		window.renderPosDistance = player.iPosX - (player.posX / window.blockHeight) + 4;

		player.iPosY = (player.posY) / window.blockHeight;

		if (player.tall == 0 || player.isDucking)
		{
			player.spriteHeight = 1;
		}
		else
		{
			player.spriteHeight = 2;
		}

		//COLLISION

		//up
		if (level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == ' ')
		{
			player.collideU = false;
		}
		else
		{
			player.collideU = true;

			if (player.velocity > 0)
			{
				player.velocity /= -2;
			}
		}

		//down
		if ((level.current[player.iPosY + (level.currentSize - 21)][player.iPosXD] == ' ' && level.current[player.iPosY + (level.currentSize - 21)][player.iPosXLD] == ' '))
		{
			player.collideD = false;
			player.isGrounded = false;
		}
		else
		{
			player.streak = 1;
			player.collideD = true;
			player.isGrounded = true;
		}

		//right
		if ((level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosX] != ' ') || (level.current[player.iPosY - 1 + (level.currentSize - 21)][player.iPosX] != ' ') && !player.collideU)
		{
			player.collideR = true;
		
			if (!player.collideU && player.sidewaysVelocity >= 0)
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosXL - 1) * window.blockHeight) - (window.renderPosX) + (34.5 * window.scale);
			}
		}
		else if (player.posX >= 15000000)
		{
			player.collideR = true;

			if (!player.collideU && player.sidewaysVelocity >= 0)
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosXL - 1) * window.blockHeight) - (window.renderPosX) + (34.5 * window.scale);
			}
		}
		else
		{
			player.collideR = false;
		}

		//left
		if ((level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXL] != ' ') || (level.current[player.iPosY - 1 + (level.currentSize - 21)][player.iPosXL] != ' ') && !player.collideU)
		{
			player.collideL = true;

			if ((!player.collideU && player.sidewaysVelocity <= 0))
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosX - 1) * window.blockHeight) - (window.renderPosX) + (27 * window.scale);
			}
		}
		else if (player.iPosXL <= 0)
		{
			player.collideL = true;

			if ((!player.collideU && player.sidewaysVelocity <= 0))
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosX - 1) * window.blockHeight) - (window.renderPosX) + (27 * window.scale);
			}
		}
		else
		{
			player.collideL = false;
		}
		
		//right + falling
		if (!player.isGrounded && (level.current[player.iPosY + (level.currentSize - 21)][player.iPosX] != ' ') && player.sidewaysVelocity >= 0)
		{
			player.collideR = true;

			if (!player.moveLeft)
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosXL - 1) * window.blockHeight) - (window.renderPosX) + (34.5 * window.scale);
			}
		}
		else
		{
			player.collideR = false;
		}
		
		//left + falling
		if (!player.isGrounded && (level.current[player.iPosY + (level.currentSize - 21)][player.iPosXL] != ' ') && player.sidewaysVelocity <= 0)
		{
			player.collideL = true;

			if (!player.moveRight)
			{
				player.sidewaysVelocity = 0;
				player.posX = ((player.iPosX - 1) * window.blockHeight) - (window.renderPosX) + (27 * window.scale);
			}
		}
		else
		{
			player.collideL = false;
		}


		//MOVEMENT

		//up
		if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_SPACE)) && !window.pause)
		{
			player.justJumped = false;
		}

		if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && !player.collideU)
		{
			if ((player.isGrounded || player.collidePlatform) && !player.justJumped)
			{
				player.velocity += player.jumpVelocity;
				player.isGrounded = false;

				if (player.collidePlatform)
				{
					player.bufferCollide = true;
					player.collidePlatform = false;
				}
				if (player.tall)
				{
					PlaySoundMulti(sound.bigJump);
				}
				else
				{
					PlaySoundMulti(sound.smallJump);
				}
			}
			player.justJumped = true;
		}
		else
		{
			player.velocity -= 4000 * window.dT;
		}

		//down
		if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && !window.pause && player.tall == 1)
		{
			player.isDucking = true;
			player.spriteHeight = 1;

			if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)))
			{
				player.direction = true;
			}
			else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)))
			{
				player.direction = false;
			}

			if (player.direction)
			{
				player.currentSprite = 13;
			}
			else
			{
				player.currentSprite = 0;
			}
		}
		else
		{
			player.isDucking = false;
		}

		//sprint
		if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
		{
			player.maxSidewaysVelocity = 600;
			player.runVelocity = 15;
		}
		else
		{
			player.maxSidewaysVelocity = 500;
			player.runVelocity = 10;
		}

		//right
		if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_A) && !player.isDucking && !window.pause)
		{
			player.direction = true;
			player.isRunning = true;
			player.moveRight = true;

			if (player.sidewaysVelocity <= player.maxSidewaysVelocity)
			{
				player.sidewaysVelocity += player.runVelocity;
			}
		}
		else
		{
			player.moveRight = false;
		}

		//left
		if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_D) && !player.isDucking && !window.pause)
		{
			player.direction = false;
			player.isRunning = true;
			player.moveLeft = true;

			if (player.sidewaysVelocity >= -player.maxSidewaysVelocity)
			{
				player.sidewaysVelocity -= player.runVelocity;
			}
		}
		else
		{
			player.moveLeft = false;
		}

		//screen
		if (player.posX >= window.width / 2)
		{
			window.renderPosX += player.posX - window.width / 2;
			player.posX = window.width / 2;
		}
		if (player.posX <= window.width / (level.size) && window.renderPosX > 0)
		{
			window.renderPosX += player.posX - window.width / (level.size);
			player.posX = window.width / (level.size);
		}


		//DECELERATE

		//up
		if ((!player.isGrounded) && (!player.collidePlatform))
		{
			player.velocity -= gravity * window.dT;
		}
		else if (!player.collidePlatform)
		{
			player.velocity = 0;
			player.posY = ((player.iPosY) * window.blockHeight);
		}

		//right
		if (player.sidewaysVelocity > 0)
		{
			player.sidewaysVelocity -= player.slowVelocity;
		}

		//left
		if (player.sidewaysVelocity < 0)
		{
			player.sidewaysVelocity += player.slowVelocity;
		}


		//ANIMATE

		//run right
		if (!player.isTurning && player.sidewaysVelocity > 0 && player.runningTime >= player.updateTime && player.collideR == false && !player.isDucking)
		{
			if (player.currentSprite == 12 || player.currentSprite == 6)
			{
				player.frame = 6;
				a = 1;
			}

			player.runningTime = 0;
			player.frame += a;
			player.currentSprite = player.frame;

			if (player.frame <= 8)
			{
				a = 1;
				player.frame = 8;
			}
			else if (player.frame >= 10)
			{
				a = -1;
				player.frame = 10;
			}
		}

		//run left
		if (player.sidewaysVelocity < 0 && player.runningTime >= player.updateTime && player.collideL == false && !player.isTurning && !player.isDucking)
		{
			if (player.currentSprite == 1 || player.currentSprite == 7 || player.currentSprite == 11)
			{
				player.frame = 7;
				a = -1;
			}

			player.runningTime = 0;
			player.frame += a;
			player.currentSprite = player.frame;

			if (player.sidewaysVelocity > 0)
			{
				player.currentSprite = 11;
				player.isTurning = true;
			}
			else if (player.sidewaysVelocity < -10)
			{
				player.isTurning = false;
			}
			if (player.frame == 3)
			{
				a = 1;
				player.frame = 3;
			}
			else if (player.frame == 5)
			{
				a = -1;
				player.frame = 5;
			}
		}

		//turn right
		if (player.sidewaysVelocity < 0 && player.moveRight && !player.isDucking)
		{
			player.currentSprite = 2;
			player.isTurning = true;
			player.frame = 7;
			a = 1;
		}
		else if (player.moveRight)
		{
			player.isTurning = false;
		}

		//turn left
		if (player.sidewaysVelocity > 0 && player.moveLeft && !player.isDucking)
		{
			player.currentSprite = 11;
			player.isTurning = true;
			player.frame = 6;
			a = -1;
		}
		else if (player.moveLeft)
		{
			player.isTurning = false;
		}

		//stand
		if (player.direction && player.runningTime >= player.updateTime && player.sidewaysVelocity == 0 && !player.isDucking)
		{
			player.currentSprite = 7;
			player.isRunning = false;
			player.isTurning = false;
		}
		else if (player.runningTime >= player.updateTime && player.sidewaysVelocity == 0 && !player.isDucking)
		{
			player.currentSprite = 6;
			player.isRunning = false;
			player.isTurning = false;
		}

		//stopped by wall
		if (player.collideR && !player.isDucking && !player.collideU)
		{
			player.currentSprite = 7;
			player.direction = true;
		}

		if (player.collideL && !player.isDucking && !player.collideU)
		{
			player.currentSprite = 6;
			player.direction = false;
		}

		//fall
		if (!player.isGrounded && !player.isDucking && !player.collidePlatform)
		{
			if (player.direction)
			{
				player.currentSprite = 12;
			}
			else
			{
				player.currentSprite = 1;
			}
		}


		//BLOCK STUFF

		//break block
		if (player.collideU && (level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == '_' || level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == '#'))
		{
			if (player.tall == 0)
			{
				block.selectedX = player.iPosXC;
				block.selectedY = player.iPosY - player.spriteHeight + (level.currentSize - 21);
				block.runningTime = 0;
				block.velocity = block.shiftHeight;
			}
			else
			{
				level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] = ' ';
			}

			if (level.currentScene[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == 'w')
			{
				level.currentScene[player.iPosY - player.spriteHeight + (level.currentSize - 22)][player.iPosXC - 1] = 'M';
				level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] = 'c';
			}
		}

		if (player.collideU && level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == 'o')
		{
			level.current[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] = 'q';
			block.selectedX = player.iPosXC;
			block.selectedY = player.iPosY - player.spriteHeight + (level.currentSize - 21);
			block.runningTime = 0;
			block.velocity = block.shiftHeight;

			if (level.currentScene[player.iPosY - player.spriteHeight + (level.currentSize - 21)][player.iPosXC] == 'w')
			{
				level.currentScene[player.iPosY - player.spriteHeight + (level.currentSize - 22)][player.iPosXC - 1] = 'M';
			}
		}

		if ((level.current[player.iPosY + (level.currentSize - 21)][player.iPosXC] == 't' || player.collideD && level.current[player.iPosY + (level.currentSize - 21)][player.iPosXC] == 'k') && (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)))
		{
			window.currentLevel += 1;
			restartLevel();
		}


		//PAUSE

		//toggle pause
		if (IsKeyPressed(KEY_ESCAPE))
		{
			window.pause = !window.pause;

			if ((!window.pause) && (!window.levelSelect))
			{
				player.posX = player.tempPosX;
				player.posY = player.tempPosY;
				player.velocity = player.tempVelocity;
				window.renderPosX = window.tempRenderX;
				pauseMenu = 0;
			}
			else if (!window.levelSelect)
			{
				player.tempPosX = player.posX;
				player.tempPosY = player.posY;
				player.tempVelocity = player.velocity;

				player.posX = 1 * window.blockHeight;
				window.tempRenderX = window.renderPosX;
				player.sidewaysVelocity = 0;
			}
		}

		if (window.pause || window.levelSelect)
		{
			window.renderPosX = (24 - ((window.blocksWide) / 2)) * window.blockHeight;
		}

		//pan up 
		if (window.pause && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
		{
			block.runningTime = 0;
			block.frame = 6;
			if (pauseMenu == 0)
			{
				pauseMenu = 2;
			}
			else
			{
				pauseMenu--;
			}
		}
		
		//pan down
		if (window.pause && (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)))
		{
			block.runningTime = 0;
			block.frame = 6;
			if (pauseMenu == 2)
			{
				pauseMenu = 0;
			}
			else
			{
				pauseMenu++;
			}
		}

		//determine selected
		if (window.pause && pauseMenu == 0)
		{
			level.pause[6][20] = 'o';
		}
		else if (window.pause)
		{
			level.pause[6][20] = 'q';
		}
		if (window.pause && pauseMenu == 1)
		{
			level.pause[8][20] = 'o';
		}
		else if (window.pause)
		{
			level.pause[8][20] = 'q';
		}
		if (window.pause && pauseMenu == 2)
		{
			level.pause[10][20] = 'o';
		}
		else if (window.pause)
		{
			level.pause[10][20] = 'q';
		}

		//select
		if (window.pause && !window.levelSelect && IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
		{
			switch (pauseMenu)
			{
			case 0:
				player.posX = player.tempPosX;
				player.posY = player.tempPosY;
				window.renderPosX = window.tempRenderX;
				window.pause = false;
				break;
			case 1:
				window.levelSelect = true;
				break;
			case 2:
				window.exit = true;
				break;
			}
			
			if (window.exit)
			{
				break;
			}
		}
		else if (window.levelSelect && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
		{
			switch (pauseMenu)
			{
			case 0:
				window.currentLevel = 1;
				restartLevel();
				window.pause = false;
				window.levelSelect = false;
				break;
			case 1:
				window.currentLevel = 2;
				restartLevel();
				window.pause = false;
				window.levelSelect = false;
				break;
			case 2:
				window.currentLevel = 3;
				restartLevel();
				window.pause = false;
				window.levelSelect = false;
				break;
			}
		}


		//DEATH
		if (player.collideD && (level.current[player.iPosY + (level.currentSize - 21)][player.iPosXD] == '-' || level.current[player.iPosY + (level.currentSize - 21)][player.iPosXLD] == '-'))
		{
			player.sidewaysVelocity = 0;
			player.velocity = 0;
			player.lives--;
			player.score = 0;
			player.tall = 0;
			PlaySoundMulti(sound.die);
			restartLevel();
		}
		else if (player.collideU && (level.current[player.iPosY + (level.currentSize - 22)][player.iPosXD] == '-' || level.current[player.iPosY + (level.currentSize - 22)][player.iPosXLD] == '-'))
		{
			player.sidewaysVelocity = 0;
			player.velocity = 0;
			player.lives--;
			player.tall = 0;
			player.score = 0;
			PlaySoundMulti(sound.die);
			restartLevel();
		}


		//do stuff here
		if (player.collision && player.tall == 0)
		{
			player.sidewaysVelocity = 0;
			player.velocity = 0;
			player.lives--;
			player.score = 0;
			PlaySoundMulti(sound.die);
			restartLevel();
		}
		else if (player.collision)
		{
			player.tall = 0;
		}

		//OUTPUT LEVEL
		if (!window.pause && !window.levelSelect)
		{
			outputLevel();

			for (int i = 0; i < window.mobCount; i++)
			{
				if (mob[i].isPlatform)
				{
					outputPlatform(mob[i].mob, i, mob[i].length);
				}
				else if (mob[i].loaded)
				{
					DrawTexturePro(
						mob[i].texture,
						Rectangle{ (mob[i].frame) * 16, ((mob[i].mob + mob[i].type) * 32), 16, 32 },
						Rectangle{ mob[i].posX - window.renderPosX - (2 * window.blockHeight), mob[i].posY - (8 * window.blockHeight) - 8, (32 * window.scale), (64 * window.scale) },
						Vector2{ 0, 0 },
						0,
						WHITE);
					DrawRectangleLines(mob[i].posX - (0.6 * window.blockHeight), mob[i].posY - ((4.0 - mob[i].stationary) * window.blockHeight) - 8, mob[i].width* window.scale * 0.4, mob[i].height* window.scale, RED);
				}
			}

			outputPipes();

			DrawTextEx(window.font, TextFormat("world %i-%i", player.world, player.level), Vector2{0 * window.blockHeight + 10, 10}, window.blockHeight / 2., 0, WHITE);
			DrawTextEx(window.font, TextFormat("score: %i", player.score), Vector2{ (window.blocksWide / 5) * window.blockHeight + 10, 10 }, window.blockHeight / 2., 0, WHITE);
			DrawTextEx(window.font, TextFormat("coins: %i", player.coins), Vector2{ (2 * window.blocksWide / 5) * window.blockHeight + 10, 10 }, window.blockHeight / 2., 0, WHITE);
			DrawTextEx(window.font, TextFormat("time: %i", player.time), Vector2{ (3 * window.blocksWide / 5) * window.blockHeight + 10, 10 }, window.blockHeight / 2., 0, WHITE);
			DrawTextEx(window.font, TextFormat("lives: %i", player.lives), Vector2{ (4 * window.blocksWide / 5) * window.blockHeight + 10, 10 }, window.blockHeight / 2., 0, WHITE);

			DrawTexturePro(
				player.texture,
				Rectangle{ player.currentSprite * 16, (32 * player.tall), -player.width / 2, -player.width },
				Rectangle{ player.posX, player.posY, ((-player.width) * window.scale), (-player.width * 2 * window.scale) },
				Vector2{ 0, 0 },
				0,
				WHITE);

			//DrawRectangle( player.posX, player.posY, ((-player.width) * window.scale), (-player.width * player.spriteHeight * window.scale), GREEN);

			for (int i = 0; i < window.mobCount; i++)
			{
				if (mob[i].loaded && mob[i].hit && mob[i].runningTime <= 4 * mob[i].updateTime)
				{
					DrawTextEx(window.font, TextFormat("%i", mob[i].scoreHit), Vector2{ mob[i].posX - window.renderPosX - (2 * window.blockHeight), mob[i].posY - (7 * window.blockHeight) - 8 - mob[i].runningTime * window.blockHeight }, window.blockHeight / 2.5, 0, WHITE);
				}
			}
		}
		else if (window.levelSelect)
		{
			if (IsKeyPressed(KEY_ESCAPE))
			{
				window.levelSelect = false;
				window.pause = true;
			}
			else
			{
				levelSelection();

				DrawTextEx(window.font, "level 1", Vector2{ 13 * window.blockHeight + 10, 6 * window.blockHeight }, window.blockHeight / 1.2, 0, WHITE);
				DrawTextEx(window.font, "level 2", Vector2{ 13 * window.blockHeight + 10, 8 * window.blockHeight }, window.blockHeight / 1.2, 0, WHITE);
				DrawTextEx(window.font, "level 3", Vector2{ 13 * window.blockHeight + 10, 10 * window.blockHeight }, window.blockHeight / 1.2, 0, WHITE);
			}
		}
		else if (window.pause)
		{
			outputPause();

			DrawTextEx(window.font, "resume", Vector2{ 13 * window.blockHeight + 10, 6 * window.blockHeight }, window.blockHeight, 0, WHITE);
			DrawTextEx(window.font, "levels", Vector2{ 13 * window.blockHeight + 10, 8 * window.blockHeight }, window.blockHeight, 0, WHITE);
			DrawTextEx(window.font, "quit", Vector2{ 13 * window.blockHeight + 10, 10 * window.blockHeight }, window.blockHeight, 0, WHITE);
		}

		EndDrawing();

		if (WindowShouldClose())
		{
			window.exit = true;
		}
	}

	for(int i = 0; i < window.mobCount; i++)
	{
		UnloadTexture(mob[i].texture);
	}
	StopSoundMulti();
	UnloadTexture(block.texture);
	UnloadTexture(block.texture2);
	UnloadTexture(player.texture);
	UnloadTexture(scenery.texture);
	UnloadSound(sound.smallJump);
	UnloadSound(sound.bigJump);
	UnloadSound(sound.die);
	UnloadMusicStream(sound.runningAbout);
	UnloadMusicStream(sound.underground);
	CloseAudioDevice();
	CloseWindow();
}