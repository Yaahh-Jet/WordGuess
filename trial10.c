#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

#define MAX_WORDS 10
#define MAX_WORD_LEN 20
#define GRID_SIZE_EASY 6
#define GRID_SIZE_MEDIUM 8
#define GRID_SIZE_HARD 11

typedef struct {
    char word[MAX_WORD_LEN];
    char hint[100];
    int found;
    int row, col, dir;
} WordData;

char grid[GRID_SIZE_HARD][GRID_SIZE_HARD];
int gridSize = 0, wordCount = 0, score = 0, penalty = 0, maxDirection = 1;
WordData words[MAX_WORDS];

const int directions[6][2] = {
    {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearGrid() {
    for (int i = 0; i < gridSize; i++)
        for (int j = 0; j < gridSize; j++)
            grid[i][j] = ' ';
}

int loadWords(const char *filename, int count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    char line[200];
    int loaded = 0;
    while (fgets(line, sizeof(line), fp) && loaded < count) {
        char *sep = strchr(line, ':');
        if (sep) {
            *sep = '\0';
            strcpy(words[loaded].word, line);
            strcpy(words[loaded].hint, sep + 1);
            words[loaded].hint[strcspn(words[loaded].hint, "\n")] = '\0';
            words[loaded].found = 0;
            loaded++;
        }
    }

    fclose(fp);
    return loaded == count;
}

int placeWordInGrid(int index) {
    int attempts = 100;
    int len = strlen(words[index].word);

    while (attempts--) {
        int row = rand() % gridSize;
        int col = rand() % gridSize;
        int dir = rand() % (maxDirection + 1);
        int dr = directions[dir][0];
        int dc = directions[dir][1];

        int r = row, c = col, fit = 1;
        for (int i = 0; i < len; i++) {
            if (r < 0 || r >= gridSize || c < 0 || c >= gridSize)
                { fit = 0; break; }
            if (grid[r][c] != ' ' && grid[r][c] != words[index].word[i])
                { fit = 0; break; }
            r += dr; c += dc;
        }

        if (fit) {
            r = row; c = col;
            for (int i = 0; i < len; i++) {
                grid[r][c] = words[index].word[i];
                r += dr; c += dc;
            }
            words[index].row = row;
            words[index].col = col;
            words[index].dir = dir;
            return 1;
        }
    }
    return 0;
}

void placeAllWords() {
    for (int i = 0; i < wordCount; i++) {
        if (!placeWordInGrid(i)) {
            printf("Failed to place word: %s\n", words[i].word);
        }
    }
}

void fillGrid() {
    for (int i = 0; i < gridSize; i++)
        for (int j = 0; j < gridSize; j++)
            if (grid[i][j] == ' ')
                grid[i][j] = 'A' + rand() % 26;
}

void displayGrid() {
    printf("\n   ");
    for (int j = 0; j < gridSize; j++) printf("%2d", j);
    printf("\n");

    for (int i = 0; i < gridSize; i++) {
        printf("%2d ", i);
        for (int j = 0; j < gridSize; j++) {
            int highlight = 0;
            for (int w = 0; w < wordCount; w++) {
                if (words[w].found) {
                    int len = strlen(words[w].word);
                    int r = words[w].row, c = words[w].col;
                    int dr = directions[words[w].dir][0];
                    int dc = directions[words[w].dir][1];

                    for (int k = 0; k < len; k++) {
                        if (i == r && j == c) highlight = 1;
                        r += dr; c += dc;
                    }
                }
            }
            if (highlight) setColor(10);  // Green
            else setColor(7);  {   Sleep(50);        // White
            printf("%c ", grid[i][j]);}
        }
        printf("\n");
    }
    setColor(7);
}

void toUpper(char *str) {
    while (*str) { *str = toupper(*str); str++; }
}

void provideHint(const char *word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(word, words[i].word) == 0 && !words[i].found) {
            printf("💡 Hint: %s\n", words[i].hint);
            return;
        }
    }
}

