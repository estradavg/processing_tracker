# processing_tracker
A computer vision program that tracks an object to move a circle inside a box on screen.

# Installation
1. Clone this repository `git clone https://github.com/estradavg/processing_tracker.git`.
2. Download and install [Processing](https://processing.org/download).
3. `pip install cvzone`. Refer to the [cvzone](https://github.com/cvzone/cvzone) repository for better understanding and usage.

# Usage
1. Open and run the processing_constrain.pde file using the Processing Development Environment.
2. Run the main.py inside the constrain_tracker folder.

demo: https://youtu.be/YAe-rv5XPXg

# Note
By default the program tracks a green object, but you can play changing the desired color to be tracked adjusting its HSV values. For this:

1. Run the main.py inside the calibrate_hsv folder. Use the trackbars for adjusting the masks and filters to reach the desired color.
2. Save the HSV values and put them in the main.py inside the constrain_tracker folder.
