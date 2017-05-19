//
//  Tabulation.cpp
//  The platform we are running is x86_64.
//
//  Created by Yi He on 5/18/17.
//
//
#include <cstdio>
#include <cstdlib>
#include <string>
#include <assert.h>
#include <sys/time.h>

#include "Common.h"

using namespace std;

int readKeys(IntType a[], const char * FilePath) {
    FILE * Fi = fopen(FilePath, "r");
    IntType x;
    int n = 0;
    while (EOF != fscanf(Fi, "%llx", &x)) {
        a[n++] = x;
    }
    return n;
}

// This function generates a 21-bit random value
inline uint32_t rand21(){
    uint32_t rand21 = (((uint32_t)rand() << 13) & 0x1fffff) |
                       (((uint32_t)rand() << 2) & 0x1fffff) |
                       (((uint32_t)rand() & 3) & 0x1fffff);
    return rand21;
}


// This function precomputes 8 random tables
// Each tables takes a uint8_t as key, returns a uint32_t 21 bit random number as hash
void makeRandTables(uint32_t** T0, uint32_t** T1, uint32_t** T2, uint32_t** T3, uint32_t** T4, uint32_t** T5, uint32_t** T6, uint32_t** T7){
    *T0 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T1 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T2 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T3 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T4 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T5 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T6 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    *T7 = (uint32_t *) malloc(256 * 4); // Table of 2^8 pairs of 32-bit integer
    // Giving random values to the tables
    int i = 0;
    for(i=0; i < 256;i++){
        (*T0)[i] = rand21();
        (*T1)[i] = rand21();
        (*T2)[i] = rand21();
        (*T3)[i] = rand21();
        (*T4)[i] = rand21();
        (*T5)[i] = rand21();
        (*T6)[i] = rand21();
        (*T7)[i] = rand21();
    }
}



// This function free 8 random tables
void freeRandTables(uint32_t** T0, uint32_t** T1, uint32_t** T2, uint32_t** T3, uint32_t** T4, uint32_t** T5, uint32_t** T6, uint32_t** T7){
    free(*T0);
    free(*T1);
    free(*T2);
    free(*T3);
    free(*T4);
    free(*T5);
    free(*T6);
    free(*T7);
}

// Tabulaiton function
// T0 - T7 are precomputed tables
void Tabulation(IntType a[], HashTable h[], uint32_t T0[], uint32_t T1[], uint32_t T2[], uint32_t T3[], uint32_t T4[],
                uint32_t T5[], uint32_t T6[], uint32_t T7[]){
    IntType x = 0, hash_val = 0;
    int w = 21 - 10;
    uint8_t x_0 = 0, x_1 = 0, x_2 = 0, x_3 = 0, x_4 = 0, x_5 = 0, x_6 = 0, x_7 = 0;
    for (int i = 0; i < N; i++){
        // Give key to value x
        x = a[i];
        h[i].key = x;
        // Get each 8-bit element of x.
        x_0 = x & 0xff;
        x_1 = x >> 8 & 0xff;
        x_2 = x >> 16 & 0xff;
        x_3 = x >> 24 & 0xff;
        x_4 = x >> 32 & 0xff;
        x_5 = x >> 40 & 0xff;
        x_6 = x >> 48 & 0xff;
        x_7 = x >> 56 & 0xff;
        // Search pre-computed tables
        hash_val = T0[x_0] ^ T1[x_1] ^ T2[x_2] ^ T3[x_3] ^ T4[x_4] ^ T5[x_5] ^ T6[x_6] ^ T7[x_7];
        // Get the top-10 bits
        h[i].hash = hash_val >> w;
    }
}

void WriteHashTable(HashTable h[], const char * FilePath) {
    FILE * Fo = fopen(FilePath, "w");
    for (int i = 0; i < N; i++){
        fprintf(Fo, "%16llx %16llx\n", h[i].key, h[i].hash);
    }
}

int main(){
    printf("Generating random table\n");
    // Generating precomputed random table
    uint32_t* T0;
    uint32_t* T1;
    uint32_t* T2;
    uint32_t* T3;
    uint32_t* T4;
    uint32_t* T5;
    uint32_t* T6;
    uint32_t* T7;
    
    makeRandTables((uint32_t**)&T0, (uint32_t**)&T1, (uint32_t**)&T2, (uint32_t**)&T3, (uint32_t**)&T4, (uint32_t**)&T5, (uint32_t**)&T6, (uint32_t**)&T7);
    

    for (int i = 0; i < M; i++){
        IntType* a = new IntType[N];
        int n = readKeys(a, KeyFile[i]);
        assert(n == N);
        
        struct timeval T;
        gettimeofday(&T, NULL);
        LL totalTime = (LL)T.tv_sec * (LL)1e6 + (LL)T.tv_usec;
        
        //printf("Values are %d.", T0[15]);
        //printf("Finish Generating random table\n");

        HashTable* H = new HashTable[N];
        Tabulation(a, H, T0, T1, T2, T3, T4, T5, T6, T7);
        
        //printf("Finish running tabulation\n");

        gettimeofday(&T, NULL);
        totalTime = (LL)T.tv_sec * (LL)1e6 + (LL)T.tv_usec - totalTime;
        printf("%s takes %d us\n", THashFile[i], totalTime);
        
        WriteHashTable(H, THashFile[i]);
        
        delete[] a;
        delete[] H;
        }
    
    freeRandTables((uint32_t**)&T0, (uint32_t**)&T1, (uint32_t**)&T2, (uint32_t**)&T3, (uint32_t**)&T4, (uint32_t**)&T5, (uint32_t**)&T6, (uint32_t**)&T7);

}