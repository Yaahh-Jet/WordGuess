//word serach game with difficulty levels and scoring system
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EASY_SIZE 6
#define MEDIUM_SIZE 8
#define HARD_SIZE 11

char grid[HARD_SIZE][HARD_SIZE]; // Max size to accommodate all difficulties
char easyWords[5][10] = { "CODE", "C", "BUG", "DATA", "DEV" };
char mediumWords[7][10] = { "PROGRAM", "VARIABLE", "FUNCTION", "LOOP", "ARRAY", "STACK", "QUEUE" };
char hardWords[10][15] = { "RECURSION", "ALGORITHM", "COMPLEXITY", "MULTITHREAD", "MEMORY", "ENCRYPT", "NETWORK", "DATABASE", "SECURITY", "OPTIMIZE" };

int gridSize, wordCount, penalty = 0, score = 0;

// Function to initialize the grid with random letters
void initializeGrid() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            grid[i][j] = 'A' + (rand() % 26); // Random uppercase letter
        }
    }
}

// Function to check if a space is free for word placement
int isSpaceAvailable(int row, int col, int length, int direction) {
    for (int i = 0; i < length; i++) {
        if (direction == 0 && col + i >= gridSize) return 0; // Horizontal
        if (direction == 1 && row + i >= gridSize) return 0; // Vertical
        if (direction == 2 && (row + i >= gridSize || col + i >= gridSize)) return 0; // Diagonal
        if (direction == 3 && col - i < 0) return 0; // Reverse

        // If occupied by another word
        if (direction == 0 && grid[row][col + i] != ' ') return 0;
        if (direction == 1 && grid[row + i][col] != ' ') return 0;
        if (direction == 2 && grid[row + i][col + i] != ' ') return 0;
        if (direction == 3 && grid[row][col - i] != ' ') return 0;
    }
    return 1;
}

// Function to place words correctly
void placeWords(char words[][15], int wordCount) {
    for (int w = 0; w < wordCount; w++) {
        int placed = 0;
        while (!placed) {
            int row = rand() % gridSize;
            int col = rand() % gridSize;
            int direction = rand() % 4; // 0: Horizontal, 1: Vertical, 2: Diagonal, 3: Reverse

            if (isSpaceAvailable(row, col, strlen(words[w]), direction)) {
                for (int i = 0; i < strlen(words[w]); i++) {
                    if (direction == 0) grid[row][col + i] = words[w][i]; // Horizontal
                    else if (direction == 1) grid[row + i][col] = words[w][i]; // Vertical
                    else if (direction == 2) grid[row + i][col + i] = words[w][i]; // Diagonal
                    else if (direction == 3) grid[row][col - i] = words[w][i]; // Reverse
                }
                placed = 1;
            }
        }
    }
}

// Function to display the grid
void displayGrid() {
    printf("\nWord Search Grid:\n");
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

// Function to check user input against the word list
int checkWord(char *input, char words[][15], int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(input, words[i]) == 0) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Function to handle difficulty selection
void selectDifficulty(char words[][15]) {
    int difficulty;
    printf("\nSelect difficulty level:\n1 - Easy\n2 - Medium\n3 - Hard\nEnter choice: ");
    scanf("%d", &difficulty);

    if (difficulty == 1) {
        gridSize = EASY_SIZE;
        wordCount = 5;
        memcpy(words, easyWords, sizeof(easyWords));
        penalty = 0;
        printf("\n[EASY MODE] - 6x6 Grid, simple words, no penalty.\n");
    } else if (difficulty == 2) {
        gridSize = MEDIUM_SIZE;
        wordCount = 7;
        memcpy(words, mediumWords, sizeof(mediumWords));
        penalty = 5;
        printf("\n[MEDIUM MODE] - 8x8 Grid, words in multiple directions, -5 points for wrong guesses.\n");
    } else {
        gridSize = HARD_SIZE;
        wordCount = 10;
        memcpy(words, hardWords, sizeof(hardWords));
        penalty = 10;
        printf("\n[HARD MODE] - 11x11 Grid, includes reversed placement, -10 points for wrong guesses!\n");
    }
}

// Main function
int main() {
    srand(time(0)); // Seed random generator

    char words[HARD_SIZE][15];

    selectDifficulty(words);
    initializeGrid();
    placeWords(words, wordCount);
    displayGrid();

    char userInput[20];
    for (int i = 0; i < wordCount; i++) {
        printf("\nEnter a word you found: ");
        scanf("%s", userInput);

        if (checkWord(userInput, words, wordCount)) {
            printf("✅ Great! '%s' is correct.\n", userInput);
            score += 10;
        } else {
            printf("❌ Oops! '%s' is not in the word search. You lose %d points.\n", userInput, penalty);
            score -= penalty;
        }

        printf("🔢 Current Score: %d\n", score);
    }

    printf("\n🎉 Game Over! Your final score: %d points\n", score);
    return 0;
}