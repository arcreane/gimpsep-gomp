[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/VDgC_hrH)

Project uml diagram:

```plaintext
                                +--------------------+
                                |    ImageEditor     |
                                +--------------------+
                                | - currentImage     |
                                +--------------------+
                                | + loadImage()      |
                                | + saveImage()      |
                                | + applyOperation() |
                                | + displayImage()   |
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
+--------------+ +------------+ +------------+ +------------------+ +-------------+
| DilationOp   | | ErosionOp  | | ResizeOp    | | LightenDarkenOp | | CannyEdgeOp |
+--------------+ +------------+ +-------------+ +-----------------+ +-------------+
| +kernelSize  | | +kernelSize| | +fx, +fy    | | +factor         | | +thresholds |
| +apply()     | | +apply()   | | +apply()    | | +apply()        | | +apply()    |
+--------------+ +------------+ +-------------+ +-----------------+ +-------------+