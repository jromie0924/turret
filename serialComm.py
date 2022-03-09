# from urllib import response
import serial
import time

class SerialComm:
    def __init__(self) -> None:
        failure_count = 0
        self.arduino = None
        while failure_count < 10:
            try:
                self.arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=0.1)
                break
            except serial.SerialException:
                failure_count += 1
                if failure_count >=10:
                    raise
                time.sleep(0.5)
                
    
    def write_data(self, data):
        try:
            self.arduino.write(bytes(data, 'utf-8'))
        except serial.SerialException:
            print("Error communicating with turret.")
            time.sleep(1)
        # time.sleep(0.05)
        # response_data = self.arduino.readline()
        # return response_data