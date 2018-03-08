#include <stdio.h>
#include <stdlib.h>

#define LENGTH 10
#define MAX_USER 512

struct Pair {
    char key[50];
    int value;
};

void printMaxTweeter(struct Pair arr[]) {
    for (int i = 0; i < LENGTH; i++)
        printf("%s: %d\n", arr[i].key, arr[i].value);
}

int findNamePosition(char line[]) {
    int position = 0;
    char *token = strtok(line, ",");
    while(token) {
        if (strcmp(token, "\"name\"") == 0)
            return position;
        token = strtok(NULL, ",");
        position += 1;
    }

    return -1;
}

char* getTweeterName(char line[], int namepos) {
    int counter = 0;
    char *token = strtok(line, ",");
    while(counter < namepos) {
        token = strtok(NULL, ",");
        counter += 1;
    }

    return token;
}

int main (int argc, char *argv[]) {
    // array of tweeters
    struct Pair tweeters[MAX_USER];

    // initialize array
    for (int i = 0; i < LENGTH; i++) {
        struct Pair temp = {"", -1};
        tweeters[i] = temp;
    }

    FILE *fp;
    char buff[512];
    fp = fopen(argv[1], "r");

    int c = getc(fp);
    while (c != EOF) {
        fgets(buff, 512, (FILE*)fp);
        printf("%s\n", buff);
        c = getc(fp);
    }

    return 0;
}