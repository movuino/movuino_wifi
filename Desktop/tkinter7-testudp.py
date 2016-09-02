#!/usr/bin/python
#
# Read stream of lines from an Arduino with a magnetic sensor. This
# produces 3 values per line every 50ms that relate to the orientation
# of the sensor. Each line looks like:
# MAG   1.00    -2.00   0.00
# with each data line starting with "MAG" and each field separated by
# tab characters. Values are floating point numbers in ASCII encoding.
#
# This script supports both Python 2.7 and Python 3

from __future__ import print_function, division, absolute_import
import sys
from threading import Thread
from Queue import Queue
#import ser_daemon
import udp
if sys.hexversion > 0x02ffffff:
    import tkinter as tk
else:
    import Tkinter as tk
from serial import Serial
ax=ay=az=0.0
global myQueue

def test_thread(threadname, q):
    while True:
        q.put('S: 1 2 3 4 a 1265 1568 1789')
    
    
class App(tk.Frame):
    def __init__(self, parent, title):
        print('init')
        tk.Frame.__init__(self, parent)
        #self.queue = queue
        #self.serialPort = serialPort
        self.npoints = 100
        #fill the array with 0 to initialize
        self.Line1 = [0 for x in range(self.npoints)]
        self.Line2 = [0 for x in range(self.npoints)]
        self.Line3 = [0 for x in range(self.npoints)]
        parent.wm_title(title)
        parent.wm_geometry("800x600")
        self.canvas = tk.Canvas(self, background="white")
        self.canvas.bind("<Configure>", self.on_resize)
        self.canvas.create_line((0, 0, 0, 0), tag='X', fill='darkblue', width=1)
        self.canvas.create_line((0, 0, 0, 0), tag='Y', fill='darkred', width=1)
        self.canvas.create_line((0, 0, 0, 0), tag='Z', fill='darkgreen', width=1)
        self.canvas.grid(sticky="news")
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid(sticky="news")
        parent.grid_rowconfigure(0, weight=1)
        parent.grid_columnconfigure(0, weight=1)
        #self.openCOM = tk.Button(self,text="Open",command=self.open_COM)
        self.grid_rowconfigure(1, weight=1)
        self.grid_columnconfigure(0, weight=1)
        tk.Button(self, text="Run", command=self.Run).grid(column=0, row=1)
        #self.open_COM
        #self.read_serial()
        self.read_queue()
    
    def on_resize(self, event):
        self.replot()
    def Run(self):
        print ("run")
    def read_serial(self):
        """
        Check for input from the serial port. On fetching a line, parse
        the sensor values and append to the stored data and post a replot
        request.
        """
        if self.serialPort.inWaiting() != 0:
            line = self.serialPort.readline()
            #line = line.decode('ascii').strip("\r\n")
            if line[0:3] == "MAG":
                print(line) # line not a valid sensor result.!= "MAG":
            else:
                try:
                    data = line.split(" ")
                    if 'a' in data[4]:
                        #print mydata[5]
                        global ax,ay,az
                        ax=float(data[5])/200
                        ay=float(data[6])/200
                        az=float(data[7])/200
                        print(ax) 
                    #x, y, z = data[1], data[2], data[3]
                    #print(data[1])
                    x,y,z =    ax, ay,az
                    self.append_values(x, y, z)
                    self.after_idle(self.replot)
                except Exception as e:
                    print (e)
        self.after(5, self.read_serial)
        
    def read_queue(self):
        #global line
        #line = myQueue.get()
        #print(line)
        if myQueue.empty() is False :
           global line
           line = myQueue.get()
           #print(line)
           data = line.split(" ")
           if 'a' in data[4]:
            global ax,ay,az
            ax=float(data[5])/200
            ay=float(data[6])/200
            az=float(data[7])/200
            #print(ay) 
            x,y,z =    ax, ay,az
            self.append_values(x, y, z)
            self.after_idle(self.replot)
        self.after(5, self.read_queue)     
    
    def append_values(self, x, y, z):
        """
        Update the cached data lists with new sensor values.
        """
        #add one point 
        self.Line1.append(float(x))
        #shift the array one to the right sel.line[-100:] the last 100 from the right 
        self.Line1 = self.Line1[-1 * self.npoints:]
        self.Line2.append(float(y))
        self.Line2 = self.Line2[-1 * self.npoints:]
        self.Line3.append(float(z))
        self.Line3 = self.Line3[-1 * self.npoints:]
        return

    def replot(self):
        """
        Update the canvas graph lines from the cached data lists.
        The lines are scaled to match the canvas size as the window may
        be resized by the user.
        """
        w = self.winfo_width()
        h = self.winfo_height()
        max_X = max(self.Line1) + 1e-5
        max_Y = max(self.Line2) + 1e-5
        max_Z = max(self.Line3) + 1e-5
        max_all = 200.0
        #create list of points to display
        coordsX, coordsY, coordsZ = [], [], []
        for n in range(0, self.npoints):
            x = (w * n) / self.npoints
            coordsX.append(x)
            coordsX.append(h - ((h * (self.Line1[n]+100)) / max_all))
            coordsY.append(x)
            coordsY.append(h - ((h * (self.Line2[n]+100)) / max_all))
            coordsZ.append(x)
            coordsZ.append(h - ((h * (self.Line3[n] + 100)) / max_all))
        #move the graph
        #print(*coordsX)
        self.canvas.coords('X', *coordsX)
        self.canvas.coords('Y', *coordsY)
        self.canvas.coords('Z', *coordsZ)

def on_closing():
    print ("closing") 
    global root
    root.destroy()
    root.quit()


        
def main(args = None):
    if args is None:
        args = sys.argv
    port,baudrate = 'COM15', 38400
    #serialPort.flushInput()
    #ser.flushOutput()
    #ser.write('p');
    #ser.write('\n');
    if len(args) > 1:
        port = args[1]
    if len(args) > 2:
        baudrate = int(args[2])
    global root
    root = tk.Tk()
    #root.protocol("WM_DELETE_WINDOW", on_closing)
    root.protocol("WM_DELETE_WINDOW", on_closing)
    #self.openCOM = tk.Button(tk.frame,text="Open",command=self.open_COM)
    #self.openCOM.pack(side=LEFT)
    global myQueue
    myQueue = Queue() 
    #t1 = Thread(target=test_thread,args=("Thread-1",myQueue))    
    #t1 = Thread(target=ser_daemon.main,args=("Thread-1",myQueue))
    t1 = Thread(target=udp.main,args=("Thread-1",myQueue))
    app = App(root, "Smooth Sailing")
    #t1 = Thread(target=app.mainloop ) 
   # app.read_serial()
    t1.start()
    app.mainloop()
   # read_serial2()
    return 0

if __name__ == '__main__':
    sys.exit(main())