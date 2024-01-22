#ifndef hihat_H_
#define hihat_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define hihat_NUM_CELLS 1650
#define hihat_SAMPLERATE 16384
 
CONSTTABLE_STORAGE(int8_t) hihat_DATA [] = {0, 0, 255, 255, 253, 251, 243, 232, 216, 208, 237, 16, 34, 26, 9, 240, 234, 51, 76, 6, 207, 208, 253, 82, 69, 211, 176, 204, 23, 99, 72, 198, 142, 231, 76, 73, 230, 185, 237, 40, 38, 2, 211, 201, 7, 65, 43, 232, 198, 226, 11, 28, 35, 5, 226, 219, 247, 35, 37, 241, 222, 237, 251, 23, 26, 10, 230, 216, 252, 30, 27, 249, 237, 254, 5, 252, 240, 13, 23, 233, 224, 254, 29, 27, 3, 244, 239, 234, 255, 27, 20, 250, 217, 236, 13, 34, 29, 243, 221, 227, 5, 31, 31, 246, 212, 237, 21, 41, 20, 228, 206, 227, 24, 71, 23, 194, 199, 18, 65, 25, 211, 201, 252, 45, 48, 245, 196, 238, 38, 30, 248, 218, 241, 29, 38, 243, 206, 236, 16, 39, 21, 239, 221, 238, 20, 36, 4, 219, 222, 5, 44, 31, 227, 216, 4, 30, 21, 238, 214, 233, 21, 59, 24, 201, 205, 17, 52, 17, 211, 219, 19, 40, 16, 234, 212, 238, 38, 44, 5, 220, 210, 7, 51, 14, 216, 228, 4, 32, 28, 242, 222, 244, 20, 11, 248, 247, 1, 6, 6, 0, 248, 239, 242, 20, 31, 251, 220, 245, 26, 25, 250, 220, 231, 16, 35, 24, 236, 217, 242, 22, 25, 255, 240, 243, 12, 9, 246, 244, 6, 3, 0, 255, 254, 12, 13, 241, 225, 240, 25, 44, 254, 215, 224, 18, 45, 16, 224, 207, 243, 46, 48, 252, 202, 204, 12, 63, 44, 235, 190, 215, 32, 62, 18, 213, 216, 255, 30, 26, 241, 227, 251, 28, 18, 229, 213, 3, 56, 38, 221, 192, 250, 31, 19, 12, 251, 238, 249, 5, 11, 246, 236, 9, 32, 2, 222, 230, 0, 38, 43, 243, 196, 225, 24, 46, 31, 224, 201, 241, 33, 41, 4, 228, 222, 255, 37, 24, 225, 219, 5, 24, 29, 0, 212, 235, 31, 32, 241, 222, 248, 27, 30, 236, 207, 252, 42, 45, 244, 198, 224, 19, 56, 25, 217, 195, 251, 52, 44, 246, 202, 219, 25, 60, 8, 217, 220, 246, 34, 41, 248, 217, 229, 9, 37, 25, 236, 205, 237, 42, 51, 250, 200, 224, 18, 43, 28, 237, 198, 236, 45, 49, 254, 207, 210, 5, 54, 41, 239, 197, 232, 35, 42, 255, 221, 226, 7, 32, 26, 244, 206, 236, 34, 41, 1, 218, 228, 6, 24, 23, 252, 230, 240, 10, 12, 252, 251, 3, 1, 247, 254, 12, 3, 245, 253, 2, 254, 0, 1, 3, 3, 249, 244, 0, 12, 11, 252, 235, 242, 5, 27, 15, 234, 233, 252, 13, 17, 4, 241, 238, 253, 16, 17, 247, 237, 255, 15, 2, 246, 245, 10, 17, 237, 239, 9, 24, 6, 219, 227, 26, 47, 3, 208, 225, 16, 46, 13, 212, 225, 14, 36, 13, 228, 230, 4, 21, 14, 246, 234, 248, 18, 19, 254, 230, 235, 15, 27, 4, 234, 236, 8, 21, 8, 240, 228, 255, 27, 19, 246, 228, 243, 7, 22, 17, 236, 222, 252, 32, 32, 250, 215, 235, 17, 25, 4, 228, 243, 17, 19, 254, 243, 245, 251, 10, 2, 0, 8, 253, 240, 245, 7, 22, 1, 233, 244, 4, 22, 18, 242, 225, 239, 19, 37, 8, 228, 225, 251, 26, 30, 249, 222, 240, 20, 28, 0, 219, 228, 23, 39, 7, 220, 226, 18, 43, 255, 214, 241, 20, 27, 255, 231, 239, 0, 15, 11, 1, 0, 247, 244, 254, 6, 14, 248, 230, 8, 24, 1, 243, 247, 1, 6, 4, 245, 247, 5, 12, 8, 247, 238, 250, 19, 9, 244, 251, 0, 255, 3, 6, 5, 243, 236, 3, 24, 4, 231, 248, 27, 18, 231, 228, 251, 19, 26, 6, 231, 225, 3, 32, 22, 244, 222, 244, 16, 21, 2, 238, 244, 5, 12, 3, 251, 243, 251, 9, 10, 0, 242, 243, 6, 9, 4, 9, 249, 236, 250, 12, 7, 253, 3, 252, 250, 1, 3, 255, 253, 253, 1, 8, 0, 249, 242, 250, 13, 16, 0, 237, 249, 7, 4, 0, 251, 3, 2, 246, 253, 12, 4, 244, 249, 7, 8, 253, 246, 247, 4, 14, 5, 243, 246, 254, 3, 14, 10, 245, 228, 245, 27, 30, 246, 224, 242, 17, 26, 3, 230, 238, 10, 20, 5, 241, 245, 3, 2, 255, 2, 2, 11, 4, 237, 234, 1, 25, 20, 241, 227, 248, 21, 30, 254, 225, 232, 11, 27, 13, 239, 228, 252, 21, 20, 246, 232, 251, 16, 14, 247, 243, 0, 4, 0, 2, 255, 249, 255, 7, 1, 245, 253, 10, 5, 246, 250, 0, 2, 6, 0, 248, 253, 6, 2, 251, 252, 3, 254, 254, 0, 2, 2, 254, 254, 254, 252, 254, 4, 6, 255, 252, 251, 250, 3, 7, 5, 250, 247, 1, 6, 1, 252, 251, 1, 11, 2, 245, 248, 6, 10, 252, 243, 253, 10, 5, 253, 251, 252, 2, 6, 0, 251, 253, 252, 2, 10, 3, 246, 246, 1, 10, 3, 248, 250, 4, 6, 255, 248, 253, 7, 3, 248, 249, 5, 9, 255, 247, 249, 6, 11, 254, 242, 250, 10, 11, 250, 241, 1, 9, 0, 254, 255, 254, 253, 255, 1, 3, 0, 251, 253, 5, 2, 252, 253, 0, 2, 255, 254, 0, 0, 0, 1, 254, 255, 3, 254, 248, 253, 8, 6, 249, 246, 0, 11, 5, 246, 245, 4, 10, 2, 249, 248, 0, 9, 3, 248, 249, 1, 8, 2, 250, 250, 0, 5, 3, 253, 251, 1, 2, 254, 255, 2, 3, 254, 250, 254, 3, 3, 254, 252, 0, 2, 0, 254, 255, 0, 1, 0, 253, 255, 1, 0, 255, 254, 255, 1, 0, 255, 0, 1, 255, 253, 255, 1, 1, 255, 254, 255, 1, 0, 254, 255, 0, 0, 255, 255, 0, 255, 254, 0, 1, 0, 254, 254, 1, 1, 255, 255, 255, 0, 0, 255, 254, 255, 0, 1, 0, 255, 253, 255, 2, 2, 254, 252, 255, 2, 2, 255, 252, 255, 2, 2, 254, 253, 255, 1, 2, 255, 253, 255, 0, 1, 0, 254, 255, 0, 1, 0, 255, 255, 0, 1, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 254, 255, 1, 1, 255, 254, 255, 0, 0, 255, 254, 255, 0, 1, 255, 254, 255, 0, 1, 255, 255, 255, 0, 0, 0, 255, 255, 0, 0, 255, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 255, 0, 0, 255, 255, 0, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#endif /* hihat_H_ */