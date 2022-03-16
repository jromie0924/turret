from serialComm import SerialComm

if __name__ == "__main__":
  serial = SerialComm()
  try:
    while True:
      serial_data = input("Enter data to send to arduino -> ")
      serial.write_data(serial_data)
  except KeyboardInterrupt:
    exit(0)