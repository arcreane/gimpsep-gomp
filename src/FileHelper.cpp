//
// Created by mathy on 24/05/2025.
//
#include "../includes/FileHelper.h"

#ifdef _WIN32

#include <windows.h>
#include <commdlg.h>

#endif


std::string FileHelper::openFileDialog() {
    char filename[MAX_PATH] = "";
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Images\0*.jpg;*.png;*.bmp;*.jpeg\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = "Select an image to edit";

    if (GetOpenFileName(&ofn)) {
        return {filename};
    }

    return {};
}


std::vector<std::string> FileHelper::openMultipleFilesDialog() {
    char buffer[4096] = "";
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Images\0*.jpg;*.png;*.bmp;*.jpeg\0All Files\0*.*\0";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = sizeof(buffer);
    ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    ofn.lpstrTitle = "Select images for panorama";
    std::vector<std::string> files;

    if (GetOpenFileName(&ofn)) {
        char *ptr = buffer;
        const std::string directory = ptr;
        ptr += directory.length() + 1;
        if (*ptr == '\0') {
            files.push_back(directory);
        } else {
            while (*ptr) {
                files.push_back(directory + "\\" + std::string(ptr));
                ptr += strlen(ptr) + 1;
            }
        }
    }

    return files;
}


std::string FileHelper::saveFileDialog() {
    char filename[MAX_PATH] = "";
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Images\0*.jpg;*.bmp;*.jpeg\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrTitle = "Save image as";

    if (GetSaveFileName(&ofn)) {
        std::string filePath = filename;
        if (filePath.find('.') == std::string::npos) {
            filePath += ".jpg";
        }
        return filePath;
    }

    return {};
}
