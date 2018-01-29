import serial
import time
import csv

#####################################################################
# CONFIG FOR SERIAL
#####################################################################

connected   = False
fieldnames  = ['time','send', 'recieve']
port        = 'COM5'
baud        = 19200
timeout     = .1
filelog     = 'com_time.csv'
ser         = serial.Serial(port, baud, timeout = timeout)
val         = 0
com1        = "R"
com2        = 100100100
com3        = "D"

#####################################################################
# PARAMETER CONFIG
#####################################################################
def cases(argument):
    switcher = {
        0: "R000000",
        1: "L100100",
        2: "L000100",
    }
    return switcher.get(argument, "L000000")

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
    while True:
        ticks = time.localtime(time.time())
        val += 1;
        send = cases(moveCase) + "\r\n"
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
        if val>loops:
            val=0
            moveCase += 1

#####################################################################
# MAIN PROGRAM
#####################################################################

if __name__=='__main__':
    reset()
    command(val)
