

#include <termios.h>	
#include <sys/ioctl.h>	
#include <signal.h>
#include <stdlib.h>		
#include <curses.h>  	//libncurses.so version 5.9


void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}


int main()
{
	WINDOW * wnd_main;
	WINDOW * wnd_main_sub;
	WINDOW * wnd_info;
	WINDOW * wnd_info_sub;
	char symbol;
	initscr();
	signal(SIGWINCH, sig_winch);
	curs_set(TRUE);
	//start_color();
	refresh();
	wnd_main = newwin(40, 100, 0, 0);
	wnd_info = newwin(3, 100, 41, 0);
	wnd_info_sub = derwin(wnd_info, 1, 99, 1, 1);
	box(wnd_main, '|', '-');
	box(wnd_info, '|', '-');
	wprintw(wnd_info_sub, "F1 - Open file  |  F2 - Save file  |  Esc - Quit");
	wrefresh(wnd_info_sub);
	wrefresh(wnd_main);
	wrefresh(wnd_info);
	wmove(stdscr, 46, 1);
	printw("Command: ");
	refresh();
	keypad(stdscr, TRUE);
	while(symbol != 27)
	{
		symbol = getch();
		switch(symbol)
		{
			case 27:													// Exit programm
				wmove(stdscr, 46, 10);
				printw("Exit programm! Press any key to continue...");
				refresh();
				break;
			case 32:
				wmove(stdscr, 46, 10);
				printw("Open file!");
				refresh();
				break;
			case 38:
				wmove(stdscr, 46, 10);
				printw("Save file!");
				refresh();
				break;
			default:
				wmove(stdscr, 46, 10);
				printw("Unknown team!");
				refresh();
				break;
		}
	}
	getch();
	delwin(wnd_info_sub);
	delwin(wnd_main);
	delwin(wnd_info);
	endwin();
	exit(EXIT_SUCCESS);
}
