import serial
from Tkinter import *
import serial.tools.list_ports
import string

root = Tk()
root.title('Telemetry system');
connectScreen = Frame(root)
connectScreen.grid(column=0,row=0, sticky=(N,W,E,S) )
connectScreen.columnconfigure(0, weight = 1)
connectScreen.rowconfigure(0, weight = 1)
connectScreen.pack(pady = 100, padx = 100)

ports = list(serial.tools.list_ports.comports())
def connect():
    f= open('record data.csv','w')
    print  portpick.get()
    for p in ports:
        if(str(portpick.get()) == str(p)):
            connectionport = str(p.device)
    ser = serial.Serial(connectionport,9600)
    while True:
        line =  ser.readline()
        words = string.split(line, "\t")
        try:
            lat   = words[0]
            lon   = words[1]
            ax    = words[2]
            ay    = words[3]
            az    = words[4]
            roll  = words[5]
            pitch = words[6]
            yaw   = words[7]
            writeline =  str(lat) + "\t" + str(lon) + "\t" + str(ax) + "\t" +str(ay) + "\t" +str(az) + "\t" +str(roll) + "\t" +str(pitch) + "\t" +str(yaw) 
            print writeline
            f.write(writeline + "\n")
        except:
            print 'error'
       



PortsLab = Label(connectScreen, text = 'Ports')
PortsLab.grid(row = 0, column = 0)

portpick = StringVar(root)
portlist = OptionMenu(connectScreen, portpick , *ports)
portlist.grid(row = 0, column = 1)
portpick.set(ports[0])

connectBut = Button(connectScreen, text = 'Connect', command = connect)
connectBut.grid(row = 1, column = 1)

mainloop()