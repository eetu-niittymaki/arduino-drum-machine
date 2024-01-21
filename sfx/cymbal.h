#ifndef cymbal_H_
#define cymbal_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define cymbal_NUM_CELLS 2048
#define cymbal_SAMPLERATE 16384
 
CONSTTABLE_STORAGE(int8_t) cymbal_DATA [] = {255, 0, 255, 0, 253, 247, 213, 174, 211, 52, 105, 80, 233, 162, 250, 73, 66, 237, 172, 236, 45, 69, 0, 196, 221, 26, 38, 241, 234, 8, 8, 17, 248, 222, 254, 20, 27, 239, 246, 220, 7, 36, 26, 222, 216, 253, 36, 41, 2, 203, 212, 26, 39, 3, 234, 250, 3, 2, 254, 238, 12, 27, 249, 234, 236, 8, 20, 18, 4, 225, 249, 247, 5, 18, 4, 248, 250, 1, 5, 13, 233, 227, 21, 13, 39, 239, 204, 243, 40, 24, 230, 247, 5, 8, 244, 5, 1, 253, 7, 245, 232, 37, 4, 231, 14, 243, 253, 32, 237, 236, 248, 25, 10, 242, 248, 247, 16, 252, 9, 15, 219, 237, 24, 15, 12, 221, 251, 15, 14, 254, 235, 7, 15, 248, 240, 253, 2, 8, 246, 0, 17, 9, 238, 244, 250, 17, 10, 0, 238, 236, 16, 254, 5, 36, 242, 209, 253, 41, 9, 239, 232, 252, 20, 19, 246, 242, 254, 7, 1, 237, 1, 23, 4, 254, 224, 232, 35, 36, 5, 241, 208, 242, 31, 32, 249, 238, 248, 2, 255, 7, 9, 2, 223, 4, 11, 18, 251, 235, 247, 11, 31, 234, 231, 0, 20, 255, 11, 242, 255, 252, 253, 4, 7, 252, 253, 252, 248, 1, 1, 18, 255, 232, 250, 21, 1, 248, 232, 17, 20, 4, 237, 248, 11, 250, 238, 2, 16, 7, 254, 1, 0, 236, 246, 15, 25, 252, 215, 253, 30, 1, 252, 0, 248, 245, 9, 21, 251, 239, 246, 4, 10, 248, 11, 255, 250, 1, 248, 0, 9, 23, 237, 214, 9, 40, 14, 229, 245, 252, 237, 28, 27, 233, 228, 16, 28, 237, 0, 246, 251, 3, 12, 4, 232, 6, 25, 246, 222, 7, 22, 20, 227, 246, 1, 2, 21, 248, 237, 6, 20, 252, 222, 9, 16, 251, 0, 5, 253, 246, 243, 9, 28, 247, 249, 240, 254, 12, 252, 11, 245, 248, 0, 28, 247, 230, 244, 31, 17, 255, 232, 227, 0, 15, 25, 11, 247, 238, 247, 4, 4, 8, 247, 239, 245, 37, 10, 7, 231, 230, 250, 32, 24, 235, 246, 248, 4, 252, 23, 240, 252, 1, 0, 16, 255, 243, 239, 8, 22, 247, 245, 250, 0, 27, 6, 234, 219, 11, 25, 26, 245, 219, 253, 25, 7, 243, 255, 6, 9, 214, 244, 54, 27, 226, 214, 3, 24, 22, 245, 236, 4, 248, 3, 29, 253, 236, 225, 9, 36, 11, 236, 235, 233, 43, 41, 213, 217, 1, 19, 34, 11, 222, 237, 7, 255, 28, 247, 229, 9, 14, 4, 239, 250, 10, 10, 4, 241, 244, 1, 5, 13, 245, 252, 1, 15, 242, 246, 10, 243, 5, 19, 8, 241, 221, 251, 23, 49, 3, 202, 195, 29, 91, 9, 193, 224, 252, 49, 38, 227, 198, 10, 35, 6, 245, 255, 2, 244, 9, 2, 236, 249, 6, 27, 19, 232, 217, 252, 30, 24, 246, 227, 243, 29, 17, 246, 248, 234, 255, 20, 24, 236, 225, 11, 2, 20, 5, 234, 2, 253, 3, 245, 248, 3, 38, 7, 225, 225, 254, 44, 22, 224, 223, 245, 23, 69, 245, 198, 220, 1, 74, 41, 209, 179, 1, 67, 39, 233, 198, 242, 34, 48, 246, 215, 221, 27, 39, 13, 225, 222, 244, 28, 43, 240, 247, 229, 248, 9, 21, 255, 247, 8, 246, 240, 10, 4, 9, 251, 1, 234, 4, 23, 254, 241, 241, 25, 0, 238, 249, 12, 16, 6, 239, 245, 231, 22, 44, 247, 219, 229, 6, 64, 4, 226, 225, 244, 29, 26, 251, 249, 236, 231, 18, 43, 6, 238, 208, 250, 28, 20, 15, 234, 227, 5, 27, 9, 238, 238, 5, 3, 2, 15, 1, 240, 236, 6, 9, 21, 12, 220, 228, 12, 38, 9, 241, 231, 240, 23, 37, 248, 216, 230, 31, 39, 253, 228, 224, 5, 48, 6, 231, 235, 237, 28, 30, 255, 232, 233, 248, 30, 24, 243, 246, 223, 3, 41, 28, 210, 223, 10, 31, 16, 250, 211, 4, 45, 253, 232, 235, 251, 31, 21, 234, 228, 19, 35, 230, 228, 252, 15, 24, 7, 226, 245, 18, 4, 234, 252, 41, 15, 220, 219, 240, 50, 39, 237, 209, 0, 51, 239, 226, 241, 8, 54, 16, 201, 222, 11, 48, 16, 235, 220, 249, 18, 10, 26, 230, 222, 4, 50, 14, 214, 212, 11, 59, 30, 227, 187, 239, 44, 69, 249, 179, 228, 49, 46, 3, 212, 219, 0, 33, 27, 2, 250, 203, 253, 49, 14, 247, 218, 230, 24, 55, 5, 203, 219, 24, 48, 251, 227, 241, 21, 9, 4, 236, 244, 4, 5, 5, 2, 10, 241, 245, 1, 8, 2, 0, 243, 250, 13, 23, 252, 207, 245, 40, 28, 244, 217, 248, 21, 11, 252, 10, 245, 215, 253, 64, 16, 218, 221, 246, 40, 14, 5, 239, 227, 248, 20, 60, 248, 193, 223, 22, 48, 33, 220, 208, 254, 24, 31, 249, 230, 250, 254, 16, 12, 10, 229, 224, 16, 31, 3, 241, 255, 241, 250, 0, 20, 27, 247, 213, 232, 21, 49, 7, 216, 243, 10, 16, 244, 228, 12, 24, 15, 240, 229, 0, 22, 12, 234, 234, 16, 28, 5, 234, 215, 248, 70, 25, 194, 207, 36, 54, 7, 223, 211, 0, 33, 33, 252, 217, 252, 247, 5, 33, 21, 218, 224, 10, 15, 22, 6, 220, 247, 9, 9, 16, 247, 236, 246, 27, 11, 237, 251, 6, 246, 255, 253, 8, 9, 251, 252, 235, 9, 20, 253, 241, 4, 12, 233, 240, 6, 48, 8, 205, 238, 19, 25, 253, 242, 245, 6, 6, 2, 252, 255, 254, 247, 1, 12, 14, 226, 235, 27, 35, 233, 228, 7, 8, 5, 0, 4, 252, 235, 255, 9, 22, 1, 234, 250, 1, 7, 251, 13, 7, 234, 235, 8, 20, 28, 237, 230, 247, 19, 11, 246, 235, 8, 16, 4, 1, 242, 248, 249, 253, 26, 20, 239, 210, 4, 31, 24, 249, 220, 250, 9, 20, 3, 231, 252, 4, 20, 4, 247, 237, 237, 19, 35, 0, 211, 237, 27, 43, 247, 219, 236, 17, 35, 8, 220, 230, 4, 44, 23, 220, 216, 10, 36, 0, 246, 248, 246, 246, 21, 35, 238, 214, 250, 28, 15, 246, 246, 5, 3, 248, 254, 6, 255, 254, 11, 243, 239, 15, 12, 246, 250, 9, 21, 236, 221, 251, 30, 49, 237, 198, 254, 27, 16, 0, 254, 243, 233, 10, 35, 3, 220, 229, 25, 46, 240, 230, 229, 23, 26, 254, 239, 234, 18, 255, 14, 6, 243, 225, 245, 38, 42, 223, 198, 10, 56, 32, 214, 193, 8, 59, 26, 220, 214, 253, 49, 25, 227, 219, 7, 20, 17, 236, 237, 253, 27, 27, 229, 210, 8, 33, 11, 3, 244, 239, 231, 10, 37, 18, 235, 223, 235, 14, 44, 24, 240, 189, 231, 63, 61, 227, 184, 3, 35, 22, 1, 227, 239, 12, 8, 9, 246, 0, 2, 254, 244, 242, 12, 30, 10, 226, 205, 27, 46, 0, 243, 215, 249, 36, 23, 0, 216, 232, 20, 32, 13, 230, 225, 3, 31, 14, 234, 228, 255, 43, 16, 225, 219, 8, 28, 6, 249, 243, 252, 13, 10, 249, 244, 234, 23, 22, 242, 244, 245, 10, 17, 245, 249, 13, 254, 236, 240, 20, 36, 241, 229, 254, 12, 3, 251, 1, 253, 5, 254, 255, 243, 254, 5, 20, 2, 231, 240, 3, 30, 18, 234, 212, 253, 50, 35, 211, 209, 15, 38, 14, 242, 229, 244, 11, 8, 19, 249, 241, 250, 5, 5, 248, 4, 252, 253, 3, 4, 0, 10, 242, 231, 25, 3, 253, 7, 249, 245, 236, 16, 35, 253, 233, 238, 249, 13, 33, 2, 227, 231, 2, 42, 13, 209, 242, 19, 21, 3, 244, 238, 234, 38, 23, 233, 228, 247, 30, 29, 234, 222, 0, 26, 20, 241, 223, 251, 28, 18, 251, 225, 255, 12, 8, 249, 4, 254, 254, 2, 248, 253, 250, 14, 2, 254, 253, 247, 248, 6, 17, 11, 242, 216, 3, 43, 22, 222, 214, 14, 40, 6, 236, 233, 240, 31, 31, 240, 221, 251, 26, 20, 232, 227, 8, 39, 6, 232, 225, 251, 39, 19, 234, 222, 0, 46, 24, 224, 198, 248, 56, 57, 225, 199, 231, 30, 52, 10, 224, 207, 3, 43, 23, 238, 222, 246, 38, 18, 233, 229, 254, 28, 18, 242, 228, 3, 10, 5, 1, 254, 231, 12, 30, 252, 229, 230, 20, 28, 255, 244, 250, 248, 252, 254, 8, 14, 3, 242, 240, 252, 18, 18, 3, 229, 221, 14, 47, 12, 233, 206, 248, 56, 23, 236, 224, 233, 20, 41, 25, 221, 194, 245, 73, 57, 210, 178, 243, 77, 55, 221, 189, 238, 56, 53, 217, 192, 4, 59, 39, 221, 194, 240, 59, 51, 235, 198, 232, 33, 46, 0, 219, 228, 16, 33, 12, 220, 227, 13, 35, 9, 242, 232, 229, 26, 42, 3, 216, 228, 5, 44, 13, 222, 240, 6, 26, 5, 236, 224, 16, 30, 8, 238, 234, 3, 10, 6, 9, 242, 232, 7, 32, 3, 231, 243, 1, 21, 12, 238, 238, 0, 24, 0, 242, 249, 12, 5, 246, 247, 13, 7, 242, 242, 4, 18, 9, 243, 246, 252, 12, 11, 228, 235, 29, 32, 244, 217, 250, 34, 11, 239, 244, 245, 13, 18, 248, 236, 5, 14, 4, 232, 255, 4, 10, 254, 251, 251, 0, 15, 255, 224, 253, 30, 17, 233, 227, 9, 34, 9, 224, 239, 4, 26, 6, 234, 236, 15, 30, 247, 233, 238, 19, 22, 2, 239, 240, 241, 24, 28, 250, 227, 238, 15, 12, 21, 245, 219, 255, 20, 17, 3, 233, 243, 2, 20, 16, 247, 226, 239, 12, 38, 14, 227, 216, 13, 28, 10, 250, 223, 240, 37, 34, 231, 215, 249, 27, 41, 252, 204, 232, 28, 43, 255, 219, 241, 2, 13, 22, 10, 220, 229, 23, 43, 241, 219, 247, 12, 26, 15, 228, 239, 3, 17, 7, 247, 247, 250, 9, 8, 251, 239, 0, 12, 18, 1, 223, 241, 15, 36, 253, 227, 242, 9, 21, 6, 231, 239, 18, 14, 2, 250, 236, 252, 17, 11, 246, 228, 8, 30, 6, 237, 234, 251, 32, 11, 232, 239, 10, 18, 3, 236, 244, 3, 33, 3, 224, 236, 22, 30, 245, 228, 253, 7, 18, 13, 237, 219, 255, 42, 26, 239, 199, 254, 41, 29, 233, 221, 6, 22, 10, 243, 239, 249, 9, 32, 1, 215, 233, 37, 27, 244, 234, 233, 13, 30, 20, 228, 215, 3, 29, 29, 250, 218, 244, 10, 21, 1, 250, 1, 244, 247, 5, 15, 251, 1, 241, 0, 12, 14, 241, 234, 6, 14, 5, 250, 254, 249, 250, 12, 16, 243, 227, 3, 20, 6, 1, 252, 231, 242, 32, 16, 251, 239, 244, 249, 14, 25, 253, 233, 241, 11, 19, 5, 237, 225, 20, 32, 249, 231, 251, 10, 9, 6, 245, 247, 255, 252, 22, 4, 229, 245, 19, 22, 241, 237, 3, 9, 1, 255, 251, 246, 6, 23, 251, 232, 246, 10, 17, 4, 255, 239, 245, 4, 29, 3, 217, 251, 31, 5, 228, 4, 24, 242, 232, 17, 17, 254, 237, 242, 13, 18, 254, 238, 0, 2, 12, 3, 233, 244, 18, 26, 248, 229, 252, 7, 12, 254, 1, 250, 240, 9, 21, 249, 240, 245, 9, 19, 254, 237, 251, 17, 255, 249, 0, 0, 1, 0, 238, 7, 10, 15, 247, 243, 245, 254, 13, 20, 250, 226, 4, 11, 7, 8, 232, 245, 14, 21, 249, 238, 242, 9, 18, 15, 241, 220, 5, 20, 17, 250, 237, 251, 2, 4, 7, 5, 254, 230, 250, 13, 26, 249, 248, 243, 238, 24, 25, 237, 241, 11, 254, 247, 0, 7, 18, 241, 221, 4, 39, 20, 225, 224, 253, 27, 30, 249, 219, 237, 16, 29, 17, 226, 220, 14, 30, 8, 241, 243, 250, 7, 2, 17, 252, 231, 248, 14, 24, 255, 225, 240, 16, 30, 2, 226, 235, 16, 23, 5, 246, 242, 238, 10, 27, 4, 240, 228, 2, 24, 19, 249, 223, 246, 17, 31, 7, 214, 225};

#endif /* cymbal_H_ */