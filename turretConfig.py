import cv2

class TurretConfig:
    def __init__(self) -> None:
        pass


    def load_neural_net(self):
        # net = cv2.dnn.readNet('yolov3.weights', 'cfg/yolov3.cfg') # original yolov3
        net = cv2.dnn.readNet('cfg/yolov3-tiny.weights', 'cfg/yolov3-tiny.cfg') # tiny yolo for speed optimization
        return net
    
    def load_classes(self):
        with open('data/coco.names', 'r') as f:
            return [line.strip() for line in f.readlines()]