import cv2


class Breadcrumbs:
  def __init__(self) -> None:
    self.breadcrumbs = []
    self.hist = dict()

  def add_breadcrumb(self, x, y):
    if not (x, y) in self.hist:
      self.breadcrumbs.append({"x": x, "y": y})
      self.hist[(x, y)] = (x, y)

  def show_breadcrumbs(self, frame):
    for breadcrumb in self.breadcrumbs:
      x = breadcrumb["x"]
      y = breadcrumb["y"]

      cv2.rectangle(frame, (x, y), (x + 2, y + 2), (0, 0, 255), 2)

    cv2.imshow(f'breadcrumbs - {len(self.breadcrumbs)} data points', frame)