void playLevel(const char *filename, int level) {
    wordCount = (level == 1) ? 2 : 3;
    gridSize = (level == 1) ? GRID_SIZE_EASY : (level == 2) ? GRID_SIZE_MEDIUM : GRID_SIZE_HARD;
    maxDirection = (level == 1) ? 1 : (level == 2) ? 2 : 5;
    penalty = (level == 1) ? 0 : (level == 2) ? 5 : 10;

    clearGrid();
    if (!loadWords(filename, wordCount)) return;
    placeAllWords();
    fillGrid();

    int found = 0;
    displayGrid();
    int hintsUsed = 0;

    while (found < wordCount) {
        char guess[MAX_WORD_LEN];
        printf("\n🔍 Enter a word: ");
        scanf("%s", guess);
        toUpper(guess);

        int correct = 0;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(guess, "HINT") == 0) {
            if (hintsUsed < 2) {
                provideHint(guess);
                hintsUsed++;
            } else {
                printf("❗ You’ve already used both your hints.\n");
                continue;
            }
        }
            if (strcmp(guess, words[i].word) == 0 && !words[i].found) {
                printf("✅ Correct!\n");
                words[i].found = 1;
                score += 10;
                correct = 1;
                found++;
                break;
            }
        }

        if (!correct) {
            printf("❌ Wrong! -%d points\n", penalty);
            score -= penalty;
            if (hintsUsed < 2) {
                provideHint(guess);
                hintsUsed++;
            }
        }

        printf("🎯 Score: %d\n", score);
        displayGrid();
    }
}

void bonusRound() {
    printf("\n🎁 BONUS ROUND!\n");
    printf("Guess the word from emojis:\n");
    printf(":) is What emotion ?\n");  
    printf("Type your answer: ");
    char guess[20];
    scanf("%s", guess);
    toUpper(guess);
    if (strcmp(toupper(guess), "HAPPY") == 0) {
        printf("🎉 Correct! Bonus 20 points!\n");
        score += 20;
    } else {
        printf("❌ Sorry! No bonus.\n");
    }
}

int main() {
    srand(time(0));
    int level = 1;
    char play;
    printf("🧠 Welcome to the Word Search Game!\n");
    printf("Enter ur name:");
    char name[50];
    scanf("%s", name);
    setColor(15);
    printf("Giddy up %s! Let's start the game!\n", name);
    setColor(7);
    Sleep(1000);
    printf("-----------------------------------------\n");
    printf("Just to set the rules straight, in the first grid\n");
    setColor(2);
    printf("(EASY) ;");
    setColor(3);
    printf("2 words are 3 to 4 letters long and will be placed either horizontally or vertically;\n");
    setColor(7);
    printf("If u choose to go to the next grid\n");
    setColor(6);
    printf("(MEDIUM),");
    setColor(3);
    printf("it contains 3 words of 4 to 5 letters long which will either be placed diagonally or horizontally or vertically;\n");
    setColor(7);
    printf("The next level \n");
    setColor(4);
    printf("(HARD) ");
    setColor(3);
    printf("also contains 3 words and is 6 to 9 letters long which will either be placed reversed or diagonally or horizontally or vertically.\n\n");
    setColor(7);
    printf("-----------------------------------------------------\n");
    Sleep(5000);


    while (1) {
        if (level == 1) playLevel("Easyy.txt", level);
        else if (level == 2) playLevel("Mediumm.txt", level);
        else playLevel("Hardd.txt", level);

        if (level == 3) {
            bonusRound();
            break;
        }

        printf("\nNext level? (y/n): ");
        scanf(" %c", &play);
        if (tolower(play) == 'y') level++;
        else break;
    }

    printf("\n🏁 Final Score: %d\n", score);
    printf("Thanks for playing %s!\n",name);
    return 0;
}
