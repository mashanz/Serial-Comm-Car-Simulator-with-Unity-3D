import serial
import time
import csv

#####################################################################
# CONFIG FOR SERIAL
#####################################################################

connected   = False
val         = 1000
fieldnames  = ['send', 'recieve']
port        = '/dev/tty.SLAB_USBtoUART'
baud        = 9600
timeout     = .1
filelog     = 'test.csv'
ser         = serial.Serial(port, baud, timeout = timeout)

#####################################################################
# FUNCTION OF SERIL
#####################################################################

def reset():
    with open(filelog, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

def write(send, recieve):
    with open(filelog, 'a', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writerow({fieldnames[0]: send, fieldnames[1]: recieve})

def command(val):
    while True:
        val += 1;
        send = str(val) + "\r\n"
        sender = bytes(send, 'UTF-8')

        print("\n[KIRIM]===================================")
        print ("Writing: ",  sender)
        ser.write(sender)
        while True:
            try:
                print("\n[TERIMA]==================================")
                readOut = ser.readline()
                print ("Reading: ", readOut)
                write(sender,readOut)
                break
            except:
                pass
        print ("Restart")
        ser.flush()

#####################################################################
# MAIN PROGRAM
#####################################################################

if __name__=='__main__':
    reset()
    command(val)
