#define IntType long long
#define LL long long
const IntType N = 1468000;
const int M = 4;
const char * KeyFile[M] = {"../data/Linear.key", "../data/Random.key",
    "../data/Grid.key", "../data/RevGrid.key"};
//Murmur data files
const char * MHashFile[M] = {"../data/Linear_Mur.hash",
    "../data/Random_Mur.hash", "../data/Grid_Mur.hash",
    "../data/RevGrid_Mur.hash"};
//Simple tabulation data files
const char * THashFile[M] = {"../data/Linear_Tab.hash",
    "../data/Random_Tab.hash", "../data/Grid_Tab.hash",
    "../data/RevGrid_Tab.hash"};
// Modulo data files
const char * ModHashFile[M] = {"../data/Linear_Mod.hash",
    "../data/Random_Mod.hash", "../data/Grid_Mod.hash",
    "../data/RevGrid_Mod.hash"};
// Multiply-Shift data files
const char * MSFHashFile[M] = {"../data/Linear_MSF.hash",
    "../data/Random_MSF.hash", "../data/Grid_MSF.hash",
    "../data/RevGrid_MSF.hash"};


struct  HashTable{
    IntType key;
    IntType hash;
};
