#include"Header.h"
#pragma warning(disable: 4996)
//Global variables
POINT snake[MAX_SIZE_SNAKE]; //snake
POINT food[MAX_SIZE_FOOD]; // food 
int CHAR_LOCK;//used to determine the direction my snake cannot move (At a moment, there is one direction my snake cannot move to) 
int MOVING;//used to determine the direction my snake moves (At a moment, there are three directions my snake can move) 
int SPEED;// Standing for level, the higher the level, the quicker the speed 
int HEIGH_CONSOLE, WIDTH_CONSOLE;// Width and height of console-screen
int FOOD_INDEX; // current food-index  
int SIZE_SNAKE; // size of snake, initially maybe 8 units and maximum size maybe 40
int STATE; // State of snake: dead or alive
int SCORE;	//player's score

//cac bien trong man choi cuoi cung
POINT lastfood;
int lastwidth, lastheight;

bool barrier[HEIGH_CONSOLE_DEFAULT][WIDTH_CONSOLE_DEFAULT];
int stage = 1;  // Biến xác nhận màn chơi hiện tại
char typegate = '\0';

//cac bien dung de xu li qua cong
POINT subsnake[MAX_SIZE_SUB_SNAKE];
int size_subsnake;

HANDLE altHandle;

//cac bien phuc vu cho viec save va load game
FILE* fp;
int NGAME = 0;				//số lượng game mà người chơi đã lưu
char player_name[50];		//lưu tên người chơi
char file_name[50];			//tên file lưu thông tin của người chơi, bao gồm họ tên và tất cả các thông số của màn chơi
FILE* file_list_file_name;	//file dùng để lưu các file_name[50]
char list_file_name[10][50];//danh sách để lưu các tên file, danh sách có tối đa 10 tên, mỗi tên tối đa 50 kí tự

//cac bien phuc vu luu va xuat highscore
//file lưu các người chơi đạt điểm cao nhất
FILE* highscore;			 //dùng để mở file highscore.dat
PLAYER list_high_score[5];	//lưu thông tin 5 người chơi có điểm số cao nhất
int NHIGHSCORE;				//lưu số lượng phần tử của mảng list_high_score



//ALL FUNCTION PROTOTYPES
// 
// 
// 
//cac ham lien quan den console window
//cố định màm kích thước màn hình console
void FixConsoleWindow();
void GotoXY(int x, int y);
void setcolor(unsigned short color);
// Xóa con trỏ nháy
void hideCursor();
// Hiện con trỏ nháy
void showCursor();
void DisableSelection(); // Ngăn người dùng bôi đen đoạn text
// Xoa man hinh
void clrscr();

//hàm về cổng
void DrawTheGateLeft(POINT gate);
void DrawTheGateRight(POINT gate);
void DrawTheGateUp(POINT gate);
void DrawTheGateDown(POINT gate);
bool checkGateLeft(int x, int y);
bool checkGateRight(int x, int y);
bool checkGateUp(int x, int y);
bool checkGateDown(int x, int y);

//cac ham  lien quan den barrier
void ResetBarrier();
//hàm vẽ các chướng ngại vật
void draw_barrier();
// Khoi tao stage game
void createStage1(int height, int width);
void createStage2(int height, int width);
void createStage3(int height, int width);
void createStage4(int height, int width);

//cac ve do hoa
//vẽ 1 cái khung
void DrawBoard(int x, int y, int width, int height);
//vẽ cái bảng bên phải khung hình chính
//để xuất 1 số thông tin lên màn hình
void Draw_information();
// hàm cập nhật các thông số lenght of snake, speed lên màn hình
void Update_information();
//Function to draw
void EraseSnakeAndFood();
//vẽ thức ăn
void DrawFood();
//ham ve con ran
void DrawSnake();
// Ve hinh con ran
void drawSnakeImage(long x, long y);
// Vẽ title stage hiện tại
void drawStageTitle(int cur_stage);
//ham de ve snake and food
void DrawFoodAndSnake();
void IntroduceSnake();
// Vẽ Text Snake
void drawSnakeTitle(long x, long y, int title);
void drawTeamTitle(long x, long y, int title);
void drawTeamTitle(long x, long y);
void drawSectionTitle(long x, long y, int title);
void drawProcessFood(int n);

//cac ham xu li qua trinh choi
//ham restart game
//tiep tuc thread da bi dung
void RestartGame(HANDLE t);
//Function exit game
void ExitGame(HANDLE t);
//Function pause game
void PauseGame(HANDLE t);
//Function to update global data
//xu li khi snake eats food
void Eat();
//ham xu li qua cong
void throughthegate();
//Function to process the dead of snake
void ProcessDead();
void process_when_snake_died(HANDLE t, bool& agreed);
//đâm đầu vào body của con rắn
bool touch_body();
//ham chuyen man
void start_new_stage();
//ham tao mot mang thuc an va cong
void GenerateFood();
//kiem tra vi tri xuat hien cua food va gate co hop le khong
bool IsValid(int x, int y);
//dat lai trang thai ban dau cua con ran
void ResetData();
//nhom cac ham di chuyen
//cac ham phuc vu chuc nang di chuyen
void determined_CHARLOCK_MOVING(int temp);
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
//cac ham di chuyen trong man cuoi
void lastmoveright();
void lastmoveleft();
void lastmovedown();
void lastmoveup();
//Subfunction for thread
void ThreadFunc();

//nhom cac ham save load game, highscore
//hàm tạo file_name khi đã biết player_name
//mặc định file_name có dạng là player_name.dat
void create_file_name(char* s);
//hàm tạo player_name khi đã biết file_name
void create_player_name(char* s);
//hàm đọc thông tin từ file_list_file_name
void read_file_list_file_name();
//hàm ghi thông tin vào file_list_file_name
void write_file_list_file_name();
//save and load game
void savegame(HANDLE t);
void loadgame(HANDLE t, int index);
//HIGH SCORE
void swap_PLAYER(PLAYER& a, PLAYER& b);
void sort_list_high_score();
void write_list_high_score();
void read_list_high_score();

//cac ham menu
void drawmenu();
void Menu(HANDLE t);
void drawNEWGAME();
void drawLOADGAME();
void drawHIGHSCORE();
void drawINTRODUCTION();
void drawEXIT();
void introduction_team_and_guide(int x, int y);
void move_in_menu(int x, int y);
void move_in_list_saved_game(char& chon, int& yindex, int yindexmax, int& variable);
void print_list_high_score();
void begin_of_exit(int title, bool& isvisit);
void move_in_exit(bool& check, bool& isvisit, int& title, int& c);

//cac ham  cho man choi cuoi cung
//cac ham  cho man choi cuoi cung
void drawlastboard(int x, int y);
void eraselastboard(int x, int y);
bool checklastfood();
void createlastfood();
void drawlastfood();
void eatlastfood();
//ham kiem tra de thu nho khung tro choi
bool checkdescrease();


// main function =========================================================================================
int main()
{
	SetConsoleTitle(L"Team_12 : 21120477 - 21120461 - 21120486 - 21120435 -21120619");
	DisableSelection();
	SetConsoleOutputCP(437); //sửa một số lỗi phông chữ
	FixConsoleWindow();
	ResetData();
	//khi tạo thread phải truyền vào 1 con trỏ hàm vào khởi tạo của nó 
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle(); //tạo 1 handle để quản lí thread t1
	SuspendThread(handle_t1);
	IntroduceSnake();	//giao diện khi chương trình vừa chạy
	Menu(handle_t1);	//tạo menu 
	int temp; bool agreed = true;
	while (1)
	{
		if (STATE == 0)	//khi con rắn chết
		{
			process_when_snake_died(handle_t1, agreed);
		}
		else
		{
			temp = toupper(_getch());	// nhập kí tự bất kì từ bàn phím
			if (temp == 'D')	//nhấn D để tạm dừng game
			{
				PauseGame(handle_t1);
			}
			else if (temp == 27)	//nhấn esc để thoát game
			{	//khi nhấn esc thì người chơi sẽ được đưa đến menu
				ExitGame(handle_t1);
				Menu(handle_t1);
			}
			else if (temp == 'L')	//nhấn L để save game
			{
				savegame(handle_t1);
			}
			else
			{
				ResumeThread(handle_t1);	//tiếp tục thread t1
				//77,75,72,80 lan luot la cac mui ten phai,trai,len va xuong
				if ((temp != CHAR_LOCK) && (temp == 77 || temp == 75 || temp == 72 || temp == 80))
				{
					determined_CHARLOCK_MOVING(temp);
				}
			}
		}
	}
}
//================================================================================================================================

void drawmenu()
{
	clrscr();
	DrawBoard(7, 3, 104, 22);
	srand((unsigned int)time(NULL));
	for (int y = 3; y <= 25; y++)
	{
		if (y == 3) {
			setcolor(15); GotoXY(35, y);
			cout << static_cast<char>(209);
			continue;
		}
		GotoXY(35, y);
		setcolor(rand() % 5 + 11);
		cout << "|";
		if (y == 25)
		{
			setcolor(12); GotoXY(35, y);
			cout << static_cast<char>(207);
			break;
		}
	}
	GotoXY(17, 6);	setcolor(15);	cout << "NEW GAME";
	GotoXY(17, 9);	setcolor(10);	cout << "LOAD GAME";
	GotoXY(17, 12);	setcolor(11);	cout << "HIGH SCORE";
	GotoXY(17, 15);	setcolor(12);	cout << "INTRODUCTION";
	GotoXY(17, 18);	setcolor(13);	cout << "EXIT";
	setcolor(15);
	for (int i = 8; i <= 34; i++)
	{
		GotoXY(i, 21);	cout << "-";
	}
	setcolor(14);
	GotoXY(8, 22);
	cout << static_cast<char>(5) << " Use arrow keys to move";
	GotoXY(8, 23);
	cout << static_cast<char>(5) << " Use arrow Right to choose";
	GotoXY(8, 24);
	cout << static_cast<char>(5) << " Use arrow Left to back";
}

void drawNEWGAME()
{
	cout << "==> NEW GAME <==";
	drawSectionTitle(50, 6, 1);
	GotoXY(65, 11);
	cout << static_cast<char>(3) << " Start a new game";
	GotoXY(65, 13);
	cout << static_cast<char>(3) << " Please read the INTRODUCTION to get more ";
	GotoXY(64, 14);
	cout << "information and guide how to use Key";
}
void drawLOADGAME()
{
	cout << "==> LOAD GAME <==";
	drawSectionTitle(48, 6, 2);
	GotoXY(65, 11);
	cout << static_cast<char>(3) << " Load the process that you saved before";
}
void drawHIGHSCORE()
{
	cout << "==> HIGH SCORE <==";
	drawSectionTitle(46, 6, 3);
	GotoXY(65, 11);
	cout << static_cast<char>(3) << " Show top 5 highest score";
}
void drawINTRODUCTION()
{
	cout << "==> INTRODUCTION <==";
	drawSectionTitle(38, 6, 4);
	GotoXY(65, 11);
	cout << static_cast<char>(3) << " Team information and in-game shortcuts";
}
void drawEXIT()
{
	cout << "==> EXIT <==";
	drawSectionTitle(63, 6, 5);
	GotoXY(65, 11);
	cout << static_cast<char>(3) << " Confirm exit game";
}

void introduction_team_and_guide(int x, int y)
{
	GotoXY(x, y);
	for (int i = 43; i < 106; i++) {
		GotoXY(i, 5);	cout << static_cast<char>(196);
		GotoXY(i, 16);	cout << static_cast<char>(196);
	}
	setcolor(15); GotoXY(69, 5);	cout << "  TEAM 12  ";
	setcolor(14);	GotoXY(45, 7);
	cout << "GV huong dan: Truong Toan Thinh";
	GotoXY(45, 9);	cout << "21120461: Dam Hong Hung - LEADER";
	GotoXY(45, 10);	cout << "21120435: Le Phan Xuan Dung";
	GotoXY(45, 11);	cout << "21120477: Vuong Huynh Khai";
	GotoXY(45, 12);	cout << "21120486: Vo Minh Khue";
	GotoXY(45, 13);	cout << "21120619: Le Quang Ngo";
	setcolor(15); GotoXY(71, 15);	cout << " GUIDE ";
	setcolor(14); GotoXY(45, 18);
	cout << "Press key up, down, left, right to move the snake";
	GotoXY(45, 19);	cout << "Press D to pause game";
	GotoXY(45, 20);	cout << "Press Esc to exit game";
	GotoXY(45, 21);	cout << "Press L to save game";
	GotoXY(45, 22);	cout << "When the snake died, you can play again or exit";
	
	char bien_chong_warning = _getch();
	
}

void move_in_menu(int x, int y)
{
	GotoXY(x, y);
	if (y == 6)
	{
		cout << "==> NEW GAME <==";
	}
	else if (y == 9)
	{
		
		cout << "==> LOAD GAME <==";
	}
	else if (y == 12)
	{
		cout << "==> HIGH SCORE <==";
	}
	else if (y == 15)
	{
		cout << "==> INTRODUCTION <==";
	}
	else if (y == 18)
	{
		cout << "==> EXIT <==";
	}
}

void move_in_list_saved_game(char& chon, int& yindex, int yindexmax, int& variable)
{
	GotoXY(50, yindex);
	create_player_name(list_file_name[yindex - 8]);
	cout << "====>" << yindex - 8 + 1 << " : " << player_name << "<====";
	chon = _getch();
	if (8 <= yindex && yindex < yindexmax)
	{
		if (yindex < yindexmax - 1 && chon == 80)
		{
			yindex++;
		}
		if (yindex > 8 && chon == 72)
		{
			yindex--;
		}
		variable = 8;
		for (int i = 0; i < NGAME; i++)
		{
			GotoXY(50, variable);
			create_player_name(list_file_name[i]);
			cout << i + 1 << " : " << player_name << "               ";
			variable++;
		}
	}
}

