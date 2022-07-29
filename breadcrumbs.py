import cv2


class Breadcrumbs:
  def __init__(self) -> None:
    self.hist = dict()

  def add_breadcrumb(self, x, y):
    if not (x, y) in self.hist:
      self.hist[(x, y)] = (x, y)

  def show_breadcrumbs(self, frame):
    for key in self.hist:
      breadcrumb = self.hist.get(key)
      x = breadcrumb[0]
      y = breadcrumb[1]

      cv2.rectangle(frame, (x, y), (x + 2, y + 2), (0, 0, 255), 2)

    cv2.imshow(f'breadcrumbs - {len(self.hist)} data points. Press "c" to continue; esc to stop', frame)