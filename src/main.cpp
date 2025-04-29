#include "ImageEditor.h"
#include "DilationOperation.h"
#include "ErosionOperation.h"
#include "ResizeOperation.h"
#include "LightenDarkenOperation.h"
#include "CannyEdgeOperation.h"
#include <iostream>
#include <memory>
#include <filesystem>

int main(int argc, char** argv) {
    ImageEditor editor;
    string imagePath;
    bool validPath = false;

    do {
        cout << "Enter the path to the image file you want to modify: " << endl;
        cin >> imagePath;
        validPath = true;
        if (imagePath.empty()) {
            cout << "Please provide a valid image path." << endl;
            validPath = false;
        } else if (!filesystem::exists(imagePath)) {
            cout << "File does not exist. Please try again." << endl;
            validPath = false;
        }
    } while (!validPath);

    if (!editor.loadImage(imagePath)) {
        cerr << "Failed to load image: " << imagePath << endl;
        return -1;
    }

    cout << "Successfully loaded image: " << imagePath << endl;

    do {
        system("cls");
        cout << "Choose an operation:\n1. Dilation\n2. Erosion\n3. Resize\n4. Lighten\n5. Darken\n6. Canny Edge Detection"
                     "\n7. Display image\n8. Save image\n9. Load new image\n0. Quit\nChoice: ";
        int choice;
        cin >> choice;

        shared_ptr<Operation> operation = nullptr;

        switch (choice) {
            case 1:
                operation = make_shared<DilationOperation>();
            break;
            case 2:
                operation = make_shared<ErosionOperation>();
            case 3:
                cout << "Not implemented yet" << endl;
            break;
            case 4:
                operation = make_shared<LightenDarkenOperation>();
            break;
            case 5:
                cout << "Not implemented yet" << endl;
            break;
            case 6:
                cout << "Not implemented yet" << endl;
            break;
            case 7:
                editor.displayImage("Current Image");
            break;
            case 8:
                cout << "Please enter the path to save the image: ";
            cin >> imagePath;
            editor.saveImage(imagePath);
            break;
            case 9:
                cout << "Please enter the path to load a new image: ";
            cin >> imagePath;
            if (!editor.loadImage(imagePath)) {
                cerr << "Failed to load image: " << imagePath << endl;
            }
            break;
            case 0:
                cout << "Exiting the program." << endl;
            return 0;
            default:
                cerr << "Invalid choice!" << endl;
            break;
        }

        if (operation) {
            editor.applyOperation(operation);
        }

    } while (true);

    return 0;
}