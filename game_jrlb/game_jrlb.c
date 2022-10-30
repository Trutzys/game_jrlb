#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAP_SIZE 20
#define LIFE_POINTS 100

typedef struct {
    int x, y;
    int orbs;
    int life;
    int kills;
} Player;

int gameState = 0;
int mapStatus = 1;
int enemiesNumber = 10, orbsNumber = 12, medkitsNumber = 2;
Player player = {0};

void renderMenu() {
    system("cls");
    printf("Defender of Nivia.\n\n");
    printf("1 - New Game\n");
    printf("2 - Load Game\n");
    printf("3 - Instructions\n");
    printf("4 - Exit Game\n");
}

void menuInput() {
    int action;
    do{
        scanf("%d", &action);
    } while(action < 1 && action > 4);

    switch(action){
        case 1:
            gameState = 1;
            break;
        case 2:
            gameState = 2;
            break;
        case 3:
            gameState = 3;
            break;
        case 4:
            gameState = 4;
            break;
    }
}

void renderInstruction() {
    system("cls");
    char line[50];
    FILE * fpointer = fopen("instructions.txt", "r");
    for(int i=0; i < 26; i++){
        fgets(line, 50, fpointer);
        printf("%s", line);
    }
    fclose(fpointer);
}

void instructionInput() {
    char action;
    do{
        scanf("%c", &action);
    } while(action != 'e');
    gameState = 0;
}

void renderStory() {
    system("cls");
    char line[255];
    FILE * fpointer = fopen("story.txt", "r");
    for(int i = 0; i < 5; i++){
        fgets(line, 255, fpointer);
        printf("%s", line);
    }
    printf("\n");
    fclose(fpointer);
}

void renderDeath() {
    system("cls");
    printf("You're dead!!! Press 'E' key to go back to the menu.\n");
    char action;
    do{
        scanf("%c", &action);
    } while(action != 'e');
    gameState = 0;
}

void renderWin() {
    system("cls");
    printf("All Enemies eliminated!!! Press 'E' key to go to the next level.\n");
    char action;
    do{
        scanf("%c", &action);
    } while(action != 'e');
}

void renderEnd() {
    system("cls");
    char action;
    char line[255];
    FILE * fpointer = fopen("ending.txt", "r");
    for(int i = 0; i < 5; i++){
        fgets(line, 255, fpointer);
        printf("%s", line);
    }
    printf("\n");
    fclose(fpointer);
    do{
        scanf("%c", &action);
    } while(action != 'e');
    gameState = 0;
}

void loadMap(const char* filename, char map[MAP_SIZE][MAP_SIZE]) {
    char line[128];
    FILE * fpointer = fopen(filename, "r");
    for(int i = 0; i < MAP_SIZE; i++){
        fgets(line, sizeof(line), fpointer);
        for(int j = 0; j < MAP_SIZE; j++){
            map[i][j] = line[j];
        }
    }
    fclose(fpointer);
}

void renderMap(char map[MAP_SIZE][MAP_SIZE]) {
    system("cls");
    printf("LIFE:%d    ORBS:%d    KILLS:%d\n\n", player.life, player.orbs, player.kills);
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}

void renderElement(char map[MAP_SIZE][MAP_SIZE], char element, int num_elem) {
    for(int e = 1; e <= num_elem;){
        int ey = rand()%20;
        int ex = rand()%20;
        if(map[ey][ex] == '_'){
            map[ey][ex] = element;
            e++;
        }
    }
}

void mapSetup(char map[MAP_SIZE][MAP_SIZE], const char* filename, int playerX, int playerY) {
  	loadMap(filename, map);
  	player.x = playerX;
  	player.y = playerY;
  	player.life = LIFE_POINTS;
  	player.orbs = 0;
  	player.kills = 0;

	map[player.y][player.x] = 'P';
	renderElement(map, 'O', orbsNumber);
    renderElement(map, 'G', enemiesNumber);
    renderElement(map, 'M', medkitsNumber);
}

