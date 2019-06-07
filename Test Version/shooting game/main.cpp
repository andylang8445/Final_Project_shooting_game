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
#define score_per_enemy_5 15
#define type5timer 32
#define type5bandwidth 8000
#define minus_score_per_enemy_5 8
#define PlayerInitialHealthGauge 3
#define HX 88
#define HY 35

int map[height][width], PX = PX_origin, PY = PY_origin;
int previous_PX = PX, previous_PY = PY;
int enemy_cnt = 0;
int level = 0;
int score = 0;
int player_health_gauge = PlayerInitialHealthGauge;
bool score_change = false;
bool break_game_trigger = false;
int playerID_charactor;
long long int count_sleep = 0;

const int Flag_Ranking_Disp_Position_x = 116, Flag_Ranking_Disp_Position_y = 4;

typedef struct Obj{
	int x = 0, y = 0;
	int x_next = x, y_next = y;
	int type = 0;
	int timer = 0;
	bool print_status = false;
}object;

typedef struct E_B {
	int type = -1;
	int distance = -1;
	int destination_x, destination_y;
}Enemy_Bullet;

object enemy[width][height];
Enemy_Bullet EB[width][height];//@
Enemy_Bullet Initial_Bullet;
bool break_check = true;

void gotoxy(int xxx, int yyy)  //x,y순서로 입력, 커서 이동(배열 좌표아닌 실제 좌표) 
{
	COORD pos = { xxx, yyy };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void health_dsp()
{
	gotoxy(HX,HY);
	RED;
	for(int i=0;i<player_health_gauge;i++)
		printf("♥");
	ORIGINAL;
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
	PY = PY_origin;
	PX = PX_origin;
	map[PY][PX] = 2;//2 represents the symbol 'A'
	map[PY][PX - 1] = map[PY][PX + 1] = 3;//3 represents the symbol '=', part of player icon
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			enemy[j][i].type = 0;
		}
	}

}
void map_print()
{
	//printing screen start
	if (count_sleep == 0)
	{
		//map[PY][PX - 1] = map[PY][PX] = map[PY][PX + 1] = 0;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (map[i][j] == 0 && enemy[j][i].type == 0)
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
			else if (map[i][j] >= type5bandwidth && map[i][j] <= type5bandwidth + type5timer)
				printf("@");
			else if (enemy[j][i].type == 5)
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
	for (int i = 0; i < level_enemy; i++)
	{
		tmp_x = (rand() % (width - 4)) + 2;
		if (enemy[tmp_x][1].type == 5)
			i--;
		enemy[tmp_x][1].type = 5;
		//enemy[tmp_x][1].timer = type5timer;
		gotoxy(tmp_x, 1);
		printf("%%");
	}
}
void move_enemy()
{
	
}
void clear_bullets()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (map[i][j] == 4 || (map[i][j] > type5bandwidth && map[i][j] <= type5bandwidth + type5timer))
			{
				map[i][j] = 0;
				gotoxy(j, i);
				printf(" ");
			}
		}
	}
}
void Blink_Player(int blink_num)
{
	for (int i = 0; i < blink_num; i++)
	{
		gotoxy(PX - 1, PY);
		printf("   ");
		Sleep(50);
		gotoxy(PX - 1, PY);
		printf("=A=");
		Sleep(50);
	}
	gotoxy(PX - 1, PY);
	printf("   ");
	map[PY][PX] = map[PY][PX + 1] = map[PY][PX - 1] = 0;
	map[PY_origin][PX_origin] = 2;//2 represents the symbol 'A'
	map[PY_origin][PX_origin - 1] = map[PY_origin][PX_origin + 1] = 3;//3 represents the symbol '=', part of player icon
	PX = PX_origin;
	PY = PY_origin;
	gotoxy(PX - 1, PY);
	printf("=A=");
	clear_bullets();
}
int get_distanace(int x1, int  x2, int  y1, int  y2)
{
	int distance = (x1 - x2) * (x1 - x2);
	distance += (y1 - y2) * (y1 - y2);
	distance = sqrt(distance);
	return distance;
}
void clear_enemy()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (enemy[j][i].type == 5)
			{
				gotoxy(j, i);
				printf(" ");
				enemy[j][i].type = 0;
			}
		}
	}
}
void clear_screen()
{
	for (int i = 1; i < height - 1; i++)
	{
		gotoxy(1, i);
		for (int j = 1; j < width - 1; j++)
		{
			printf(" ");
		}
	}

}
void clear_player()
{
	gotoxy(PX, PY);
	printf("   ");
	map[PX][PY] = map[PX + 1][PY] = map[PX + 2][PY] = 0;
}
int game_over()
{
	clear_bullets();
	clear_enemy();
	clear_screen();
	gotoxy(50, 18);
	RED;
	printf("GAME");
	SKY_BLUE;
	printf(" OVER");
	ORIGINAL;
	clear_player();
	return 0;
}
void lose_health_gague()
{
	player_health_gauge--;
	gotoxy(HX + 2 * player_health_gauge, HY);
	printf(" ");
	if (player_health_gauge == 0)
	{
		game_over();
		_getch();
		system("cls");
		map_print();
		break_game_trigger = true;
	}
}

