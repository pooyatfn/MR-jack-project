#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "map.h"

struct character_t *Create_node(FILE *fp);

struct character_t *create_odd_even_node(struct character_t *head);

void add_odd_even_node(character **first, character **second, character *head);

void Add_node(struct character_t **head, FILE *fp);

void char_action(char name[3], int row, int column, map map_info[row][column]);

void swap_map(int x1, int y1, int x2, int y2, int row, int column, map map_info[row][column]);

void delete_node(struct character_t **head, int index);

void shuffle_list(character *head);

void swap_char_struct(character *head, int index);

void odd_even_round_characters(character *head, int turn);

void char_info_print(char name[2]);

void character_actions(int row, int column, int round, int turn, map map_info[row][column]);

void character_actions(int row, int column, int round, int turn, map map_info[row][column]) {
    FILE *fp = fopen("action_characters.txt", "r");
    if (fp == NULL) {
        printf("CAN'T OPEN action_characters!");
        exit(-1);
    }
    character *head = NULL;
    for (int i = 0; i < 8; ++i) {
        Add_node(&head, fp);
    }
    fclose(fp);
    character *first = NULL;
    character *second = NULL;
    int char_num;
    for (; round <= 8; ++round) {
        if (round % 2 != 0) {
            shuffle_list(head);
            add_odd_even_node(&first, &second, head);
        }
        for (; turn <= 4; ++turn) {
            insert_map(row, column, map_info);
            printf("Round %d. Turn %d. Choose a character :\n", round, turn);
            if (round % 2 != 0) {
                odd_even_round_characters(first, turn);
            } else {
                odd_even_round_characters(second, turn);
            }
            scanf("%d", &char_num);
            if (round % 2 != 0) {
                character *tmp = first;
                for (int i = 1; i < char_num; ++i) {
                    tmp = tmp->next;
                }
                char_action(tmp->name, row, column, map_info);
                delete_node(&first, char_num - 1);
            } else {
                character *tmp = second;
                for (int i = 1; i < char_num; ++i) {
                    tmp = tmp->next;
                }
                char_action(tmp->name, row, column, map_info);
                delete_node(&second, char_num - 1);
            }
            printf("\n");
        }
    }
}

struct character_t *Create_node(FILE *fp) {
    struct character_t *new_node = (struct character_t *) malloc(sizeof(struct character_t));
    fscanf(fp, "%s %d\n", new_node->name, &new_node->move);
    return new_node;
}

void Add_node(struct character_t **head, FILE *fp) {
    if (*head == NULL) {
        (*head) = Create_node(fp);
        (*head)->next = NULL;
        return;
    }
    struct character_t *seek = *head;
    while (seek->next) {
        seek = seek->next;
    }
    struct character_t *new_node = Create_node(fp);
    seek->next = new_node;
    new_node->next = NULL;
}

struct character_t *create_odd_even_node(struct character_t *head) {
    struct character_t *new_node = (struct character_t *) malloc(sizeof(character));
    strcpy(new_node->name, head->name);
    new_node->map_coordinates.y = head->map_coordinates.y;
    new_node->map_coordinates.x = head->map_coordinates.x;
    new_node->move = head->move;
    new_node->is_suspect = head->is_suspect;
    return new_node;
}

void add_odd_even_node(character **first, character **second, character *head) {
    character *seek = head;
    (*first) = create_odd_even_node(seek);
    seek = seek->next;
    (*first)->next = create_odd_even_node(seek);
    seek = seek->next;
    (*first)->next->next = create_odd_even_node(seek);
    seek = seek->next;
    (*first)->next->next->next = create_odd_even_node(seek);
    seek = seek->next;
    (*second) = create_odd_even_node(seek);
    seek = seek->next;
    (*second)->next = create_odd_even_node(seek);
    seek = seek->next;
    (*second)->next->next = create_odd_even_node(seek);
    seek = seek->next;
    (*second)->next->next->next = create_odd_even_node(seek);
}

void delete_node(struct character_t **head, int index) {
    character *current = *head;
    if (index == 0) {
        *head = (*head)->next;
        free(current);
        return;
    }
    for (int i = 0; i < index - 1; ++i, current = current->next);
    character *hold = current->next;
    current->next = current->next->next;
    free(hold);
}

void shuffle_list(character *head) {
    int rand_num;
    character *tmp = head;
    srand(time(NULL));
    for (int i = 8; i > 1; --i) {
        rand_num = rand() % i;
        swap_char_struct(tmp, rand_num);
        tmp = tmp->next;
    }
}

void swap_char_struct(character *head, int index) {
    character tmp;
    character *seek = head;
    for (int i = 0; i < index; ++i) {
        seek = seek->next;
    }
    strcpy(tmp.name, seek->name);
    tmp.move = seek->move;
    tmp.map_coordinates.x = seek->map_coordinates.x;
    tmp.map_coordinates.y = seek->map_coordinates.y;
    tmp.is_suspect = seek->is_suspect;
    strcpy(seek->name, head->name);
    seek->move = head->move;
    seek->map_coordinates.x = head->map_coordinates.x;
    seek->map_coordinates.y = head->map_coordinates.y;
    seek->is_suspect = head->is_suspect;
    strcpy(head->name, tmp.name);
    head->move = tmp.move;
    head->map_coordinates.x = tmp.map_coordinates.x;
    head->map_coordinates.y = tmp.map_coordinates.y;
    head->is_suspect = tmp.is_suspect;
}

