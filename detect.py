import cv2
from cv2 import VideoCapture
import numpy as np
import time

# Load YOLO
# net = cv2.dnn.readNet('yolov3.weights', 'cfg/yolov3.cfg') # original yolov3
net = cv2.dnn.readNet('yolov3-tiny.weights', 'cfg/yolov3-tiny.cfg') # tiny yolo for speed optimization

classes = []
with open('data/coco.names', 'r') as f:
    classes = [line.strip() for line in f.readlines()]

# print(classes)

layer_names = net.getLayerNames()
outputlayers = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()]

# print(outputlayers)

colors = np.random.uniform(0, 255, size=(len(classes), 3))

# load video
cap = cv2.VideoCapture(0)
font = cv2.FONT_HERSHEY_PLAIN
starting_time=time.time()
frame_id = 0

while True:
    _, frame = cap.read()
    frame_id += 1

    height, width, channels = frame.shape

    blob = cv2.dnn.blobFromImage(frame, 0.00392, (320, 320), (0, 0, 0), True, crop=False)

    net.setInput(blob)
    outs = net.forward(outputlayers)

    # show infor on screen and get confidence score of algo in detecting an object in blob
    class_ids = []
    confidences = []
    boxes = []

    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]
            if confidence > 0.3: # tuneable
                # object detected
                center_x = int(detection[0] * width)
                center_y = int(detection[1] * height)
                w = int(detection[2] * width)
                h = int(detection[3] * height)

                x = int(center_x - w/2)
                y = int(center_y - h/2)

                boxes.append([x,y,w,h]) # put all rectangle areas
                confidences.append(float(confidence)) # add detection confidence
                class_ids.append(class_id) # name of the object that was detected

    indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.4, 0.6)

    for i in range(len(boxes)):
        if i in indexes:
            x,y,w,h = boxes[i]
            label = str(classes[class_ids[i]])
            confidence = confidences[i]
            color = colors[class_ids[i]]
            cv2.rectangle(frame, (x,y), (x+w, y+h), color, 2)
            cv2.putText(frame,label+" "+str(round(confidence,2)),(x,y+30),font,1,(255,255,255),2)

    elapsed_time = time.time() - starting_time
    fps = frame_id / elapsed_time
    cv2.putText(frame,"FPS:"+str(round(fps,2)),(10,50),font,2,(0,0,0),1)

    cv2.imshow('Image', frame)
    key = cv2.waitKey(1) # wait 1ms each time

    if key == 27: # kill on ESC key
        break

cap.release()
cv2.destroyAllWindows()