void print_list_high_score()
{
	read_list_high_score();
	GotoXY(44, 7);
	cout << "HIGHSCORE";
	drawTeamTitle(61, 7, 3);
	if (NHIGHSCORE == 0)
	{
		GotoXY(37, 9);	cout << "There no highscore to show";
	}
	else
	{
		int variable = 10;
		for (int i = 0; i < NHIGHSCORE; i++)
		{
			GotoXY(38, variable);
			if (strlen(list_high_score[i].name) == 0)
			{
				cout << "Unknown player";
			}
			else
			{
				cout << list_high_score[i].name;
			}
			GotoXY(55, variable);
			cout << list_high_score[i].score;
			variable++;
		}
	}
	char bien_chong_warning = _getch();
}

void begin_of_exit(int title, bool& isvisit)
{
	setcolor(12);	GotoXY(58, 7);	cout << "=> NO <=";
	setcolor(14);
	isvisit = true;
	GotoXY(81, 7);	cout << "   YES   ";
	GotoXY(56, 21);
	cout << "Don't give up, we believe you can do it";
	drawSnakeTitle(50, 11, title);
}

void move_in_exit(bool& check, bool& isvisit, int& title, int& c)
{
	if (c == 77 && isvisit == true && check == false) {
		GotoXY(81, 7);	setcolor(12);	cout << "=> YES <=";
		setcolor(14);
		check = true;
		GotoXY(58, 7);	cout << "   NO   ";
		GotoXY(56, 21);	cout << "                                       ";
		GotoXY(61, 21);	cout << "We hope to see you again T_T";
		title = -1;	drawSnakeTitle(50, 11, title);
		title = 3;	drawSnakeTitle(53, 11, title);
	}
	else if (c == 77 && isvisit == true && check == true) {
		GotoXY(81, 7);	cout << "   YES   ";
		setcolor(12);
		check = false;
		GotoXY(58, 7);	cout << "=> NO <=";
		setcolor(14);	GotoXY(56, 21);
		cout << "Don't give up, we believe you can do it";
		title = -3;	drawSnakeTitle(53, 11, title);
		title = 1;	drawSnakeTitle(50, 11, title);
	}
	else if (c == 75 && isvisit == true && check == false) {
		GotoXY(58, 7);	cout << "   NO   ";
		setcolor(12);
		check = true;
		GotoXY(81, 7);	cout << "=> YES <=";
		setcolor(14);	GotoXY(56, 21);
		cout << "                                       ";
		GotoXY(61, 21);	cout << "We hope to see you again T_T";
		title = -1;	drawSnakeTitle(50, 11, title);
		title = 3;	drawSnakeTitle(53, 11, title);
	}
	else if (c == 75 && isvisit == true && check == true) {
		GotoXY(58, 7);	setcolor(12);	cout << "=> NO <=";
		setcolor(14);
		check = false;
		GotoXY(81, 7);	cout << "   YES   ";
		GotoXY(56, 21);
		cout << "Don't give up, we believe you can do it";
		title = -3;	drawSnakeTitle(53, 11, title);
		title = 1; drawSnakeTitle(50, 11, title);
	}
}

void Menu(HANDLE t)
{
	drawmenu();
	int key, x = 13, y = 6;
	bool ischoose = false;
	while (true)
	{
		if (ischoose == false)
		{	//vẽ các mục của menu
			GotoXY(x, y);
			if (y == 6)
			{
				drawNEWGAME();
			}
			else if (y == 9)
			{
				drawLOADGAME();
			}
			else if (y == 12)
			{
				drawHIGHSCORE();
			}
			else if (y == 15)
			{
				drawINTRODUCTION();
			}
			else if (y == 18)
			{
				drawEXIT();
			}
		}
		key = _getch();
		if (y >= 6 && y <= 18)
		{
			if ((key == 72 || key == 80) && ischoose == false) {
				drawmenu();
				if (key == 72) {
					if (y <= 6)
						y = 18;
					else y = y - 3;
				}
				else if (key == 80) {
					if (y >= 18)
						y = 6;
					else y = y + 3;
				}
				move_in_menu(x, y);
			}
			if (ischoose == false && key == 77)
			{
				drawmenu();
				ischoose = true;
				if (y == 6)
				{
					RestartGame(t);	//chơi màn chơi mới
					break;
				}
				else if (y == 9)
				{
					read_file_list_file_name();
					GotoXY(66, 6);
					cout << "LIST GAME SAVED";
					if (NGAME > 0)
					{
						int variable = 8;
						for (int i = 0; i < NGAME; i++)
						{
							GotoXY(50, variable);
							create_player_name(list_file_name[i]);
							cout << i + 1 << " : " << player_name;
							variable++;
						}
						int yindex = 8, yindexmax = 8 + NGAME;
						char chon;
						while (1)
						{	//di chuyển trong danh sach saved game
							move_in_list_saved_game(chon, yindex, yindexmax, variable);
							if (chon == 75)
							{
								clrscr();	drawmenu();
								ischoose = false;	break;
							}
							if (chon == 13)	//nhấn enter để load game
							{
								loadgame(t, yindex - 8);
								ResumeThread(t);	break;
							}
						}
						if (chon == 13)   break;
					}
					else {	//trường hợp chưa có game nào được lưu trước đó
						GotoXY(50, 8);	setcolor(15);
						cout << "There are no game to load";
						setcolor(14);
					}
				}
				else if (y == 12)
				{
					print_list_high_score();
				}
				else if (y == 15)
				{
					introduction_team_and_guide(x, y);
				}
				else if (y == 18)
				{
					GotoXY(57, 5);
					cout << "Are you sure you want to quit game?";
					bool check = false, isvisit = false;
					if (key == 77) {
						int title = 1;	begin_of_exit(title, isvisit);
						while (true) {
							int c = getch();
							if (c == 13) {
								if (check == false)
								{
									goto back;
								}
								else {
									clrscr();
									TerminateThread(t, 0);	//hủy thread đã tạo
									exit(0);
								}
							}
							move_in_exit(check, isvisit, title, c);
						}
					}
				}
			}
			if (ischoose == true && key == 75) {
			back:
				ischoose = false;
				clrscr();	drawmenu();
				move_in_menu(x, y);
			}
		}
	}
}

// Xóa màn hình
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0,0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}


void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}



void DisableSelection()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}

// Ẩn con trỏ nháy
void hideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Hiện con trỏ nháy
void showCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = true;
	cursor.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

//hàm đi đến 1 vị trí x,y bất kì trong màn hình console
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Set màu chữ cho toàn bộ text
void setcolor(unsigned short color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//kiem tra vi tri xuat hien cua food va gate co hop le khong
bool IsValid(int x, int y)
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (snake[i].x == x && snake[i].y == y)
		{
			return false;
		}
	}
	return true;
}

// Vẽ art khi loading game
void drawTeamTitle(long x, long y) {
	SetConsoleOutputCP(65001);
	long xx = x;	setcolor(10);
	GotoXY(xx, y);         cout << u8"                       ████████████████                       ";
	GotoXY(xx, y + 1);       cout << u8"                   ████  ██░░░░██  ██▒▒████                   ";
	GotoXY(xx, y + 2);       cout << u8"                 ██      ██░░██      ██▒▒▒▒██                 ";
	GotoXY(xx, y + 3);       cout << u8"               ██      ██░░░░██        ██▒▒██                 ";
	GotoXY(xx, y + 4);       cout << u8"             ██      ████░░████        ██▒▒██                 ";
	GotoXY(xx, y + 5);       cout << u8"             ██    ██████░░██████      ██▒▒▒▒██               ";
	GotoXY(xx, y + 6);       cout << u8"             ██    ██████░░██████      ██▒▒▒▒██               ";
	GotoXY(xx, y + 7);       cout << u8"             ██    ████░░░░██████      ██▒▒▒▒██               ";
	GotoXY(xx, y + 8);       cout << u8"             ██      ██░░░░████      ██▒▒▒▒▒▒██               ";
	GotoXY(xx, y + 9);       cout << u8"             ████████░░░░░░██      ██▒▒▒▒▒▒▒▒██               ";
	GotoXY(xx, y + 10);      cout << u8"             ██▒▒▒▒▒▒░░░░░░░░██████▒▒▒▒▒▒▒▒▒▒██               ";
	GotoXY(xx, y + 11);      cout << u8"               ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒██                 ";
	GotoXY(xx, y + 12);      cout << u8"                 ████████████████████▒▒▒▒▒▒██                 ";
	Sleep(30);
	GotoXY(xx, y + 13);      cout << u8"                 ▒▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒██                   ";
	GotoXY(xx, y + 14);      cout << u8"               ▒▒▒▒▒▒▒▒  ████▒▒▒▒▒▒▒▒▒▒██                     ";
	GotoXY(xx, y + 15);      cout << u8"           ▒▒▒▒▒▒▒▒▒▒    ██    ▒▒▒▒▒▒▒▒████████               ";
	GotoXY(xx, y + 16);      cout << u8"       ▒▒▒▒▒▒▒▒▒▒▒▒    ██░░░░░░░░▒▒▒▒████▒▒▒▒▒▒██             ";
	GotoXY(xx, y + 17);      cout << u8"   ▒▒▒▒▒▒▒▒▒▒▒▒        ██        ▒▒▒▒██▒▒▒▒▒▒▒▒▒▒██           ";
	GotoXY(xx, y + 18);      cout << u8"         ▒▒▒▒         ██░░░░░░░▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒████████   ";
	GotoXY(xx, y + 19);      cout << u8"       ▒▒▒▒            ██      ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒▒▒██ ";
	GotoXY(xx, y + 20);      cout << u8"                       ██░░░░░░▒▒░░▒▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒██ ";
	GotoXY(xx, y + 21);      cout << u8"                        ██   ▒▒░░▒▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒██   ";
	GotoXY(xx, y + 22);      cout << u8"                       ██░░░░░░▒▒▒▒▒▒▒▒██  ██  ▒▒▒▒▒▒▒▒██     ";
	GotoXY(xx, y + 23);      cout << u8"                         ██░░░░░░▒▒▒▒██      ██  ▒▒████       ";
	GotoXY(xx, y + 24);      cout << u8"                           ██████████        ██████           ";
	Sleep(30);
	SetConsoleOutputCP(437);	setcolor(11);
	for (int j = 7; j >= 0; j--) {
		GotoXY(10 + j, 18 + y);	cout << static_cast<char>(219);
		if (j < 7)
		{
			GotoXY(10 + j + 1, 18 + y);	cout << " ";
		}
		Sleep(50);
	}
	drawTeamTitle(60, 0, 1);
	for (int j = 18 + y; j <= 24; j++) {
		setcolor(11);	GotoXY(10, j);	cout << static_cast<char>(219);
		if (j > 16 + y)
		{
			GotoXY(10, j - 1);	cout << " ";
		}
		if (j == 20)
		{
			Sleep(5);	drawTeamTitle(80, 12, 2);
		}
		Sleep(50);
		if (j == 24)
		{
			GotoXY(10, j); cout << " ";
		}
	}
	setcolor(14);
}

