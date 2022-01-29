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

void character_actions(int row, int column, map map_info[row][column], game_info gameInfo);

void character_actions(int row, int column, map map_info[row][column], game_info gameInfo) {
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
    for (; gameInfo.round <= 8; ++gameInfo.round) {
        if (gameInfo.round % 2 != 0) {
            shuffle_list(head);
            add_odd_even_node(&first, &second, head);
        }
        for (gameInfo.turn = 1; gameInfo.turn <= 4; ++gameInfo.turn) {
            insert_map(row, column, map_info);
            if (gameInfo.round % 2 == 1) {
                if (gameInfo.turn == 1 || gameInfo.turn == 4) {
                    printf("\nROUND %d. TURN %d. CHOOSE A CHARACTER %s:\n\n", gameInfo.round, gameInfo.turn,gameInfo.detective_name);
                }
                else {
                    printf("\nROUND %d. TURN %d. CHOOSE A CHARACTER %s:\n\n", gameInfo.round, gameInfo.turn,gameInfo.mr_jack_name);
                }
            }
            else if (gameInfo.turn == 1 || gameInfo.turn == 4){
                printf("\nROUND %d. TURN %d. CHOOSE A CHARACTER %s:\n\n", gameInfo.round, gameInfo.turn,gameInfo.mr_jack_name);
            }
            else {
                printf("\nROUND %d. TURN %d. CHOOSE A CHARACTER %s:\n\n", gameInfo.round, gameInfo.turn,gameInfo.detective_name);
            }
            if (gameInfo.round % 2 != 0) {
                odd_even_round_characters(first, gameInfo.turn);
            } else {
                odd_even_round_characters(second, gameInfo.turn);
            }
            scanf("%d", &char_num);
            if (gameInfo.round % 2 != 0) {
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
    fscanf(fp, "%s %*d\n", new_node->name);
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
    strcpy(seek->name, head->name);
    strcpy(head->name, tmp.name);
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
        printf("SH: 1) MOVE 1 TO 3 HOUSES. 2) KNOWING THE IDENTITY OF A SUSPECT\n");
    } else if (!strcmp(name, "JW")) {
        printf("JW: 1) MOVE 1 TO 3 HOUSES. 2) GIVE DIRECTION TO THE LANTERN\n");
    } else if (!strcmp(name, "JS")) {
        printf("JS: 1) MOVE 1 TO 3 HOUSES. 2) MOVE THE LIGHTS\n");
    } else if (!strcmp(name, "IL")) {
        printf("IL: 1) MOVE 1 TO 3 HOUSES. 2) BLOCK OR OPEN A WAY OUT\n");
    } else if (!strcmp(name, "MS")) {
        printf("MS: 1) MOVE 1 TO 4 HOUSES. 2) SHE CAN PASS OVER BUILDINGS\n");
    } else if (!strcmp(name, "SG")) {
        printf("SG: 1) MOVE 1 TO 3 HOUSES. 2) BRING OTHER CHARACTERS TOTALLY 3 MOVEMENT CLOSER TO HIMSELF\n");
    } else if (!strcmp(name, "WG")) {
        printf("WG: 1) MOVE 1 TO 3 HOUSES OR EXCHANGE HIS LOCATION WITH THE LOCATION OF ANY OTHER CHARACTER.\n");
    } else if (!strcmp(name, "JB")) {
        printf("JB: 1) MOVE 1 TO 3 HOUSES. 2)  OPENS A MANHOLE AND CLOSES ANOTHER\n");
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