#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USER 10000
#define MAX_LINE 20000
#define MAX_BUFFER 512

struct Pair {
    char key[50]; // tweeter's name
    int value;    // number of tweets
};

// helper function to print first n element
void printArray(struct Pair arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (arr[i].value != -1) {
            printf("%s: %d\n", arr[i].key, arr[i].value);
        } // if
    } // for
} // printArray()

// helper function to find the column called "name"
// return -1 if such a column does not exist
int findNamePosition(char line[]) {
    int position = 0;

    // split the string using commas
    char *running = strdup(line);
    char *token = strsep(&running, ",");

    // go through the columns and find "name"
    while (token) {
        // if exists, return its position
        if (strcmp(token, "\"name\"") == 0 || strcmp(token, "name") == 0) {
            return position;
        }
        // if not, keep looping
        token = strsep(&running, ",");
        position += 1;
    } // while

    return -1;
} // findNamePosition()

// helper function to get the name of tweeter
char *getTweeterName(char line[], int namepos) {
    // split the string using commas
    char *running = strdup(line);
    char *token = strsep(&running, ",");

    // go to the specified position
    for (int i = 0; i < namepos; i++) {
        token = strsep(&running, ",");
    } // for

    return token;
} // getTweeterName()

// helper function to check if the array contains the key
int containsKey(struct Pair arr[], char key[]) {
    for (int pos = 1; pos < MAX_USER; pos++) {
        // if the array contains the key,
        // return its position
        if (strcmp(arr[pos].key, key) == 0) {
            return pos;
        }
        // if the array does not contain the key,
        // return the position it can be added
        if (arr[pos].value == -1) {
            return -1 * pos;
        }
    } // for
    return 0;
} // containsKey()

// the merge procedure in Merge-Sort
void merge(struct Pair arr[], int l, int m, int r) {
    int i, j, k;        // iterators
    int n1 = m - l + 1; // legnth of Left array
    int n2 = r - m;     // length of Right array

    struct Pair L[n1], R[n2];

    // populate Left array
    for (i = 0; i < n1; i++) {
        strcpy(L[i].key, arr[l + i].key);
        L[i].value = arr[l + i].value;
    } // for

    // populate Right array
    for (j = 0; j < n2; j++) {
        strcpy(R[j].key, arr[m + 1 + j].key);
        R[j].value = arr[m + 1 + j].value;
    } // for

    // reset iterators
    i = 0;
    j = 0;
    k = l;

    // merge two arrays
    while (i < n1 && j < n2) {
        // arrange the Pair in decreasing order
        if (L[i].value <= R[j].value) {
            strcpy(arr[k].key, R[j].key);
            arr[k].value = R[j].value;
            j++;
        } else {
            strcpy(arr[k].key, L[i].key);
            arr[k].value = L[i].value;
            i++;
        } // else

        k++;
    } // while

    // fill in the remaining element
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    } // while

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    } // while
} // merge()

// body of Merge-Sort Procedure
void mergeSort(struct Pair arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    } // if
} // mergeSort()

int main(int argc, char *argv[]) {
    // array of tweeters
    struct Pair tweeters[MAX_USER];

    // initialize array
    for (int i = 0; i < MAX_USER; i++) {
        struct Pair temp = {"", -1};
        tweeters[i] = temp;
    } // for

    FILE *fp;
    // declare buffer
    char buff[MAX_BUFFER];
    // open the file, whose path is given by argv[1]
    fp = fopen(argv[1], "r");
    // fp = fopen("/Users/zhaihy/Desktop/ecs-160-hw4/cl-tweets-short.csv", "r");

    // if the file does not exist
    if (!fp) {
        // print error message and exist
        printf("ERROR: File not exist\n");
        exit(1);
    } // if

    // read in the first line (header)
    fgets(buff, MAX_BUFFER, (FILE *) fp);
    // find the position of column "name"
    int namepos = findNamePosition(buff);

    // if the file does not have "name" colum
    if (namepos == -1) {
        // print error message and exist
        printf("ERROR: Invalid CSV File\n");
        exit(1);
    } // if

    // read in the next char
    int c = getc(fp);

    // keep track of the number of lines
    int lineCounter = 0;

    // while EOF is not reached
    while (c != EOF) {
        // if the file has too many lines
        if (lineCounter >= MAX_LINE) {
            // print error message and exist
            printf("ERROR: File Too Large\n");
            exit(1);
        } // if

        char tname[50];
        // read in the next line
        fgets(buff, MAX_BUFFER, (FILE *) fp);
        // get tweeter's name and "assign" it to tname
        strcpy(tname, getTweeterName(buff, namepos));

        // check if we have the info about this tweeter
        int tpos = containsKey(tweeters, tname);

        if (tpos > 0) {
            // we have his/her info, increment counter
            tweeters[tpos].value += 1;
        } else {
            // we don't have his/her info
            int pos = -1 * tpos;
            // create new entry
            strcpy(tweeters[pos].key, tname);
            // initialize the count to be 1
            tweeters[pos].value = 1;
        } // else

        c = getc(fp);
        lineCounter++;
    } // while

    // sort tweeters by value in decreasing order
    mergeSort(tweeters, 0, MAX_USER - 1);

    // print the first 10 in the array
    printArray(tweeters, 10);

    // close the file
    fclose(fp);

    return 0;
} // main
