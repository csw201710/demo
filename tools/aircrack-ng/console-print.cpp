#include<stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define channel stdout

//隐藏鼠标
void hide_cursor(void)
{
	char command[13];

	snprintf(command, sizeof(command), "%c[?25l", 0x1B);
	fprintf(channel, "%s", command);
	fflush(channel);
}

//显示鼠标
void show_cursor(void)
{
	char command[13];

	snprintf(command, sizeof(command), "%c[?25h", 0x1B);
	fprintf(channel, "%s", command);
	fflush(channel);
}
//移动输入位置
void moveto(int x, int y)
{
	char command[64];

	// clamp the X coordinate.
	if (x < 0)
	{
		x = 0;
	}

	// clamp the Y coordinate.
	if (y < 0)
	{
		y = 0;
	}

	// send ANSI sequence to move the cursor.
	snprintf(command, sizeof(command), "%c[%d;%dH", 0x1B, y, x);
	fprintf(channel, "%s", command);
	fflush(channel);
}

void erase_line(int n)
{
	char command[13];

	snprintf(command, sizeof(command), "%c[%dK", 0x1B, n);
	fprintf(channel, "%s", command);
	fflush(channel);
}

//清空shell显示
void erase_display(int n)
{
	char command[13];

	snprintf(command, sizeof(command), "%c[%dJ", 0x1B, n);
	fprintf(channel, "%s", command);
	fflush(channel);
}

/**
 * Styling attributes for \a textstyle function.
 */
#define TEXT_RESET 0
#define TEXT_BRIGHT 1
#define TEXT_DIM 2
#define TEXT_UNDERLINE 3
#define TEXT_BLINK 4
#define TEXT_REVERSE 7
#define TEXT_HIDDEN 8
#define TEXT_MAX_STYLE 8

/**
 * Color definitions for \a textcolor functions.
 */
#define TEXT_BLACK 0
#define TEXT_RED 1
#define TEXT_GREEN 2
#define TEXT_YELLOW 3
#define TEXT_BLUE 4
#define TEXT_MAGENTA 5
#define TEXT_CYAN 6
#define TEXT_WHITE 7
#define TEXT_MAX_COLOR 7

void textcolor_fg(int fg)
{
	char command[64];

	/* Command is the control command to the terminal */
	snprintf(command, sizeof(command), "\033[%dm", fg + 30);
	fprintf(channel, "%s", command);
	fflush(channel);
}

void textcolor_bg(int bg)
{
	char command[64];

	/* Command is the control command to the terminal */
	snprintf(command, sizeof(command), "\033[%dm", bg + 40);
	fprintf(channel, "%s", command);
	fflush(channel);
}

void textstyle(int attr)
{
	char command[13];

	/* Command is the control command to the terminal */
	snprintf(command, sizeof(command), "\033[%im", attr);
	fprintf(channel, "%s", command);
	fflush(channel);
}
void textcolor_normal(void)
{
	char command[13];

	snprintf(command, sizeof(command), "%c[22m", 0x1B);
	fprintf(channel, "%s", command);
	fflush(channel);
}

int main(){
  printf("this is a test!!!");fflush(stdout);
  
  
  hide_cursor();
  //erase_display(2);
  erase_line(1);
  //moveto(1,1);
  textstyle(TEXT_BRIGHT);
		textcolor_fg(TEXT_RED);
  fprintf(stdout,"this is a test!!");fflush(stdout);
  //for ( int i=0;i<35;i++) printf("\n");
  //erase_display(0);
  textstyle(TEXT_RESET); //恢复
  sleep(2);
  
  show_cursor();
  sleep(2);
	return 0;
}


