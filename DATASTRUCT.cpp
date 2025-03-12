#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define INITIAL_TABLE_SIZE 10

int TABLE_SIZE = INITIAL_TABLE_SIZE;

struct HashTable {
    char key[200];
} *ht;

struct Node {
    char key[200];
    struct Node *next;
};

struct Node **htChain;

void initializationHT() {
    ht = (struct HashTable *)malloc(sizeof(struct HashTable) * TABLE_SIZE);
    htChain = (struct Node **)malloc(sizeof(struct Node *) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        strcpy(ht[i].key, "");
        htChain[i] = NULL;
    }
}

int StoI(char key[]) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) sum += key[i];
    return sum;
}

int MidSquareHash(char key[]) {
    int sq = StoI(key) * StoI(key);
    return (sq / 10) % TABLE_SIZE;
}

int FoldingHash(char key[]) {
    int sum = 0, num = StoI(key);
    while (num > 0) { sum += num % 10; num /= 10; }
    return sum % TABLE_SIZE;
}

int DigitExtractionHash(char key[]) {
    int sum = 0, num = StoI(key);
    while (num > 0) { sum += num % 10; num /= 10; }
    return sum % TABLE_SIZE;
}

int RotatingHash(char key[]) {
    int sum = 0, num = StoI(key);
    while (num > 0) {
        sum = (sum << 5) | (sum >> (sizeof(int) * 8 - 5));
        sum += num % 10;
        num /= 10;
    }
    return sum % TABLE_SIZE;
}

int linearProbing(int index) {
    int i = 1;
    while (strcmp(ht[(index + i) % TABLE_SIZE].key, "") != 0 && i < TABLE_SIZE) i++;
    return (index + i) % TABLE_SIZE;
}

void rehash() {
    int oldSize = TABLE_SIZE;
    TABLE_SIZE *= 2;
    struct HashTable *oldHT = ht;
    
    initializationHT();
    
    for (int i = 0; i < oldSize; i++) {
        if (strcmp(oldHT[i].key, "") != 0) {
            int index = MidSquareHash(oldHT[i].key) % TABLE_SIZE;
            strcpy(ht[index].key, oldHT[i].key);
        }
    }
    free(oldHT);
}

void insertChaining(char key[], int (*hashFunction)(char[])) {
    int index = hashFunction(key);
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    strcpy(newNode->key, key);
    newNode->next = htChain[index];
    htChain[index] = newNode;
    printf("Key '%s' dimasukkan ke index %d dengan Chaining.\n", key, index);
}

void insert(char key[], int (*hashFunction)(char[]), int collisionChoice) {
    int index = hashFunction(key);
    if (collisionChoice == 3) {
        insertChaining(key, hashFunction);
        return;
    }

    if (strcmp(ht[index].key, "") == 0) {
        strcpy(ht[index].key, key);
        printf("Key '%s' dimasukkan ke index %d.\n", key, index);
    } else {
        printf("Collision terjadi! Menggunakan metode collision handling...\n");
        if (collisionChoice == 1) {
            index = linearProbing(index);
            strcpy(ht[index].key, key);
            printf("Key '%s' dimasukkan ke index %d setelah Linear Probing.\n", key, index);
        } else if (collisionChoice == 2) {
            rehash();
            insert(key, hashFunction, collisionChoice);
        }
    }
}

void displayHT() {
    printf("\nHash Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: %s\n", i, strcmp(ht[i].key, "") ? ht[i].key : "Kosong");
    }
}

void displayChainingHT() {
    printf("\nHash Table dengan Chaining:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct Node *temp = htChain[i];
        printf("Index %d: ", i);
        while (temp) {
            printf("%s -> ", temp->key);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void menu() {
    int hashChoice, collisionChoice;
    int (*hashFunction)(char[]) = NULL;
    
    printf("Pilih metode Hashing:\n");
    printf("1. Mid-Square Hashing\n2. Folding Hashing\n3. Digit Extraction Hashing\n4. Rotating Hashing\n");
    scanf("%d", &hashChoice);
    switch (hashChoice) {
        case 1: hashFunction = MidSquareHash; break;
        case 2: hashFunction = FoldingHash; break;
        case 3: hashFunction = DigitExtractionHash; break;
        case 4: hashFunction = RotatingHash; break;
        default: hashFunction = MidSquareHash; break;
    }

    printf("Pilih metode Penanganan Collision:\n");
    printf("1. Linear Probing\n2. Rehashing\n3. Chaining\n");
    scanf("%d", &collisionChoice);
    getchar();

    char key[200];
    while (1) {
        printf("Masukkan key (maks 2 kata) atau 'done' untuk berhenti: ");
        fgets(key, 200, stdin);
        key[strcspn(key, "\n")] = 0;
        if (strcmp(key, "done") == 0) break;
        insert(key, hashFunction, collisionChoice);
    }
    
    if (collisionChoice == 3) displayChainingHT();
    else displayHT();
}

int main() {
    initializationHT();
    menu();
    return 0;
}
