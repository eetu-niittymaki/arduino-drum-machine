#ifndef hihat_H_
#define hihat_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define hihat_NUM_CELLS 1650
#define hihat_SAMPLERATE 32768
 
CONSTTABLE_STORAGE(int8_t) hihat_DATA [] = {0, 0, -1, -1, -3, -5, -13, -24, -40, -48, -19, 16, 34, 26, 9, -16, -22, 51, 76, 6, -49, -48, -3, 82, 69, -45, -80, -52, 23, 99, 72, -58, -114, -25, 76, 73, -26, -71, -19, 40, 38, 2, -45, -55, 7, 65, 43, -24, -58, -30, 11, 28, 35, 5, -30, -37, -9, 35, 37, -15, -34, -19, -5, 23, 26, 10, -26, -40, -4, 30, 27, -7, -19, -2, 5, -4, -16, 13, 23, -23, -32, -2, 29, 27, 3, -12, -17, -22, -1, 27, 20, -6, -39, -20, 13, 34, 29, -13, -35, -29, 5, 31, 31, -10, -44, -19, 21, 41, 20, -28, -50, -29, 24, 71, 23, -62, -57, 18, 65, 25, -45, -55, -4, 45, 48, -11, -60, -18, 38, 30, -8, -38, -15, 29, 38, -13, -50, -20, 16, 39, 21, -17, -35, -18, 20, 36, 4, -37, -34, 5, 44, 31, -29, -40, 4, 30, 21, -18, -42, -23, 21, 59, 24, -55, -51, 17, 52, 17, -45, -37, 19, 40, 16, -22, -44, -18, 38, 44, 5, -36, -46, 7, 51, 14, -40, -28, 4, 32, 28, -14, -34, -12, 20, 11, -8, -9, 1, 6, 6, 0, -8, -17, -14, 20, 31, -5, -36, -11, 26, 25, -6, -36, -25, 16, 35, 24, -20, -39, -14, 22, 25, -1, -16, -13, 12, 9, -10, -12, 6, 3, 0, -1, -2, 12, 13, -15, -31, -16, 25, 44, -2, -41, -32, 18, 45, 16, -32, -49, -13, 46, 48, -4, -54, -52, 12, 63, 44, -21, -66, -41, 32, 62, 18, -43, -40, -1, 30, 26, -15, -29, -5, 28, 18, -27, -43, 3, 56, 38, -35, -64, -6, 31, 19, 12, -5, -18, -7, 5, 11, -10, -20, 9, 32, 2, -34, -26, 0, 38, 43, -13, -60, -31, 24, 46, 31, -32, -55, -15, 33, 41, 4, -28, -34, -1, 37, 24, -31, -37, 5, 24, 29, 0, -44, -21, 31, 32, -15, -34, -8, 27, 30, -20, -49, -4, 42, 45, -12, -58, -32, 19, 56, 25, -39, -61, -5, 52, 44, -10, -54, -37, 25, 60, 8, -39, -36, -10, 34, 41, -8, -39, -27, 9, 37, 25, -20, -51, -19, 42, 51, -6, -56, -32, 18, 43, 28, -19, -58, -20, 45, 49, -2, -49, -46, 5, 54, 41, -17, -59, -24, 35, 42, -1, -35, -30, 7, 32, 26, -12, -50, -20, 34, 41, 1, -38, -28, 6, 24, 23, -4, -26, -16, 10, 12, -4, -5, 3, 1, -9, -2, 12, 3, -11, -3, 2, -2, 0, 1, 3, 3, -7, -12, 0, 12, 11, -4, -21, -14, 5, 27, 15, -22, -23, -4, 13, 17, 4, -15, -18, -3, 16, 17, -9, -19, -1, 15, 2, -10, -11, 10, 17, -19, -17, 9, 24, 6, -37, -29, 26, 47, 3, -48, -31, 16, 46, 13, -44, -31, 14, 36, 13, -28, -26, 4, 21, 14, -10, -22, -8, 18, 19, -2, -26, -21, 15, 27, 4, -22, -20, 8, 21, 8, -16, -28, -1, 27, 19, -10, -28, -13, 7, 22, 17, -20, -34, -4, 32, 32, -6, -41, -21, 17, 25, 4, -28, -13, 17, 19, -2, -13, -11, -5, 10, 2, 0, 8, -3, -16, -11, 7, 22, 1, -23, -12, 4, 22, 18, -14, -31, -17, 19, 37, 8, -28, -31, -5, 26, 30, -7, -34, -16, 20, 28, 0, -37, -28, 23, 39, 7, -36, -30, 18, 43, -1, -42, -15, 20, 27, -1, -25, -17, 0, 15, 11, 1, 0, -9, -12, -2, 6, 14, -8, -26, 8, 24, 1, -13, -9, 1, 6, 4, -11, -9, 5, 12, 8, -9, -18, -6, 19, 9, -12, -5, 0, -1, 3, 6, 5, -13, -20, 3, 24, 4, -25, -8, 27, 18, -25, -28, -5, 19, 26, 6, -25, -31, 3, 32, 22, -12, -34, -12, 16, 21, 2, -18, -12, 5, 12, 3, -5, -13, -5, 9, 10, 0, -14, -13, 6, 9, 4, 9, -7, -20, -6, 12, 7, -3, 3, -4, -6, 1, 3, -1, -3, -3, 1, 8, 0, -7, -14, -6, 13, 16, 0, -19, -7, 7, 4, 0, -5, 3, 2, -10, -3, 12, 4, -12, -7, 7, 8, -3, -10, -9, 4, 14, 5, -13, -10, -2, 3, 14, 10, -11, -28, -11, 27, 30, -10, -32, -14, 17, 26, 3, -26, -18, 10, 20, 5, -15, -11, 3, 2, -1, 2, 2, 11, 4, -19, -22, 1, 25, 20, -15, -29, -8, 21, 30, -2, -31, -24, 11, 27, 13, -17, -28, -4, 21, 20, -10, -24, -5, 16, 14, -9, -13, 0, 4, 0, 2, -1, -7, -1, 7, 1, -11, -3, 10, 5, -10, -6, 0, 2, 6, 0, -8, -3, 6, 2, -5, -4, 3, -2, -2, 0, 2, 2, -2, -2, -2, -4, -2, 4, 6, -1, -4, -5, -6, 3, 7, 5, -6, -9, 1, 6, 1, -4, -5, 1, 11, 2, -11, -8, 6, 10, -4, -13, -3, 10, 5, -3, -5, -4, 2, 6, 0, -5, -3, -4, 2, 10, 3, -10, -10, 1, 10, 3, -8, -6, 4, 6, -1, -8, -3, 7, 3, -8, -7, 5, 9, -1, -9, -7, 6, 11, -2, -14, -6, 10, 11, -6, -15, 1, 9, 0, -2, -1, -2, -3, -1, 1, 3, 0, -5, -3, 5, 2, -4, -3, 0, 2, -1, -2, 0, 0, 0, 1, -2, -1, 3, -2, -8, -3, 8, 6, -7, -10, 0, 11, 5, -10, -11, 4, 10, 2, -7, -8, 0, 9, 3, -8, -7, 1, 8, 2, -6, -6, 0, 5, 3, -3, -5, 1, 2, -2, -1, 2, 3, -2, -6, -2, 3, 3, -2, -4, 0, 2, 0, -2, -1, 0, 1, 0, -3, -1, 1, 0, -1, -2, -1, 1, 0, -1, 0, 1, -1, -3, -1, 1, 1, -1, -2, -1, 1, 0, -2, -1, 0, 0, -1, -1, 0, -1, -2, 0, 1, 0, -2, -2, 1, 1, -1, -1, -1, 0, 0, -1, -2, -1, 0, 1, 0, -1, -3, -1, 2, 2, -2, -4, -1, 2, 2, -1, -4, -1, 2, 2, -2, -3, -1, 1, 2, -1, -3, -1, 0, 1, 0, -2, -1, 0, 1, 0, -1, -1, 0, 1, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -2, -1, 1, 1, -1, -2, -1, 0, 0, -1, -2, -1, 0, 1, -1, -2, -1, 0, 1, -1, -1, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#endif /* hihat_H_ */