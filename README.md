[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/VDgC_hrH)

Project uml diagram:

```plaintext
                                +--------------------+
                                |    ImageEditor     |
                                +--------------------+
                                | - currentImage     |
                                | - originalImage    |
                                | - resultImage      |
                                | - lockedImage      |
                                +--------------------+
                                | + loadImage()      |
                                | + saveImage()      |
                                | + applyOperation() |
                                | + displayImage()   |
                                | + resetToOriginal()|
                                | + getResultImage() |
                                +--------------------+
                                           |
                                           v
                                  +----------------+
                                  |   Operation    |  <<abstract>>
                                  +----------------+
                                  | + apply() = 0  |
                                  +----------------+
                                           ^
                                           |
                                +--------------------+    +------------------+    +------------------+    +-----------------------+    +------------------+    +----------------------+
                                | DilationOperation  |    | ErosionOperation |    | ResizeOperation  |    | LightenDarkenOperation|    | CannyEdgeOperation|    | PanoramaStitcher     |
                                +--------------------+    +------------------+    +------------------+    +-----------------------+    +------------------+    +----------------------+
                                | - size             |    | - size           |    | - fx             |    | - delta               |    | - lowThreshold    |    | - imagePaths         |
                                |                    |    |                  |    | - fy             |    |                       |    | - highThreshold   |    |                      |
                                |                    |    |                  |    |                  |    |                       |    | - kernelSize      |    |                      |
                                | + apply() override |    | + apply()        |    | + apply()        |    | + apply()             |    | + apply()         |    | + apply()            |
                                +--------------------+    +------------------+    +------------------+    +-----------------------+    +-------------------+    +----------------------+
                                                                                                                                       

