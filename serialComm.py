from urllib import response
import serial
import time

class SerialComm:
    def __init__(self) -> None:
        failure_count = 0
        while failure_count < 10:
            try:
                self.arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=0.1)
                break;
            except serial.SerialException:
                failure_count += 1
                time.sleep(0.5)
    
    def write_data(self, data):
        self.arduino.write(bytes(data, 'utf-8'))
        time.sleep(0.05)
        response_data = self.arduino.readline()
        return response_data
    
    def feed_data(self, data):
        try:
            response_data = self.write_data(data)
            return (True, response_data)
        except serial.SerialException:
            return (False, None)