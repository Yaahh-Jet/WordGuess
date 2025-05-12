#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

void display();
int call();
void place_word(char grid[10][10], char *word);

int main() {
    printf("\t*********************************************** Word Guessing Game *********************************************************\n\n");

    char Name[100];
    printf("Enter name  : ");
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name, "\n")] = 0; // Remove newline character

    int score = 0;
    char n;

    do {
        system("cls"); // Clears screen for better experience
        display();
        score += call();

        // Ensure proper input handling
        do {
            printf("Do you want to play again (y/n)? ");
            scanf(" %c", &n);
            getchar(); // Clears the newline buffer
            n = tolower(n); // Converts input to lowercase for consistency
        } while (n != 'y' && n != 'n');

    } while (n != 'n');

    printf("%s, your final score is: %d\n", Name, score);
    printf("\n\t***************************************************************************************************************\n\n");

    return 0;
}

void place_word(char grid[10][10], char *word) {
    int wordLen = strlen(word);
    int direction = rand() % 3; // 0 = vertical, 1 = horizontal, 2 = diagonal
    int row, col;

    if (direction == 0) { // Vertical
        row = rand() % (10 - wordLen);
        col = rand() % 10;
        for (int i = 0; i < wordLen; i++) {
            grid[row + i][col] = word[i];
        }
    } else if (direction == 1) { // Horizontal
        row = rand() % 10;
        col = rand() % (10 - wordLen);
        for (int i = 0; i < wordLen; i++) {
            grid[row][col + i] = word[i];
        }
    } else { // Diagonal
        row = rand() % (10 - wordLen);
        col = rand() % (10 - wordLen);
        for (int i = 0; i < wordLen; i++) {
            grid[row + i][col + i] = word[i];
        }
    }
}

int call() {
    char *words[10] = {"Efficient", "Learning", "World", "Hello", "Name", "Jani", "Share", "Complete", "Global", "Comment"};
    char grid[10][10];
    srand(time(NULL));

    // Fill grid with random letters
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 'a' + (rand() % 26);
        }
    }

    // Choose random word and place it
    int index = rand() % 10;
    char *selectedWord = words[index];
    place_word(grid, selectedWord);

    // Display the grid
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf(" %c", grid[i][j]);
            Sleep(50); // Slows down output for better effect
        }
        printf("\n");
    }

    // User input for guessing
    int hint;
    char guess[100];

    printf("Press 1 to guess the word\nPress 2 for a hint\n");
    do {
        printf("Enter choice: ");
        scanf("%d", &hint);
        getchar(); // Clears newline buffer
    } while (hint != 1 && hint != 2);

    if (hint == 2) {
        printf("Hint: The guessed word has %lu letters\n", strlen(selectedWord));
    }

    printf("Enter your guessed word: ");
    scanf("%99s", guess);

    if (strcmp(guess, selectedWord) == 0) {
        printf("Congratulations! You guessed the correct word.\n");
        return 1;
    } else {
        printf("Sorry, incorrect guess. Try again.\n");
        return 0;
    }
}

void display() {
    printf("\nWelcome to the Word Guessing Game!\nTry to find the hidden word in the puzzle.\n\n");
}