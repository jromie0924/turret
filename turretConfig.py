import cv2


class TurretConfig:
    def __init__(self) -> None:
        self.show_object_boxes = False
        self.show_target_points = True

        # False for now - If you want to communicate with an Arduino, set this to True.
        self.enable_serial_comm = False
        self.accepted_classes = ['person']
        self.serial_connect_retry_delay_seconds = 1
        self.serial_connect_num_retries = 10
        self.enable_breadcrumbs = True # print breadcrumbs of all target positions when True

    def load_neural_net(self):
        # tiny yolo for speed optimization
        net = cv2.dnn.readNet('cfg/yolov4-tiny.weights', 'cfg/yolov4-tiny.cfg')
        return net

    def load_classes(self):
        with open('data/coco.names', 'r') as f:
            return [line.strip() for line in f.readlines()]
