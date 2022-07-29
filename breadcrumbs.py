import cv2


class Breadcrumbs:
  def __init__(self) -> None:
    self.breadcrumbs = []

  def add_breadcrumb(self, x, y):
    self.breadcrumbs.append({"x": x, "y": y})

  def show_breadcrumbs(self, frame):
    for breadcrumb in self.breadcrumbs:
      x = breadcrumb["x"]
      y = breadcrumb["y"]

      cv2.rectangle(frame, (x, y), (x + 2, y + 2), (0, 0, 255), 2)

    cv2.imshow('breadcrumbs', frame)