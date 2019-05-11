#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<iostream>
#define COL                   GetStdHandle(STD_OUTPUT_HANDLE)        // 콘솔창의 핸들정보 받기 Receieve console screen's handle information
#define SKY_BLUE           SetConsoleTextAttribute(COL, 0x000b);        // 하늘색 Skyblue
#define GREEN                SetConsoleTextAttribute(COL, 0x0002);        // 녹색 Green
#define YELLOW             SetConsoleTextAttribute(COL, 0x000e);        // 노란색 Yellow
#define RED                   SetConsoleTextAttribute(COL, 0x000c);        // 빨간색 Red
#define ORIGINAL            SetConsoleTextAttribute(COL, 0x0007);        // 밝은 회색 (ORIGINAL CONSOLE COLOR)
#define BLOOD               SetConsoleTextAttribute(COL, 0x0004);        // 검붉은색 Redish Black
#define height 35
#define width 111
#define TX_origin 30
#define TY_origin 1
#define PX_origin 55
#define PY_origin 31
#define original_kb_y 13
#define score_per_enemy_5 5

int map[height][width], PX = PX_origin, PY = PY_origin;
int previous_PX = PX, previous_PY = PY;
int bullit[(height * width) - 3][2], bullit_top = 0;
int enemy_cnt = 0;
int level = 0;
int score = 0;
bool score_change = false;
int playerID_charactor;

const int Flag_Ranking_Disp_Position_x = 116, Flag_Ranking_Disp_Position_y = 4;

typedef struct object {
	int x = 0, y = 0;
	int type = 0;
	int timer = 0;
	bool print_status = false;
};

object enemy[128];
bool break_check = true;

void gotoxy(int xxx, int yyy)  //x,y순서로 입력, 커서 이동(배열 좌표아닌 실제 좌표) 
{
	COORD pos = { xxx, yyy };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void fff(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0; // bVisible 멤버 변경
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo); // 변경값 적용
}
void setting_screen()
{
	for (int i = 0; i < height; i++)
	{
		if (i == 0 || i == height - 1)
		{
			for (int j = 0; j < width; j++)
			{
				map[i][j] = 1;//1 represents the symbol '*', movning restricted section
			}
		}
		else
		{
			map[i][0] = 1;
			for (int j = 1; j < width - 1; j++)
			{
				map[i][j] = 0;//0 represents the symbol ' '
			}
			map[i][width - 1] = 1;
		}
	}
	map[PY_origin][PX_origin] = 2;//2 represents the symbol 'A'
	map[PY_origin][PX_origin - 1] = map[PY_origin][PX_origin + 1] = 3;//3 represents the symbol '=', part of player icon


}
void map_print()
{
	//printing screen start
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (map[i][j] == 0)
				printf(" ");
			else if (map[i][j] == 1)
				printf("*");
			else if (map[i][j] == 2)
				printf("A");
			else if (map[i][j] == 3)
				printf("=");
			else if (map[i][j] == 4)
				printf("|");
			else if (map[i][j] == 5)
				printf("%%");
		}
		if (i < height - 1)
			printf("\n");
	}//printing game screen end

	//print logo
	for (int i = 0; i < 7; i++)
	{
		YELLOW
			gotoxy(width + 4, 1 + i);
		if (i == 0 || i == 6)
			printf("*********************");
		
		else
		{
			printf("*  ");
			if (i == 1)
			{
				printf("###");
				printf("    ");
				printf("##");
				printf("   ");
				printf("###");
				printf("  ");
			}
			else if (i == 2)
			{
				for (int j = 0; j < 5; j++)
				{
					printf("#  ");
				}
				printf("# ");
			}
			else if (i == 3)
			{
				printf("###");
				printf("   ");
				printf("#");
				printf("  ");
				printf("#");
				printf("  ");
				printf("###");
				printf("  ");
			}
			else if (i == 4)
			{
				printf("#");
				printf("     ");
				printf("#");
				printf("  ");
				printf("#");
				printf("  ");
				printf("#");
				printf("    ");
			}
			else
			{
				printf("#");
				printf("      ");
				printf("##");
				printf("   ");
				printf("#");
				printf("    ");
			}
			printf("*");
		}
		ORIGINAL
	}

	//print personalized/active information
	for (int i = 1; i <= 5; i++)
	{
		gotoxy(Flag_Ranking_Disp_Position_x, Flag_Ranking_Disp_Position_y + 5 + (i - 1) * 2);
		printf("%d. Player1  %7d", i, 512 - 4 * (i - 1));
	}

	SKY_BLUE
	gotoxy(Flag_Ranking_Disp_Position_x, Flag_Ranking_Disp_Position_y + 20);
	printf(" andylang8445: %5d", score);
	ORIGINAL
}
void enemay_creake(int level_enemy)
{
	int tmp_x = (rand() % (width - 5)) + 1;
	for (int i = 0; i < level_enemy*2-1; i+=2)
	{
		map[1][tmp_x + i] = 5;
	}
}
void move_enemy()
{
	
}

