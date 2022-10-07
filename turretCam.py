import cv2


class TurretCam:
    def __init__(self):
        self.vid = cv2.VideoCapture(0)
        self.vid.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
        width=1920
        height=1080
        self.vid.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.vid.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    def capture(self):
        _, frame = self.vid.read()
        return frame

    def destroy(self):
        self.vid.release()
