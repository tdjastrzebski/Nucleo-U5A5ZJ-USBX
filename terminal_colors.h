/*---------------------------------------------------------------------------------------------------
 *  Copyright (c) 2023 Tomasz Jastrzębski. All rights reserved.
 *-------------------------------------------------------------------------------------------------*/

#pragma once

/*
DEFAULT = 0,  // actually it is reset
BRIGHT = 1,
DIM = 2,
ITALIC = 3,
UNDERLINE = 4,
SBLINK = 5,   // slow blink
RBLINK = 6,   // rapid blink
REVERSE = 7,  // inverse
// HIDDEN = 8, // rarely supported
CROSSED = 9,
*/

// regular color
#define RED(msg) "\e[0;31m" msg "\e[0m"
#define GREEN(msg) "\e[0;32m" msg "\e[0m"
#define YELLOW(msg) "\e[0;33m" msg "\e[0m"
#define BLUE(msg) "\e[0;34m" msg "\e[0m"
#define MAGENTA(msg) "\e[0;35m" msg "\e[0m"
#define CYAN(msg) "\e[0;36m" msg "\e[0m"
#define WHITE(msg) "\e[0;37m" msg "\e[0m"
// RGB = 38, // Next arguments are 5;n or 2;r;g;b
// DEFAULT = 39,

// bright color
#define B_GRAY(msg) "\e[0;90m" msg "\e[0m"
#define B_RED(msg) "\e[0;91m" msg "\e[0m"
#define B_GREEN(msg) "\e[0;92m" msg "\e[0m"
#define B_YELLOW(msg) "\e[0;93m" msg "\e[0m"
#define B_BLUE(msg) "\e[0;94m" msg "\e[0m"
#define B_MAGENTA(msg) "\e[0;95m" msg "\e[0m"
#define B_CYAN(msg) "\e[0;96m" msg "\e[0m"
#define B_WHITE(msg) "\e[0;97m" msg "\e[0m"

// regular background color
#define RED_BG(msg) "\e[0;39;41m" msg "\e[0m"
#define GREEN_BG(msg) "\e[0;39;42m" msg "\e[0m"
#define YELLOW_BG(msg) "\e[0;39;43m" msg "\e[0m"
#define BLUE_BG(msg) "\e[0;39;44m" msg "\e[0m"
#define MAGENTA_BG(msg) "\e[0;39;45m" msg "\e[0m"
#define CYAN_BG(msg) "\e[0;39;46m" msg "\e[0m"
#define WHITE_BG(msg) "\e[0;39;47m" msg "\e[0m"
// RGB = 48, 5;n or 2;r;g;b

// bright background color
#define B_GRAY_BG(msg) "\e[0;100m" msg "\e[0m"
#define B_RED_BG(msg) "\e[0;101m" msg "\e[0m"
#define B_GREEN_BG(msg) "\e[0;102m" msg "\e[0m"
#define B_YELLOW_BG(msg) "\e[0;103m" msg "\e[0m"
#define B_BLUE_BG(msg) "\e[0;104m" msg "\e[0m"
#define B_MAGENTA_BG(msg) "\e[0;105m" msg "\e[0m"
#define B_CYAN_BG(msg) "\e[0;106m" msg "\e[0m"
#define B_WHITE_BG(msg) "\e[0;107m" msg "\e[0m"

// underlined bright color
#define B_GRAY_U(msg) "\e[4;90m" msg "\e[0m"
#define B_RED_U(msg) "\e[4;91m" msg "\e[0m"
#define B_GREEN_U(msg) "\e[4;92m" msg "\e[0m"
#define B_YELLOW_U(msg) "\e[4;93m" msg "\e[0m"
#define B_BLUE_U(msg) "\e[4;94m" msg "\e[0m"
#define B_MAGENTA_U(msg) "\e[4;95m" msg "\e[0m"
#define B_CYAN_U(msg) "\e[4;96m" msg "\e[0m"
#define B_WHITE_U(msg) "\e[4;97m" msg "\e[0m"
