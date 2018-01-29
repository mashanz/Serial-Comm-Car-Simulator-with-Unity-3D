import serial
import time

ser = serial.Serial('/dev/tty.SLAB_USBtoUART8', 9600, timeout = .1)
# ser = serial.Serial('/dev/tty.usbmodem1411', 9600, timeout = 1)

print ("Starting up")
connected = False
commandToSend = "1324\r\n" # get the distance in mm

while True:
    print("ARDUINO")
    print("\n[KIRIM]===================================")
    print ("Writing: ",  bytes(commandToSend, 'UTF-8'))
    ser.write(bytes(commandToSend, 'UTF-8'))
    while True:
        try:
            print("\n[TERIMA]==================================")
            readOut = ser.readline()
            print ("Reading: ", readOut) 
            break
        except:
            pass
    print ("Restart")
    ser.flush() #flush the buffer