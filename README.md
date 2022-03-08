# TURRET

Utilizes the YOLO classifying algorithm to identify possible targets.

## Setup

NOTE: This requires Python 3.8, as well as an NVIDIA GPU that has CUDA cores.

### Install pipenv
`pip3 install pipenv`

### Install OpenCV for Python
1. Activate pipenv virtual environment (`pipenv shell`)
2. `python -m pip install opencv-python`
3. Test that it works: `python -c "import cv2; print(cv2.__version__)"`

### Install other packages
`pipenv install`

## Running

1. `pipenv shell` (if you're not already inside the virtual environment)
2. `python app.py`

Note that it's helpful if you have a webcam plugged into your machine; the program will likely not like it if you don't.

## Troubleshooting
You may run into an issue with `yolo34py-gpu` package.

1. `sudo apt update`
2. `sudo apt install nvidia-cuda-toolkit`
3. `git clone git@github.com:AlexeyAB/darknet.git`
4. `cd darknet`
5. `make`
6. Change directories back to the turret repo and run `pipenv install` again.