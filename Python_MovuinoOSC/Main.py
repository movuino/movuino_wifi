import sys
import time
import numpy as np
import OSC_communication as osc
import Movuino_Streamer as mov


def main(args = None):

	print "Hello Unity"
	# Initialize streaming thread
	streamer= mov.DataStreamer()

	# Initializa data collection and run the thread
	streamer.start_newCollection() 

	osc_client = osc.OSCclient('127.0.0.1', 7000) # Init client communication on specific Ip and port

	# Start OSCServer
	print "Starting OSCServer"
	osc_server = osc.OSCserver('127.0.0.1', 5000) # Init server communication on specific Ip and port
	osc_server.addListener("test") # add listener on address "test"
	osc_server.addListener("fader") # add listener on address "fader"
	
	timer0 = time.time()
	timer1 = timer0
	#while (timer1-timer0 < 5):
	while True:	
		timer1 = time.time()
		time.sleep(0.05)

		print osc_server.get_CurrentMessage()
		print "---------------"

		# Return current data received
		accX = streamer.get_lastData()[1]
		accY = streamer.get_lastData()[2]
		accZ = streamer.get_lastData()[3]
		gyrX = streamer.get_lastData()[4]
		gyrY = streamer.get_lastData()[5]
		gyrZ = streamer.get_lastData()[6]

		osc_client.sendOSCMessage('accX', accX)
		osc_client.sendOSCMessage('accY', accY)
		osc_client.sendOSCMessage('accZ', accZ)
		# osc_client.sendOSCMessage('gyrX', gyrX)
		# osc_client.sendOSCMessage('gyrY', gyrY)
		# osc_client.sendOSCMessage('gyrZ', gyrZ)
		print "---------------"

	# Return all data received since last collection initialization
	print streamer.get_dataCollection()

	osc_server.closeServer() # ERROR MESSAGE but close the OSC server without killing the app
	streamer.stop() # Stop streaming
	streamer.join() # Close thread

if __name__ == '__main__':
    sys.exit(main())