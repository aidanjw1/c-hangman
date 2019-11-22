#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct State {
    char *word;
    int wordLen;
    int *displayed;
    char *guessedLetters;
    int guessedLettersLen;
    int strikesRemaining;
};

typedef struct State State;

void display(State *state) {
     for(int i = 0; i < state->wordLen; i++) {
        if(state->displayed[i] == 1) {
             printf("%c ", state->word[i]);
        }
        else {
            printf("_ ");
        }
    }
    printf("\n\n");
    printf("Guessed Letters: ");
    for(int i = 0; i < state->guessedLettersLen; i++) {
        printf("%c, ", state->guessedLetters[i]);
    }
    printf("\n");
    printf("Strikes Remaining: %i\n", state->strikesRemaining);
}

// Return 1 if game has been won, -1 if lost, 0 if game still in progress
int evalGameProgress(State *state) {
    if(state->strikesRemaining < 1) {
        return -1;
    }

    for(int i = 0; i < state->wordLen; i++) {
        if(state->displayed[i] == 0) {
            return 0;
        }
    }

    return 1;
}

char *getRandomWord() {
    FILE *fp;
    char *line;

    // Generate a random number between 1 and 10000 (10000 words in file)
    srand(time(0));
    int n = (int) rand() % 10000;

    fp = fopen("./google-10000-english.txt", "r");
    char *s = malloc(sizeof(char) * 255);

    int i = 0;
    while(fgets(s, 255, fp) && i < n) {
        i++;
    }

    // Trim the tailing newline from the string
    for(int i = 0; i < 255; i++) {
        if(s[i] == '\n') {
            s[i] = '\0';
            break;
        }
    }

    return s;
}

void guessLetter(char letter, State *state) {
    for(int i = 0; i < state->guessedLettersLen; i++) {
        if(state->guessedLetters[i] == letter) {
            printf("Letter already guessed!");
            return;
        }
    }

    int found = 0;
    for(int i = 0; i < state->wordLen; i++) {
        if(state->word[i] == letter) {
            if (!found) {
                found = 1;
            }
            state->displayed[i] = 1;
        }
    }
    if(!found) {
        state->guessedLetters[state->guessedLettersLen++] = letter;
        state->strikesRemaining--;
    }
}

int main() {
    printf("Welcome to hangman!\n\n");

    char *word = getRandomWord();

    State *state = malloc(sizeof(State));
    state->word = word;
    state->wordLen = (int) strlen(word);
    state->displayed = malloc(sizeof(int) * state->wordLen);
    for(int i = 0; i < state->wordLen; i++) {
        state->displayed[i] = 0;
    }
    state->guessedLetters = malloc(sizeof(char) * 26);
    state->guessedLettersLen = 0;
    state->strikesRemaining = 5;



    int progress;
    while(1) {
        display(state);
        printf("Enter a guess: ");
        char c = getchar();

        // Clear the <enter> in input 
        char _ = c;
        while(_ != '\n') {
            _ = getchar();
        }

        guessLetter(c, state);
        progress = evalGameProgress(state);
        if (progress != 0) {
            break;
        }

        printf("======================================\n\n\n");
    }

    printf("======================================\n\n");
    if (progress == 1) {
        printf("You Won!\n\n");
        printf("The word was \"%s\"\n\n", word);
    }
    else if (progress == -1) {
        printf("You Lost...\n\n");
        printf("The word was \"%s\"\n\n", word);
    }
}
