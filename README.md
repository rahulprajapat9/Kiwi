# readme

## Pointers to browse this repo:
- src/sensor.c and src/gateway.c contains the handle communication function for sensor nodes and gateway respectively.

- includes folder contains various header files

- protocol.md describes the custom internet protocol implemented by me in this project


- TestCases includes various unit testcases, joint testcase with backend, end-to-end test cases scenarios
	- unit_testcase_sensor.c for sensor.c: faking incoming data from gateway
	- unit_testcase_gateway.c for gatway.c: faking incoming data from backend
	- pthreads : attemps to simulated envrionment using pthreads
	- joint_test_case : XML or JSON for backend compatibility


## Data Packet Specification:

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
			
			
3.  Set of commands to be implemented:
* Ping Gateway/Sensor
   * The backend wishes to see if the gateway or a specified sensor is alive,
     it expects some form of a PONG response from it.
 * Restart Gateway/Sensor
   * The backend wishes to restart the gateway or a specified sensor. In this
     one case the device does not need to send back an ACK as the restart will
     be verified out of band.
 * Add New Ki Token to Sensor
   * The backend wishes to add a new authentication token for a Ki to a
     specified sensor, this token is a 128 bit binary value, see
     `includes/sensor/ki_store.h` for functions related to adding this.
 * Remove Ki Token from Sensor
   * The backend wishes to remove an authentication token for a Ki from a
     specified sensor, this token is a 128 bit binary value, see
     `includes/sensor/ki_store.h` for functions related to removing this.
 * Open Door Connected to Sensor
   * The backend wishes to unlock the door connected to a specified sensor, see
     `includes/sensor/door.h` for functions related to this.