void drawTeamTitle(long x, long y, int title) {
	srand((unsigned int)time(NULL));
	SetConsoleOutputCP(65001);
	long xx = x;
	if (title == 1) {
		setcolor(9);
		GotoXY(xx, y);              cout << u8"     s                                               ";
		GotoXY(xx, y + 1);          cout << u8"    :8                                               ";
		setcolor(12);
		GotoXY(xx, y + 2);          cout << u8"   .88                              ..    .     :    ";
		GotoXY(xx, y + 3);          cout << u8"  :888ooo      .u          u      .888: x888  x888.  ";
		setcolor(13);
		GotoXY(xx, y + 4);          cout << u8"-*8888888   ud8888.     us888u.  ~`8888~'888X`?888f` ";
		GotoXY(xx, y + 5);          cout << u8"  8888    :888'8888. .@88 '8888'   X888  888X '888>  ";
		setcolor(11);
		GotoXY(xx, y + 6);          cout << u8"  8888    d888 '88%' 9888  9888    X888  888X '888>  ";
		GotoXY(xx, y + 7);          cout << u8"  8888    8888.+'    9888  9888    X888  888X '888>  ";
		setcolor(14);
		GotoXY(xx, y + 8);          cout << u8" .8888Lu= 8888L      9888  9888    X888  888X '888>  ";
		GotoXY(xx, y + 9);          cout << u8" ^%888*   '8888c. .+ 9888  9888   '*88% ''*88''888!` ";
		setcolor(15);
		GotoXY(xx, y + 10);         cout << u8"   'Y'     '88888%   '88899888'    'Y'    '    'Y'   ";
		GotoXY(xx, y + 11);         cout << u8"             'YP'        ^Y'                         ";
	}
	if (title == 2) {
		setcolor(10);
		GotoXY(xx, y + 1);           cout << u8"      oe      .--~*teu.    ";
		GotoXY(xx, y + 2);           cout << u8"    .@88     dF     988Nx  ";
		GotoXY(xx, y + 3);           cout << u8" =*88888    d888b   `8888> ";
		setcolor(15);
		GotoXY(xx, y + 4);           cout << u8"   88888    ?8888>  98888F ";
		GotoXY(xx, y + 5);           cout << u8"   88888    ' * *'  x88888~";
		GotoXY(xx, y + 6);           cout << u8"   88888          d8888*`  ";
		setcolor(9);
		GotoXY(xx, y + 7);           cout << u8"   88888        z8**`    : ";
		GotoXY(xx, y + 8);           cout << u8"   88888      :?....  ...F ";
		setcolor(11);
		GotoXY(xx, y + 9);           cout << u8"   88888     <' 888888888~ ";
		GotoXY(xx, y + 10);          cout << u8"   88888     8:  '888888*  ";
		GotoXY(xx, y + 11);          cout << u8"**%%%%%%%**  '    ' * *`   ";
	}
	if (title == 3) {
		setcolor(14);
		GotoXY(xx, y);             cout << u8"         ╥                              ╥         ";
		GotoXY(xx, y + 1);           cout << u8"     ┐  ▐▓╬╣@W╖                         ▓╣Ç       ";
		GotoXY(xx, y + 2);           cout << u8"     ╫║  ▓▓` , `╙╣▓▓▓▓╣▒▒▒▒▒░▒▒░]╫▓╣Ü╚╖╓╓]▌ r║    ";
		GotoXY(xx, y + 3);           cout << u8"    j▓▒U╓▓▓▒▒╜   ▓▓▓▓▓╣▒▒▒▒▒░▒▒░▒▓▓╣  ╙╨▒╫▓ ╣▓▌   ";
		GotoXY(xx, y + 4);           cout << u8" ╖,  ╙▓N▓▓▓▌╓    ▓▓╣▓▓╣▒▒▒▒▒░░░░▒▓▓▓  ╘╖╓╠▒ ╫▀ ╓@╬";
		GotoXY(xx, y + 5);           cout << u8"  ╣▒▓▄ Ü▓▒▒▓▌    ┌╣▓▓▓▓╣▒▒▒▒░░░░╫▓╣M   ╙╫▓▓▓∩┌▓╣║ ";
		GotoXY(xx, y + 6);           cout << u8"   ╙▓▓Ü▓▓╜╟Ü╢W    ▓▓▓▓▓╣▒▒▒▒░░░╠▓▓▓    ╔╫▓╖╙▓▓▓`  ";
		GotoXY(xx, y + 7);           cout << u8"     ╘╣╜╓▒▒∩  ╝╗   ▓▓▓▓╣╣▒▒▒▒▒▒▓▓╣C  ╓Ñ`L╢▒▓ M╓╦@@";
		GotoXY(xx, y + 8);           cout << u8"└▓╣▒╢▓▄t▓▓╝▐▓  '╙N,╙▓▓▓▓╣╣▒▒▒▒╫█▓▓,╗╜  ╟▓╗╚▓╬▓▓▓╢╩";
		GotoXY(xx, y + 9);           cout << u8"  '╙╩▓▓▓▓▓║▒▒[▓   ╙▓▓▓▓▓╣╣╣▒╢█▓▓╜` ╓▓ ║╢╢▌▐╬C`    ";
		GotoXY(xx, y + 10);          cout << u8"     ,╓╟▒╕▓▓▓░║▒╢    ╙▓▓▓▓▓▓▓█▓▀  ,~╬╢▓ ▓▓▌╬▓▓╣╫▓▄";
		GotoXY(xx, y + 11);          cout << u8"  ╚▓▓▒╢╫▓▓▄▓▌ ▓╣╫╙@╖   ▀▓▓▓▓█▀  ┌╬▓ ▓▓▓╖░▓▓▓▓▓▓▓╜ ";
		GotoXY(xx, y + 12);          cout << u8"     ╙╜▀▀▀╙▓*╟▀▓▓,╢▒╢   ╢▓▓▓r  ░▓╢╣,╘▓▀╫▄▄▄,      ";
		GotoXY(xx, y + 13);          cout << u8"        æ╢▓▓▓▓▓╙V║╜▓▓▓░ ]▓▓▌   ▐▓▓,▒▒▓ ▀▓▓▓▒╬W    ";
		GotoXY(xx, y + 14);          cout << u8"      ╙▀▓Ñ╩╨▀  ▓▓▓▓▓▓▀▓,▒▓╣▓U,w╨'╫▓▓▓▓▒╖  `╙╙'    ";
		GotoXY(xx, y + 15);          cout << u8"             æ▓╣▒▓▀'   ╓▒▓╣▓▒,     ╙╝╢▒╫╩═        ";
		GotoXY(xx, y + 16);          cout << u8"            ``╙╙     ▄█▓▓▓▓▓▓▓█▄                  ";
		GotoXY(xx, y + 17);          cout << u8"                   ███████████████                ";
	}
	setcolor(14);
	SetConsoleOutputCP(437);
}

// Introduce Snake when loading game 
void IntroduceSnake() {
	hideCursor();
	for (int i = 9; i <= 111; i++) {
		GotoXY(i, 26);
		if (i == 9) {
			cout << static_cast<char>(218);
			GotoXY(120 - i, 28);
			cout << static_cast<char>(217);
		}
		else if (i == 111) {
			cout << static_cast<char>(191);
			GotoXY(120 - i, 28);
			cout << static_cast<char>(192);
			Sleep(1); GotoXY(i, 27);
			cout << static_cast<char>(179);
			GotoXY(120 - i, 27);
			cout << static_cast<char>(179);
		}
		else {
			cout << static_cast<char>(196);
			GotoXY(120 - i, 28);
			cout << static_cast<char>(196);
		}
		
		Sleep(1);
	}
	Sleep(50);	drawTeamTitle(9, 1);
	GotoXY(93, 25);	cout << "NOW LOADING...";
	for (int i = 10; i <= 110; i++) {
		GotoXY(i, 27);	setcolor(11);
		if (i < 30)
			Sleep(60);
		else if (i < 70)
			Sleep(45);
		else
			Sleep(35);
		cout << static_cast<char>(178);
		hideCursor();
		GotoXY(107, 25); cout << i - 10 << "%";
		
	}
	setcolor(15); Sleep(50);
	clrscr();	GotoXY(30, 10);
	drawSnakeImage(0, 0);
	drawSnakeTitle(90, 0, 2);
	Sleep(800);
	setcolor(15);	GotoXY(82, 26);
	cout << "Type ENTER to continue";
	for (int i = 1; i <= 9; i++) {
		if (kbhit())
		{
			int c = getch();
			if (c == 13) return;
		}
		if (i % 2 == 1)
			setcolor(15);
		else
			setcolor(0);
		GotoXY(87, 26); cout << "ENTER";
		GotoXY(0, 0);	Sleep(700);
	}
	int key = 0;
	while (key != 13)
	{
		key = _getch();
	}
}

// Ve hinh con ran
void drawSnakeImage(long x, long y) {
	SetConsoleOutputCP(65001);
	GotoXY(x, y);    cout << u8"▒▒              ██████                                                         ";
	GotoXY(x, ++y);  cout << u8"████▒▒        ██▒▒▒▒▒▒██████████          ████████                             ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                           ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████        ██▓▓▒▒▒▒▒▒▓▓██                         ";
	GotoXY(x, ++y);  cout << u8"▓▓██  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██    ▒▒▒▒▒▒▒▒  ██▒▒▒▒▒▒██                           ";
	GotoXY(x, ++y);  cout << u8"▓▓██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██  ▒▒▒▒██████▒▒▒▒  ██▒▒██▒▒████                       ";
	GotoXY(x, ++y);  cout << u8"██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████  ▒▒▒▒██░░░░░░██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██                     ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██░░  ██    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████               ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████░░    ██    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████         ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██    ██    ██▒▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██       ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒      ▒▒▒▒████    ██▒▒▒▒▓▓████▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██     ";
	GotoXY(x, ++y);  cout << u8" ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒  ▒▒▒▒▒▒████▒▒▒▒▒▒██▒▒▒▒  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██      ";
	GotoXY(x, ++y);  cout << u8"▒▒  ░░████  ▒▒▒▒  ▒▒▒▒▒▒████  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░██  ▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒██     ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒▒▒▒▒▓▓██    ▒▒▒▒▒▒██▓▓▓▓██  ▒▒▒▒▒▒▒▒▒▒    ░░██░░██  ▒▒▒▒▓▓██▒▒▒▒▒▒██       ";
	GotoXY(x, ++y);  cout << u8"▓▓▒▒▒▒▒▒██▓▓▓▓▒▒▒▒▒▒██▓▓▓▓▒▒▓▓▓▓░░▒▒▒▒░░░░▓▓▓▓▓▓░░▓▓██  ░░  ▒▒▒▒▒▒▒▒▓▓░░       ";
	GotoXY(x, ++y);  cout << u8"▓▓██▒▒▒▒▒▒▒▒▒▒██████▓▓▓▓▓▓▓▓▒▒████░░░░████▓▓▓▓▓▓██▓▓▓▓██████  ██████           ";
	GotoXY(x, ++y);  cout << u8"▒▒▒▒██▒▒▒▒▒▒▒▒▓▓▒▒▓▓██▓▓▓▓▒▒██░░░░████▓▓▓▓████▓▓▓▓▓▓██      ██                 ";
	GotoXY(x, ++y);  cout << u8"  ██▓▓██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▒▒██░░░░░░▒▒████░░░░██████░░                       ";
	GotoXY(x, ++y);  cout << u8"       ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▓▓░░░░░░░░░░░░░░░░░░░░██▒▒▓▓                    ";
	GotoXY(x, ++y);  cout << u8"         ▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓                  ";
	GotoXY(x, ++y);  cout << u8"             ██████▒▒▒▒▒▒▒▒▒▒▒▒▓▓██░░░░░░████████░░░░░░░░░░  ██                ";
	GotoXY(x, ++y);  cout << u8"                   ████▒▒▒▒▒▒▒▒▒▒▓▓██░░██▓▓▓▓██  ██████░░░░░░  ██              ";
	GotoXY(x, ++y);  cout << u8"                       ████▒▒▒▒▒▒▒▒▓▓████▒▒▒▒██  ██▓▓▓▓████░░░░  ████    ██    ";
	GotoXY(x, ++y);  cout << u8"                           ████▒▒▒▒▒▒▒▒▒▒██▒▒██    ████▓▓▓▓██░░░░    ████  ██  ";
	GotoXY(x, ++y);  cout << u8"                               ██▒▒▒▒▒▒▒▒▓▓██░░░░████▓▓▒▒██  ▓▓██░░░░░░░░░░██  ";
	GotoXY(x, ++y);  cout << u8"                                 ████▒▒▒▒▒▒▓▓████▓▓▓▓▒▒██        ████░░░░██    ";
	GotoXY(x, ++y);  cout << u8"                                 ░░░░▓▓▒▒▒▒▒▒▓▓▓▓▒▒████░░          ░░██▓▓░░    ";
	GotoXY(x, ++y);  cout << u8"                                       ████▒▒▒▒████                            ";
	GotoXY(x, ++y);  cout << u8"                                           ████                                ";
	SetConsoleOutputCP(437);
	return;
}

// Vẽ art cho các section trong menu
void drawSectionTitle(long x, long y, int title) {
	SetConsoleOutputCP(65001);	long xx = x;
	// Draw section title 1
	if (title == 1) {
		GotoXY(xx, y);     cout << u8"░█▄─░█ ░█▀▀▀ ░█──░█    ░█▀▀█ ─█▀▀█ ░█▀▄▀█ ░█▀▀▀";
		GotoXY(xx, y + 1); cout << u8"░█░█░█ ░█▀▀▀ ░█░█░█    ░█─▄▄ ░█▄▄█ ░█░█░█ ░█▀▀▀";
		GotoXY(xx, y + 2); cout << u8"░█──▀█ ░█▄▄▄ ░█▄▀▄█    ░█▄▄█ ░█─░█ ░█──░█ ░█▄▄▄";
	}
	// Draw section title 2
	if (title == 2) {
		GotoXY(xx, y);     cout << u8"░█─── ░█▀▀▀█ ─█▀▀█ ░█▀▀▄    ░█▀▀█ ─█▀▀█ ░█▀▄▀█ ░█▀▀▀";
		GotoXY(xx, y + 1); cout << u8"░█─── ░█──░█ ░█▄▄█ ░█─░█    ░█─▄▄ ░█▄▄█ ░█░█░█ ░█▀▀▀";
		GotoXY(xx, y + 2); cout << u8"░█▄▄█ ░█▄▄▄█ ░█─░█ ░█▄▄▀    ░█▄▄█ ░█─░█ ░█──░█ ░█▄▄▄";
	}
	// Draw section title 3
	if (title == 3) {
		GotoXY(xx, y);     cout << u8"░█─░█ ▀█▀ ░█▀▀█ ░█─░█    ░█▀▀▀█ ░█▀▀█ ░█▀▀▀█ ░█▀▀█ ░█▀▀▀";
		GotoXY(xx, y + 1); cout << u8"░█▀▀█ ░█─ ░█─▄▄ ░█▀▀█    ─▀▀▀▄▄ ░█─── ░█──░█ ░█▄▄▀ ░█▀▀▀";
		GotoXY(xx, y + 2); cout << u8"░█─░█ ▄█▄ ░█▄▄█ ░█─░█    ░█▄▄▄█ ░█▄▄█ ░█▄▄▄█ ░█─░█ ░█▄▄▄";
	}
	// Draw section title 4
	if (title == 4) {
		GotoXY(xx, y);     cout << u8"▀█▀ ░█▄─░█ ▀▀█▀▀ ░█▀▀█ ░█▀▀▀█ ░█▀▀▄ ░█─░█ ░█▀▀█ ▀▀█▀▀ ▀█▀ ░█▀▀▀█ ░█▄─░█";
		GotoXY(xx, y + 1); cout << u8"░█─ ░█░█░█ ─░█── ░█▄▄▀ ░█──░█ ░█─░█ ░█─░█ ░█─── ─░█── ░█─ ░█──░█ ░█░█░█";
		GotoXY(xx, y + 2); cout << u8"▄█▄ ░█──▀█ ─░█── ░█─░█ ░█▄▄▄█ ░█▄▄▀ ─▀▄▄▀ ░█▄▄█ ─░█── ▄█▄ ░█▄▄▄█ ░█──▀█";
	}
	// Draw section title 5
	if (title == 5) {
		GotoXY(xx, y);     cout << u8"░█▀▀▀ ▀▄░▄▀ ▀█▀ ▀▀█▀▀";
		GotoXY(xx, y + 1); cout << u8"░█▀▀▀ ─░█── ░█─ ─░█──";
		GotoXY(xx, y + 2); cout << u8"░█▄▄▄ ▄▀░▀▄ ▄█▄ ─░█──";
	}
	xx = 37;	y = 10;	setcolor(10);
	GotoXY(xx, y);       cout << u8"  ▄▄▓█████▓▄     ▄▄▓█▓▄  ";
	GotoXY(xx, y + 1);   cout << u8" ▄▓▀▀▀██████▓▄ ▄▓█████▓▌ ";
	GotoXY(xx, y + 2);   cout << u8"       ▄▓███████████▓▀▀▓ ";
	GotoXY(xx, y + 3);   cout << u8"   ▄▓█████████▓████▓▄    ";
	GotoXY(xx, y + 4);   cout << u8" ▄▓████▓███▓█████████▓▄  ";
	GotoXY(xx, y + 5);   cout << u8"▐▓██▓▓▀▀▓▓███████▓▓▀▓█▓▄ ";
	GotoXY(xx, y + 6);   cout << u8"▓▀▀    ▄▓██▓██████▓▄ ▀▓█▓";
	GotoXY(xx, y + 7);   cout << u8"      ▓██▓▀ ██ ▀▓██▓▄  ▀▌";
	GotoXY(xx, y + 8);   cout << u8"     ▄███▀ ▐█▌   ▀▓█▓▌   ";
	GotoXY(xx, y + 9);   cout << u8"    ▐▓██▓  ██▌     ▓▓█   ";
	GotoXY(xx, y + 10);  cout << u8"    ▐▓█▓  ▐██       ▀▓▌  ";
	GotoXY(xx, y + 11);  cout << u8"     ▓█▀  ██▌        ▀   ";
	GotoXY(xx, y + 12);  cout << u8"      ▀   ██▌            ";
	GotoXY(xx, y + 13);  cout << u8"         ▐██▌            ";
	GotoXY(xx, y + 14);  cout << u8"         ███             ";
	setcolor(9);	 xx = 63;	y = 4;
	GotoXY(xx, y);       cout << u8"¸¸.•*¨*•♫♪♪♫•*¨*•.¸¸";
	setcolor(15);	y = 15;	xx = 85;
	GotoXY(xx, y + 1);    cout << u8"  •   °▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄  °";
	GotoXY(xx, y + 2);    cout << u8"˛   ˛• █▒▒░░░░░░░░░▒▒█ ˚ ";
	GotoXY(xx, y + 3);    cout << u8"  ˚˛ ˚  █░░█░░░░░█░░█  ˚•";
	GotoXY(xx, y + 4);    cout << u8"     ▄▄  █░░░▀█▀░░░█  ▄▄  ";
	GotoXY(xx, y + 5);    cout << u8"  * █░░█ ▀▄░░░░░░░▄▀ █░░█ ";
	GotoXY(xx, y + 6);    cout << u8"    █▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█";
	GotoXY(xx, y + 7);    cout << u8"    █░░╦ ╦╔╗╦ ╔╗╔╗╔╦╗╔╗░░█";
	GotoXY(xx, y + 8);    cout << u8"    █░░║║║╠ ║ ║ ║║║║║╠ ░░█";
	GotoXY(xx, y + 9);    cout << u8"    █░░╚╩╝╚╝╚╝╚╝╚╝╩ ╩╚╝░░█";
	SetConsoleOutputCP(437);	setcolor(14);
}

