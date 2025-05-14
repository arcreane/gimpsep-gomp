//
// Created by victo on 12/05/2025.
//

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <vector>

class FileHelper {
public:
    static std::string openFileDialog();
    static std::vector<std::string> openMultipleFilesDialog();
    static std::string saveFileDialog();
};

#endif //FILEHELPER_H
