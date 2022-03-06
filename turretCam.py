import cv2


class TurretCam:
    def __init__(self):
        self.vid = cv2.VideoCapture(0)

    def capture(self):
        _, frame = self.vid.read()
        return frame

    def destroy(self):
        self.vid.release()
