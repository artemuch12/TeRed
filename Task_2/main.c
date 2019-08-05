#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>	
#include <sys/ioctl.h>	
#include <signal.h>
#include <stdlib.h>		
#include <curses.h>  	//libncurses.so version 5.9

#define MAX_LEN 31

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}
void escape(int *fd, int *flag)
{
	*flag = TRUE;
	wmove(stdscr, 51, 1);
	printw("The program closes. To continue, press any key ...");
	refresh();
	if (*fd > 1)
	{
		close(*fd);
	}
}
int main()
{
	//Windows 4
	WINDOW *wnd;
	WINDOW *subwnd;
	WINDOW *info;
	WINDOW *subinfo;
	
	//Variable
	int i = 0;
	
	int flag = FALSE;
	char sym;
	char name[MAX_LEN+1];
	char buff;
	
	//File
	int fd = -1;
	off_t file_end = 0 ;
	off_t protect = 0;
	
	
	//Text program
	initscr();
	signal(SIGWINCH, sig_winch);
	curs_set(TRUE);
	wnd = newwin(40, 100, 0, 0);
	subwnd = derwin(wnd, 38, 98, 1, 1);
	info = newwin(3, 100, 41, 0);
	subinfo = derwin(info, 1, 98, 1, 1);
	refresh();
	
	box(wnd, '|', '-');
	box(info, '|', '-');
	wprintw(subinfo, "F1 - Open file  |  F3 - Save file  |  Esc - Quit");
	wrefresh(wnd);
	wrefresh(info);
	wrefresh(subinfo);
	
	keypad(stdscr, TRUE);
	keypad(wnd, TRUE);
	keypad(info, TRUE);
	keypad(subwnd, TRUE);
	keypad(subinfo, TRUE);
	
	wmove(stdscr, 46, 1);
	printw("Command: ");
	while(flag != TRUE)
	{
		noecho();
		sym = wgetch(stdscr);
		if(sym == 9) //Symbol F3
		{
			echo();
			wmove(stdscr, 51, 1);
			printw("File: ");
			refresh();
			getnstr(name, MAX_LEN);
			noecho();
			fd = open(name, O_RDWR);
			if(fd < 2)
			{
				printw(" No file!");
				refresh();
			}
			else
			{
				printw(" Open!");
				refresh();
				wmove(subwnd, 1, 1);
				file_end = lseek(fd, 0, SEEK_END);
				lseek(fd, 0 , SEEK_SET);
				for(i = 0; i < file_end; i++)
				{
					read(fd, &buff, 1);
					wprintw(subwnd, "%c", buff);
					wrefresh(subwnd);
				}
				while(flag != TRUE)
				{
					sym = getch();
					if(sym == 27)
					{
						escape(&fd, &flag);
					}
					else if(sym == 11)
					{
						fsync(fd);
						wmove(stdscr, 51, 1);
						printw("Save!");
						refresh();
					}
					else if(sym == 10) 	//symbol Enter
					{
						write(fd, &sym, 1);
					}
					else if(sym == 4)
					{
						protect = lseek(fd, -1, SEEK_CUR);
						if(protect < 2)
						{
							lseek(fd, 1, SEEK_CUR);
						}
					}
					else 
					{
						write(fd, &sym, 1);
						wprintw(subwnd, "%c", sym);
						wrefresh(subwnd);
					}
				}
			}
			
		}
		else if(sym == 11) //Symbol F3
		{
			wmove(stdscr, 51, 1);
			printw("No file!");
			refresh();
		}
		else if(sym == 27)	//Symbol ESC
		{
			escape(&fd, &flag);
		}
		else
		{
		}
	}
	getch();
	delwin(subwnd);
	delwin(subinfo);
	delwin(wnd);
	delwin(wnd);
	endwin();
	exit(EXIT_SUCCESS);
}
