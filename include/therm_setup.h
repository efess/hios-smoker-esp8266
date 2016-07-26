#ifndef THERM_SETUP_H
#define THERM_SETUP_H

typedef struct {
    uint8_t probId;
    uint16_t target;
    uint8_t enabled;
    char name[16];
} Therm;

typedef struct {
    
} ThermSetup;

#endif // THERM_SETUP_H

