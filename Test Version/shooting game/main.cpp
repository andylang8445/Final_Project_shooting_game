#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<iostream>
#define COL                   GetStdHandle(STD_OUTPUT_HANDLE)        // 콘솔창의 핸들정보 받기
#define SKY_BLUE           SetConsoleTextAttribute(COL, 0x000b);        // 하늘색
#define GREEN                SetConsoleTextAttribute(COL, 0x0002);        // 녹색
#define YELLOW             SetConsoleTextAttribute(COL, 0x000e);        // 노란색
#define RED                   SetConsoleTextAttribute(COL, 0x000c);        // 빨간색
#define ORIGINAL            SetConsoleTextAttribute(COL, 0x0007);        // 밝은 회색 (ORIGINAL CONSOLE COLOR)
#define BLOOD               SetConsoleTextAttribute(COL, 0x0004);        // 검붉은색
#define height 35
#define width 111
#define check_width PX+1==TX&&PY==TY||PX+2==TX&&PY==TY||PX+3==TX&&PY==TY||PX+4==TX&&PY==TY||PX==TX&&PY==TY||PX-4==TX&&PY==TY||PX-1==TX&&PY==TY||PX-2==TX&&PY==TY||PX-3==TX&&PY==TY||exPX+1==exTX&&exPY==exTY||exPX+2==exTX&&exPY==exTY||exPX+3==exTX&&exPY==exTY||exPX+4==exTX&&exPY==exTY||exPX==exTX&&exPY==exTY||exPX-4==exTX&&exPY==exTY||exPX-1==exTX&&exPY==exTY||exPX-2==exTX&&exPY==exTY||exPX-3==exTX&&exPY==exTY
#define check_height PX==TX&&PY+1==TY||PX==TX&&PY+2==TY||PX==TX&&PY+3==TY||PX==TX&&PY-1==TY||PX==TX&&PY-2==TY||PX==TX&&PY-3==TY||exPX==exTX&&exPY+1==exTY||exPX==exTX&&exPY+2==exTY||exPX==exTX&&exPY+3==exTY||exPX==exTX&&exPY-1==exTY||exPX==exTX&&exPY-2==exTY||exPX==exTX&&exPY-3==exTY
#define TX_origin 30
#define TY_origin 1
#define PX_origin 55
#define PY_origin 31
#define original_kb_y 13
int map[height][width],PX = PX_origin, PY = PY_origin;
int previous_PX = PX, previous_PY = PY;
int bullit[(height * width) - 3][2], bullit_top = 0;

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
		}
		if (i < height - 1)
			printf("\n");
	}//printing screen end
}

int main()
{

	bool trick_triger = false;
	char kb_in;//contain the status of keyboard pressed

	fff();//hide blinking cerser
	system("title Shooting Game");//set the title of the screen
	system("mode con cols=111 lines=35");//set the size of the screen
	setting_screen();//set the map array

	map_print();//prints the map

	for (int count_sleep = 0; break_check; count_sleep++)//while loop where the actual game is operated
	{
		if (_kbhit())
		{
			kb_in = _getch();
			if (kb_in == 27)
			{
				int kb_x, kb_y;
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
					else if (kb_hit == 13)
					{
						if (kb_y == original_kb_y)
						{
							system("cls");
							map_print();
							break;
						}
						else if (kb_y == original_kb_y + 2)
						{
							system("cls");
							setting_screen();
							map_print();
							break;
						}
						else if (kb_y == original_kb_y + 4)
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
						else
						{
							return 0;
						}

					}
				}
				

			}
			if (kb_in == 'w')
			{
				if (PY > 1)
				{
					previous_PY = PY;
					previous_PX = PX;
					PY--;
				}
				
			}
			else if (kb_in == 's')
			{
				if (PY < height - 2)
				{
					previous_PY = PY;
					previous_PX = PX;
					PY++;
				}
			}
			else if (kb_in == 'a')
			{
				if (PX > 2)
				{
					previous_PY = PY;
					previous_PX = PX;
					PX--;
				}
			}
			else if (kb_in == 'd')
			{
				if (PX < width - 3)
				{
					previous_PY = PY;
					previous_PX = PX;
					PX++;
				}
			}

			if (previous_PX != PX || previous_PY != PY)
			{
				map[previous_PY][previous_PX - 1] = map[previous_PY][previous_PX] = map[previous_PY][previous_PX + 1] = 0;
				gotoxy(previous_PX - 1, previous_PY);
				printf("   ");

				map[PY][PX] = 2;
				map[PY][PX - 1] = map[PY][PX + 1] = 3;
				gotoxy(PX - 1, PY);
				printf("=A=");
			}

			if (kb_in == 'l' && map[PY - 1][PX] == 0)
			{
				map[PY - 1][PX] = 4;
				gotoxy(PX, PY - 1);
				printf("|");

			}
		}
		if (count_sleep % 9 == 0)
		{
			//always move bullit forword
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (map[i][j] == 4)
					{
						gotoxy(j, i);
						printf(" ");
						map[i][j] = 0;
						if (map[i - 1][j] == 0)
						{
							map[i - 1][j] = 4;
							gotoxy(j, i - 1);
							printf("|");
						}
					}
				}
			}
		}
		Sleep(5);
	}
}