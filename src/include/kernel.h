#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Common types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

// Kernel main function
void kernel_main(void);

// Utility functions
void kernel_panic(const char* message);
void* memset(void* dest, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);

#endif