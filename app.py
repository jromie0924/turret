import cv2
from frameProcessor import FrameProcessor


processor = FrameProcessor()


def start():
    while True:
        frame = processor.capture_and_process()
        cv2.imshow('Turret Targeting', frame)
        key = cv2.waitKey(1)  # wait 1ms each time

        if key == 27:  # kill on ESC key
            break
    stop()


def stop():
    processor.destroy()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    start()
