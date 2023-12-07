# processing_tracker
A computer vision program that tracks an object to move a circle inside a box on screen.

# Installation
1. Clone this repository `git clone https://github.com/estradavg/processing_tracker.git`.
2. Download and install [Processing](https://processing.org/download).
3. `pip install cvzone`. Refer to the [cvzone](https://github.com/cvzone/cvzone) repository for better understanding and usage.

# Usage
**Python:**
1. Open and run the processing_constrain.pde file using the Processing Development Environment.
2. Run the constrain_tracker.py script inside the constrain_tracker folder.

**C++:**
1. Open and run the processing_constrain.pde file using the Processing Development Environment.
2. Compile and execute the constrain_tracker.cpp file inside the constrain_tracker folder.

demo: https://youtu.be/YAe-rv5XPXg

# Compiling (g++)
CMakeLists.txt:

```
mkdir build && cd build
cmake ..
make
./tracker
```

Compile separately:

`g++ file_name.cpp -I/usr/include/opencv4/ -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs`

# Note
By default, the program tracks a green object in the Python script and an orange object in the CPP file, but you can change the desired color to be tracked by adjusting its HSV values. For this:

1. Run the calibrate_hsv.py script, or compile and execute the calibrate_hsv.cpp file inside the calibrate_hsv folder. Use the trackbars to reach the desired color.
2. Save the HSV values and put them in the constrain_tracker.py script or constrain_tracker.cpp file inside the constrain_tracker folder.