// Vẽ art title snake 
void drawSnakeTitle(long x, long y, int title)
{
	SetConsoleOutputCP(65001);
	int color[][5] = { {9, 11,13,14,15},{12,10,15,9,14},
		{14,11,13,12,9},{15,9,12,10,14},{10,14,11,12,13} };
	// Draw title 1
	if (title == 1) {
		srand((unsigned int)time(NULL));
		int i = rand() % 4 + 1;
		if (i == 5) i = 1;
		long xx = x;
		setcolor(color[i][0]);
		GotoXY(xx, y);     cout << u8"███████╗";
		GotoXY(xx, y + 1); cout << u8"██╔════╝";
		GotoXY(xx, y + 2); cout << u8"███████╗";
		GotoXY(xx, y + 3); cout << u8"╚════██║";
		GotoXY(xx, y + 4); cout << u8"███████║";
		GotoXY(xx, y + 5); cout << u8"╚══════╝";
		setcolor(color[i][1]);
		xx += 10;
		GotoXY(xx, y);     cout << u8"███╗   ██╗";
		GotoXY(xx, y + 1); cout << u8"████╗  ██║";
		GotoXY(xx, y + 2); cout << u8"██╔██╗ ██║";
		GotoXY(xx, y + 3); cout << u8"██║╚██╗██║";
		GotoXY(xx, y + 4); cout << u8"██║ ╚████║";
		GotoXY(xx, y + 5); cout << u8"╚═╝  ╚═══╝";
		setcolor(color[i][2]);
		xx += 12;
		GotoXY(xx, y);     cout << u8" █████╗ ";
		GotoXY(xx, y + 1); cout << u8"██╔══██╗";
		GotoXY(xx, y + 2); cout << u8"███████║";
		GotoXY(xx, y + 3); cout << u8"██╔══██║";
		GotoXY(xx, y + 4); cout << u8"██║  ██║";
		GotoXY(xx, y + 5); cout << u8"╚═╝  ╚═╝";
		setcolor(color[i][3]);
		xx += 10;
		GotoXY(xx, y);     cout << u8"██╗  ██╗";
		GotoXY(xx, y + 1); cout << u8"██║ ██╔╝";
		GotoXY(xx, y + 2); cout << u8"█████╔╝ ";
		GotoXY(xx, y + 3); cout << u8"██╔═██╗ ";
		GotoXY(xx, y + 4); cout << u8"██║  ██╗";
		GotoXY(xx, y + 5); cout << u8"╚═╝  ╚═╝";
		setcolor(color[i][4]);
		xx += 10;
		GotoXY(xx, y);     cout << u8"███████╗";
		GotoXY(xx, y + 1); cout << u8"██╔════╝";
		GotoXY(xx, y + 2); cout << u8"█████╗  ";
		GotoXY(xx, y + 3); cout << u8"██╔══╝  ";
		GotoXY(xx, y + 4); cout << u8"███████╗";
		GotoXY(xx, y + 5); cout << u8"╚══════╝";
		i++;
	}
	// Clear title 1
	if (title == -1) {
		long xx = x;
		GotoXY(xx, y);     cout << u8"        ";
		GotoXY(xx, y + 1); cout << u8"        ";
		GotoXY(xx, y + 2); cout << u8"        ";
		GotoXY(xx, y + 3); cout << u8"        ";
		GotoXY(xx, y + 4); cout << u8"        ";
		GotoXY(xx, y + 5); cout << u8"        ";
		xx += 11;
		GotoXY(xx, y);     cout << u8"        ";
		GotoXY(xx, y + 1); cout << u8"        ";
		GotoXY(xx, y + 2); cout << u8"        ";
		GotoXY(xx, y + 3); cout << u8"        ";
		GotoXY(xx, y + 4); cout << u8"        ";
		GotoXY(xx, y + 5); cout << u8"        ";
		xx += 11;
		GotoXY(xx, y);     cout << u8"        ";
		GotoXY(xx, y + 1); cout << u8"        ";
		GotoXY(xx, y + 2); cout << u8"        ";
		GotoXY(xx, y + 3); cout << u8"        ";
		GotoXY(xx, y + 4); cout << u8"        ";
		GotoXY(xx, y + 5); cout << u8"        ";
		xx += 11;
		GotoXY(xx, y);     cout << u8"        ";
		GotoXY(xx, y + 1); cout << u8"        ";
		GotoXY(xx, y + 2); cout << u8"        ";
		GotoXY(xx, y + 3); cout << u8"        ";
		GotoXY(xx, y + 4); cout << u8"        ";
		GotoXY(xx, y + 5); cout << u8"        ";
		xx += 11;
		GotoXY(xx, y);     cout << u8"        ";
		GotoXY(xx, y + 1); cout << u8"        ";
		GotoXY(xx, y + 2); cout << u8"        ";
		GotoXY(xx, y + 3); cout << u8"        ";
		GotoXY(xx, y + 4); cout << u8"        ";
		GotoXY(xx, y + 5); cout << u8"        ";
	}
	// Draw title 2
	else if (title == 2) {
		setcolor(12);
		GotoXY(x, y + 1); cout << u8"   ▄████████";
		GotoXY(x, y + 2); cout << u8"  ███    ███";
		GotoXY(x, y + 3); cout << u8"  ███    █▀ ";
		GotoXY(x, y + 4); cout << u8"  ███       ";
		GotoXY(x, y + 5); cout << u8"▀███████████";
		GotoXY(x, y + 6); cout << u8"         ███";
		GotoXY(x, y + 7); cout << u8"   ▄█    ███";
		GotoXY(x, y + 8); cout << u8" ▄████████▀ ";
		Sleep(250); setcolor(10);
		x += 18;  y += 3;
		GotoXY(x, y + 1); cout << u8"███▄▄▄▄  ";
		GotoXY(x, y + 2); cout << u8"███▀▀▀██▄";
		GotoXY(x, y + 3); cout << u8"███   ███";
		GotoXY(x, y + 4); cout << u8"███   ███";
		GotoXY(x, y + 5); cout << u8"███   ███";
		GotoXY(x, y + 6); cout << u8"███   ███";
		GotoXY(x, y + 7); cout << u8"███   ███";
		GotoXY(x, y + 8); cout << u8" ▀█   █▀ ";
		Sleep(250); setcolor(11);
		x -= 32;  y += 6;
		GotoXY(x, y + 1); cout << u8"   ▄████████";
		GotoXY(x, y + 2); cout << u8"  ███    ███";
		GotoXY(x, y + 3); cout << u8"  ███    ███";
		GotoXY(x, y + 4); cout << u8"  ███    ███";
		GotoXY(x, y + 5); cout << u8"▀███████████";
		GotoXY(x, y + 6); cout << u8"  ███    ███";
		GotoXY(x, y + 7); cout << u8"  ███    ███";
		GotoXY(x, y + 8); cout << u8"  ███    █▀ ";
		Sleep(250); setcolor(13);
		x += 16; y += 3;
		GotoXY(x, y + 1); cout << u8"   ▄█   ▄█▄";
		GotoXY(x, y + 2); cout << u8"  ███ ▄███▀";
		GotoXY(x, y + 3); cout << u8"  ███▐██▀  ";
		GotoXY(x, y + 4); cout << u8" ▄█████▀   ";
		GotoXY(x, y + 5); cout << u8"▀▀█████▄   ";
		GotoXY(x, y + 6); cout << u8"  ███▐██▄  ";
		GotoXY(x, y + 7); cout << u8"  ███ ▀███▄";
		GotoXY(x, y + 8); cout << u8"  ███   ▀█▀";
		GotoXY(x, y + 9); cout << u8"  ▀        ";
		Sleep(150); setcolor(14);
		x += 14;  y += 4;
		GotoXY(x, y + 1); cout << u8"   ▄████████";
		GotoXY(x, y + 2); cout << u8"  ███    ███";
		GotoXY(x, y + 3); cout << u8"  ███    █▀ ";
		GotoXY(x, y + 4); cout << u8" ▄███▄▄▄    ";
		GotoXY(x, y + 5); cout << u8"▀▀███▀▀▀    ";
		GotoXY(x, y + 6); cout << u8"  ███    █▄ ";
		GotoXY(x, y + 7); cout << u8"  ███    ███";
		GotoXY(x, y + 8); cout << u8"  ██████████";
		Sleep(100);	SetConsoleOutputCP(437);
	}
	// Draw title 3
	else if (title == 3) {
		setcolor(15);
		GotoXY(x, y);
		cout << u8"  ██████  ███▄    █  ▄▄▄       ██ ▄█▀▓█████ ";
		GotoXY(x, y + 1);
		cout << u8"▒██    ▒  ██ ▀█   █ ▒████▄     ██▄█▒ ▓█   ▀ ";
		GotoXY(x, y + 2);
		cout << u8"░ ▓██▄   ▓██  ▀█ ██▒▒██  ▀█▄  ▓███▄░ ▒███   ";
		GotoXY(x, y + 3);
		cout << u8"  ▒   ██▒▓██▒  ▐▌██▒░██▄▄▄▄██ ▓██ █▄ ▒▓█  ▄ ";
		GotoXY(x, y + 4);
		cout << u8"▒██████▒▒▒██░   ▓██░ ▓█   ▓██▒▒██▒ █▄░▒████▒";
		GotoXY(x, y + 5);
		cout << u8"▒ ▒▓▒ ▒ ░░ ▒░   ▒ ▒  ▒▒   ▓▒█░▒ ▒▒ ▓▒░░ ▒░ ░";
		GotoXY(x, y + 6);
		cout << u8"░ ░▒  ░ ░░ ░░   ░ ▒░  ▒   ▒▒ ░░ ░▒ ▒░ ░ ░  ░";
		GotoXY(x, y + 7);
		cout << u8"░  ░  ░     ░   ░ ░   ░   ▒   ░ ░░ ░    ░   ";
		GotoXY(x, y + 8);
		cout << u8"░           ░       ░  ░░  ░      ░  ░      ";
	}
	// Clear title 3
	else if (title == -3) {
		setcolor(15);
		GotoXY(x, y);
		cout << u8"                                            ";
		GotoXY(x, y + 1);
		cout << u8"                                            ";
		GotoXY(x, y + 2);
		cout << u8"                                            ";
		GotoXY(x, y + 3);
		cout << u8"                                            ";
		GotoXY(x, y + 4);
		cout << u8"                                            ";
		GotoXY(x, y + 5);
		cout << u8"                                            ";
		GotoXY(x, y + 6);
		cout << u8"                                            ";
		GotoXY(x, y + 7);
		cout << u8"                                            ";
		GotoXY(x, y + 8);
		cout << u8"                                            ";
	}
	SetConsoleOutputCP(437);	setcolor(14);
}

// Vẽ art các thông tin màn hiện tại

