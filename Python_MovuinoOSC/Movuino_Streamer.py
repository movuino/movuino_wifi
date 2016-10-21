#!/usr/bin/python
#
# Read stream of lines from an Arduino through wifi connection
# Store data into collection array (dataCollection)
# At any time you can
#      -  reset the collection with                           start_newCollection() 
#      -  get all data of the current collection with         get_dataCollection()
#      -  get current data stream from the Movuino with       get_lastData()
#
# /!\ Beware of the data format received :
#      -  getMotion6 in the Movuino firmware will send 7 values : index, accX, accY, accZ, gyrX, gyrY, gyrZ
#      -  getMotion9 in the Movuino firmware will send 10 values : index, accX, accY, accZ, gyrX, gyrY, gyrZ, mX, mY, mZ

import sys
from threading import Thread
from socket import *
import traceback
import time
import numpy as np


#####################################################################
#######################    DATA STREAMER    #########################
#####################################################################

class DataStreamer(Thread):
    # Recieve Movuino data through wifi connection
    def __init__(self):
        Thread.__init__(self)

        try:
            self.serverSocket = socket(AF_INET, SOCK_DGRAM)
            self.serverSocket.bind(('', 2390))
            print "Wifi connection with Movuino"
        except:
            print "No wifi connection, no Movuino, no data, nothing"

        #Initialize variables
        self.dataCollection = np.empty((1,1))
        self.isStreaming = True

    def run(self):
        #Get first data values
        message, address = self.serverSocket.recvfrom(1024) #receive data
        new_data = message.split(" ") # split data 

        if len(new_data)== 7: # check data format
            self.dataCollection = np.array([new_data]) #initialize dataCollection array
        else:
            print "Wrong data shape"

        #Get data stream values 
        while self.isStreaming:
            try: 
                message, address = self.serverSocket.recvfrom(1024) #receive data
                new_data = message.split(" ") # split data 

                if len(new_data)== 7: # check data format
                    self.dataCollection = np.concatenate((self.dataCollection, np.array([new_data])),axis=0) #add current data to dataCollection
                else:
                    print "Wrong data shape"
            except:
                traceback.print_exc()


    def start_newCollection(self):
        #Launch thread if not already running and reset dataCollection array

        # Initialization
        self.dataCollection = np.empty((1,7)) # reset data collection array
        self.isStreaming = True # turn ON data collect
        
        try:
            # case where thread is not already running (>> self.isAlive() = False)
            self.start()
            print "Stream thread launch with new data collection"
        except:
            # case where thread is not already running (>> self.isAlive() = True)
            print "Data collection reset"

    def get_dataCollection(self):
        #return all data of the current collection
        return self.dataCollection

    def get_lastData(self):
        #return last data received from Movuino data stream (= current data from Movuino)
        return self.dataCollection[-1,:]

    def stop(self):
        self.isStreaming = False # stop data collect (into the self.run() function)
        time.sleep(0.2)
        print 'Good bye Movuino'


#####################################################################
######################          MAIN          #######################
#####################################################################
def main(args = None):

    #############  EXAMPLES  #############
    # Initialize streaming thread
    streamer=DataStreamer()

    # Initializa data collection and run the thread
    streamer.start_newCollection() 

    time.sleep(2) # collecting data ...

    # Return current data received
    print streamer.get_lastData()

    # Return all data received since last collection initialization
    print streamer.get_dataCollection()

    # Start a new collection
    streamer.start_newCollection() 

    time.sleep(1) # collecting data ...
    
    # Return all data received since collection initialization
    print streamer.get_dataCollection()
    
    
    streamer.stop() # Stop streaming
    streamer.join() # Close thread
    
    ######################################

    # ADD CLOSE SOCKET IN STOP() FUNCTION ??? ESPECIALLY TO HANDLE CRASH


if __name__ == '__main__':
    sys.exit(main())