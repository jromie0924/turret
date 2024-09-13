import cv2
from frameProcessor import FrameProcessor

processor = FrameProcessor()
STOP_KEY = 27 # ESC key

def start():
    while True:
        frame = processor.capture_and_process()
        cv2.imshow('Turret Targeting', frame)
        key = cv2.waitKey(1) # Wait for 1ms for a keypress - else continue processing the next frame.

        if key == STOP_KEY:
            break
    stop()


def stop():
    show_breadcrumbs = processor.destroy()
    stop = True

    while show_breadcrumbs:
        key = cv2.waitKey(1)

        if key == STOP_KEY:
            break
        
    cv2.destroyAllWindows()


if __name__ == '__main__':
    try:
        start()
    except KeyboardInterrupt:
        stop()
        exit(0)
