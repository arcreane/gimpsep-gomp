//
// Created by hippolyte on 31/03/25.
//
#include "ImageEditor.h"
#include "DilationOperation.h"
#include "ErosionOperation.h"
#include "ResizeOperation.h"
#include "LightenDarkenOperation.h"
#include "CannyEdgeOperation.h"
#include "PanoramaStitcher.h"
#include <iostream>
#include <memory>
#include <filesystem>

int main(int argc, char** argv) {
    ImageEditor editor;
    string imagePath;

    do {
        cout << "Choose an operation:\n1. Dilation\n2. Erosion\n3. Resize\n4. Lighten\n5. Canny Edge Detection"
                     "\n6. Display image\n7. Save image\n8. Load new image"
                     "\n9. Panorama Stitcher\n0. Quit\nChoice: ";
        int choice;
        cin >> choice;

        shared_ptr<Operation> operation = nullptr;

        switch (choice) {
            case 1:
                operation = make_shared<DilationOperation>();
            break;
            case 2:
                operation = make_shared<ErosionOperation>();
            break;
            case 3:
                operation = make_shared<ResizeOperation>();
            break;
            case 4:
                operation = make_shared<LightenDarkenOperation>();
            break;
            case 5:
                operation = make_shared<CannyEdgeOperation>();
            break;
            case 6:
                editor.displayImage("Current Image");
            break;
            case 7:
                cout << "Please enter the path to save the image: ";
                cin >> imagePath;
                editor.saveImage(imagePath);
            break;
            case 8:
                cout << "Please enter the path to load a new image: ";
                cin >> imagePath;
                if (!editor.loadImage(imagePath)) {
                    cerr << "Failed to load image: " << imagePath << endl;
                }
            break;
            case 9:
                operation = make_shared<PanoramaStitcher>();
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