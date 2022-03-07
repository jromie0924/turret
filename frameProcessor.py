import cv2
import numpy as np
import time
from turretConfig import TurretConfig
from turretCam import TurretCam


class FrameProcessor:
    def __init__(self):
        self.ACCEPTED_CLASSES = ['person']

        config = TurretConfig()
        self.net = config.load_neural_net()
        self.classes = config.load_classes()
        self.camera = TurretCam()
        self.font = cv2.FONT_HERSHEY_PLAIN
        # self.colors = np.random.uniform(0, 255, size=(len(self.classes), 3))
        self.starting_time = time.time()
        self.frame_id = 0

        self.show_boxes = config.show_object_boxes
        self.show_target_points = config.show_target_points

        layer_names = self.net.getLayerNames()
        self.outputlayers = [layer_names[i - 1]
                             for i in self.net.getUnconnectedOutLayers()]

    def capture_and_process(self):
        frame = self.camera.capture()
        self.frame_id += 1

        height, width, channels = frame.shape

        # blob = cv2.dnn.blobFromImage(frame, 0.00392, (320, 320), (0, 0, 0), True, crop=False)
        blob = cv2.dnn.blobFromImage(image=frame, scalefactor=0.01, size=(
            320, 320), mean=(0, 0, 0), swapRB=True, crop=False)

        self.net.setInput(blob)
        outs = self.net.forward(self.outputlayers)

        # show infor on screen and get confidence score of algo in detecting an object in blob
        class_ids = []
        confidences = []
        boxes = []

        for out in outs:
            for detection in out:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]
                if confidence > 0.3 and self.classes[class_id].lower() in self.ACCEPTED_CLASSES:
                    # object detected
                    center_x = int(detection[0] * width)
                    center_y = int(detection[1] * height)
                    w = int(detection[2] * width)
                    h = int(detection[3] * height)

                    x = int(center_x - w/2)
                    y = int(center_y - h/2)

                    boxes.append([x, y, w, h])  # put all rectangle areas
                    # add detection confidence
                    confidences.append(float(confidence))
                    # name of the object that was detected
                    class_ids.append(class_id)

        # Acquire non maximum suppression boxes around detected objects
        indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.4, 0.6)

        for i in range(len(boxes)):
            if i in indexes:
                x, y, w, h = boxes[i]
                label = str(self.classes[class_ids[i]])
                confidence = confidences[i]
                box_color = (0, 255, 0)  # Green
                # Red (the format is BGR for some reason)
                if self.show_target_points:
                    target_color = (0, 0, 255)
                    target_start_x = int(x + w/2 - 5)
                    target_start_y = int(y + h/2 - 5)
                    target_end_x = int(target_start_x + 10)
                    target_end_y = int(target_start_y + 10)
                    cv2.rectangle(frame, (target_start_x, target_start_y),
                                    (target_end_x, target_end_y), target_color, 2)
                    cv2.putText(frame, "Target", (target_start_x,
                                target_start_y - 8), self.font, 1, (255, 255, 255), 2)
                if self.show_boxes:
                    cv2.rectangle(frame, (x, y), (x+w, y+h), box_color, 2)
                    cv2.putText(frame, f"{label} {str(round(confidence,2))}",
                                (x, y+30), self.font, 1, (255, 255, 255), 2)
        elapsed_time = time.time() - self.starting_time
        fps = self.frame_id / elapsed_time
        cv2.putText(frame, f"FPS:{str(round(fps, 2))}",
                    (10, 50), self.font, 2, (0, 0, 0), 1)
        return frame

    def destroy(self):
        self.camera.destroy()
