#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


typedef struct coordinates_t {
    int x;
    int y;
} coordinates;
typedef struct map_t {
    char name[3];
    int info;
    char second_name[3];
    int second_info;
    int is_on;
} map;

void insert_map(int row, int column, map map_info[row][column]);

void initialize_game();

void set_color(int row, int column, int x, int y, map map_info[row][column]);

void initialize_game() {
    FILE *fp, *cfp;
    int menu, row = 9, column = 13;
    label :
    printf("\n\t1- PLAY NEW GAME\n\t2- LOAD GAME\n\t3- CUSTOMIZE MAP\n\t4- BACK TO MAIN MENU\n");
    scanf("%d", &menu);
    switch (menu) {
        case 1: {
            fp = fopen("default_map.txt", "r");
            if (!fp) {
                printf("CAN'T OPEN default_map!");
                goto label;
            }
            map map_info[row][column];
            fscanf(fp, "%*[^\n]%*c");
            for (int i = 0; i < row; ++i) {
                fscanf(fp, "%*c%*c%*c");
                for (int j = 0; j < column; ++j) {
                    fscanf(fp, " %s %d", map_info[i][j].name, &map_info[i][j].info);
                }
            }
            fclose(fp);
            insert_map(row, column, map_info);
            break;
        }
        case 2: {

            break;
        }
        case 3: {
            printf("ENTER MAP SIZE :\nrow:");
            scanf("%d", &row);
            printf("column:");
            scanf("%d", &column);
            cfp = fopen("custom_map.txt", "w+");
            fprintf(cfp, "   ");
            for (int i = 0; i < column; ++i) {
                fprintf(cfp, "%-6d", i);
            }
            fprintf(cfp, "\n");
            for (int i = 0; i < row; ++i) {
                fprintf(cfp, "%d- \n", i);
            }
            printf("ENTER THE MAP YOU WANT IN custom_map TEXT FILE IN PROGRAM'S DIRECTORY\nENTER ANY CHARACTER WHEN IT'S READY\n");
            fclose(cfp);
            scanf("%*c%*c");
            cfp = fopen("custom_map.txt", "r");
            if (!cfp) {
                printf("CAN'T OPEN custom_map!");
                exit(-1);
            }
            map map_info[row][column];
            fscanf(cfp, "%*[^\n]%*c");
            for (int i = 0; i < row; ++i) {
                fscanf(cfp, "%*c%*c%*c");
                for (int j = 0; j < column; ++j) {
                    fscanf(cfp, " %s %d", map_info[i][j].name, &map_info[i][j].info);
                }
            }
            fclose(cfp);
            insert_map(row, column, map_info);
            break;
        }
        case 4:
            goto end;
        default:
            printf("WRONG INPUT! TRY AGAIN\n");
            goto label;
    }
    end :
    return;
}

void insert_map(int row, int column, map map_info[row][column]) {

    coordinates map_coordinates[row][column];
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            map_coordinates[i][j].x = j;
            map_coordinates[i][j].y = i;
        }
    }
    printf("  ");
    for (int i = 0; i < column; ++i) {
        if (i % 2 == 0) {
            printf("_____");
        } else {
            printf("         ");
        }
    }
    printf("\n");
    static int x, y;
    for (int i = 0; i < row * 4 + 2; ++i) {
        for (int j = 0; j < column; ++j) {
            x = j;
            int tmp;
            if (j % 2 == 0) {
                tmp = i;
            } else {
                tmp = i - 2;
            }
            y = tmp / 4;
            if (y < 0)
                y = 0;
            if (j == 0) {
                if (i % 4 == 0 || i % 4 == 3) {
                    printf(" ");
                }
            }
            if (i % 4 == 0) {
                if (j % 2 == 0) {
                    if (i == row * 4 && j == 0) {
                        printf("      \\");
                    } else {
                        if (i != row * 4 || j != column - 1) {
                            printf("/     \\");
                        } else {
                            printf("/      ");
                        }
                    }
                } else {
                    printf("       ");
                }
            } else if (i % 4 == 1) {
                if (j % 2 == 0) {
                    if (i != 4 * row + 1 || j != 0) {
                        printf("/  ");
                    } else {
                        printf("   ");
                    }
                    set_color(row, column, x, y, map_info);
                    if (i != 4 * row + 1 || j != column - 1) {
                        printf("  \\");
                    } else {
                        printf("   ");
                    }
                } else {
                    if (i == 1) {
                        printf("_____");
                    } else if (x >= 10) {
                        printf("%d_%d_", x, y);
                    } else {
                        printf("_%d_%d_", x, y);
                    }
                }
            } else if (i % 4 == 2) {
                if (j % 2 == 0) {
                    printf("\\       /");
                } else {
                    printf("     ");
                }
            } else {
                if (j % 2 == 0) {
                    if (x >= 10) {
                        printf("\\%d_%d_/", x, y);
                    } else {
                        printf("\\_%d_%d_/", x, y);
                    }
                } else {
                    printf("  ");
                    set_color(row, column, x, y, map_info);
                    printf("  ");
                }
            }
            if (column % 2 == 0 && j == column - 1) {
                if (i % 4 == 0 && i != 0) {
                    printf("/");
                } else if (i % 4 == 1 && i != 1) {
                    printf("/");
                } else if (i % 4 == 2) {
                    printf("\\");
                } else if (i % 4 == 3) {
                    printf("\\");
                }
            }
        }
        printf("\n");
    }
}

void set_color(int row, int column, int x, int y, map map_info[row][column]) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!strcmp(map_info[y][x].name, "SH") || !strcmp(map_info[y][x].name, "JW") || !strcmp(map_info[y][x].name, "JS")
        || !strcmp(map_info[y][x].name, "IL") || !strcmp(map_info[y][x].name, "MS") ||
        !strcmp(map_info[y][x].name, "SG")
        || !strcmp(map_info[y][x].name, "WG") || !strcmp(map_info[y][x].name, "JB")) {
        SetConsoleTextAttribute(hConsole, 9);
        printf("%-2s%d", map_info[y][x].name, map_info[y][x].info);
    } else if (!strcmp(map_info[y][x].name, "##")) {
        SetConsoleTextAttribute(hConsole, 2);
        printf("# #");
    } else if (!strcmp(map_info[y][x].name, "#_")) {
        SetConsoleTextAttribute(hConsole, 10);
        printf(" # ");
    } else if (!strcmp(map_info[y][x].name, "**")) {
        SetConsoleTextAttribute(hConsole, 14);
        printf("%-2s%d", map_info[y][x].name, map_info[y][x].info);
    } else if (!strcmp(map_info[y][x].name, "!!")) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("%-2s%d", map_info[y][x].name, map_info[y][x].info);
    } else if (!strcmp(map_info[y][x].name, "OO")) {
        printf("()%d", map_info[y][x].info);
    } else {
        printf("   ");
    }
    SetConsoleTextAttribute(hConsole, 7);
}