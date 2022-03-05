import cv2

class camera:
    def __init__(self) -> None:
        self.vid = cv2.VideoCapture(0)

    def capture(self):
        while True:
            ret, frame = self.vid.read()

            # cv2.imshow('frame', frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        self.vid.release()
        cv2.destroyAllWindows()