void finalMap(char map[MAP_SIZE][MAP_SIZE], const char* filename, int playerX, int playerY) {
  	loadMap(filename, map);
  	player.x = playerX;
  	player.y = playerY;
  	player.life = LIFE_POINTS;
  	player.orbs = 0;
  	player.kills = 0;

	map[player.y][player.x] = 'P';
	renderElement(map, 'O', orbsNumber);
    renderElement(map, 'G', enemiesNumber);
    renderElement(map, 'M', medkitsNumber);
    renderElement(map, 'C', 2);
}

void saveGame(char map[MAP_SIZE][MAP_SIZE]) {
    FILE * fpointer = fopen("mapSave.txt", "w");
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            fprintf(fpointer, "%c", map[i][j]);
        }
    fprintf(fpointer, "\n");
    }

    fprintf(fpointer, "%d\n", mapStatus);
    fprintf(fpointer, "%d\n%d\n", player.y, player.x);
    fprintf(fpointer, "%d\n%d\n%d", player.life, player.orbs, player.kills);
    fclose(fpointer);
}

void loadGame(const char* filename, char map[MAP_SIZE][MAP_SIZE]) {
    char line[128];
    FILE * fpointer = fopen(filename, "r");
    for(int i = 0; i < MAP_SIZE; i++){
        fgets(line, sizeof(line), fpointer);
        for(int j = 0; j < MAP_SIZE; j++){
            map[i][j] = line[j];
        }
    }

    fscanf(fpointer, "%d", &mapStatus);
    fscanf(fpointer, "%d", &player.y);
    fscanf(fpointer, "%d", &player.x);
    fscanf(fpointer, "%d", &player.life);
    fscanf(fpointer, "%d", &player.orbs);
    fscanf(fpointer, "%d", &player.kills);
    fclose(fpointer);
}

void loadedSetup(char map[MAP_SIZE][MAP_SIZE], const char* filename) {
    loadGame(filename, map);
}

int checkBoundary(char map[MAP_SIZE][MAP_SIZE], int x, int y) {
    return x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && map[y][x] != 'b';
}

void checkElement(char map[MAP_SIZE][MAP_SIZE]) {
    if(map[player.y][player.x] == 'G'){
        if(player.orbs > 0){
            player.orbs--;
            player.life -= 5;
            player.kills++;
        }
        else{
            player.life -= 50;
            player.kills++;
        }
    }
    else if(map[player.y][player.x] == 'O'){
        player.orbs++;
    }
    else if(map[player.y][player.x] == 'M'){
        if(player.life <= 90){
            player.life += 10;
        }
        else if(player.life > 90){
        player.life = player.life + (100 - player.life);
        }
    }
    else if(map[player.y][player.x] == 'C'){
        if(player.orbs > 5){
            player.orbs -=3;
            player.life -=20;
            player.kills++;
        }
        else{
            player.life -= 999;
        }
    }
}

void handleGameInput(char map[MAP_SIZE][MAP_SIZE]) {
    char input;
    scanf(" %c", &input);
    switch(input) {
        case 'w':
            if(checkBoundary(map, player.x, player.y - 1)) {
                player.y--;
                checkElement(map);
                map[player.y][player.x] = 'P';
                map[player.y + 1][player.x] = '_';
            }
            break;
        case 's':
            if(checkBoundary(map, player.x, player.y + 1)) {
                player.y++;
                checkElement(map);
                map[player.y][player.x] = 'P';
                map[player.y - 1][player.x] = '_';
            }
            break;
        case 'a':
            if(checkBoundary(map, player.x - 1, player.y)) {
                player.x--;
                checkElement(map);
                map[player.y][player.x] = 'P';
                map[player.y][player.x + 1] = '_';
            }
            break;
        case 'd':
            if(checkBoundary(map, player.x + 1, player.y)) {
                player.x++;
                checkElement(map);
                map[player.y][player.x] = 'P';
                map[player.y][player.x - 1] = '_';
            }
            break;
        case 'q':
            saveGame(map);
            break;
        default:
            break;
    }

}

void startGame() {
    srand(time(NULL));
    char any;
    renderStory();
    scanf(" %c", &any);

    char map[MAP_SIZE][MAP_SIZE];
  	mapSetup(map, "map01.txt", 0, 10);

  	do {
        renderMap(map);
        handleGameInput(map);
        if(player.kills == enemiesNumber){
            break;
        }
    } while(player.life > 0);

    if(player.life <= 0){
        gameState = 5;
    }
    else if(player.kills == enemiesNumber){
        gameState = 6;
    }
}

