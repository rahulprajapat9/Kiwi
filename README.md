# Kiwi

Data Packet Specification:

1. Backend to Gateway Data Flow
	- Size
		- Max = 128 bytes. Maximum required for this assignment = 33 Bytes.
	- Protocol
		- Since cellular device, so HTTP (XML/JSON/Binary), TCP/IP etc supported.
	- Data Packet Format
		- I am assuming that backend guys are converting the commands into a binary data before sending it to gateway/sensor.
			- Data format (binary)
				- Device ID upon which the command has to be executed (16 byte)
				- Command (1 byte)
				- Authentication Token Ki which has to be added/removed from a particular sensor (16)

2. Gateway to Sensor Data Flow
	- Size
		- Max = 32 bytes. Maximum required for this assignment = 17 Bytes.
	- Data Packet Format
		- Data format (binary)
			- Command (1 byte)
			- Authentication Token Ki which has to be added/removed from a particular sensor (16)