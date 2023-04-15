#ifndef FrogScreenLib
#define FrogScreenLib

#include <unistd.h>
#include <termios.h>
#include <fstream>
#include <map>
#include <iostream>

using namespace std;
/*
Класс для переключения терминала
Основные функции: .scan_std() .set_nstd() .set_ostd()
*/
class terml
{
private:
	struct termios oldt, newt;
public:
	void scan_std()
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON|ECHO);
	}
	void set_nstd()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}
	void set_ostd()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	}
	terml()
	{
		scan_std();
	};
};
terml term = terml();

/*
Создаёт экземпляр экрана с размером x,y
set_res(x, y)
*/

const int colarr_size = 11;
class new_screen
{
private:
	int x, y;
	bool main;
	char screen[128][128][8][colarr_size+1];
public:
	void set_res(int a, int b)
	{
		x = a;
		y = b;	
	}
	void get_res(int &a, int &b)
	{
		a = x;
		b = y;
	}
	auto get_screen(void)
	{
		return(screen);
	}
	void set_symbol(int x, int y, int z, char symbol)
	{
		screen[x][y][z][colarr_size] = symbol;
	}
	char get_symbol(int X, int Y, int Z)
	{
		return(screen[X][Y][Z][colarr_size]);
	}
	void clear()
	{
		cout << "\033c";
	}

	new_screen(){cout << "Окно создано\n";};
	new_screen(int a, int b, bool main)
	{
		set_res(a,b);
		char tempcolor[colarr_size] = {'\033','[','3','9',';','4','9',';','2','2','m'};
		for (int i = 0; i < y; i++)
		{
			for (int k = 0; k < x; k++)
			{
				for (int j = 0; j<colarr_size; j++)
					screen[k][i][0][j] = tempcolor[j];
				screen[k][i][0][colarr_size] = '0';
			}
		}
	};
	~new_screen(){};
	

	void render(void)
	{
		clear();
		cout << "\x1b[8;"<<y<<";"<<x<<";t";					//Задание размера экрана
		for (int Y = 0; Y < y; Y++)
		{
			for (int X = 0; X < x; X++)
			{
				for (int j = 0; j < colarr_size+1; j++)
				{
					int Z = 7;
					while(get_symbol(X,Y,Z) == '\0')
					{
						Z--;
					}
					cout << screen[X][Y][Z][j];
				}
				//cout << " " << X << Y << "\n";
			}
		}
	}
};


class draw
{
public:

	/*
		Установки
	*/

	char color[colarr_size] = {'\033','[','3','9',';','4','9',';','2','2','m'};
	
	new_screen *refScreen;
	
	void set_screen(new_screen &a)
	{
		refScreen = &a;
	}
	void take_color(int i)
	{
		cout << "\033[3" << char(i+48) << ";49m";
	}	
	void set_color(int i)
	{
		color[3] = char(i+48);
	}
	void reset_color()
	{
		color[3] = char(9+48);
		color[6] = char(9+48);	
	}
	void get_color_table(void)
	{
		cout << "\033[30;49;22mBlack = 0\033[39;49m\n";
		cout << "\033[31;49;22mRed = 1\033[39;49m\n";
		cout << "\033[32;49;22mGreen = 2\033[39;49m\n";
		cout << "\033[33;49;22mYellow = 3\033[39;49m\n";
		cout << "\033[34;49;22mBlue = 4\033[39;49m\n";
		cout << "\033[35;49;22mMagenta = 5\033[39;49m\n";
		cout << "\033[36;49;22mCyan = 6\033[39;49m\n";
		cout << "\033[37;49;22mWhite = 7\033[39;49m\n";
	}

	void set_brColor(int i)
	{
		color[3] = char(i+48);
		color[8] = char(0+48);
		color[9] = char(1+48);
	}
	void set_fadeColor(int i)
	{
		color[3] = char(i+48);
		color[8] = char(0+48);
		color[9] = char(2+48);
	}

	void set_foreColor(int i)
	{
		color[6] = char(i+48);
	}
	

	/*
		Функции рисования
	*/

	//	Цветовые
	void draw_color(int x, int y, int z)
	{
		for (int j = 0; j < colarr_size; j++)
			{
				refScreen->get_screen()[x][y][z][j] = color[j];
			}		
	}

	void draw_color(int x, int y, int z, int colorize)
	{
		set_color(colorize);
		for (int j = 0; j < colarr_size; j++)
			{
				refScreen->get_screen()[x][y][z][j] = color[j];
			}		
	}
	


	void draw_symbol(int x, int y, int z, char sym)
	{
		refScreen->get_screen()[x][y][z][colarr_size] = sym;
	}

	void draw_symbol(int x, int y, int z, char sym, int colorize)
	{
		draw_color(x, y, z, colorize);
		refScreen->get_screen()[x][y][z][colarr_size] = sym;
	}



	//	Фигурные
	void draw_line(int x, int y, int z, char direction, int lenght, char sym)
	{
		switch (direction)
		{
			case 'U':
				for (int i = y; i < y + lenght; i++)
					draw_symbol(x,i,z,sym);
				break;
			case 'D':
				for (int i = y; i > y - lenght; i--)
					draw_symbol(x,i,z,sym);
				break;
			case 'L':
				for (int k = x; k > x - lenght; k--)
					draw_symbol(k,y,z,sym);
				break;
			case 'R':
				for (int k = x; k < x + lenght; k++)
					draw_symbol(k,y,z,sym);
				break;
		}
	}
	void draw_circle(int cx, int cy, int r);
	void draw_fill_square(int x1, int y1, int x2, int y2, int z, char sym)
	{
		for (int i = y1; i < y2; i++)
			for (int k = x1; k < x2; k++)
				{
					for (int j = 0; j < colarr_size; j++)
						{
							refScreen->get_screen()[i][k][z][j] = color[j];
						}
					draw_symbol(i, k, z, sym);
				}
		}
	void clear()
	{
		cout << "\033c";
	}
};


/*
Незаконченный класс для работы с файлами
get_res(path) - считывает разрешение текстуры .txt файла по пути
*/

class f_file
{
public:
	int get_res(char *path)
	{
		ifstream mmenu_file(path);
		//mmenu_file.open();
		if (mmenu_file.is_open())
			return(1);		
	}
	f_file(){};
};


#endif