void drawStageTitle(int cur_stage) {
	SetConsoleOutputCP(65001);	setcolor(15);
	if (cur_stage == 1) {
		long x = 101; long y = 3;
		GotoXY(x, y);       cout << u8"██╗";
		GotoXY(x, y + 1);   cout << u8"██║";
		GotoXY(x, y + 2);   cout << u8"██║";
		GotoXY(x, y + 3);   cout << u8"██║";
		GotoXY(x, y + 4);   cout << u8"██║";
		GotoXY(x, y + 5);   cout << u8"╚═╝";
	}
	if (cur_stage == 2) {
		long x = 98; long y = 3;
		GotoXY(x, y);       cout << u8"██╗  ██╗";
		GotoXY(x, y + 1);   cout << u8"██║  ██║";
		GotoXY(x, y + 2);   cout << u8"██║  ██║";
		GotoXY(x, y + 3);   cout << u8"██║  ██║";
		GotoXY(x, y + 4);   cout << u8"██║  ██║";
		GotoXY(x, y + 5);   cout << u8"╚═╝  ╚═╝";
	}
	if (cur_stage == 3) {
		long x = 96; long y = 3;
		GotoXY(x, y);       cout << u8"██╗  ██╗  ██╗";
		GotoXY(x, y + 1);   cout << u8"██║  ██║  ██║";
		GotoXY(x, y + 2);   cout << u8"██║  ██║  ██║";
		GotoXY(x, y + 3);   cout << u8"██║  ██║  ██║";
		GotoXY(x, y + 4);   cout << u8"██║  ██║  ██║";
		GotoXY(x, y + 5);   cout << u8"╚═╝  ╚═╝  ╚═╝";
	}
	if (cur_stage == 4) {
		long x = 95; long y = 3;
		GotoXY(x, y);       cout << u8"██╗  ██╗   ██╗";
		GotoXY(x, y + 1);   cout << u8"██║  ██║   ██║";
		GotoXY(x, y + 2);   cout << u8"██║  ╚██╗ ██╔╝";
		GotoXY(x, y + 3);   cout << u8"██║   ╚████╔╝ ";
		GotoXY(x, y + 4);   cout << u8"██║    ╚██╔╝  ";
		GotoXY(x, y + 5);   cout << u8"╚═╝     ╚═╝   ";
	}
	if (cur_stage == 5) {
		long x = 94; long y = 3;
		GotoXY(x, y);       cout << u8" █████╗  █████╗ ";
		GotoXY(x, y + 1);   cout << u8"██   ██╗██   ██╗";
		GotoXY(x, y + 2);   cout << u8"██   ██║██   ██║";
		GotoXY(x, y + 3);   cout << u8"██   ██║██   ██║";
		GotoXY(x, y + 4);   cout << u8"╚█████╔╝╚█████╔╝";
		GotoXY(x, y + 5);   cout << u8" ╚════╝  ╚════╝ ";
	}
	SetConsoleOutputCP(437);	setcolor(14);
}

void drawProcessFood(int n) {
	if (n == 1) {
		GotoXY(107, 23);
		cout << static_cast<char>(176) << static_cast<char>(176) << static_cast<char>(176) << static_cast<char>(176)
			<< static_cast<char>(176) << static_cast<char>(176) << static_cast<char>(176) << static_cast<char>(176);
	}
	if (n == 2) {
		GotoXY(107, 22);
		cout << static_cast<char>(177) << static_cast<char>(177) << static_cast<char>(177) << static_cast<char>(177)
			<< static_cast<char>(177) << static_cast<char>(177) << static_cast<char>(177) << static_cast<char>(177);
	}
	if (n == 3) {
		GotoXY(107, 21);
		cout << static_cast<char>(178) << static_cast<char>(178) << static_cast<char>(178) << static_cast<char>(178)
			<< static_cast<char>(178) << static_cast<char>(178) << static_cast<char>(178) << static_cast<char>(178);
	}
	if (n == 4) {
		GotoXY(107, 20);
		cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219)
			<< static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219);
	}
}

//hàm tạo một mảng chứa cả thức ăn và cổng
void GenerateFood()
{
	int x, y;
	srand((unsigned int)time(NULL));
	if (FOOD_INDEX == MAX_SIZE_FOOD - 2)
	{
		FOOD_INDEX++;
	}
	if (FOOD_INDEX < MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX++;
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false || barrier[y][x]);
		food[FOOD_INDEX] = { x,y };
	}
	//tao cong
	else
	{
		while (true)
		{
			do {
				x = rand() % (WIDTH_CONSOLE - 1) + 1;
				y = rand() % (HEIGH_CONSOLE - 1) + 1;
			} while (IsValid(x, y) == false || barrier[y][x]);
			for (int i = 0; i < SIZE_SNAKE; i++)
			{
				if (((snake[i].x - 4) < x && x < (snake[i].x + 4))
					|| ((snake[i].y - 4) < y && y < (snake[i].y + 4)))
				{
					continue;
				}
			}
			if (stage % 2 == 0) {
				if (x > WIDTH_CONSOLE / 2) {
					if (checkGateRight(x, y)) {
						typegate = 'r';
						food[MAX_SIZE_FOOD - 1] = { x,y };
						break;
					}
				}
				if (x < WIDTH_CONSOLE / 2) {
					if (checkGateLeft(x, y)) {
						typegate = 'l';
						food[MAX_SIZE_FOOD - 1] = { x,y };
						break;
					}
				}
			}
			if (stage % 2 == 1) {
				if (y < HEIGH_CONSOLE / 2) {
					if (checkGateUp(x, y)) {
						typegate = 'u';
						food[MAX_SIZE_FOOD - 1] = { x,y };
						break;
					}
				}
				if (y > HEIGH_CONSOLE / 2) {
					if (checkGateDown(x, y)) {
						typegate = 'd';
						food[MAX_SIZE_FOOD - 1] = { x,y };
						break;
					}
				}
			}
		}
	}
}

// Reset Barrier
void ResetBarrier()
{
	for (int i = 0; i < HEIGH_CONSOLE; i++)
	{
		for (int j = 0; j < WIDTH_CONSOLE; j++)
		{
			barrier[i][j] = false;
		}
	}
}

//dat lai trang thai ban dau cua con ran
void ResetData()
{
	CHAR_LOCK = 75, MOVING = 77;
	SPEED = 1; SCORE = 0; FOOD_INDEX = -1;
	SIZE_SNAKE = 8; stage = 1;
	WIDTH_CONSOLE = WIDTH_CONSOLE_DEFAULT;
	HEIGH_CONSOLE = HEIGH_CONSOLE_DEFAULT;
	// khởi tạo giá trị ban đầu cho con rắn
	snake[0] = { 10, 2 }; snake[1] = { 11, 2 };
	snake[2] = { 12, 2 }; snake[3] = { 13, 2 };
	snake[4] = { 14, 2 }; snake[5] = { 15, 2 };
	snake[6] = { 16, 2 }; snake[7] = { 17, 2 };
	//khởi tạo các chướng ngại vật
	ResetBarrier();
	createStage1(HEIGH_CONSOLE, WIDTH_CONSOLE);
	setcolor(mainColorText);
	GenerateFood();
}

// Crate Stage game 1
void createStage1(int height, int width)
{
	//if (createdStage) return;
	ResetBarrier();
	int midX = width / 2;
	int midY = height / 2;
	int maxY = midY - 4;
	int maxX = (int)(0.7 * midX);

	for (int i = 1; i <= maxY; i++)
	{
		barrier[midY - i][midX + i] = true;

		barrier[midY + i][midX + i] = true;

		barrier[midY - i][midX - i] = true;

		barrier[midY + i][midX - i] = true;

		if (i == maxY)
		{
			for (int k = 0; k < 0.8 * maxX; k++)
			{

				barrier[midY - i][midX + i + k] = true;

				barrier[midY + i][midX + i + k] = true;
			}
			for (int k = 0; k < 0.8 * maxX; k++)
			{
				barrier[midY - i][midX - i - k] = true;

				barrier[midY + i][midX - i - k] = true;
			}
		}
	}
	return;
}
// Crate Stage game 2
void createStage2(int height, int width)
{
	//if (createdStage) return;
	ResetBarrier();
	int midX = width / 2;
	int midY = height / 2;
	int lenX = 10;
	int lenY = (int)(0.2 * height);
	int maxY = (int)(0.25 * height);
	int maxX = (int)(0.3 * width);

	for (int j = 0; j < maxY; j++)
	{
		barrier[lenY + j][lenX] = true;

		barrier[height - lenY - j][lenX] = true;

		barrier[lenY + j][width - lenX] = true;

		barrier[height - lenY - j][width - lenX] = true;
	}

	for (int i = 0; i <= maxX; i++)
	{
		barrier[lenY][lenX + i] = true;

		barrier[lenY][width - lenX - i] = true;

		barrier[height - lenY][lenX + i] = true;

		barrier[height - lenY][width - lenX - i] = true;
	}

	for (int i = 0; i < maxY; i++)
	{
		barrier[midY][midX + 3 * i] = true;

		barrier[midY][midX - 3 * i] = true;

		barrier[midY - i][midX] = true;

		barrier[midY + i][midX] = true;
	}
	return;
}
// Crate Stage game 3
void createStage3(int height, int width)
{
	//if (createdStage) return;
	ResetBarrier();
	int midX = width / 2;
	int midY = height / 2;
	int lenX = midX / 3;
	int lenY = midY / 3;
	int maxY = (int)(0.27 * height);
	int maxX = (int)(0.27 * width);

	// Vẽ trung tâm
	for (int i = 1; i <= 3; i++)
	{
		barrier[midY - i][midX + i] = true;

		barrier[midY + i][midX + i] = true;

		barrier[midY - i][midX - i] = true;

		barrier[midY + i][midX - i] = +true;
	}

	// Vẽ chiều đứng
	for (int j = 0; j < maxY; j++)
	{
		barrier[midY + lenY + j][midX - lenX] = true;

		barrier[midY - lenY - j][midX - lenX] = true;

		barrier[midY + lenY + j][midX + lenX] = true;

		barrier[midY - lenY - j][lenX + midX] = true;
	}

	// Vẽ chiều ngang
	for (int i = 0; i < maxX; i++)
	{
		barrier[midY - lenY][midX + lenX + i] = true;

		barrier[midY - lenY][midX - lenX - i] = true;

		barrier[midY + lenY][midX + lenX + i] = true;

		barrier[midY + lenY][midX - lenX - i] = true;
	}
	return;
}
// Crate Stage game 4
void createStage4(int height, int width)
{
	//if (createdStage) return;
	ResetBarrier();
	int lenX = width / 5;
	int lenY = height / 5;
	// Vẽ cột ngang
	for (int i = 1; i <= 0.8 * (width - lenX); i++)
	{
		barrier[lenY][i] = true;
		barrier[height - lenY][width - i] = true;
	}
	// Vẽ chiều đứng
	for (int j = 1; j <= 0.8 * (height - lenY); j++)
	{
		barrier[j][width - lenX] = true;
		barrier[height - j][lenX] = true;
	}
	return;
}

void EraseTheGate()
{
	POINT gate = { 9,2 };
	//Ria tren
	GotoXY(gate.x, gate.y - 1);
	cout << " ";
	GotoXY(gate.x + 1, gate.y - 1);
	cout << " ";
	GotoXY(gate.x + 2, gate.y - 1);
	cout << " ";
	GotoXY(gate.x + 3, gate.y - 1);
	cout << " ";
	GotoXY(gate.x, gate.y);
	setcolor(rand() % 5 + 11);
	cout << " ";// xoa trung diem
	setcolor(mainColorText);
	//Ria duoi
	GotoXY(gate.x, gate.y + 1);
	cout << " ";
	GotoXY(gate.x + 1, gate.y + 1);
	cout << " ";
	GotoXY(gate.x + 2, gate.y + 1);
	cout << " ";
	GotoXY(gate.x + 3, gate.y + 1);
	cout << " ";
	// Ria phai
	setcolor(12);
	GotoXY(gate.x - 1, gate.y - 1);
	cout << " ";
	GotoXY(gate.x - 1, gate.y);
	cout << " ";
	GotoXY(gate.x - 1, gate.y + 1);
	cout << " ";
	setcolor(mainColorText);
}

void DrawTheGate()
{
	POINT gate = { 9,2 };
	//Ria tren
	GotoXY(gate.x, gate.y - 1);
	cout << static_cast<char>(201);
	GotoXY(gate.x + 1, gate.y - 1);
	cout << static_cast<char>(209);
	GotoXY(gate.x + 2, gate.y - 1);
	cout << static_cast<char>(181);
	GotoXY(gate.x + 3, gate.y - 1);
	cout << static_cast<char>(16);
	GotoXY(gate.x, gate.y);
	setcolor(rand() % 5 + 11);
	cout << static_cast<char>(174); // Ve trung diem
	setcolor(mainColorText);

	//Ria duoi
	GotoXY(gate.x, gate.y + 1);
	cout << static_cast<char>(200);
	GotoXY(gate.x + 1, gate.y + 1);
	cout << static_cast<char>(207);
	GotoXY(gate.x + 2, gate.y + 1);
	cout << static_cast<char>(181);
	GotoXY(gate.x + 3, gate.y + 1);
	cout << static_cast<char>(16);

	// Ria phai
	setcolor(12);
	GotoXY(gate.x - 1, gate.y - 1);
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y);
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y + 1);
	cout << static_cast<char>(15);
	setcolor(mainColorText);
}

//ham ve cai cong qua man trai
void DrawTheGateLeft(POINT gate)
{	//Ria tren
	GotoXY(gate.x, gate.y - 1);
	barrier[gate.y - 1][gate.x] = true;
	cout << static_cast<char>(201);
	GotoXY(gate.x + 1, gate.y - 1);
	barrier[gate.y - 1][gate.x + 1] = true;
	cout << static_cast<char>(209);
	GotoXY(gate.x + 2, gate.y - 1);
	barrier[gate.y - 1][gate.x + 2] = true;
	cout << static_cast<char>(181);
	GotoXY(gate.x + 3, gate.y - 1);
	barrier[gate.y - 1][gate.x + 3] = true;
	cout << static_cast<char>(16);
	GotoXY(gate.x, gate.y);
	setcolor(rand() % 5 + 11);
	cout << static_cast<char>(174); // Ve trung diem
	setcolor(mainColorText);
	//Ria duoi
	GotoXY(gate.x, gate.y + 1);
	barrier[gate.y + 1][gate.x] = true;
	cout << static_cast<char>(200);
	GotoXY(gate.x + 1, gate.y + 1);
	barrier[gate.y + 1][gate.x + 1] = true;
	cout << static_cast<char>(207);
	GotoXY(gate.x + 2, gate.y + 1);
	barrier[gate.y + 1][gate.x + 2] = true;
	cout << static_cast<char>(181);
	GotoXY(gate.x + 3, gate.y + 1);
	barrier[gate.y + 1][gate.x + 3] = true;
	cout << static_cast<char>(16);
	// Ria phai
	setcolor(12);
	GotoXY(gate.x - 1, gate.y - 1);
	barrier[gate.y - 1][gate.x - 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y);
	barrier[gate.y][gate.x - 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y + 1);
	barrier[gate.y + 1][gate.x - 1] = true;
	cout << static_cast<char>(15);
	setcolor(mainColorText);
}

