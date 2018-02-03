import serial
import time
import csv
from case import *

#####################################################################
# PARAMETER CONFIG
#####################################################################

port        = 'COM8'
baud        = 19200
looping     = 20

#####################################################################
# CONFIG FOR SERIAL
#####################################################################

connected   = False
fieldnames  = ['time','send', 'recieve']
timeout     = .1
filelog     = 'com_time.csv'
ser         = serial.Serial(port, baud, timeout = timeout)

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

def command(loops):
    moveCase = 0
    val = 0
    while True:
        ticks = time.localtime(time.time())
        val += 1;
        send = cases(moveCase) + "\r\n"
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
        if val==loops:
            val=0
            moveCase += 1
            if moveCase==len(switcher):
                ser.close()
                break

#####################################################################
# MAIN PROGRAM
#####################################################################

if __name__=='__main__':
    while True:
        print("==========================")
        print("        SIMULATOR         ")
        print("==========================")
        print(" MODE: ")
        print("  t: TUNNING")
        print("  p: POTENSIO")
        print("  f: FINDING MIN/MAX SENSOR")
        print("  s: SIMULATION")
        print("")
        pilihan = input("Pilih Mode: (t/f/s)")
        if(pilihan=="t"):
            print("TUNNING")
            sender = bytes("T\r\n", 'UTF-8')
            ser.write(sender)
            ser.flush()
            ser.close()
        elif(pilihan=="f"):
            print("FINDING")
            sender = bytes("F\r\n", 'UTF-8')
            ser.write(sender)
            ser.flush()
            ser.close()
        elif(pilihan=='p'):
            print("POTENSIO")
            sender = bytes("P\r\n", 'UTF-8')
            ser.write(sender)
            ser.flush()
            ser.close()
        elif(pilihan=="s"):
            print("START SIMULATION")
            reset()
            command(looping)

