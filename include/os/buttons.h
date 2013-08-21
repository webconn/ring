#ifndef OS_BUTTONS_H
#define OS_BUTTONS_H

#include <stdint.h>
#include <hard/tasks.h>

#define BUT_BACK 3
#define BUT_OK 2
#define BUT_NO 1
#define BUT_FWD 0

void TButtons_init(void);
void TButtons_task(void);
void TButtons_add_handler(uint8_t button, void * handler);

#endif