//ham ve cai cong qua man phai
void DrawTheGateRight(POINT gate)
{	//Ria tren
	GotoXY(gate.x, gate.y - 1);
	barrier[gate.y - 1][gate.x] = true;
	cout << static_cast<char>(187);
	GotoXY(gate.x - 1, gate.y - 1);
	barrier[gate.y - 1][gate.x - 1] = true;
	cout << static_cast<char>(209);
	GotoXY(gate.x - 2, gate.y - 1);
	barrier[gate.y - 1][gate.x - 2] = true;
	cout << static_cast<char>(198);
	GotoXY(gate.x - 3, gate.y - 1);
	barrier[gate.y - 1][gate.x - 3] = true;
	cout << static_cast<char>(17);
	GotoXY(gate.x, gate.y);	setcolor(rand() % 5 + 11);
	cout << static_cast<char>(175); // Ve trung diem
	setcolor(mainColorText);
	//Ria duoi
	GotoXY(gate.x, gate.y + 1);
	barrier[gate.y + 1][gate.x] = true;
	cout << static_cast<char>(188);
	GotoXY(gate.x - 1, gate.y + 1);
	barrier[gate.y + 1][gate.x + 1] = true;
	cout << static_cast<char>(207);
	GotoXY(gate.x - 2, gate.y + 1);
	barrier[gate.y + 1][gate.x + 2] = true;
	cout << static_cast<char>(198);
	GotoXY(gate.x - 3, gate.y + 1);
	barrier[gate.y + 1][gate.x - 3] = true;
	cout << static_cast<char>(17);
	// Ria phai
	setcolor(12); GotoXY(gate.x + 1, gate.y - 1);
	barrier[gate.y - 1][gate.x + 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 1, gate.y);
	barrier[gate.y][gate.x + 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 1, gate.y + 1);
	barrier[gate.y + 1][gate.x + 1] = true;
	cout << static_cast<char>(15);
	setcolor(mainColorText);
}
//ham ve cai cong qua man tren
void DrawTheGateUp(POINT gate)
{
	//Ria trai
	GotoXY(gate.x - 2, gate.y);
	barrier[gate.y][gate.x - 2] = true;
	cout << static_cast<char>(203);
	GotoXY(gate.x - 2, gate.y + 1);
	barrier[gate.y + 1][gate.x - 2] = true;
	cout << static_cast<char>(204);
	GotoXY(gate.x - 2, gate.y + 2);
	barrier[gate.y + 2][gate.x - 2] = true;
	cout << static_cast<char>(193);


	GotoXY(gate.x - 1, gate.y);
	barrier[gate.y][gate.x - 1] = true;
	cout << static_cast<char>(209);

	GotoXY(gate.x, gate.y);
	setcolor(rand() % 5 + 11);
	cout << static_cast<char>(6); // Ve trung diem
	setcolor(mainColorText);

	GotoXY(gate.x + 1, gate.y);
	barrier[gate.y][gate.x + 1] = true;
	cout << static_cast<char>(209);

	//Ria phai
	GotoXY(gate.x + 2, gate.y);
	barrier[gate.y][gate.x + 2] = true;
	cout << static_cast<char>(203);
	GotoXY(gate.x + 2, gate.y + 1);
	barrier[gate.y + 1][gate.x + 2] = true;
	cout << static_cast<char>(185);
	GotoXY(gate.x + 2, gate.y + 2);
	barrier[gate.y + 2][gate.x + 2] = true;
	cout << static_cast<char>(193);


	// Ria tren
	setcolor(12);
	GotoXY(gate.x, gate.y - 1);
	barrier[gate.y - 1][gate.x] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 1, gate.y - 1);
	barrier[gate.y - 1][gate.x + 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 2, gate.y - 1);
	barrier[gate.y - 1][gate.x + 2] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y - 1);
	barrier[gate.y - 1][gate.x - 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 2, gate.y - 1);
	barrier[gate.y - 1][gate.x - 2] = true;
	cout << static_cast<char>(15);
	setcolor(mainColorText);
}
//ham ve cai cong qua man duoi
void DrawTheGateDown(POINT gate)
{
	//Ria trai
	GotoXY(gate.x - 2, gate.y);
	barrier[gate.y][gate.x - 2] = true;
	cout << static_cast<char>(202);
	GotoXY(gate.x - 2, gate.y - 1);
	barrier[gate.y - 1][gate.x - 2] = true;
	cout << static_cast<char>(204);
	GotoXY(gate.x - 2, gate.y - 2);
	barrier[gate.y - 2][gate.x - 2] = true;
	cout << static_cast<char>(194);


	GotoXY(gate.x - 1, gate.y);
	barrier[gate.y][gate.x - 1] = true;
	cout << static_cast<char>(207);

	GotoXY(gate.x, gate.y);
	setcolor(rand() % 5 + 11);
	cout << static_cast<char>(6); // Ve trung diem
	setcolor(mainColorText);

	GotoXY(gate.x + 1, gate.y);
	barrier[gate.y][gate.x + 1] = true;
	cout << static_cast<char>(207);

	//Ria phai
	GotoXY(gate.x + 2, gate.y);
	barrier[gate.y][gate.x + 2] = true;
	cout << static_cast<char>(202);
	GotoXY(gate.x + 2, gate.y - 1);
	barrier[gate.y + 1][gate.x + 2] = true;
	cout << static_cast<char>(185);
	GotoXY(gate.x + 2, gate.y - 2);
	barrier[gate.y - 2][gate.x + 2] = true;
	cout << static_cast<char>(194);


	// Ria duoi
	setcolor(12);
	GotoXY(gate.x, gate.y + 1);
	barrier[gate.y + 1][gate.x] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 1, gate.y + 1);
	barrier[gate.y + 1][gate.x + 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x + 2, gate.y + 1);
	barrier[gate.y + 1][gate.x + 2] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 1, gate.y + 1);
	barrier[gate.y + 1][gate.x - 1] = true;
	cout << static_cast<char>(15);
	GotoXY(gate.x - 2, gate.y + 1);
	barrier[gate.y + 1][gate.x - 2] = true;
	cout << static_cast<char>(15);
	setcolor(mainColorText);
}

// Kiểm tra các vị trí xung quanh cổng để tránh cổng xuất hiện trùng với chướng ngại vật
bool checkGateLeft(int x, int y) {
	if (x - 2 <= 0 || y - 2 <= 0 || y + 2 >= HEIGH_CONSOLE) return false;
	if (!barrier[y - 2][x + 4] && !barrier[y - 1][x + 4] &&
		!barrier[y + 2][x + 4] && !barrier[y + 1][x + 4] && !barrier[y][x + 4] &&
		!barrier[y][x + 3] && !barrier[y - 1][x + 3] && !barrier[y + 1][x + 3] &&
		!barrier[y + 2][x + 3] && !barrier[y - 2][x + 3] && !barrier[y + 1][x + 2] &&
		!barrier[y - 1][x + 2] && !barrier[y + 2][x + 2] &&
		!barrier[y - 2][x + 2] && !barrier[y][x + 2] &&
		!barrier[y + 1][x + 1] && !barrier[y - 1][x + 1] && !barrier[y - 2][x + 1] &&
		!barrier[y + 2][x + 1] && !barrier[y][x + 1] && !barrier[y - 2][x - 1] &&
		!barrier[y + 2][x - 1] && !barrier[y - 1][x] && !barrier[y + 1][x] &&
		!barrier[y - 1][x - 1] && !barrier[y + 1][x - 1]
		&& !barrier[y][x - 1] && !barrier[y][x])
	{
		return true;
	}
	return false;
}
bool checkGateRight(int x, int y) {
	if (x + 2 >= WIDTH_CONSOLE || y - 2 <= 0 || y + 2 >= HEIGH_CONSOLE) return false;
	if (!barrier[y - 2][x - 4] && !barrier[y - 1][x - 4] && !barrier[y][x - 4] &&
		!barrier[y + 1][x - 4] && !barrier[y + 2][x - 4] &&
		!barrier[y - 2][x - 3] && !barrier[y - 1][x - 3] && !barrier[y][x - 3] &&
		!barrier[y + 1][x - 3] && !barrier[y + 2][x - 3] && !barrier[y - 2][x - 2] &&
		!barrier[y - 1][x - 2] && !barrier[y][x - 2] && !barrier[y + 1][x - 2] &&
		!barrier[y + 2][x - 2] && !barrier[y - 2][x - 1] && !barrier[y - 1][x - 1] &&
		!barrier[y][x - 1] && !barrier[y + 1][x - 1] && !barrier[y + 2][x - 1] &&
		!barrier[y - 2][x] && !barrier[y - 1][x] && !barrier[y + 2][x] &&
		!barrier[y][x] && !barrier[y + 1][x] && !barrier[y + 1][x + 1]
		&& !barrier[y - 1][x + 1] && !barrier[y][x + 1])
	{
		return true;
	}
	return false;
}
bool checkGateUp(int x, int y) {
	if (x - 2 <= 0 || x + 2 >= WIDTH_CONSOLE || y - 2 <= 0 || y + 2 >= HEIGH_CONSOLE) return false;
	if (!barrier[y + 3][x + 3] && !barrier[y + 3][x - 3] && !barrier[y + 3][x + 1] &&
		!barrier[y + 3][x - 2] && !barrier[y + 3][x + 2] && !barrier[y + 3][x - 1] &&
		!barrier[y + 3][x] && !barrier[y + 2][x + 3] && !barrier[y + 2][x - 3] &&
		!barrier[y + 2][x + 1] && !barrier[y + 2][x - 2] &&
		!barrier[y + 2][x + 2] && !barrier[y + 2][x - 1] && !barrier[y + 2][x] &&
		!barrier[y + 1][x + 3] && !barrier[y + 1][x - 3] && !barrier[y + 1][x + 1] &&
		!barrier[y + 1][x - 2] && !barrier[y + 1][x + 2] && !barrier[y + 1][x - 1] &&
		!barrier[y + 1][x] && !barrier[y][x - 1] && !barrier[y][x + 1] &&
		!barrier[y][x - 2] && !barrier[y][x + 2] && !barrier[y - 1][x + 2] &&
		!barrier[y - 1][x + 1] && !barrier[y + 1][x] && !barrier[y - 1][x - 2] &&
		!barrier[y - 1][x - 1] && !barrier[y - 1][x] && !barrier[y][x])
	{
		return true;
	}
	return false;
}
bool checkGateDown(int x, int y) {
	if (x - 2 <= 0 || x + 2 >= WIDTH_CONSOLE || y - 2 <= 0 || y + 2 >= HEIGH_CONSOLE) return false;
	if (!barrier[y - 3][x - 3] && !barrier[y - 3][x + 3] && !barrier[y - 3][x + 1] &&
		!barrier[y - 3][x + 2] && !barrier[y - 3][x - 2] && !barrier[y - 3][x] &&
		!barrier[y - 3][x - 1] && !barrier[y - 2][x - 1] && !barrier[y - 2][x + 1] &&
		!barrier[y - 2][x] && !barrier[y - 2][x + 2] && !barrier[y - 2][x - 2] &&
		!barrier[y - 1][x + 2] && !barrier[y - 1][x + 1] && !barrier[y - 1][x - 1] &&
		!barrier[y - 1][x] && !barrier[y - 1][x - 2] && !barrier[y][x + 2] &&
		!barrier[y][x - 2] && !barrier[y][x + 1] && !barrier[y + 1][x - 2] &&
		!barrier[y + 1][x] && !barrier[y + 1][x - 1] && !barrier[y][x] &&
		!barrier[y + 1][x + 2])
	{
		return true;
	}
	return false;
}

//hàm vẽ các chướng ngại vật
void draw_barrier()
{
	setcolor(mainColorBarrier);
	for (int i = 0; i < HEIGH_CONSOLE_DEFAULT; i++)
	{
		for (int j = 0; j < WIDTH_CONSOLE_DEFAULT; j++)
		{
			if (barrier[i][j])
			{
				GotoXY(j, i);
				cout << static_cast<char>(219);
			}
		}
	}
	setcolor(14);
}

//vẽ 1 cái khung có góc trái trên là x,y và kích thước width x height
void DrawBoard(int x, int y, int width, int height)
{
	GotoXY(x, y);	setcolor(15);
	cout << static_cast<char>(201);
	setcolor(15);
	for (int i = 1; i < width; i++)
	{
		cout << static_cast<char>(205);
	}
	cout << static_cast<char>(187);
	setcolor(12);	GotoXY(x, height + y);
	cout << static_cast<char>(200);
	for (int i = 1; i < width; i++)
	{
		setcolor(12); cout << static_cast<char>(205);
	}
	setcolor(12); cout << static_cast<char>(188);
	for (int i = y + 1; i < height + y; i++)
	{
		setcolor(11); GotoXY(x, i);
		cout << static_cast<char>(186);
		setcolor(14); GotoXY(x + width, i);
		cout << static_cast<char>(186);
	}
}

//vẽ cái bảng bên phải khung hình chính để xuất 1 số thông tin
void Draw_information()
{
	DrawBoard(84, 0, 34, 17);
	DrawBoard(84, 18, 34, 7);
	GotoXY(86, 20);
	cout << static_cast<char>(249) << " SPEED:  ";
	GotoXY(86, 21);
	cout << static_cast<char>(249) << " SCORE: ";
	GotoXY(86, 22);
	cout << static_cast<char>(249) << " POINT:  " << " / " << 4;
	GotoXY(86, 23);
	cout << static_cast<char>(249) << " LENGTH:  ";
	if (stage < 5) {
		GotoXY(106, 19);
		cout << static_cast<char>(218) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196)
			<< static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(191);
		GotoXY(106, 24);
		cout << static_cast<char>(192) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196)
			<< static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(196) << static_cast<char>(217);
	}
}

