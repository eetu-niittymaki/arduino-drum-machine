#ifndef snare_H_
#define snare_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define snare_NUM_CELLS 4096
#define snare_SAMPLERATE 32768
 
CONSTTABLE_STORAGE(int8_t) snare_DATA [] = {255, 255, 255, 0, 2, 7, 12, 18, 24, 29, 35, 40, 45, 49, 53, 56, 59, 62, 64, 66, 67, 68, 68, 69, 68, 68, 68, 67, 66, 63, 62, 59, 53, 42, 28, 15, 1, 240, 226, 215, 186, 164, 155, 149, 155, 164, 174, 177, 167, 157, 167, 179, 184, 198, 206, 183, 176, 187, 180, 181, 191, 192, 180, 174, 185, 188, 183, 193, 196, 194, 195, 198, 197, 194, 195, 192, 189, 188, 181, 185, 191, 189, 192, 193, 199, 209, 210, 201, 196, 195, 205, 212, 206, 215, 216, 195, 191, 201, 199, 206, 215, 212, 216, 230, 239, 228, 219, 221, 223, 231, 244, 250, 240, 231, 228, 232, 237, 229, 228, 239, 248, 248, 0, 15, 21, 15, 1, 250, 254, 2, 9, 28, 37, 26, 8, 15, 25, 26, 30, 23, 20, 28, 38, 34, 39, 46, 45, 46, 47, 50, 46, 48, 58, 41, 32, 43, 56, 66, 49, 40, 41, 60, 64, 43, 35, 43, 51, 54, 55, 51, 48, 46, 40, 43, 49, 41, 39, 35, 37, 41, 40, 40, 27, 28, 45, 40, 24, 26, 34, 39, 39, 30, 22, 17, 24, 32, 25, 14, 9, 11, 9, 8, 17, 18, 2, 7, 17, 16, 6, 254, 248, 1, 2, 248, 0, 244, 237, 245, 252, 246, 246, 255, 252, 240, 227, 228, 243, 1, 2, 244, 237, 245, 241, 232, 226, 226, 238, 246, 252, 5, 252, 232, 233, 240, 243, 242, 247, 0, 246, 238, 244, 1, 250, 244, 254, 9, 4, 5, 10, 4, 255, 255, 12, 15, 14, 3, 249, 252, 3, 17, 27, 22, 12, 9, 10, 20, 21, 15, 23, 30, 18, 15, 30, 35, 34, 32, 24, 21, 12, 16, 29, 23, 37, 45, 31, 26, 27, 31, 40, 41, 39, 14, 0, 29, 42, 31, 27, 50, 40, 7, 22, 43, 42, 45, 35, 20, 20, 14, 12, 11, 20, 23, 15, 10, 8, 21, 38, 30, 25, 31, 20, 7, 5, 2, 246, 250, 252, 5, 31, 34, 10, 247, 239, 240, 254, 5, 2, 6, 7, 1, 253, 241, 236, 237, 239, 247, 252, 7, 3, 249, 246, 242, 234, 241, 255, 246, 235, 235, 0, 0, 233, 238, 250, 240, 231, 235, 247, 255, 246, 242, 250, 251, 244, 242, 240, 243, 246, 236, 231, 239, 247, 250, 244, 244, 5, 22, 21, 253, 232, 235, 240, 242, 250, 7, 12, 255, 254, 10, 4, 2, 9, 14, 12, 253, 0, 12, 22, 23, 11, 15, 10, 2, 8, 4, 1, 16, 28, 21, 3, 5, 19, 39, 38, 12, 11, 16, 19, 26, 17, 4, 5, 9, 18, 34, 33, 22, 12, 9, 8, 13, 21, 16, 6, 12, 18, 23, 26, 24, 23, 9, 5, 20, 13, 252, 248, 3, 16, 13, 4, 9, 11, 7, 8, 11, 16, 253, 246, 6, 255, 253, 5, 251, 245, 242, 244, 248, 238, 234, 247, 10, 9, 245, 244, 1, 6, 254, 1, 9, 253, 239, 231, 238, 248, 250, 241, 234, 235, 233, 236, 250, 255, 0, 1, 250, 243, 232, 244, 254, 244, 240, 238, 235, 238, 236, 232, 245, 0, 9, 9, 250, 243, 241, 238, 230, 241, 253, 255, 10, 4, 247, 246, 8, 22, 15, 252, 248, 251, 249, 1, 250, 4, 8, 254, 2, 5, 4, 255, 11, 13, 11, 5, 245, 251, 12, 20, 13, 5, 2, 11, 8, 4, 9, 11, 12, 250, 0, 26, 29, 12, 0, 0, 255, 6, 13, 22, 22, 10, 254, 2, 9, 7, 15, 16, 7, 254, 2, 9, 253, 3, 17, 16, 13, 4, 0, 0, 254, 2, 1, 254, 12, 17, 253, 243, 239, 242, 9, 13, 254, 244, 255, 11, 0, 248, 238, 233, 250, 6, 4, 254, 246, 250, 0, 8, 11, 254, 248, 242, 235, 249, 3, 246, 246, 249, 244, 238, 245, 1, 1, 243, 235, 245, 244, 231, 241, 10, 15, 6, 251, 242, 236, 242, 248, 245, 243, 250, 1, 253, 252, 245, 235, 223, 229, 254, 6, 4, 251, 250, 1, 7, 15, 5, 241, 231, 238, 1, 9, 14, 20, 14, 249, 244, 240, 238, 253, 13, 18, 247, 221, 230, 1, 13, 21, 20, 3, 3, 17, 3, 247, 17, 23, 2, 251, 248, 254, 252, 248, 6, 252, 240, 250, 9, 17, 15, 1, 6, 8, 2, 5, 3, 5, 4, 1, 13, 18, 11, 5, 9, 13, 2, 2, 254, 250, 9, 9, 252, 241, 235, 249, 255, 0, 13, 12, 3, 255, 3, 255, 254, 252, 241, 245, 3, 1, 252, 253, 249, 246, 251, 254, 0, 255, 254, 248, 236, 235, 252, 17, 15, 3, 251, 241, 234, 238, 246, 251, 252, 1, 0, 239, 241, 253, 253, 250, 3, 5, 241, 228, 246, 10, 3, 237, 233, 243, 254, 3, 2, 5, 251, 246, 250, 4, 1, 232, 229, 238, 1, 12, 7, 6, 255, 251, 3, 7, 250, 233, 235, 3, 22, 26, 2, 236, 229, 239, 8, 11, 244, 244, 3, 7, 11, 7, 242, 235, 254, 11, 8, 4, 5, 255, 3, 5, 250, 249, 249, 244, 245, 248, 250, 255, 1, 254, 0, 6, 2, 255, 4, 1, 254, 2, 0, 0, 4, 6, 14, 23, 23, 254, 247, 7, 255, 244, 241, 247, 247, 252, 14, 9, 242, 246, 6, 8, 5, 5, 5, 2, 8, 11, 0, 240, 231, 238, 1, 1, 255, 251, 252, 1, 244, 239, 240, 252, 7, 14, 9, 253, 254, 247, 242, 239, 235, 236, 3, 17, 17, 20, 5, 235, 219, 234, 251, 6, 1, 250, 6, 11, 252, 243, 238, 240, 254, 253, 247, 241, 243, 251, 253, 3, 3, 247, 242, 246, 254, 255, 1, 5, 10, 10, 243, 249, 5, 244, 233, 235, 251, 14, 9, 245, 245, 255, 247, 242, 0, 17, 8, 247, 246, 249, 249, 1, 3, 254, 255, 241, 240, 3, 10, 252, 238, 244, 3, 4, 1, 3, 255, 1, 248, 237, 250, 11, 19, 7, 242, 245, 5, 1, 251, 4, 6, 8, 5, 1, 7, 8, 11, 9, 251, 239, 238, 247, 4, 0, 243, 247, 250, 251, 9, 17, 0, 244, 231, 237, 14, 27, 20, 253, 245, 244, 243, 249, 249, 7, 14, 245, 242, 1, 249, 243, 237, 251, 13, 18, 10, 6, 18, 15, 249, 235, 233, 231, 234, 251, 13, 8, 7, 242, 228, 251, 5, 7, 8, 254, 250, 245, 248, 254, 254, 4, 14, 4, 250, 248, 235, 239, 246, 254, 3, 2, 252, 248, 248, 243, 254, 8, 1, 245, 245, 254, 250, 248, 239, 236, 3, 21, 19, 12, 251, 232, 229, 252, 254, 238, 238, 242, 7, 22, 15, 245, 236, 253, 12, 11, 253, 248, 0, 2, 250, 1, 7, 7, 249, 237, 239, 245, 12, 13, 8, 0, 252, 241, 228, 240, 254, 254, 250, 244, 249, 8, 0, 255, 5, 5, 6, 5, 6, 250, 252, 8, 14, 20, 15, 251, 232, 239, 3, 9, 255, 246, 247, 255, 8, 8, 1, 246, 238, 245, 247, 246, 246, 7, 25, 13, 1, 242, 244, 253, 243, 249, 0, 245, 246, 14, 27, 252, 232, 240, 250, 1, 246, 247, 1, 9, 12, 7, 3, 248, 248, 0, 8, 254, 240, 244, 243, 246, 2, 16, 15, 3, 241, 241, 253, 250, 252, 252, 7, 253, 235, 240, 247, 5, 14, 5, 244, 243, 255, 254, 251, 6, 0, 244, 254, 0, 8, 15, 252, 233, 232, 235, 241, 8, 14, 4, 248, 236, 235, 1, 4, 245, 244, 249, 5, 9, 5, 254, 1, 16, 6, 250, 251, 255, 255, 243, 250, 10, 15, 16, 17, 6, 251, 238, 231, 246, 255, 1, 7, 252, 245, 1, 14, 12, 240, 226, 249, 15, 14, 255, 245, 241, 231, 241, 255, 10, 11, 250, 248, 244, 242, 7, 7, 9, 4, 252, 244, 245, 1, 4, 247, 240, 7, 17, 7, 249, 248, 255, 252, 252, 6, 8, 3, 3, 3, 3, 0, 244, 249, 0, 237, 245, 5, 9, 10, 248, 247, 5, 13, 253, 236, 239, 245, 6, 2, 251, 8, 5, 243, 252, 5, 10, 5, 244, 251, 245, 240, 246, 255, 7, 5, 247, 247, 1, 243, 246, 1, 2, 9, 7, 249, 245, 253, 0, 0, 0, 2, 7, 4, 7, 5, 245, 238, 235, 3, 16, 3, 247, 236, 231, 238, 9, 19, 14, 13, 253, 239, 246, 255, 5, 5, 250, 236, 247, 2, 6, 9, 9, 253, 239, 249, 251, 6, 14, 6, 252, 252, 247, 246, 253, 238, 253, 18, 7, 249, 236, 243, 255, 4, 9, 3, 252, 245, 251, 3, 1, 7, 8, 3, 247, 246, 5, 0, 251, 5, 4, 244, 238, 247, 8, 17, 9, 1, 244, 237, 241, 255, 5, 251, 238, 243, 11, 25, 18, 249, 243, 252, 254, 6, 10, 8, 254, 247, 242, 238, 232, 231, 246, 253, 7, 3, 255, 11, 16, 11, 8, 252, 237, 245, 1, 13, 10, 251, 241, 244, 252, 10, 13, 9, 6, 0, 250, 236, 231, 248, 2, 2, 3, 252, 254, 1, 250, 251, 0, 253, 1, 0, 8, 9, 249, 243, 251, 5, 3, 5, 253, 251, 9, 6, 246, 239, 240, 246, 248, 0, 13, 17, 14, 10, 5, 253, 250, 241, 231, 236, 252, 2, 0, 253, 249, 246, 253, 7, 15, 24, 2, 246, 247, 253, 254, 245, 253, 2, 5, 252, 244, 248, 3, 11, 0, 254, 252, 253, 253, 255, 244, 241, 248, 242, 248, 9, 10, 251, 250, 1, 10, 14, 10, 255, 244, 237, 231, 252, 21, 7, 249, 250, 255, 18, 8, 241, 243, 252, 3, 8, 1, 246, 250, 11, 17, 1, 233, 233, 0, 11, 7, 3, 251, 246, 4, 12, 2, 4, 15, 14, 247, 223, 211, 221, 5, 27, 33, 28, 4, 231, 245, 5, 252, 252, 250, 245, 251, 10, 7, 244, 240, 255, 10, 7, 5, 0, 251, 250, 254, 1, 253, 0, 251, 243, 246, 6, 15, 13, 16, 10, 247, 231, 233, 246, 253, 249, 3, 8, 8, 13, 2, 242, 241, 248, 252, 9, 16, 252, 240, 249, 4, 1, 1, 251, 245, 254, 7, 11, 5, 4, 4, 251, 248, 254, 8, 18, 251, 229, 247, 254, 249, 0, 7, 8, 253, 5, 10, 247, 231, 229, 252, 26, 33, 29, 14, 238, 228, 230, 245, 255, 2, 6, 7, 13, 255, 231, 232, 2, 16, 6, 251, 0, 10, 255, 247, 246, 244, 245, 0, 15, 4, 240, 238, 6, 14, 14, 26, 9, 247, 242, 235, 241, 249, 2, 255, 246, 255, 252, 249, 252, 251, 245, 246, 4, 15, 15, 7, 2, 248, 249, 0, 1, 10, 6, 245, 7, 22, 10, 254, 236, 233, 254, 18, 20, 245, 234, 249, 247, 244, 248, 254, 245, 252, 25, 35, 19, 255, 243, 231, 236, 2, 14, 5, 1, 6, 3, 1, 253, 243, 243, 245, 2, 2, 242, 255, 3, 9, 12, 8, 7, 252, 240, 232, 240, 255, 5, 3, 20, 28, 11, 245, 235, 247, 246, 4, 16, 248, 244, 9, 6, 6, 14, 249, 231, 242, 4, 6, 8, 251, 235, 253, 9, 13, 6, 251, 4, 8, 255, 252, 2, 250, 241, 244, 243, 251, 0, 4, 18, 14, 246, 227, 231, 255, 21, 17, 255, 243, 242, 246, 252, 10, 28, 26, 9, 249, 238, 250, 1, 246, 241, 249, 9, 9, 240, 229, 245, 249, 0, 15, 24, 11, 9, 9, 0, 248, 226, 237, 1, 16, 16, 1, 255, 253, 254, 253, 250, 244, 243, 2, 11, 6, 1, 5, 8, 5, 2, 252, 243, 240, 251, 1, 8, 15, 8, 249, 240, 254, 13, 10, 3, 2, 0, 9, 5, 243, 245, 239, 238, 249, 0, 7, 7, 15, 17, 0, 240, 245, 243, 241, 242, 3, 13, 3, 7, 2, 248, 249, 4, 11, 8, 246, 236, 238, 254, 10, 13, 19, 7, 248, 1, 13, 255, 254, 248, 230, 234, 249, 4, 18, 37, 31, 8, 247, 253, 253, 247, 249, 252, 3, 253, 248, 244, 250, 8, 7, 248, 240, 240, 255, 11, 12, 8, 4, 3, 2, 5, 3, 251, 241, 240, 244, 248, 251, 2, 7, 6, 2, 2, 14, 12, 249, 238, 0, 17, 11, 4, 244, 236, 248, 255, 249, 0, 5, 242, 243, 6, 10, 3, 10, 14, 8, 8, 251, 241, 247, 251, 254, 249, 252, 5, 5, 255, 1, 0, 2, 7, 5, 3, 252, 244, 248, 3, 0, 250, 240, 237, 247, 0, 10, 16, 13, 4, 0, 250, 251, 3, 7, 2, 4, 3, 240, 236, 248, 1, 254, 253, 1, 12, 18, 15, 3, 243, 239, 249, 6, 10, 10, 10, 2, 1, 1, 251, 3, 6, 254, 244, 242, 2, 12, 18, 11, 251, 255, 7, 4, 9, 254, 235, 234, 241, 248, 1, 2, 248, 252, 9, 9, 1, 248, 238, 253, 4, 253, 1, 5, 4, 247, 232, 231, 255, 21, 22, 15, 7, 8, 1, 245, 246, 250, 1, 2, 3, 10, 10, 253, 237, 237, 243, 251, 3, 9, 14, 16, 10, 10, 6, 248, 231, 239, 5, 4, 10, 14, 13, 252, 236, 231, 251, 14, 254, 2, 8, 11, 3, 253, 249, 244, 255, 254, 248, 253, 6, 9, 251, 255, 16, 0, 237, 243, 9, 14, 11, 4, 251, 253, 4, 0, 253, 9, 3, 2, 3, 253, 252, 252, 252, 255, 247, 241, 249, 4, 7, 7, 6, 0, 244, 239, 252, 6, 12, 8, 1, 10, 10, 2, 243, 239, 1, 254, 2, 9, 7, 0, 240, 239, 241, 253, 14, 24, 13, 253, 4, 8, 3, 251, 248, 254, 4, 1, 1, 3, 250, 243, 243, 253, 4, 1, 253, 249, 251, 255, 4, 7, 5, 255, 2, 11, 3, 253, 3, 250, 243, 243, 241, 4, 11, 1, 252, 252, 7, 6, 252, 252, 6, 11, 9, 8, 4, 6, 11, 254, 241, 238, 243, 255, 0, 9, 14, 2, 248, 10, 21, 3, 252, 242, 245, 4, 9, 247, 241, 3, 7, 9, 10, 252, 227, 242, 15, 13, 8, 3, 4, 2, 0, 0, 248, 246, 241, 240, 5, 4, 248, 8, 10, 9, 0, 242, 255, 8, 7, 2, 252, 243, 238, 249, 253, 5, 15, 19, 15, 8, 253, 242, 244, 251, 10, 12, 1, 1, 255, 2, 0, 253, 1, 4, 0, 250, 249, 247, 253, 254, 0, 4, 6, 7, 7, 7, 2, 3, 0, 254, 246, 237, 235, 243, 9, 15, 1, 3, 9, 8, 3, 255, 249, 242, 246, 5, 6, 1, 2, 3, 7, 6, 1, 251, 254, 8, 2, 240, 246, 7, 12, 1, 247, 247, 255, 7, 3, 4, 14, 14, 3, 2, 248, 247, 5, 4, 254, 244, 247, 255, 2, 9, 6, 252, 247, 246, 253, 5, 6, 254, 249, 251, 2, 3, 5, 3, 3, 5, 253, 254, 255, 5, 13, 7, 1, 5, 8, 253, 245, 250, 1, 255, 254, 2, 2, 0, 249, 247, 3, 9, 1, 240, 242, 255, 6, 6, 3, 5, 252, 245, 244, 254, 3, 2, 10, 7, 4, 253, 255, 10, 11, 4, 251, 245, 241, 251, 10, 16, 7, 2, 253, 254, 6, 4, 2, 250, 242, 239, 251, 14, 14, 6, 4, 5, 5, 253, 248, 250, 252, 4, 5, 250, 245, 1, 5, 248, 253, 10, 10, 4, 2, 255, 254, 0, 0, 2, 253, 251, 0, 1, 254, 255, 6, 3, 0, 251, 254, 255, 254, 0, 254, 4, 9, 6, 1, 253, 247, 247, 251, 254, 0, 4, 252, 250, 13, 21, 20, 6, 248, 243, 246, 243, 243, 1, 7, 5, 8, 8, 6, 4, 254, 250, 249, 254, 3, 249, 248, 0, 8, 8, 255, 250, 249, 2, 5, 4, 5, 2, 0, 0, 4, 4, 2, 252, 254, 251, 251, 3, 3, 252, 248, 255, 8, 8, 253, 252, 252, 254, 3, 254, 254, 8, 8, 1, 3, 3, 255, 251, 255, 250, 249, 4, 7, 3, 253, 2, 1, 249, 253, 2, 254, 255, 253, 254, 255, 254, 1, 253, 0, 3, 1, 0, 254, 1, 0, 0, 2, 6, 1, 250, 251, 251, 1, 7, 5, 1, 0, 0, 2, 252, 246, 252, 5, 8, 4, 1, 0, 1, 0, 1, 3, 1, 1, 0, 0, 252, 244, 247, 254, 5, 8, 7, 5, 3, 2, 3, 4, 4, 5, 254, 250, 254, 253, 1, 4, 253, 253, 0, 253, 255, 255, 249, 252, 3, 3, 1, 6, 3, 247, 244, 253, 255, 252, 3, 6, 8, 11, 6, 252, 252, 2, 1, 253, 253, 254, 254, 0, 5, 6, 254, 254, 1, 255, 255, 2, 3, 254, 252, 255, 0, 1, 0, 254, 252, 252, 255, 1, 5, 1, 255, 2, 1, 1, 3, 2, 2, 1, 1, 3, 0, 255, 1, 1, 2, 1, 0, 1, 0, 252, 255, 254, 250, 250, 254, 3, 5, 253, 251, 1, 4, 0, 252, 253, 254, 1, 2, 4, 5, 3, 3, 2, 255, 254, 254, 252, 253, 1, 6, 7, 2, 251, 252, 2, 1, 255, 2, 0, 255, 2, 3, 1, 253, 255, 0, 255, 254, 1, 2, 0, 0, 255, 250, 254, 4, 2, 3, 3, 1, 1, 1, 0, 1, 254, 252, 253, 255, 0, 255, 255, 254, 2, 4, 4, 2, 0, 0, 254, 252, 250, 248, 0, 5, 4, 2, 1, 2, 2, 1, 253, 254, 2, 2, 255, 254, 255, 1, 3, 3, 0, 2, 2, 1, 1, 2, 253, 253, 0, 3, 1, 254, 255, 0, 254, 252, 1, 2, 3, 3, 3, 1, 253, 253, 2, 5, 2, 0, 0, 255, 253, 251, 253, 2, 5, 3, 0, 0, 1, 2, 255, 254, 1, 1, 0, 253, 251, 254, 3, 4, 2, 1, 1, 255, 0, 2, 0, 255, 255, 1, 1, 0, 1, 2, 3, 2, 1, 255, 253, 254, 0, 1, 255, 1, 1, 1, 1, 0, 0, 253, 254, 1, 255, 254, 255, 1, 1, 1, 1, 0, 0, 0, 255, 255, 255, 0, 1, 2, 0, 1, 1, 0, 0, 1, 1, 255, 255, 1, 2, 2, 1, 0, 1, 1, 0, 255, 254, 253, 253, 0, 2, 2, 1, 0, 1, 1, 0, 1, 255, 252, 255, 2, 1, 0, 255, 0, 2, 0, 0, 0, 255, 255, 0, 1, 1, 1, 255, 0, 0, 0, 255, 255, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 255, 255, 255, 0, 0, 1, 1, 1, 1, 1, 0, 254, 252, 254, 2, 3, 2, 1, 1, 0, 0, 1, 1, 0, 255, 0, 0, 0, 1, 1, 1, 1, 0, 255, 0, 1, 1, 1, 1, 1, 0, 0, 255, 255, 0, 1, 1, 1, 0, 0, 1, 1, 0, 255, 0, 1, 1, 0, 1, 1, 255, 255, 1, 2, 1, 1, 0, 0, 1, 1, 0, 255, 1, 1, 0, 255, 0, 0, 1, 1, 1, 0, 255, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 255, 255, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 1, 1, 1, 1, 0, 0, 0, 255, 255, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 255, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#endif /* snare_H_ */