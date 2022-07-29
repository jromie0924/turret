import cv2
from frameProcessor import FrameProcessor

processor = FrameProcessor()


def start():
    while True:
        frame = processor.capture_and_process()
        cv2.imshow('Turret Targeting', frame)
        key = cv2.waitKey(1)

        # kill on ESC key
        if key == 27:
            break
    stop()


def stop():
    show_breadcrumbs = processor.destroy()

    while show_breadcrumbs:
        key = cv2.waitKey(1)

        if key == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    try:
        start()
    except KeyboardInterrupt:
        stop()
        exit(0)