//cập nhật các thông số lenght of snake, speed
void Update_information()
{
	GotoXY(95, 1);	cout << "CURRENT STAGE: ";
	drawStageTitle(stage);
	GotoXY(85, 10); cout << static_cast<char>(254) << " Key:";
	setcolor(15); GotoXY(93, 12); cout << "UP";
	GotoXY(92, 16); cout << "DOWN";
	GotoXY(93, 13); cout << static_cast<char>(30);
	GotoXY(93, 15); cout << static_cast<char>(31);
	GotoXY(86, 14);
	cout << "LEFT " << static_cast<char>(17) << "   "
		<< static_cast<char>(16) << " RIGHT";
	setcolor(14); GotoXY(105, 10);
	cout << static_cast<char>(254) << " TIPS:";
	setcolor(15); GotoXY(105, 12);
	cout << static_cast<char>(249) << "'D': Pause";
	GotoXY(105, 13); cout << static_cast<char>(249) << "'L': Save";
	GotoXY(105, 14); cout << static_cast<char>(249) << "'ESC': Exit";
	GotoXY(96, 23);	cout << SIZE_SNAKE;
	GotoXY(95, 20); cout << SPEED << "  ";
	GotoXY(95, 21); cout << SCORE;
	GotoXY(95, 22); cout << FOOD_INDEX;
	if (stage < 5)
		drawProcessFood(FOOD_INDEX);
}


//xóa sạch màn hình console
//reset thông tin, STATE=1, drawboard
//resumethread, tiep tuc thread da bi dung

void RestartGame(HANDLE t)
{
	clrscr();
	ResetData(); // khởi tạo các giá trị ban đầu
	//vẽ khung hình và chướng ngại vật
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	draw_barrier();
	Draw_information();
	STATE = 1;	//đặt trạng thái còn sống cho con rắn
	ResumeThread(t);
}
//Function exit game 
void ExitGame(HANDLE  t)
{
	clrscr();
	SuspendThread(t);
}

//Function pause game
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}

//Function to update global data
void Eat()
{
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	SIZE_SNAKE++;
	SCORE = SCORE + SPEED * 10;
	GenerateFood();
}

//ham xu li qua cong 
void throughthegate()
{
	size_subsnake = SIZE_SNAKE;
	for (int i = 0; i < size_subsnake; i++)
	{
		subsnake[i] = snake[i];
	}
	int l = 0;
	while (l < size_subsnake)
	{
		for (int k = size_subsnake - 1; k >= 0; k--)
		{
			GotoXY(subsnake[k].x, subsnake[k].y);
			cout << " ";
		}
		char str[] = "2112046121120435211204772112048621120619";
		int j = l;
		for (int i = size_subsnake - 1; i >= l; i--)
		{
			GotoXY(subsnake[i].x, subsnake[i].y);
			cout << str[j];
			j++;
		}
		l++;
		switch (SPEED)
		{
		case 1:
			if (MOVING == 75 || MOVING == 77)
			{
				Sleep(150 / 2);
			}
			else
			{
				Sleep(225 / 2);
			}
			break;
		case 2:
			if (MOVING == 75 || MOVING == 77)
			{
				Sleep(125 / 2);
			}
			else
			{
				Sleep(187 / 2);
			}
			break;
		case 3:
			if (MOVING == 75 || MOVING == 77)
			{
				Sleep(100 / 2);
			}
			else
			{
				Sleep(150 / 2);
			}
			break;
		case 4:
			if (MOVING == 75 || MOVING == 77)
			{
				Sleep(80 / 2);
			}
			else
			{
				Sleep(120 / 2);
			}
			break;
		case 5:
			if (MOVING == 75 || MOVING == 77)
			{
				Sleep(65 / 2);
			}
			else
			{
				Sleep(97 / 2);
			}
			break;
		}
	}
}

void start_new_stage()
{
	clrscr();//khởi tạo các thông số khi qua màn
	CHAR_LOCK = 75, MOVING = 77; SPEED++;
	WIDTH_CONSOLE = WIDTH_CONSOLE_DEFAULT;
	HEIGH_CONSOLE = HEIGH_CONSOLE_DEFAULT;
	snake[0] = { 10, 2 };
	for (int i = 1; i <= 27; i++)
	{
		snake[i] = { 10 + i , 2 };
	}
	if (stage == 1) {
		stage = 2;
		createStage2(HEIGH_CONSOLE, WIDTH_CONSOLE);
	}
	else if (stage == 2) {
		stage = 3;
		createStage3(HEIGH_CONSOLE, WIDTH_CONSOLE);
	}
	else if (stage == 3) {
		stage = 4;
		createStage4(HEIGH_CONSOLE, WIDTH_CONSOLE);
	}
	else if (stage == 4) {
		stage = 5;
		lastwidth = WIDTH_CONSOLE_DEFAULT;
		lastheight = HEIGH_CONSOLE_DEFAULT;
		ResetBarrier();	createlastfood();
		drawlastboard(0, 0);
	}
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	Draw_information(); draw_barrier();
	setcolor(mainColorText);  DrawTheGate();
	size_subsnake = SIZE_SNAKE;
	//xử lí cho con rắn chuôi ra từ cổng
	for (int i = 0; i < size_subsnake; i++)
	{
		subsnake[i] = snake[i];
	}
	int l = 0;
	while (l < size_subsnake) {
		for (int k = size_subsnake - 1; k >= 0; k--)
		{
			GotoXY(subsnake[k].x, subsnake[k].y); cout << " ";
		}
		char str[] = "2112046121120435211204772112048621120619";
		int j = 0;
		for (int i = 0; i <= l; i++) {
			GotoXY(subsnake[i].x, subsnake[i].y);
			cout << str[l - j];	j++;
		}
		l++;
		switch (SPEED)
		{
		case 1:
			Sleep(150 / 2);
			break;
		case 2:
			Sleep(125 / 2);
			break;
		case 3:
			Sleep(100 / 2);
			break;
		case 4:
			Sleep(80 / 2);
			break;
		case 5:
			Sleep(65 / 2);
		}
	}
	EraseTheGate();
	if (stage != 5)
	{
		FOOD_INDEX = -1; GenerateFood();
	}//Create food array}
}

//Function to process the dead of snake
void ProcessDead()
{
	for (int i = 0; i < 9; i++) {
		if (i % 2 == 0)
		{
			setcolor(0);
		}
		else { setcolor(14); }
		Sleep(300);	DrawSnake();
	}
	setcolor(14);
	STATE = 0;
	SuspendThread(altHandle);
	DrawBoard(27, 8, 27, 8);
	for (int i = 28; i < 54; i++) {
		for (int j = 9; j < 16; j++) {
			GotoXY(i, j);	cout << " ";
		}
	}
	GotoXY(32, 9);	cout << "S N A K E  D I E D" << endl;
	GotoXY(36, 11);	 cout << "Play again?" << endl;
	setcolor(15);	GotoXY(30, 13);	cout << "   NO   ";
	setcolor(11);	GotoXY(43, 13);	cout << "=> YES <=";
}

void process_when_snake_died(HANDLE t, bool& agreed)
{
	char c = _getch();
	if (c == 75 || c == 77 && agreed == true) {
		GotoXY(30, 13);	setcolor(11);	cout << "=> NO <=";
		GotoXY(43, 13);	setcolor(15);	cout << "   YES   ";
		agreed = false;
	}
	else if (c == 77 || c == 75 && agreed == false) {
		GotoXY(30, 13);	setcolor(15);	cout << "   NO   ";
		GotoXY(43, 13);	setcolor(11);	cout << "=> YES <=";
		agreed = true;
	}
	if (c == 13 && agreed == true)
	{
		GotoXY(0, HEIGH_CONSOLE + 3);
		read_list_high_score();
		write_list_high_score();
		RestartGame(t);
	}
	if (c == 13 && agreed == false) {
		GotoXY(0, HEIGH_CONSOLE + 3);
		read_list_high_score();
		write_list_high_score();
		ExitGame(t);
		Menu(t);
	}
}


void EraseSnakeAndFood()
{
	GotoXY(snake[0].x, snake[0].y);
	cout << " ";
}

//vẽ thức ăn 
void DrawFood()
{
	if (stage == 5)
	{
		return;
	}
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	int rands = rand() % 7 + 9;
	setcolor(rands);
	cout << static_cast<char>(4);
	setcolor(mainColorText);
	GotoXY(105, 25);
}

//hàm vẽ con rắn là mã số sinh viên
void DrawSnake()
{
	if (STATE == 1)
	{
		char str[] = "2112046121120435211204772112048621120619";
		int j = 0;
		for (int i = SIZE_SNAKE - 1; i >= 0; i--)
		{
			GotoXY(snake[i].x, snake[i].y);
			cout << str[j];
			j++;
		}
	}
}

void DrawFoodAndSnake()
{
	if (STATE == 1)
	{
		if (FOOD_INDEX == (MAX_SIZE_FOOD - 1) && stage < 5)
		{
			if (typegate == 'l')
				DrawTheGateLeft(food[FOOD_INDEX]);
			else if (typegate == 'r') {
				DrawTheGateRight(food[FOOD_INDEX]);
			}
			else if (typegate == 'd')
				DrawTheGateDown(food[FOOD_INDEX]);
			else if (typegate == 'u')
			{
				DrawTheGateUp(food[FOOD_INDEX]);
			}
		}
		else
		{
			DrawFood();
		}
		DrawSnake();
	}
}

//đâm đầu vào body của con rắn 
bool touch_body()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[i] == snake[SIZE_SNAKE - 1])
		{
			return true;	//đã đâm đầu vào body
		}
	}
	return false;	//còn sống
}


void determined_CHARLOCK_MOVING(int temp)
{
	if (temp == 77)
	{
		CHAR_LOCK = 75;
	}
	else if (temp == 72)
	{
		CHAR_LOCK = 80;
	}
	else if (temp == 80)
	{
		CHAR_LOCK = 72;
	}
	else
	{
		CHAR_LOCK = 77;
	}
	MOVING = temp;
}

void MoveRight()
{
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE || touch_body() || barrier[snake[SIZE_SNAKE - 1].y][snake[SIZE_SNAKE - 1].x + 1])
	{
		ProcessDead();
	}
	else
	{
		if ((snake[SIZE_SNAKE - 1].x + 1) == food[FOOD_INDEX].x && (snake[SIZE_SNAKE - 1].y) == food[FOOD_INDEX].y)
		{
			if (FOOD_INDEX < MAX_SIZE_FOOD - 1)
			{
				Eat();
			}
			else
			{
				throughthegate();
				start_new_stage();
			}
		}
		else
		{
			for (int i = 0; i < SIZE_SNAKE - 1; i++)
			{
				snake[i] = snake[i + 1];
			}
			snake[SIZE_SNAKE - 1].x++;
		}
	}
}

void MoveLeft() {
	if ((snake[SIZE_SNAKE - 1].x - 1 == 0 || touch_body() || barrier[snake[SIZE_SNAKE - 1].y][snake[SIZE_SNAKE - 1].x - 1]))
	{
		ProcessDead();
	}
	else
	{
		if ((snake[SIZE_SNAKE - 1].x - 1) == food[FOOD_INDEX].x && (snake[SIZE_SNAKE - 1].y) == food[FOOD_INDEX].y)
		{
			if (FOOD_INDEX < MAX_SIZE_FOOD - 1)
			{
				Eat();
			}
			else
			{
				throughthegate();
				start_new_stage();
			}
		}
		else
		{
			for (int i = 0; i < SIZE_SNAKE - 1; i++)
			{
				snake[i] = snake[i + 1];
			}
			snake[SIZE_SNAKE - 1].x--;
		}
	}
}

void MoveDown()
{
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE || touch_body() || barrier[snake[SIZE_SNAKE - 1].y + 1][snake[SIZE_SNAKE - 1].x])
	{
		ProcessDead();
	}
	else
	{
		if ((snake[SIZE_SNAKE - 1].y + 1) == food[FOOD_INDEX].y && (snake[SIZE_SNAKE - 1].x) == food[FOOD_INDEX].x)
		{
			if (FOOD_INDEX < MAX_SIZE_FOOD - 1)
			{
				Eat();
			}
			else
			{
				throughthegate();
				start_new_stage();
			}
		}
		else
		{
			for (int i = 0; i < SIZE_SNAKE - 1; i++)
			{
				snake[i] = snake[i + 1];
			}
			snake[SIZE_SNAKE - 1].y++;
		}
	}
}

void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || touch_body() || barrier[snake[SIZE_SNAKE - 1].y - 1][snake[SIZE_SNAKE - 1].x])
	{
		ProcessDead();
	}
	else
	{
		if ((snake[SIZE_SNAKE - 1].y - 1) == food[FOOD_INDEX].y && (snake[SIZE_SNAKE - 1].x) == food[FOOD_INDEX].x)
		{
			if (FOOD_INDEX < MAX_SIZE_FOOD - 1)
			{
				Eat();
			}
			else
			{
				throughthegate();
				start_new_stage();

			}
		}
		else
		{
			for (int i = 0; i < SIZE_SNAKE - 1; i++)
			{
				snake[i] = snake[i + 1];
			}
			snake[SIZE_SNAKE - 1].y--;
		}
	}
}


void lastmoveright()
{
	if (snake[SIZE_SNAKE - 1].x + 1 == lastwidth || touch_body())
	{
		ProcessDead();
	}
	if ((snake[SIZE_SNAKE - 1].x + 1) == lastfood.x && (snake[SIZE_SNAKE - 1].y) == lastfood.y)
	{
		eatlastfood();
	}
	else
	{
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i] = snake[i + 1];
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}

void lastmoveleft()
{
	if (snake[SIZE_SNAKE - 1].x - 1 == 0 || touch_body())
	{
		ProcessDead();
	}

	if ((snake[SIZE_SNAKE - 1].x - 1) == lastfood.x && (snake[SIZE_SNAKE - 1].y) == lastfood.y)
	{
		eatlastfood();
	}
	else
	{
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i] = snake[i + 1];
		}
		snake[SIZE_SNAKE - 1].x--;
	}

}

