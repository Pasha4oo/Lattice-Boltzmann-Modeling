#include "files.h"

#include <Windows.h>
#include <commdlg.h>

#include "consts.h"

int FILES_BASE_LENGTH = 490;
int FILES_BASE_HIGHT = 60;

bool get_open_path(const char* filter, const char* extension, char* dir, size_t max_size) {
    OPENFILENAMEA ofn = {
        .lStructSize = sizeof(ofn),
        .lpstrFile = dir,
        .nMaxFile = (DWORD)max_size,
        .lpstrDefExt = extension,
        .lpstrFilter = filter,
        .nFilterIndex = 1,
        .Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY
    };

    if (GetOpenFileNameA(&ofn)) { 
        return true; 
    }
    return false;
}

bool get_save_path(const char* filter, const char* extension, char* dir, size_t max_size) {
    OPENFILENAMEA ofn = {
        .lStructSize = sizeof(ofn),
        .lpstrFile = dir,
        .nMaxFile = (DWORD)max_size,
        .lpstrDefExt = extension,
        .lpstrFilter = filter,
        .nFilterIndex = 1,
        .Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY
    };

    if (GetSaveFileNameA(&ofn)) {
        return true;
    }
    return false;
}

bool load_bin(const char* path, void* array, size_t element_size, size_t array_size, size_t* skip_bytes) {
    FILE* file = NULL;

    if (fopen_s(&file, path, "rb") != 0 || file == NULL) {
        return false;
    }

    if (fseek(file, *skip_bytes, SEEK_SET) != 0) {
        fclose(file);

        return false;
    }

    size_t elements_read = fread(array, element_size, array_size, file);
    if (elements_read < array_size || ferror(file)) {
        fclose(file);

        return false;
    }

    fclose(file);

    *skip_bytes += elements_read * element_size;
    return true;
}

bool save_bin(const char* path, const void* array, size_t element_size, size_t array_size) {
    FILE* file = NULL;

    if (fopen_s(&file, path, "ab") != 0 || file == NULL) {
        return false;
    }

    if (fwrite(array, element_size, array_size, file) != array_size || ferror(file)) {
        fclose(file);

        return false;
    }

    fclose(file);

    return true;
}

bool load_bmp(const char* path, bool* array, size_t array_size) {
    FILE* file = NULL;

    if (fopen_s(&file, path, "rb") != 0 || file == NULL) {
        return false;
    }

    if (fseek(file, 54, SEEK_SET) != 0) {
        fclose(file);

        return false;
    }

    unsigned char buffer[800 * 3];

    for (int y = Ny - 1; y >= 0; y--) {
        if (fread(buffer, 1, Nx * 3, file) != Nx * 3) {
            fclose(file);

            return false;
        }

        for (int x = 0; x < Nx; x++) {
            unsigned char b = buffer[x * 3];
            unsigned char g = buffer[x * 3 + 1];
            unsigned char r = buffer[x * 3 + 2];

            unsigned char brightness = (r + g + b) / 3;

            if (y * Nx + x >= array_size) { 
                fclose(file);

                return false; 
            }
            array[y * Nx + x] = (brightness < 128);
        }

        int padding = (4 - ((Nx * 3) % 4)) % 4;

        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }

    fclose(file);

    return true;
}

bool clear_bin(const char* path) {
    FILE* file = NULL;

    if (fopen_s(&file, path, "wb") != 0 || file == NULL) {
        return false;
    }

    fclose(file);

    return true;
}