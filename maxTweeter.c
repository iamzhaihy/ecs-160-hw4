#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH 10
#define MAX_USER 10000
#define MAX_LINE 20000
#define MAX_BUFFER 512

struct Pair {
    char key[50];
    int value;
};

void printMaxTweeter(struct Pair arr[]) {
    // for (int i = 0; i < 10; i++)
    //     printf("%s: %d\n", arr[i].key, arr[i].value);

    int j = 1;
    // print all noempty entries
    while (j < MAX_USER && arr[j].value != -1) {
        printf("%s: %d\n", arr[j].key, arr[j].value);
        j += 1;
    } 
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

int containsKey(struct Pair arr[], char key[]){
    int pos = 1;
    for (pos = 1; pos < MAX_USER; pos++) {
        if (strcmp(arr[pos].key,key) == 0)
            return pos;

        if (arr[pos].value == -1)
            return -1 * pos;
    }
    return 0;
}


int cmpfunc (const void * p1, const void * p2) {
    int l = ((struct Pair *)p1)->value;
    int r = ((struct Pair *)p2)->value; 
    if (l < r)
        return -1;
    else if (l > r)
        return +1;
    else
        return 0;
}

int main (int argc, char *argv[]) {
    // array of tweeters
    struct Pair tweeters[MAX_USER];

    // initialize array
    for (int i = 0; i < MAX_USER; i++) {
        struct Pair temp = {"", -1};
        tweeters[i] = temp;
    }

    FILE *fp;
    char buff[512];
//    fp = fopen(argv[1], "r");
    fp = fopen("/Users/zhaihy/Desktop/ecs-160-hw4/cl-tweets-short.csv", "r");

    fgets(buff, MAX_BUFFER, (FILE*)fp);
    int namepos = findNamePosition(buff);

    int c = getc(fp);
    while (c != EOF) {
        fgets(buff, MAX_BUFFER, (FILE*)fp);
        char name[50];

        strcpy(name, getTweeterName(buff, namepos));

        int tweeterpos = containsKey(tweeters, name);

        if (tweeterpos > 0) {
            strcpy(tweeters[tweeterpos].key, name);
            tweeters[tweeterpos].value += 1;
        } else {
            int pos = -1 * tweeterpos;
            strcpy(tweeters[pos].key, name);
            tweeters[pos].value = 1;
        }
        c = getc(fp);
    }

    printMaxTweeter(tweeters);
    
    fclose(fp);

    return 0;
}