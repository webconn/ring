#ifndef STRINGS_H
#define STRINGS_H

// Configuring strings and service symbols according to display setting

#if defined(CONFIG_DISPLAY_OLED)

// Symbols
#define SYM_BELL 0xED
#define SYM_ON 0xEF
#define SYM_OFF 0xEE
#define SYM_FILL 0xFF

#ifdef SHOW_STRINGS

const char str_days[7][2] = {
    { 'B', 'c' }, // Вс
    { 0xA8, 0xBD }, // Пн
    { 'B', 0xBF }, // Вт
    { 'C', 'p' }, // Ср
    { 0xAB, 0xBF }, // Чт
    { 0xA8, 0xBF }, // Пт
    { 'C', 0xB2 } // Сб
};

// Synchronisation with PC
const char str_sync[] = { 'c', 'o', 'e', 0xE3, 0xB8, 0xBD, 'e', 0xBD, 0xB8, 'e', ' ', 'c', 0xA8, 'K', ' ' }; // соединение с ПК

// Reboot
const char str_reboot[] = { 0xA8, 'e', 'p', 'e', 0xB7, 'a', 0xB4, 'p', 'y', 0xB7, 0xBA, 'a' , ' ', ' ', ' ', ' '}; // перезагрузка

// Configuration
const char str_config[] = { 0xF4, ' ', 'H', 'a', 'c', 0xBF, 'p', 'o', 0xB9, 0xBA, 'a', ' ', ' ', ' ', ' ', ' ' }; // Настройка

// Brightness
const char str_brightness[] = { 0xB1, 'p', 0xBA, 'o', 'c', 0xBF, 0xC4, ' ', 0xE3, 0xB8, 'c', 0xBE, 0xBB, 'e', 0xC7, ' ' }; // Яркость дисплея

// Time setting
const char str_time[] = { 0xA9, 'c', 0xBF, 'a', 0xBD, 'o', 0xB3, 0xB8, 0xBF, 0xC4, ' ', 0xB3, 'p', 'e', 0xBC, 0xC7 }; // Установить время

// Timer enabled
const char str_timer_e[] = { ' ', 'T', 'a', 0xB9, 0xBC, 'e', 'p', ' ', 0xB3, 0xBA, 0xBB, 0xC6, 0xC0, 'e', 0xBD, ' ' }; // Таймер включен

// Timer disabled
const char str_timer_d[] = { ' ', 'T', 'a', 0xB9, 0xBC, 'e', 'p', ' ', 0xB3, 0xC3, 0xBA, 0xBB, 0xC6, 0xC0, 'e', 0xBD }; // Таймер выключен
#endif


#elif defined(CONFIG_DISPLAY_LCD_NOCYR)

#define SYM_BELL 0x7F
#define SYM_ON 0xEF
#define SYM_OFF 0xEE
#define SYM_FILL 0xFC 
#define SYM_MENU 0xFE

#ifdef SHOW_STRINGS

const char str_days[7][2] = {
    { 'B', 'C' }, // Вс
    { 0xD9, 'H' }, // Пн
    { 'B', 'T' }, // Вт
    { 'C', 'P' }, // Ср
    { '4', 'T' }, // Чт
    { 0xD9, 'T' }, // Пт
    { 'C', '6' } // Сб
};

// Reboot
const char str_reboot[] = { 0xD9, 'E', 'P', 'E', '3', 'A', 0xD4, 'P', 'Y', '3', 'K', 'A' , ' ', ' ', ' ', ' '}; // перезагрузка

// Configuration
const char str_config[] = { SYM_MENU, ' ', 'H', 'A', 'C', 'T', 'P', 'O', 'U', 'K', 'A', ' ', ' ', ' ', ' ', ' ' }; // Настройка

// Brightness
const char str_brightness[] = { '9', 'P', 'K', 'O', 'C', 'T', 'b', ' ', 'D', 'U', 'C', 0xD9, 0xD7, 'E', '9', ' ' }; // Яркость дисплея

// Time setting
const char str_time[] = { 'Y', 'C', 'T', 'A', 'H', 'O', 'B', 'U', 'T', 'b', ' ', 'B', 'P', 'E', 'M', '9' }; // Установить время

// Timer enabled
const char str_timer_e[] = { ' ', 'T', 'a', 0xB9, 0xBC, 'e', 'p', ' ', 0xB3, 0xBA, 0xBB, 0xC6, 0xC0, 'e', 0xBD, ' ' }; // Таймер включен

// Timer disabled
const char str_timer_d[] = { ' ', 'T', 'a', 0xB9, 0xBC, 'e', 'p', ' ', 0xB3, 0xC3, 0xBA, 0xBB, 0xC6, 0xC0, 'e', 0xBD }; // Таймер выключен
#endif

#endif

#endif
