#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define LCTRL 1001
#define LSHIFT 1002
#define RSHIFT 1003
#define PRNTSCREEN 1004
#define LALT 1005
#define CAPLK 1007


void keyboard_interrupt();

int keyboard_getline(char * q, unsigned n);

#endif
