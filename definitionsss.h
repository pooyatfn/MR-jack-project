#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

typedef struct map_t {
    char name[3];
    int info;
    char second_name[3];
    int second_info;
    int is_on;
} map;

typedef struct game_info_t {
    char mr_jack_name[20];
    char detective_name[20];
    int round;
    int turn;
} game_info;

typedef struct character_t {
    char name[3];
    struct character_t *next;
} character;