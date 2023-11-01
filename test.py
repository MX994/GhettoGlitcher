import serial

sock = serial.Serial('/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A1012XEH-if00-port0', 115200)
while True:
  sock.write('yeo'.encode())
