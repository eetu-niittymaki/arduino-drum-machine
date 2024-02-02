#ifndef cowbell_H_
#define cowbell_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define cowbell_NUM_CELLS 4096
#define cowbell_SAMPLERATE 32768
 
CONSTTABLE_STORAGE(int8_t) cowbell_DATA [] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 255, 255, 255, 255, 254, 254, 253, 251, 250, 248, 245, 242, 239, 235, 231, 226, 222, 216, 211, 206, 201, 197, 192, 189, 186, 184, 182, 182, 182, 184, 187, 190, 195, 200, 206, 213, 219, 226, 234, 241, 247, 253, 3, 8, 12, 15, 18, 19, 19, 18, 16, 13, 10, 5, 0, 251, 246, 240, 234, 229, 224, 219, 216, 213, 211, 210, 209, 210, 212, 215, 219, 224, 229, 235, 241, 248, 254, 4, 10, 15, 20, 24, 27, 30, 31, 32, 32, 31, 29, 27, 24, 21, 17, 13, 9, 4, 0, 251, 246, 242, 237, 233, 228, 224, 220, 216, 212, 209, 205, 202, 200, 198, 196, 195, 194, 194, 195, 196, 197, 199, 202, 205, 209, 214, 219, 224, 230, 237, 244, 251, 2, 9, 16, 23, 30, 36, 42, 47, 52, 56, 60, 63, 66, 69, 70, 72, 73, 73, 74, 73, 73, 72, 71, 70, 68, 66, 64, 61, 58, 55, 50, 45, 40, 33, 26, 19, 11, 2, 249, 240, 230, 221, 212, 203, 195, 187, 180, 174, 170, 166, 164, 163, 163, 164, 167, 172, 177, 183, 191, 199, 208, 217, 227, 237, 247, 1, 11, 20, 29, 38, 45, 52, 59, 64, 68, 72, 74, 76, 78, 78, 78, 77, 76, 74, 72, 69, 66, 62, 58, 54, 49, 43, 38, 32, 26, 20, 14, 9, 3, 254, 249, 245, 241, 238, 236, 234, 234, 234, 235, 237, 240, 243, 246, 251, 255, 3, 7, 11, 14, 17, 19, 20, 21, 22, 21, 21, 19, 18, 16, 13, 11, 8, 5, 2, 0, 254, 252, 250, 249, 248, 248, 248, 249, 250, 251, 252, 253, 254, 255, 255, 255, 255, 254, 253, 252, 250, 248, 246, 244, 242, 240, 239, 238, 238, 238, 240, 242, 245, 249, 254, 3, 9, 15, 21, 28, 34, 40, 46, 51, 57, 61, 65, 68, 71, 74, 75, 76, 77, 77, 76, 75, 73, 71, 68, 65, 61, 57, 52, 47, 41, 34, 27, 20, 12, 3, 250, 241, 231, 222, 213, 204, 195, 186, 178, 172, 166, 161, 157, 154, 153, 154, 156, 161, 166, 173, 182, 192, 203, 214, 226, 238, 250, 6, 17, 28, 37, 46, 54, 61, 66, 71, 74, 77, 79, 80, 80, 80, 80, 78, 77, 75, 73, 71, 69, 66, 64, 61, 58, 54, 51, 47, 42, 37, 32, 27, 21, 15, 9, 4, 254, 248, 242, 237, 232, 228, 224, 221, 218, 216, 215, 214, 214, 215, 216, 218, 220, 223, 225, 228, 230, 233, 235, 237, 239, 241, 242, 244, 245, 247, 249, 250, 252, 254, 1, 4, 7, 10, 14, 17, 21, 25, 28, 31, 34, 36, 37, 38, 37, 36, 34, 31, 27, 23, 18, 13, 7, 2, 252, 247, 243, 239, 235, 232, 231, 230, 230, 232, 234, 238, 243, 248, 254, 4, 11, 18, 24, 31, 37, 42, 47, 52, 55, 58, 60, 62, 62, 62, 61, 59, 56, 53, 49, 45, 40, 35, 29, 23, 17, 11, 5, 255, 248, 242, 235, 228, 221, 214, 208, 201, 195, 189, 184, 180, 177, 174, 172, 172, 173, 174, 177, 181, 186, 192, 200, 207, 216, 225, 234, 244, 253, 6, 15, 23, 31, 38, 44, 50, 54, 59, 62, 65, 67, 69, 70, 70, 70, 70, 70, 69, 68, 67, 66, 64, 63, 61, 59, 56, 54, 51, 47, 43, 38, 33, 27, 21, 14, 7, 0, 248, 240, 232, 224, 216, 209, 201, 195, 189, 183, 179, 176, 174, 173, 173, 174, 176, 180, 183, 188, 193, 198, 204, 210, 216, 223, 229, 236, 243, 250, 1, 8, 15, 21, 28, 33, 38, 43, 47, 50, 53, 55, 56, 57, 57, 57, 55, 53, 50, 46, 42, 37, 32, 26, 21, 14, 8, 2, 252, 247, 242, 237, 233, 229, 226, 225, 224, 224, 225, 227, 230, 233, 238, 243, 248, 254, 4, 9, 14, 19, 23, 26, 29, 31, 32, 32, 32, 30, 28, 26, 23, 20, 16, 12, 7, 3, 255, 251, 247, 243, 239, 236, 232, 230, 227, 224, 222, 220, 218, 217, 216, 215, 214, 214, 214, 214, 214, 214, 215, 215, 216, 217, 218, 220, 222, 225, 228, 232, 237, 242, 248, 254, 4, 10, 16, 22, 28, 33, 38, 43, 47, 51, 55, 58, 60, 62, 64, 65, 66, 66, 66, 65, 64, 62, 61, 58, 56, 53, 50, 47, 42, 37, 32, 26, 19, 11, 3, 250, 241, 232, 223, 213, 204, 195, 187, 179, 171, 165, 160, 155, 152, 150, 149, 150, 153, 157, 162, 169, 177, 186, 195, 206, 216, 227, 238, 249, 3, 13, 22, 31, 38, 45, 51, 56, 60, 63, 65, 66, 67, 67, 66, 65, 64, 62, 60, 57, 54, 51, 48, 44, 40, 35, 30, 25, 20, 14, 9, 4, 254, 249, 244, 239, 235, 231, 227, 224, 222, 220, 220, 220, 220, 222, 224, 227, 230, 233, 236, 239, 241, 244, 246, 248, 249, 250, 250, 251, 251, 250, 249, 249, 247, 246, 245, 244, 243, 242, 241, 241, 241, 242, 243, 245, 247, 250, 252, 255, 1, 2, 3, 4, 4, 4, 2, 0, 254, 251, 248, 245, 242, 238, 235, 232, 230, 228, 226, 226, 227, 228, 231, 234, 239, 244, 249, 255, 6, 12, 19, 25, 30, 36, 41, 46, 49, 53, 56, 58, 59, 60, 60, 60, 58, 57, 54, 52, 48, 44, 40, 35, 30, 24, 18, 12, 5, 254, 247, 239, 231, 223, 215, 207, 199, 191, 184, 177, 171, 165, 161, 157, 155, 153, 154, 156, 159, 164, 170, 178, 187, 197, 207, 218, 229, 240, 251, 5, 14, 23, 31, 38, 45, 50, 55, 58, 61, 64, 65, 66, 66, 66, 65, 64, 63, 61, 59, 57, 56, 53, 51, 49, 46, 43, 40, 36, 32, 27, 23, 17, 12, 6, 0, 250, 245, 239, 233, 227, 222, 216, 212, 207, 203, 200, 198, 196, 196, 196, 196, 198, 200, 203, 205, 208, 211, 214, 217, 221, 223, 226, 229, 232, 235, 239, 242, 246, 250, 254, 2, 6, 11, 15, 19, 23, 27, 30, 33, 35, 37, 38, 38, 38, 36, 33, 30, 26, 22, 17, 12, 6, 1, 251, 246, 241, 236, 232, 228, 225, 223, 222, 223, 224, 226, 230, 234, 239, 244, 250, 0, 6, 13, 18, 24, 29, 33, 37, 40, 43, 44, 45, 45, 44, 42, 40, 37, 33, 29, 25, 20, 15, 10, 5, 0, 251, 246, 241, 236, 231, 225, 220, 215, 210, 206, 201, 197, 194, 191, 189, 187, 186, 186, 187, 188, 190, 192, 195, 199, 204, 209, 215, 222, 229, 236, 243, 251, 2, 9, 16, 22, 28, 34, 39, 43, 47, 50, 53, 56, 58, 59, 60, 61, 61, 61, 61, 60, 59, 58, 56, 55, 53, 51, 48, 46, 42, 39, 35, 30, 25, 19, 13, 6, 255, 247, 240, 232, 224, 216, 208, 201, 194, 187, 182, 177, 173, 170, 168, 167, 168, 170, 172, 176, 180, 185, 191, 197, 203, 211, 218, 226, 234, 242, 250, 2, 9, 17, 24, 30, 36, 41, 45, 49, 51, 54, 55, 56, 56, 56, 55, 53, 52, 49, 46, 42, 38, 34, 29, 23, 18, 12, 7, 2, 252, 247, 242, 238, 234, 231, 228, 226, 225, 224, 224, 226, 228, 231, 234, 238, 243, 247, 252, 0, 5, 8, 11, 13, 15, 16, 17, 16, 16, 14, 13, 11, 8, 5, 3, 0, 252, 249, 246, 244, 241, 239, 237, 235, 234, 233, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 231, 230, 229, 228, 227, 226, 226, 226, 226, 228, 229, 232, 235, 239, 244, 249, 254, 3, 9, 15, 21, 26, 31, 36, 41, 45, 48, 51, 54, 56, 58, 59, 59, 59, 59, 58, 56, 55, 52, 50, 47, 44, 40, 36, 31, 26, 21, 14, 8, 1, 249, 241, 234, 226, 218, 210, 202, 195, 188, 182, 176, 171, 166, 163, 160, 159, 159, 160, 164, 168, 174, 181, 189, 198, 208, 218, 228, 239, 249, 3, 12, 20, 28, 35, 42, 47, 52, 55, 58, 60, 61, 62, 62, 62, 61, 60, 58, 56, 54, 52, 50, 47, 44, 41, 37, 33, 29, 25, 20, 15, 10, 5, 0, 251, 247, 242, 238, 234, 230, 227, 224, 222, 221, 220, 220, 220, 221, 222, 224, 225, 228, 230, 232, 234, 236, 237, 239, 240, 241, 242, 242, 243, 243, 243, 243, 243, 243, 244, 244, 245, 246, 248, 250, 252, 255, 2, 5, 8, 11, 14, 15, 17, 17, 17, 16, 15, 12, 10, 6, 3, 255, 251, 247, 243, 239, 235, 232, 230, 228, 227, 227, 228, 230, 233, 237, 242, 247, 253, 3, 9, 14, 20, 26, 31, 35, 39, 43, 46, 48, 50, 50, 51, 50, 49, 47, 45, 42, 38, 35, 30, 26, 21, 16, 10, 5, 0, 250, 245, 239, 233, 227, 221, 215, 210, 204, 198, 193, 189, 184, 181, 178, 176, 175, 175, 176, 178, 181, 186, 191, 198, 205, 213, 221, 230, 239, 248, 0, 8, 16, 23, 29, 35, 40, 44, 48, 51, 54, 56, 57, 58, 58, 58, 57, 57, 56, 54, 53, 52, 50, 48, 46, 44, 41, 38, 35, 31, 27, 23, 18, 13, 8, 3, 253, 248, 243, 237, 232, 227, 222, 217, 212, 208, 204, 201, 199, 197, 196, 196, 197, 198, 200, 202, 205, 207, 210, 213, 217, 220, 223, 227, 231, 235, 239, 244, 249, 253, 2, 7, 12, 17, 21, 25, 29, 33, 36, 38, 40, 41, 42, 42, 41, 40, 38, 35, 31, 27, 23, 18, 13, 8, 2, 253, 249, 244, 240, 236, 233, 230, 229, 228, 227, 228, 230, 232, 236, 240, 244, 249, 254, 3, 8, 13, 17, 21, 25, 27, 30, 31, 32, 32, 31, 30, 28, 25, 22, 19, 15, 12, 8, 4, 0, 252, 249, 245, 242, 239, 236, 233, 230, 227, 224, 221, 219, 217, 215, 214, 212, 212, 211, 211, 211, 211, 212, 213, 214, 215, 217, 220, 223, 226, 230, 234, 239, 244, 250, 0, 5, 11, 16, 21, 26, 31, 35, 39, 42, 45, 48, 50, 52, 53, 54, 54, 54, 54, 53, 52, 50, 48, 46, 43, 41, 38, 34, 31, 27, 22, 18, 13, 8, 2, 252, 247, 241, 235, 230, 224, 218, 213, 208, 203, 199, 195, 191, 188, 186, 184, 183, 183, 184, 186, 189, 193, 197, 202, 208, 214, 221, 228, 235, 243, 250, 2, 9, 16, 22, 28, 33, 38, 42, 45, 48, 50, 51, 52, 52, 52, 51, 50, 48, 46, 44, 41, 38, 34, 30, 26, 21, 17, 12, 7, 3, 255, 250, 246, 243, 239, 236, 234, 232, 230, 230, 229, 230, 231, 233, 235, 238, 241, 244, 247, 250, 253, 255, 1, 3, 4, 5, 5, 5, 4, 4, 3, 1, 0, 254, 253, 251, 249, 247, 246, 245, 244, 243, 243, 243, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 250, 250, 249, 248, 246, 244, 242, 240, 238, 236, 235, 233, 233, 232, 233, 234, 236, 239, 243, 246, 251, 0, 5, 10, 15, 20, 25, 29, 33, 37, 40, 43, 45, 46, 47, 48, 48, 47, 46, 44, 42, 39, 36, 33, 29, 25, 21, 17, 12, 8, 3, 255, 250, 246, 242, 237, 233, 229, 225, 220, 216, 212, 208, 204, 201, 197, 194, 191, 189, 188, 188, 188, 190, 193, 197, 203, 209, 215, 223, 230, 238, 246, 253, 4, 11, 18, 24, 29, 34, 38, 42, 45, 47, 49, 50, 51, 51, 51, 50, 49, 48, 47, 45, 43, 41, 38, 35, 32, 29, 25, 21, 17, 13, 9, 5, 1, 253, 250, 246, 243, 240, 237, 235, 233, 231, 229, 228, 227, 227, 226, 227, 227, 228, 229, 231, 232, 233, 234, 236, 237, 238, 239, 239, 240, 241, 241, 242, 242, 243, 244, 245, 247, 248, 250, 253, 0, 2, 5, 8, 12, 14, 17, 19, 21, 22, 23, 23, 22, 21, 19, 17, 14, 10, 7, 3, 0, 252, 248, 245, 242, 239, 237, 235, 234, 234, 234, 236, 238, 241, 244, 248, 252, 1, 5, 10, 14, 18, 22, 25, 28, 30, 32, 33, 34, 34, 33, 32, 30, 28, 25, 23, 19, 16, 13, 9, 6, 2, 255, 252, 249, 246, 244, 241, 238, 236, 233, 231, 228, 226, 223, 221, 219, 217, 215, 213, 212, 211, 211, 211, 211, 212, 214, 216, 220, 223, 228, 232, 238, 243, 249, 254, 4, 9, 14, 19, 24, 28, 32, 35, 38, 41, 43, 44, 45, 46, 46, 46, 46, 45, 43, 42, 40, 37, 35, 32, 28, 25, 21, 18, 14, 10, 6, 2, 254, 251, 248, 245, 242, 240, 238, 236, 234, 233, 231, 230, 229, 228, 227, 226, 225, 225, 224, 224, 224, 224, 224, 225, 225, 226, 227, 228, 229, 231, 233, 236, 239, 242, 246, 250, 254, 2, 7, 11, 15, 19, 22, 25, 28, 31, 32, 34, 35, 35, 35, 34, 33, 31, 28, 26, 22, 19, 15, 11, 8, 4, 0, 253, 250, 247, 244, 242, 240, 239, 238, 238, 238, 239, 240, 242, 244, 247, 249, 252, 255, 2, 5, 7, 9, 11, 13, 14, 14, 14, 14, 14, 13, 12, 10, 9, 7, 5, 3, 2, 0, 254, 252, 251, 249, 248, 247, 246, 245, 244, 243, 243, 242, 242, 241, 241, 241, 240, 240, 239, 239, 238, 238, 237, 236, 236, 235, 235, 235, 236, 237, 238, 241, 243, 246, 249, 253, 1, 5, 9, 13, 17, 20, 24, 27, 30, 32, 34, 36, 37, 37, 38, 37, 36, 35, 33, 31, 28, 25, 22, 19, 15, 12, 8, 5, 2, 255, 252, 249, 247, 245, 243, 242, 241, 240, 240, 239, 239, 239, 238, 238, 237, 236, 235, 233, 232, 230, 228, 227, 226, 225, 225, 225, 226, 227, 229, 231, 234, 237, 241, 245, 249, 253, 1, 6, 10, 14, 17, 21, 24, 27, 29, 31, 32, 33, 34, 34, 33, 33, 32, 30, 28, 25, 23, 20, 17, 14, 10, 7, 4, 1, 254, 251, 249, 247, 245, 244, 243, 242, 241, 241, 241, 242, 242, 243, 244, 245, 247, 248, 250, 251, 252, 253, 254, 255, 0, 0, 0, 0, 0, 0, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 250, 250, 250, 251, 252, 252, 253, 254, 255, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 255, 254, 252, 251, 249, 248, 247, 246, 245, 244, 244, 244, 245, 246, 248, 250, 253, 255, 2, 5, 7, 10, 12, 14, 16, 18, 19, 20, 21, 21, 20, 20, 19, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 254, 252, 251, 250, 249, 248, 248, 247, 247, 247, 247, 247, 247, 247, 246, 246, 245, 244, 243, 242, 240, 239, 237, 236, 235, 234, 234, 234, 235, 236, 237, 239, 241, 244, 246, 249, 253, 0, 3, 6, 9, 12, 15, 18, 20, 22, 24, 25, 26, 26, 26, 26, 25, 24, 23, 21, 19, 17, 14, 12, 9, 7, 4, 2, 255, 253, 251, 249, 248, 247, 246, 245, 245, 245, 245, 246, 246, 247, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250, 250, 249, 249, 249, 249, 250, 251, 251, 253, 254, 255, 1, 2, 3, 5, 6, 7, 8, 9, 10, 10, 10, 9, 9, 8, 7, 6, 5, 4, 3, 1, 0, 255, 254, 253, 252, 251, 250, 250, 249, 249, 249, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 5, 6, 6, 6, 6, 6, 6, 5, 5, 4, 3, 3, 2, 1, 1, 0, 0, 255, 255, 254, 254, 254, 253, 253, 253, 253, 253, 252, 252, 252, 252, 252, 251, 251, 251, 250, 250, 249, 249, 248, 248, 247, 247, 246, 246, 246, 246, 247, 248, 249, 250, 252, 254, 0, 2, 3, 5, 7, 9, 11, 12, 13, 14, 15, 15, 15, 15, 15, 14, 13, 12, 11, 9, 8, 6, 5, 3, 2, 0, 255, 254, 253, 252, 251, 251, 250, 250, 250, 250, 250, 251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 252, 252, 251, 251, 251, 250, 250, 250, 250, 251, 251, 252, 253, 254, 255, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 8, 8, 8, 8, 7, 7, 6, 5, 4, 4, 3, 2, 1, 0, 255, 255, 254, 254, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 255, 255, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 254, 254, 254, 255, 0, 0, 1, 2, 2, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 3, 2, 2, 1, 1, 0, 0, 0, 255, 255, 255, 255, 255, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 254, 254, 254, 254, 254, 253, 253, 254, 254, 254, 254, 254, 255, 255, 0, 0, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, 255, 255, 255, 255, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#endif /* cowbell_H_ */