int main()
{

	bool trick_triger = false;
	char kb_in;//contain the status of keyboard pressed

	fff();//hide blinking cerser
	system("title Shooting Game");//set the title of the screen
	system("mode con cols=140 lines=37");//set the size of the screen
	setting_screen();//set the map array

	enemay_creake(4);
	map_print();//prints the map

	playerID_charactor = 15;

	for (int count_sleep = 0; break_check; count_sleep++)//while loop where the actual game is operated
	{
		if (_kbhit())//if the keyboard is pressed
		{
			kb_in = _getch();//save the presse key
			if (kb_in == 27)//if it's 'esc' key
			{
				//show the option tab and operate the selection
				int kb_x, kb_y;

				//Prints the outside of the option tab
				for (int i = 0; i < 19; i++)
				{
					gotoxy(41, 7 + i);
					if (i == 0 || i == 18)
					{
						for (int j = 0; j < 30; j++)
						{
							printf("#");
						}
					}
					else
					{
						printf("##");
						for (int j = 0; j < 26; j++)
						{
							printf(" ");
						}
						printf("##");
					}
				}

				//print the inside part of the option tab
				kb_x = 49;
				kb_y = 13;

				gotoxy(kb_x, kb_y);
				printf("▶");

				gotoxy(52, 13);
				printf("RESUME");

				gotoxy(52, 15);
				printf("RESTART");

				gotoxy(52, 17);
				printf("SETTINGS");

				gotoxy(52, 19);
				printf("END GAME");

				bool kb_resume = false;

				//Operates the selection part of the option tab using 'w','s'
				while (!kb_resume)
				{
					int kb_hit = _getch();
					if (kb_hit == 'w')
					{
						gotoxy(kb_x, kb_y);
						printf(" ");

						if (kb_y != original_kb_y)
							kb_y -= 2;
						else
							kb_y = 19;

						gotoxy(kb_x, kb_y);
						printf("▶");
					}
					else if (kb_hit == 's')
					{
						gotoxy(kb_x, kb_y);
						printf(" ");

						if (kb_y != 19)
							kb_y += 2;
						else
							kb_y = original_kb_y;

						gotoxy(kb_x, kb_y);
						printf("▶");
					}
					else if (kb_hit == 27)
					{
						system("cls");
						map_print();
						break;
					}
					else if (kb_hit == 13)//if the 'enter' is hit
					{
						if (kb_y == original_kb_y)//Resume option
						{
							system("cls");
							map_print();
							break;
						}
						else if (kb_y == original_kb_y + 2)//Restart option
						{
							system("cls");
							setting_screen();
							map_print();
							break;
						}
						else if (kb_y == original_kb_y + 4)//Setting option
						{
							for (int i = 0; i < 19; i++)
							{
								gotoxy(41, 7 + i);
								if (i == 0 || i == 18)
								{
									for (int j = 0; j < 30; j++)
									{
										printf("#");
									}
								}
								else
								{
									printf("##");
									for (int j = 0; j < 26; j++)
									{
										printf(" ");
									}
									printf("##");
								}
							}
							gotoxy(50, original_kb_y + 3);
							printf("COMING SOON");
							Sleep(750);
							system("cls");
							map_print();
							break;
						}
						else//end the game
						{
							return 0;
						}

					}//enter hit ends
				}//go to the result the player seleced


			}//end the option tab

			if (kb_in == 'w')//if the pressed keyboard is 'w'
			{
				if (PY > 1)//if the front of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PY--;
				}
			}
			else if (kb_in == 's')//if the pressed keyboard is 'w'
			{
				if (PY < height - 2)//if the back of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PY++;
				}
			}
			else if (kb_in == 'a')//if the pressed keyboard is 'w'
			{
				if (PX > 2)//if the left of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PX--;
				}
			}
			else if (kb_in == 'd')//if the pressed keyboard is 'w'
			{
				if (PX < width - 3)//if the right of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PX++;
				}
			}

			if (previous_PX != PX || previous_PY != PY)//if the player unit is moved
			{
				map[previous_PY][previous_PX - 1] = map[previous_PY][previous_PX] = map[previous_PY][previous_PX + 1] = 0;//erase the player unit from the array
				gotoxy(previous_PX - 1, previous_PY);//move the cersur to player unit's previous location
				printf("   ");//erase the player unit's previous location from the screen

				map[PY][PX] = 2;//mark the player unit to the array
				map[PY][PX - 1] = map[PY][PX + 1] = 3;
				gotoxy(PX - 1, PY);//move the cersur to player's current location
				printf("=A=");//print the player unit on the screen
			}

			if (kb_in == 'l' && map[PY - 1][PX] == 0)//if the pressed key is 'l'
			{
				map[PY - 1][PX] = 4;//save the bullet to the array
				gotoxy(PX, PY - 1);//move the cersur to the location where the bullet is fired
				printf("|");//print the bullet on the screen

			}
		}
		if (count_sleep % 8 == 0)//in certain amount of time
		{
			//always move bullit forword
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (map[i][j] == 4)//find the bullet from the array
					{
						gotoxy(j, i);//move to the bullet's location
						printf(" ");//erase the bullet from the screen
						map[i][j] = 0; //erase the bullet from the array
						if (map[i - 1][j] == 0)//if the bullet's front is blank
						{
							map[i - 1][j] = 4;
							gotoxy(j, i - 1);
							printf("|");
						}
						else if (map[i - 1][j] == 5)
						{
							map[i - 1][j] = 0;
							gotoxy(j, i - 1);
							printf(" ");
							score += score_per_enemy_5;
							score_change = true;
						}
					}
				}
			}
		}
		if (score_change == true)
		{
			SKY_BLUE;
			gotoxy(Flag_Ranking_Disp_Position_x + playerID_charactor, Flag_Ranking_Disp_Position_y + 20);
			printf("%5d", score);
			ORIGINAL;
		}
		Sleep(5);//give delay to operate the game properly that player can play
		break_check = true;
	}
}