void odd_even_round_characters(character *head, int turn) {
    character *seek = head;
    for (int i = 0; i < 5 - turn; ++i) {
        printf("%d- ", i + 1);
        char_info_print(seek->name);
        seek = seek->next;
    }
}

void char_info_print(char name[2]) {
    if (!strcmp(name, "SH")) {
        printf("SH: 1) Move 1 to 3 houses. 2) Knowing the identity of a suspect\n");
    } else if (!strcmp(name, "JW")) {
        printf("JW: 1) Move 1 to 3 houses. 2) Give direction to the lantern\n");
    } else if (!strcmp(name, "JS")) {
        printf("JS: 1) Move 1 to 3 houses. 2) Move the lights\n");
    } else if (!strcmp(name, "IL")) {
        printf("IL: 1) Move 1 to 3 houses. 2) Block or open the way out\n");
    } else if (!strcmp(name, "MS")) {
        printf("MS: 1) Move 1 to 4 houses. 2) She can pass over buildings\n");
    } else if (!strcmp(name, "SG")) {
        printf("SG: 1) Move 1 to 3 houses. 2) Bring other characters totally 3 movement closer to him\n");
    } else if (!strcmp(name, "WG")) {
        printf("WG: 1) Move 1 to 3 houses or exchange his location with the location of any other character.\n");
    } else if (!strcmp(name, "JB")) {
        printf("JB: 1) Move 1 to 3 houses. 2)  Opens a manhole and closes another\n");
    }
}

void char_action(char name[3], int row, int column, map map_info[row][column]) {
    int x1, y1, x2, y2, opt, tmp = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            if (!strcmp(name, map_info[i][j].name)) {
                x1 = i;
                y1 = j;
                break;
            }
        }
    }
    if (!strcmp(name, "SH")) {
        printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
        scanf("%d%d", &x2, &y2);
        swap_map(x1, y1, x2, y2, row, column, map_info);
    } else if (!strcmp(name, "JW")) {
        printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
        scanf("%d%d", &x2, &y2);
        swap_map(x1, y1, x2, y2, row, column, map_info);
    } else if (!strcmp(name, "JS")) {
        printf("1- MOVE\n2- USE ABILITY\n");
        scanf("%d", &opt);
        switch (opt) {
            case 1: {
                printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
                scanf("%d%d", &x2, &y2);
                swap_map(x1, y1, x2, y2, row, column, map_info);
            }
            case 2: {
            }
        }
    } else if (!strcmp(name, "IL")) {
        printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
        scanf("%d%d", &x2, &y2);
        swap_map(x1, y1, x2, y2, row, column, map_info);
    } else if (!strcmp(name, "MS")) {
        printf("WHERE DO YOU WANT TO GO  ? (YOU CAN PASS OVER BUILDINGS) ENTER COORDINATES x y:\n");
        scanf("%d%d", &x2, &y2);
        swap_map(x1, y1, x2, y2, row, column, map_info);
    } else if (!strcmp(name, "SG")) {
        printf("1- MOVE\n2- USE ABILITY\n");
        scanf("%d", &opt);
        switch (opt) {
            case 1: {
                printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
                scanf("%d%d", &x2, &y2);
                swap_map(x1, y1, x2, y2, row, column, map_info);
            }
            case 2: {
            }
        }
    } else if (!strcmp(name, "WG")) {
        printf("WHERE DO YOU WANT TO GO ? (YOU CAN CHANGE YOUR LOCATION WITH OTHER CHARACTER'S)\n");
        scanf("%d%d", &x2, &y2);
        swap_map(x1, y1, x2, y2, row, column, map_info);
    } else {
        printf("1- MOVE\n2- USE ABILITY\n");
        scanf("%d", &opt);
        switch (opt) {
            case 1: {
                printf("WHERE DO YOU WANT TO GO ? ENTER COORDINATES x y:\n");
                scanf("%d%d", &x2, &y2);
                swap_map(x1, y1, x2, y2, row, column, map_info);
            }
            case 2: {
            }
        }
    }
}

void swap_map(int x1, int y1, int x2, int y2, int row, int column, map map_info[row][column]) {
    map tmp;
    strcpy(tmp.name, map_info[x1][y1].name);
    tmp.info = map_info[x1][y1].info;
    tmp.is_on = map_info[x1][y1].is_on;
    tmp.second_info = map_info[x1][y1].second_info;
    strcpy(tmp.second_name, map_info[x1][y1].second_name);
    strcpy(map_info[x1][y1].name, map_info[x2][y2].name);
    map_info[x1][y1].info = map_info[x2][y2].info;
    map_info[x1][y1].second_info = map_info[x2][y2].second_info;
    map_info[x1][y1].is_on = map_info[x2][y2].is_on;
    strcpy(map_info[x1][y1].second_name, map_info[x2][y2].second_name);
    strcpy(map_info[x2][y2].name, tmp.name);
    strcpy(map_info[x2][y2].second_name, tmp.second_name);
    map_info[x2][y2].is_on = tmp.is_on;
    map_info[x2][y2].info = tmp.info;
    map_info[x2][y2].second_info = tmp.second_info;
}