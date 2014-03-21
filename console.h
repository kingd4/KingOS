#ifndef CONSOLE_H_
#define CONSOLE_H_

typedef enum Color_{BLACK,NAVY,GREEN,CYAN,RED,PURPLE,OLIVE,GREY,BLACK2,BLUE,LIME,TURQUOISE,PINK,MAGENTA,YELLOW,WHITE} Color;


void console_init();
void console_clear();
void console_putc(char c);
char console_get_color( Color fg, Color bg );
void set_screen_location(int r, int c);

#endif

