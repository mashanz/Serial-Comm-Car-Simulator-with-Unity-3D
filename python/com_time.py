import serial
import time
import csv

#####################################################################
# CONFIG FOR SERIAL
#####################################################################

connected   = False
fieldnames  = ['time','send', 'recieve']
port        = '/dev/tty.SLAB_USBtoUART'
baud        = 115200
timeout     = .1
filelog     = 'com_time.csv'
ser         = serial.Serial(port, baud, timeout = timeout)
val         = 1000
com1        = "R"
com2        = 100100100
com3        = "D"
#####################################################################
# FUNCTION OF SERIL
#####################################################################

def reset():
    with open(filelog, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

def write(time, send, recieve):
    with open(filelog, 'a', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writerow({fieldnames[0]: time, fieldnames[1]: send, fieldnames[2]: recieve})

def command(val):
    while True:
        ticks = time.localtime(time.time())
        val += 1;
        send = str(val) + "\r\n"
        #com2 = val
        #com2 += 15
        #send = str(com1) + str(com2) + str(com3) + "\r\n"
        sender = bytes(send, 'UTF-8')

        print("\n[KIRIM]===================================")
        print ("Writing: ",  sender)
        ser.write(sender)
        while True:
            try:
                print("\n[TERIMA]==================================")
                readOut = ser.readline()
                print ("Reading: ", readOut)
                print(ticks)
                write(ticks, sender, readOut)
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
