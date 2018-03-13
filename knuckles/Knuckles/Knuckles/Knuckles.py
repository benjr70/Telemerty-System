import serial
from Tkinter import *
import serial.tools.list_ports

root = Tk()
root.title('Telemetry system');
connectScreen = Frame(root)
connectScreen.grid(column=0,row=0, sticky=(N,W,E,S) )
connectScreen.columnconfigure(0, weight = 1)
connectScreen.rowconfigure(0, weight = 1)
connectScreen.pack(pady = 100, padx = 100)

ports = list(serial.tools.list_ports.comports())
def connect():
    PortsLab.forget()
    portlist.forget()
    connectBut.forget()
    print  portpick.get()
    for p in ports:
        if(str(portpick.get()) == str(p)):
            connectionport = str(p.device)
    ser = serial.Serial(connectionport,115200)
    while True:
        print ser.readline()


PortsLab = Label(connectScreen, text = 'Ports')
PortsLab.grid(row = 0, column = 0)

portpick = StringVar(root)
portlist = OptionMenu(connectScreen, portpick , *ports)
portlist.grid(row = 0, column = 1)
portpick.set(ports[0])

connectBut = Button(connectScreen, text = 'Connect', command = connect)
connectBut.grid(row = 1, column = 1)

mainloop()

#https://pythonprogramming.net/change-show-new-frame-tkinter/