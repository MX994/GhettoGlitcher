import serial
import time

sock = serial.Serial('COM7', 9600)
sock.write(b'ping\n')
sock.timeout = 2
time.sleep(0.3)
while sock.in_waiting > 0:
  print(sock.read())