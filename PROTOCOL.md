# Kiwi Backbone Communication Protocol

1. _Protocol between Back-end and Gateway/Sensor_
2. _Protocol between Gateway and Sensor_



**Protocol Specification**

18th June, 2017





By

**Rahul Prajapat**





















# Introduction

This communication protocol has been defined and prepared to facilitate communication between back-end (cloud) and hardware (embedded devices running on bare metal). There are gateways and sensor devices in Kiwi&#39;s infrastructure backbone which needs to be controlled remotely by Kiwi control centre (/ back-end).

# Kiwi Backbone Topology

Kiwi backbone is a star-of-stars topology. The central back end is connected to number of gateways over the internet. These gateways are further connected to number of Sensor Nodes over a custom 868 MHz protocol.

# Role of various devices in the backbone

Back-end should be able to control all the devices in the backbone. The gateway device is there for providing internet connectivity to a number of sensors. Main device in Kiwi backbone is the sensor node which is connected to a door and is capable of authenticating the user and further opening this door.

# Data Transmission Path:

Kiwi Back-end data format --&gt; API to convert Kiwi Back-end data format to Binary Data --&gt; Back-end / Cloud exit point --&gt; Gateway\_Modem --&gt; Gateway\_custom\_868M\_wireless --&gt; Sensor\_custom\_868M\_wireless

# Data Format

## Back end to Gateway Data Flow

### Device
- Modem (internet enabled)

### Size of payload
- Payload size is maximum 128 bytes. Maximum size required for this assignment is 17 Bytes for specific set of commands.

### Relation to other Protocols
- Since back end and gateway is connected over internet, so application layer protocols like HTTP (XML/JSON/Binary) and transport layer protocols like TCP/UDP will be supported.

### Data Payload Format
    1. It is a binary format.
      1. Device ID (16 Bytes)
        1. Initial 16 bytes describes the device id which the command has to be executed
      2. Command (1 byte)
        1. This is a task which back end wishes to perform on a specific device in the backbone. It can be ping(), restart\_device()etc as specified for this specific assignment.
        2. This command field can have up to 256 types of commands as per this design.
        3. Mapping of the commands: Commands are mapped to binary data and stored in the database. They are explained in detail in next section.
    2. Features
      1. Since all the devices in the Kiwi backbone, be it a gateway device or a sensor node, has unique feature i.e. their device ID, so whatever task back end wishes to perform, they have to very first mention the related device id. Once device id is know, gateway figures out on its own whether this device id belongs to any of the sensor nodes connected to it or to itself. Hence back end needs not to be worried about reaching to a specific device in the backbone.
      2. As far as backend is implementing the correct rules as described in Kiwi Communication Protocol Specification document, they don&#39;t have to comply with data format. The gateway will handle invalid data being sent to it using an data format conversion api.
    3. Data format conversion needs
      1. Let us assume that Backend has its own format and wishes to perform specific set of tasks over the Kiwi Backbone Communication Protocol, then there has to be an API which converts the commands into a binary serial data before sending it to gateway.

## Backend to Sensor Node via Gateway

### Backend to Gateway
    1. Device
      1. Modem (internet enabled)
    2. Size of Payload
      1. Payload size is maximum 128 bytes. Maximum size required for this assignment is 33 Bytes for specific set of commands.
    3. Data Payload Format
      1. It is a binary format.
        1. Device ID (16 Bytes)
          1. Initial 16 bytes describes the device id which the command has to be executed
        2. Command (1 byte)
          1. This is a task which back end wishes to perform on a specific device in the backbone. It can be ping(), restart\_device(), add\_ki, remove\_ki, open\_door etc as specified for this specific assignment.
          2. Rest features are same as described above.
        3. Authentication Token Ki (16 Byte)
          1. This token ki field is of the size 128 bits / 16 bytes which has to be added/removed from a particular sensor node. Note that, as per Kiwi Backbone Communication Protocol, the payload exiting from backend exit point can be either 17 bytes long or 33 bytes long. Depending upon the device id, third token ki field will be filled or will be NULL.

### Gateway to Sensor
    1. Device
      1. 868MHz custom wireless module
    2. Size of Payload
      1. Maximum size is 32 bytes. Maximum required for this assignment = 17 Bytes. Please note that the gateway knows which sensor node to communicate with, so device id of that sensor node is not needed here.
    3. Data Payload Format
      1. It is a binary format.
        1. Command (1 byte)
          1. Described as above
        2. Authentication Token Ki (16 Byte)
          1. Described as above

## Sensor to Backend Data Flow
    1. Data Payload Format
	1. Device id of the sensor (16 bytes)
	2. Data of variable size (Max is MODEM_MAX_PAYLOAD_LENGTH-16)


# Command Mapping to Binary Data
As explained above, there is a data field &quot;command&quot; which is 1 byte long. In this section, the commands are mapped to a specific 1 byte long integer. Device will send back an acknowledgement (Pong()) corresponding to every command except command 2 as described below.
## Ping Gateway/Sensor
    1. Command value is 1
      1. To see if the gateway or a specified sensor is alive, backend will receive a PONG response (ack) from the device which is of 32 byte long constant binary data. Every byte of this 32 byte long PONG payload is 0x01 (hex).
## Restart Gateway/Sensor
    1. Command value is 2
      1. To restart the gateway or a specified sensor. Back end will not receive any PONG response since the device does not need to send back an ACK as the restart will be verified out of band.
## Add New Ki Token to Sensor Node
    1. Command value is 3
      1. To add a new authentication token for a Ki to a specified sensor, this token is a 128 bit binary value. In case of any payload whose device id is of a gateway&#39;s device id, this field will be NULL.
## Remove Ki Token from Sensor Node
    1. Command value is 4
      1. To remove an authentication token for a Ki from a specified sensor, this token is a 128 bit binary value. In case of any payload whose device id is of a gateway&#39;s device id, this field will be NULL.
## Open Door Connected to Sensor Node
    1. Command value is 5
      1. To unlock the door connected to a specified sensor.