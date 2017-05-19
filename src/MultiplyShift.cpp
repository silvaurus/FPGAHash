//
//  MultiplyShift.cpp
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

void MultiplyShift(IntType a[], HashTable h[]){
    IntType x = 0, mod_2 = 0;
    unsigned IntType multi_result = 0, mod_1 = 0;
    // Z is a 64-bit odd integer
    IntType Z = 0x1010101010101011;
    int shift_num = 64 - 21;
    int w = 21 - 10;
    for (int i = 0; i < N; i++){
        x = a[i];
        h[i].key = x;
        // First step: Multiply
        multi_result = (unsigned IntType) x * Z;
        // Second step: modulo 2^64 --> Just AND 0xffff_ffff_ffff_ffff
        mod_1 = (unsigned IntType) multi_result & 0xffffffffffffffff;
        // Third step: Divide by 2^(64-21)
        mod_2 = mod_1 >> shift_num;
        // Get the top 10 bits
        h[i].hash = mod_2 >> w;
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
        MultiplyShift(a, H);
        
        gettimeofday(&T, NULL);
        totalTime = (LL)T.tv_sec * (LL)1e6 + (LL)T.tv_usec - totalTime;
        printf("%s takes %d us\n", MSFHashFile[i], totalTime);
        
        WriteHashTable(H, MSFHashFile[i]);
        
        delete[] a;
        delete[] H;
    }
}