void midGame() {
    system("cls");
    mapStatus++;
    enemiesNumber += 5;
    orbsNumber += 4;
    medkitsNumber += 3;
    srand(time(NULL));
    char map[MAP_SIZE][MAP_SIZE];
    if(mapStatus == 2){
        mapSetup(map, "map02.txt", 4, 19);
    }
    else if(mapStatus == 3){
        mapSetup(map, "map03.txt", 19, 3);
    }
    else if(mapStatus == 4){
        mapSetup(map, "map04.txt", 10, 0);
    }

  	do {
        renderMap(map);
        handleGameInput(map);
        if(player.kills == enemiesNumber){
            break;
        }
    } while(player.life > 0);

    if(player.life <= 0){
        gameState = 5;
    }
    else if(player.kills == enemiesNumber){
        if(mapStatus == 4){
            gameState = 7;
        }
        else{
            gameState = 6;
        }
    }
}

void endGame() {
    system("cls");
    mapStatus = 5;
    orbsNumber = 40;
    medkitsNumber = 8;
    srand(time(NULL));
    char map[MAP_SIZE][MAP_SIZE];
    finalMap(map, "map05.txt", 19, 9);

  	do {
        renderMap(map);
        handleGameInput(map);
        if(player.kills == enemiesNumber + 2){
            break;
        }
    } while(player.life > 0);

    if(player.life <= 0){
        gameState = 5;
    }
    else if(player.kills == enemiesNumber +2){
        gameState = 8;
    }
}

void loadedGame() {
    system("cls");
    enemiesNumber = 10;
    char map[MAP_SIZE][MAP_SIZE];
    loadedSetup(map, "mapSave.txt");
    switch(mapStatus){
        case 1:
            do {
                renderMap(map);
                handleGameInput(map);
                if(player.kills == enemiesNumber){
                    break;
                }
            } while(player.life > 0);

            if(player.life <= 0){
                gameState = 5;
            }
            else if(player.kills == enemiesNumber){
                gameState = 6;
            }
            break;
        case 2:
            do {
                renderMap(map);
                handleGameInput(map);
                if(player.kills == enemiesNumber + 5){
                    break;
                }
            } while(player.life > 0);

            if(player.life <= 0){
                gameState = 5;
            }
            else if(player.kills == enemiesNumber + 5){
                    gameState = 6;
            }
            break;
        case 3:
            do {
                renderMap(map);
                handleGameInput(map);
                if(player.kills == enemiesNumber + 10){
                    break;
                }
            } while(player.life > 0);

            if(player.life <= 0){
                gameState = 5;
            }
            else if(player.kills == enemiesNumber + 10){
                gameState = 6;
            }
            break;
        case 4:
            do {
                renderMap(map);
                handleGameInput(map);
                if(player.kills == enemiesNumber + 15){
                    break;
                }
            } while(player.life > 0);

            if(player.life <= 0){
                gameState = 5;
            }
            else if(player.kills == enemiesNumber + 15){
                    gameState = 7;
            }
            break;
        case 5:
          	do {
                renderMap(map);
                handleGameInput(map);
                if(player.kills == enemiesNumber + 22){
                    break;
                }
            } while(player.life > 0);

            if(player.life <= 0){
                gameState = 5;
            }
            else if(player.kills == enemiesNumber + 22){
                gameState = 8;
            }
            break;
    }
}

void render() {
    switch(gameState){
        case 0:
            renderMenu();
            menuInput();
            break;
        case 1:
            startGame();
            break;
        case 2:
            loadedGame();
            break;
        case 3:
            renderInstruction();
            instructionInput();
            break;
        case 5:
            renderDeath();
            break;
        case 6:
            renderWin();
            midGame();
            break;
        case 7:
            renderWin();
            endGame();
            break;
        case 8:
            renderEnd();
            break;
    }
}

int main() {
    while(gameState != 4){
        render();
    }

    system("cls");
    printf("Game Closed!!!\n");
    return 0;
}