int main()
{
	srand(time(NULL));

	bool trick_triger = false;
	char kb_in;//contain the status of keyboard pressed

	fff();//hide blinking cerser
	system("title Shooting Game");//set the title of the screen
	system("mode con cols=140 lines=37");//set the size of the screen
	setting_screen();//set the map array

	map_print();//prints the map
	health_dsp();

	playerID_charactor = 15;

	for (count_sleep = 0; break_check; count_sleep++)//while loop where the actual game is operated
	{
		if (break_game_trigger == true)
			break;
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
							enemy_cnt = 0;
							map_print();
							count_sleep = 0;
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
							int nkb_x = 49;
							int nkb_y = 13;
							gotoxy(nkb_x, nkb_y);
							printf("▶");

							gotoxy(52, 13);
							printf("THEME COLOUR");
							gotoxy(52, 15);
							printf("CHANGE LEVEL");
							gotoxy(52, 17);
							printf("PLAYER COLOUR");
							gotoxy(52, 19);
							printf("USER MANUAL");
							while (!kb_resume)
							{
								int nkb_hit = _getch();
								if (nkb_hit == 'w')
								{
									gotoxy(nkb_x, nkb_y);
									printf(" ");

									if (nkb_y != original_kb_y)
										nkb_y -= 2;
									else
										nkb_y = 19;

									gotoxy(nkb_x, nkb_y);
									printf("▶");
								}
								else if (nkb_hit == 's')
								{
									gotoxy(nkb_x, nkb_y);
									printf(" ");

									if (nkb_y != 19)
										nkb_y += 2;
									else
										nkb_y = original_kb_y;

									gotoxy(nkb_x, nkb_y);
									printf("▶");
								}
								else if (nkb_hit == 27)//esc
								{
									gotoxy(nkb_x, nkb_y);
									printf(" ");
									break;
								}
								else if (nkb_hit == 13)//if the 'enter' is hit
								{

								}
							}
							gotoxy(kb_x, kb_y);
							printf("▶");

							gotoxy(52, 13);
							printf("RESUME      ");
							gotoxy(52, 15);
							printf("RESTART     ");
							gotoxy(52, 17);
							printf("SETTINGS     ");
							gotoxy(52, 19);
							printf("END GAME   ");
						}
						else//end the game
						{
							game_over();
							_getch();
							system("cls");
							map_print();
							break_game_trigger = true;
							break;
						}

					}//enter hit ends
				}//go to the result the player seleced

			}//end the option tab


			if (break_game_trigger == true)
				break;

			if (kb_in == 'w')//if the pressed keyboard is 'w'
			{
				if (PY > (3 * height / 7))//if the front of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PY--;
				}
				else if (PY == (3 * height / 7))
				{
					Beep(512, 2);
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
				else
					Beep(512, 2);
			}
			else if (kb_in == 'a')//if the pressed keyboard is 'w'
			{
				if (PX > 2)//if the left of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PX--;
				}
				else
					Beep(512, 2);
			}
			else if (kb_in == 'd')//if the pressed keyboard is 'w'
			{
				if (PX < width - 3)//if the right of player unit if not wall
				{
					previous_PY = PY;
					previous_PX = PX;
					PX++;
				}
				else
					Beep(512, 2);
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
				Beep(1512, 2);
				map[PY - 1][PX] = 4;//save the bullet to the array
				gotoxy(PX, PY - 1);//move the cersur to the location where the bullet is fired
				printf("|");//print the bullet on the screen

			}
		}
		if (enemy_cnt == 0)
		{
			enemay_creake(4);
			enemy_cnt += 4;
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
						if (enemy[j][i - 1].type == 5)
						{
							enemy[j][i - 1].type = 0;
							gotoxy(j, i - 1);
							printf(" ");
							score += score_per_enemy_5;
							score_change = true;
							enemy_cnt--;
							Beep(812, 2);
						}
						else if (map[i - 1][j] == 0)//if the bullet's front is blank
						{
							map[i - 1][j] = 4;
							gotoxy(j, i - 1);
							printf("|");
						}
						else if (map[i - 1][j] > type5bandwidth && map[i - 1][j] <= type5bandwidth + type5timer)
						{
							map[i - 1][j] = 0;
							gotoxy(j, i - 1);
							printf(" ");
						}
					}
				}
			}
		}
		if (count_sleep % 16 == 0)
		{

			for (int i = height - 1; i >= 0; i--)
			{
				for (int j = 0; j < width; j++)
				{
					if (map[i][j] > type5bandwidth && map[i][j] <= type5bandwidth + type5timer)
					{
						//move enemy bullet
						gotoxy(j, i);
						printf(" ");

						if (map[i + 1][j] == 0)
						{
							map[i + 1][j] = map[i][j] - 1;
							gotoxy(j, i + 1);
							printf("@");
						}
						else if (map[i + 1][j] == 2 || map[i + 1][j] == 3)
						{
							//Player Hit
							Beep(1106, 5);
							lose_health_gague();
							if (score - minus_score_per_enemy_5 >= 0)
								score -= minus_score_per_enemy_5;
							if (map[i + 1][j] == 2)
							{
								if (map[i + 1][j + 1] == 3)
								{
									Blink_Player(3);
								}
								else
									Blink_Player(3);
							}
							else
								Blink_Player(3);
							
						}
						map[i][j] = 0;

					}
					if (type5bandwidth == map[i][j])
					{
						//bullet moving time over
						map[i][j] = 0;
						gotoxy(j, i);
						printf(" ");
					}
					if (enemy[j][i].type == 5 && enemy[j][i].timer == 0 && rand() % 6 == 0)
					{
						if (map[i + 1][j] == 4)
						{
							map[i + 1][j] = 0;
							gotoxy(j, i + 1);
							printf(" ");
						}
						else if (map[i + 1][j] == 0)
						{
							//enemy shoots the bulletdss
							map[i + 1][j] = type5bandwidth + type5timer;
							Beep(1551, 2);
							gotoxy(j, i + 1);
							printf("@");
							enemy[j][i].timer = type5timer;
						}
					}
					else if (enemy[j][i].type == 5 && enemy[j][i].timer > 0)
					{
						enemy[j][i].timer--;
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
	system("cls");
	gotoxy(width/2, height/2);
	RED;
	printf("Thank you for playing!");
	while (1);
}