void lastmovedown()
{
	if (snake[SIZE_SNAKE - 1].y + 1 == lastheight || touch_body())
	{
		ProcessDead();
	}

	if ((snake[SIZE_SNAKE - 1].y + 1) == lastfood.y && (snake[SIZE_SNAKE - 1].x) == lastfood.x)
	{
		eatlastfood();
	}
	else
	{
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i] = snake[i + 1];
		}
		snake[SIZE_SNAKE - 1].y++;
	}

}

void lastmoveup()
{
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || touch_body())
	{
		ProcessDead();
	}

	if ((snake[SIZE_SNAKE - 1].y - 1) == lastfood.y && (snake[SIZE_SNAKE - 1].x) == lastfood.x)
	{
		eatlastfood();
	}
	else
	{
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i] = snake[i + 1];
		}
		snake[SIZE_SNAKE - 1].y--;
	}

}

//cac ham  cho man choi cuoi cung
void drawlastboard(int x, int y)
{
	GotoXY(x, y);
	setcolor(15);
	cout << static_cast<char>(201);
	setcolor(15);
	for (int i = 1; i < lastwidth; i++)
	{
		cout << static_cast<char>(205);
	}
	cout << static_cast<char>(187);
	setcolor(12);
	GotoXY(x, lastheight + y);
	cout << static_cast<char>(200);
	for (int i = 1; i < lastwidth; i++)
	{
		setcolor(12);
		cout << static_cast<char>(205);
	}
	setcolor(12);
	cout << static_cast<char>(188);

	for (int i = y + 1; i < lastheight + y; i++)
	{
		setcolor(11);
		GotoXY(x, i);
		cout << static_cast<char>(186);
		setcolor(14);
		GotoXY(x + lastwidth, i);
		cout << static_cast<char>(186);
	}
}

void eraselastboard(int x, int y)
{
	GotoXY(x, y); cout << " ";
	for (int i = 1; i < lastwidth; i++)
	{
		cout << " ";
	}
	cout << " ";
	GotoXY(x, lastheight + y);
	{
		cout << " ";
	}
	for (int i = 1; i < lastwidth; i++)
	{
		cout << " ";
	}
	cout << " ";
	for (int i = y + 1; i < lastheight + y; i++)
	{
		GotoXY(x, i); cout << " ";
		GotoXY(x + lastwidth, i); cout << " ";
	}
}

bool checklastfood()
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (snake[i] == lastfood)
		{
			return false;
		}
	}
	return true;
}

void createlastfood()
{
	do
	{
		srand((unsigned int)time(NULL));
		lastfood.x = rand() % (lastwidth - 3) + 1;
		lastfood.y = rand() % (lastheight - 3) + 1;
	} while (checklastfood() == false);
}

void drawlastfood()
{
	GotoXY(lastfood.x, lastfood.y);
	int rands = rand() % 16 + 1;
	setcolor(rands);
	cout << static_cast<char>(4);
	setcolor(mainColorText);
	GotoXY(105, 25);
}

void eatlastfood()
{
	snake[SIZE_SNAKE] = lastfood;
	if (SIZE_SNAKE < MAX_SIZE_SNAKE)
	{
		SIZE_SNAKE++;
	}
	SCORE = SCORE + SPEED * 10;
	createlastfood();
}


//ham kiem tra de thu nho khung tro choi
bool checkdescrease()
{
	if (lastfood.x >= (lastwidth - 2) || lastfood.y >= (lastheight - 2))
	{
		return false;
	}
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (snake[i].x >= (lastwidth - 2) || snake[i].y >= (lastheight - 2))
		{
			return false;
		}
	}
	return true;
}

//Subfunction for thread

void ThreadFunc()
{
	int counter = 0;
	while (1) {
		if (STATE == 1) {	//con rắn còn sống
			EraseSnakeAndFood();
			if (stage != 5) {
				switch (MOVING) {
				case 75:  MoveLeft();  break;
				case 77:  MoveRight(); break;
				case 72:  MoveUp();    break;
				case 80:  MoveDown();
				}
			}
			else {
				counter++;
				if (counter % 100 == 0) {
					if (checkdescrease() == false)
					{
						counter--;
					}
					else {
						eraselastboard(0, 0);
						lastwidth -= 2;  lastheight -= 2;
						drawlastboard(0, 0);
					}
				}
				drawlastfood();
				switch (MOVING) {
				case 75: lastmoveleft(); break;
				case 77: lastmoveright(); break;
				case 72: lastmoveup();   break;
				case 80: lastmovedown(); break;
				}
			}
			DrawFoodAndSnake();
			Update_information();
			switch (SPEED)
			{	//điều chỉnh tốc độ cho con rắn
			case 1:
				if (MOVING == 75 || MOVING == 77)
				{
					Sleep(150);
				}
				else
				{
					Sleep(225);
				}
				break;
			case 2:
				if (MOVING == 75 || MOVING == 77)
				{
					Sleep(125);
				}
				else
				{
					Sleep(187);
				}
				break;
			case 3:
				if (MOVING == 75 || MOVING == 77)
				{
					Sleep(100);
				}
				else
				{
					Sleep(150);
				}
				break;
			case 4:
				if (MOVING == 75 || MOVING == 77)
				{
					Sleep(80);
				}
				else
				{
					Sleep(120);
				}
				break;
			case 5:
				if (MOVING == 75 || MOVING == 77)
				{
					Sleep(65);
				}
				else
				{
					Sleep(97);
				}
			}
		}
	}
}


//hàm tạo file_name khi đã biết player_name 
//mặc định file_name có dạng là player_name.dat 
void create_file_name(char* s)
{
	strcpy(file_name, s);
	strcat(file_name, ".dat");
}
//hàm tạo player_name khi đã biết file_name
void create_player_name(char* s)
{
	int lenght_player_name = int(strlen(s) - 4);
	strncpy(player_name, s, lenght_player_name);
	player_name[lenght_player_name] = '\0';
}


//hàm đọc thông tin từ file_list_file_name
void read_file_list_file_name()
{
	file_list_file_name = fopen("file_list_file_name.dat", "rb");
	if (file_list_file_name != NULL)
	{
		fseek(file_list_file_name, 0, SEEK_END);
		if (ftell(file_list_file_name) > 0)	//kiểm tra trong file có dữ liệu hay không
		{
			fseek(file_list_file_name, 0, SEEK_SET);//dời về đầu tập tin
			fread(&NGAME, sizeof(int), 1, file_list_file_name);
			for (int i = 0; i < NGAME; i++)
			{
				fread(list_file_name[i], 1, 49, file_list_file_name);
			}
		}
		else
		{
			GotoXY(50, 8);
			cout << "There are no game to load";
		}
		fclose(file_list_file_name);
	}
	else
	{
		GotoXY(50, 8);
		setcolor(15);
		cout << "There are no game to load";
		setcolor(14);
	}
}

//hàm ghi thông tin vào file file_list_file_name
void write_file_list_file_name()
{
	file_list_file_name = fopen("file_list_file_name.dat", "wb");
	if (file_list_file_name != NULL)
	{
		fwrite(&NGAME, sizeof(int), 1, file_list_file_name);
		for (int i = 0; i < NGAME; i++)
		{
			fwrite(list_file_name[i], 1, 49, file_list_file_name);
		}
		fclose(file_list_file_name);
	}
}

//save and load game
void savegame(HANDLE t)
{
	ExitGame(t);
	system("cls");
	GotoXY(0, 0);
	cout << "Nhap ten cua ban: ";
	cin.getline(player_name, 50);
	create_file_name(player_name);
	fp = fopen(file_name, "wb");
	if (fp != NULL)
	{
		fwrite(player_name, 1, 49, fp);
		fwrite(snake, sizeof(snake), 1, fp);
		fwrite(food, sizeof(food), 1, fp);
		fwrite(&CHAR_LOCK, sizeof(int), 1, fp);
		fwrite(&MOVING, sizeof(int), 1, fp);
		fwrite(&SPEED, sizeof(int), 1, fp);
		fwrite(&HEIGH_CONSOLE, sizeof(int), 1, fp);
		fwrite(&WIDTH_CONSOLE, sizeof(int), 1, fp);
		fwrite(&FOOD_INDEX, sizeof(int), 1, fp);
		fwrite(&SIZE_SNAKE, sizeof(int), 1, fp);
		fwrite(&STATE, sizeof(int), 1, fp);
		fwrite(&SCORE, sizeof(int), 1, fp);
		fwrite(&stage, sizeof(int), 1, fp);
		//fwrite(&createdStage, sizeof(int), 1, fp);
		for (int i = 0; i < HEIGH_CONSOLE_DEFAULT; i++)
		{
			for (int j = 0; j < WIDTH_CONSOLE_DEFAULT; j++)
			{
				fwrite(&barrier[i][j], sizeof(bool), 1, fp);
			}
		}
		fclose(fp);
	}
	strcpy(list_file_name[NGAME], file_name);
	NGAME++;
	write_file_list_file_name();
	system("cls");
	Menu(t);
}

void loadgame(HANDLE t, int index)
{
	strcpy(file_name, list_file_name[index]);
	fp = fopen(file_name, "rb");
	if (fp != NULL) {
		//đọc ra các thông số của trò chơi
		fread(player_name, 1, 49, fp);
		fread(snake, sizeof(snake), 1, fp);
		fread(food, sizeof(food), 1, fp);
		fread(&CHAR_LOCK, sizeof(int), 1, fp);
		fread(&MOVING, sizeof(int), 1, fp);
		fread(&SPEED, sizeof(int), 1, fp);
		fread(&HEIGH_CONSOLE, sizeof(int), 1, fp);
		fread(&WIDTH_CONSOLE, sizeof(int), 1, fp);
		fread(&FOOD_INDEX, sizeof(int), 1, fp);
		fread(&SIZE_SNAKE, sizeof(int), 1, fp);
		fread(&STATE, sizeof(int), 1, fp);
		fread(&SCORE, sizeof(int), 1, fp);
		fread(&stage, sizeof(int), 1, fp);
		for (int i = 0; i < HEIGH_CONSOLE_DEFAULT; i++)
		{
			for (int j = 0; j < WIDTH_CONSOLE_DEFAULT; j++) {
				fread(&barrier[i][j], sizeof(bool), 1, fp);
			}
		}
		fclose(fp);
	}
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);	//vẽ lại màn chơi
	Draw_information();
	draw_barrier();
	STATE = 1;
	ResumeThread(t);
	//xóa dữ liệu trong file vừa load
	fp = fopen(file_name, "wb");
	if (fp != NULL) { fclose(fp); }
	int vt = -1;
	for (int i = 0; i < NGAME; i++) {
		if (strcmp(file_name, list_file_name[i]) == 0)
		{
			vt = i;  break;
		}
	}
	if (vt == -1) { return; }
	for (int i = vt; i < NGAME - 1; i++)
	{
		strcpy(list_file_name[i], list_file_name[i + 1]);
	}
	NGAME--;
	write_file_list_file_name();
}

void swap_PLAYER(PLAYER& a, PLAYER& b)
{
	PLAYER c;
	strcpy(c.name, a.name);
	c.score = a.score;
	strcpy(a.name, b.name);
	a.score = b.score;
	strcpy(b.name, c.name);
	b.score = c.score;
}
void sort_list_high_score()
{
	for (int i = 0; i < NHIGHSCORE - 1; i++) {
		for (int j = i + 1; j < NHIGHSCORE; j++) {
			if (list_high_score[i].score < list_high_score[j].score)
			{
				swap_PLAYER(list_high_score[i], list_high_score[j]);
			}
		}
	}
}

void read_list_high_score()
{
	highscore = fopen("highscore.dat", "rb");
	if (highscore != NULL)
	{
		fseek(highscore, 0, SEEK_END);
		if (ftell(highscore) > 0)		//kiểm tra file highscore.dat đã có dữ liệu hay chưa
		{
			fseek(highscore, 0, SEEK_SET);
			fread(&NHIGHSCORE, sizeof(int), 1, highscore);
			char hoten[50];
			for (int i = 0; i < NHIGHSCORE; i++)
			{
				//đọc họ tên và điểm số của người chơi, lưu vào mảng list_high_score
				fread(hoten, 1, 49, highscore);
				hoten[strlen(hoten)] = '\0';
				strcpy(list_high_score[i].name, hoten);
				fread(&(list_high_score[i].score), sizeof(int), 1, highscore);
			}
		}
		else   //trường hợp file highscore.dat chưa có dữ liệu
		{
			NHIGHSCORE = 0;
		}
		fclose(highscore);
	}
	sort_list_high_score();		//sắp xếp danh sách giảm dần theo điểm số
}

void write_list_high_score()
{
	bool have_exchange = false;
	if (NHIGHSCORE < 5)		//Trường hợp chưa đủ 5 người có điểm cao nhất
	{
		//thêm tên và điểm số vào list_high_score
		cout << "    CONGRATULATION! YOUR SCORE: " << SCORE << endl << "    Let enter your name: ";
		showCursor();
		cin.getline(list_high_score[NHIGHSCORE].name, 50);
		hideCursor();
		list_high_score[NHIGHSCORE].score = SCORE;
		NHIGHSCORE++;
		sort_list_high_score();
		have_exchange = true;
	}
	else
	{	//nếu điểm số người chơi đạt được lớn hơn điểm số của người chơi có trong danh sách
		if (list_high_score[NHIGHSCORE - 1].score < SCORE)
		{	//thêm tên và điểm số vào list_high_score
			showCursor();
			cout << "CONGRATULATION! YOU GOT NEW HIGHSCORE: " << SCORE << endl << " .Let enter your name: ";
			cin.getline(list_high_score[NHIGHSCORE - 1].name, 50);
			list_high_score[NHIGHSCORE - 1].score = SCORE;
			sort_list_high_score();
			have_exchange = true;
			hideCursor();
		}
	}
	if (have_exchange)	//cập nhật lại dữ liệu trong file highscore.dat
	{
		highscore = fopen("highscore.dat", "wb");
		if (highscore != NULL)
		{
			fwrite(&NHIGHSCORE, sizeof(int), 1, highscore);
			for (int i = 0; i < NHIGHSCORE; i++)
			{
				fwrite(list_high_score[i].name, 1, 49, highscore);
				fwrite(&(list_high_score[i].score), sizeof(int), 1, highscore);
			}
			fclose(highscore);
		}
	}
}


