#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <stdint.h>
#include <sstream>
#include <memory>
#include <assert.h>
#include <stdexcept>

using namespace std;

const uint32_t TILE_BYTE_SIZE = 5;
const uint32_t TILE_SIZE = TILE_BYTE_SIZE * 8;

const uint32_t START_SIZE = 10000;

//! It seems that a simple boolean array is the fasted way (if tile size is small
// USE RECTMAP?
//#define RECT_MAP
// PACK TILE CONTENT?
//#define PACK_TILE_CONTENT

#endif // DEFINES_H
