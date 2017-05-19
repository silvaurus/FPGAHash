//
//  Modulo.cpp
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

void Modulo(IntType a[], HashTable h[]){
    IntType x = 0;
    int w = 21 - 10;
    for (int i = 0; i < N; i++){
        x = a[i];
        h[i].key = x;
        // Get the least significant 21 bits
        IntType modulo = 0;
        modulo = x & 0x1fffff;
        // Get the top 10 bits
        h[i].hash = modulo >> w;
    }
}

void WriteHashTable(HashTable h[], const char * FilePath) {
    FILE * Fo = fopen(FilePath, "w");
    for (int i = 0; i < N; i++){
        fprintf(Fo, "%16llx %16llx\n", h[i].key, h[i].hash);
    }
}

int main(){
    for (int i = 0; i < M; i++){
        IntType* a = new IntType[N];
        int n = readKeys(a, KeyFile[i]);
        assert(n == N);
        
        struct timeval T;
        gettimeofday(&T, NULL);
        LL totalTime = (LL)T.tv_sec * (LL)1e6 + (LL)T.tv_usec;
        
        HashTable* H = new HashTable[N];
        Modulo(a, H);
        
        gettimeofday(&T, NULL);
        totalTime = (LL)T.tv_sec * (LL)1e6 + (LL)T.tv_usec - totalTime;
        printf("%s takes %d us\n", ModashFile[i], totalTime);
        
        WriteHashTable(H, ModHashFile[i]);
        
        delete[] a;
        delete[] H;
    }
}