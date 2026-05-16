#pragma once

#include <stdio.h>
#include <stdbool.h>

extern int FILES_BASE_LENGTH;
extern int FILES_BASE_HIGHT;

bool get_save_path(const char* filter, const char* extension, char* dir, size_t max_size);
bool get_open_path(const char* filter, const char* extension, char* dir, size_t max_size);
bool load_bin(const char* path, void* array, size_t element_size, size_t array_size, size_t skip_bytes);
bool save_bin(const char* path, const void* array, size_t element_size, size_t array_size);
bool load_bmp(const char* path, bool* array, size_t array_size);
bool clear_bin(const char* path);