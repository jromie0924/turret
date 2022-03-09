import serial
import time
from turretConfig import TurretConfig


class SerialComm:
    def __init__(self) -> None:
        self.arduino = None
        self.config = TurretConfig()
        failure_count = 0
        wait_time = self.config.serial_connect_retry_delay_seconds
        num_retries = self.config.serial_connect_num_retries
        
        if self.config.enable_serial_comm:
            while failure_count < num_retries:
                try:
                    self.arduino = serial.Serial(
                        port='/dev/ttyACM0', baudrate=115200, timeout=0.1)
                    break
                except serial.SerialException:
                    failure_count += 1
                    if failure_count >= num_retries:
                        raise
                    time.sleep(wait_time)

    def write_data(self, data):
        if self.config.enable_serial_comm:
            try:
                self.arduino.write(bytes(data, 'utf-8'))
            except serial.SerialException:
                print("Error communicating with turret.")
                time.sleep(1)
