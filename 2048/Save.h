#ifndef SAVEFILE_2048
#define SAVEFILE_2048

#include <stdint.h>


typedef struct SaveHeader {
    uint16_t identifier;
    uint8_t userCount;
} SaveHeader;


typedef struct SaveData {
    char *username;
    uint32_t highestScore;
    // uint32_t maxMergesPerMove;
    uint8_t maxMergesPerMove;
} SaveData;

#endif
