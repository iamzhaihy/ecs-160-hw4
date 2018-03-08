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

void printArray(struct Pair arr[], int n) {
    for (int i = 0; i < n; i++)
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

void merge(struct Pair arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    struct Pair L[n1], R[n2];
 
    for (i = 0; i < n1; i++) {
        strcpy(L[i].key, arr[l + i].key);
        L[i].value = arr[l + i].value;
    }

    for (j = 0; j < n2; j++) {
        strcpy(R[j].key,arr[m + 1+ j].key);
        R[j].value = arr[m + 1+ j].value;        
    }
 
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i].value <= R[j].value) {
            strcpy(arr[k].key, R[j].key);
            arr[k].value = R[j].value;
            j++;
        } else {
            strcpy(arr[k].key, L[i].key);
            arr[k].value = L[i].value;
            i++;
        }
        k++;
    }
 
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(struct Pair arr[], int l, int r) {
    if (l < r) {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r); 
        merge(arr, l, m, r);
    }
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
    fp = fopen(argv[1], "r");
    // fp = fopen("/Users/zhaihy/Desktop/ecs-160-hw4/cl-tweets-short.csv", "r");

    if (!fp) {
        printf("ERROR: File not exist\n");
        exit(1);
    }

    fgets(buff, MAX_BUFFER, (FILE*)fp);
    int namepos = findNamePosition(buff);

    if (namepos == -1) {
        printf("ERROR: Invalid CSV File\n");
        exit(1);
    }

    int c = getc(fp);
    while (c != EOF) {
        char name[50];        
        fgets(buff, MAX_BUFFER, (FILE*)fp);
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

    mergeSort(tweeters, 0, MAX_USER-1);
    printArray(tweeters, 10);    

    fclose(fp);

    return 0;
}