#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct coordinates_t {
    int x;
    int y;
} coordinates;
typedef struct character_t {
    char name[3];
    int move;
    coordinates map_coordinates;
    int is_suspect;
    struct character_t *next;
} character;

struct character_t *Create_node(FILE *fp);

struct character_t *create_odd_even_node(struct character_t *head);

void add_odd_even_node(character **first, character **second, character *head);

void Add_node(struct character_t **head, FILE *fp);

void delete_node(struct character_t **head, int index);


void shuffle_list(character *head);

void swap_char_struct(character *head, int index);

void odd_even_round_characters(character *head, int turn);

void char_info_print(char name[2]);

int main() {
    FILE *fp = fopen("action_characters.txt", "r");
    character *head = NULL;
    for (int i = 0; i < 8; ++i) {
        Add_node(&head, fp);
    }
    fclose(fp);

    character *first = NULL;
    character *second = NULL;

    int char_num;
    for (int i = 1; i <= 8; ++i) {
        if (i % 2 != 0) {
            shuffle_list(head);
            add_odd_even_node(&first, &second, head);
        }
        for (int j = 1; j <= 4; ++j) {
            printf("Round %d. Turn %d. Choose a character :\n", i, j);
            if (i % 2 != 0) {
                odd_even_round_characters(first, j);
            } else {
                odd_even_round_characters(second, j);
            }
            scanf("%d", &char_num);
            if (i % 2 != 0) {
                delete_node(&first, char_num - 1);
            } else {
                delete_node(&second, char_num - 1);
            }
            printf("\n");
        }
    }
    return 0;
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