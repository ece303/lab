#ifndef SAFE_H
#define SAFE_H

#include <Arduino.h>

class Safe {
public:
    Safe();

    int getPassword();

private:
    int password;
};

#endif
