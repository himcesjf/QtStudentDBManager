# Simple Client-Server Message Exchange Application

## Overview
This project consists of a basic client-server application built using Qt. The server listens for incoming connections and exchanges string messages with the client. This application serves as a foundational example for network communication in Qt.

## Components
The application is divided into two main components:
1. **Server** (`server.cpp`)
2. **Client** (`client.cpp`)

## Functionality
- The **server** establishes a listening socket using `QTcpServer` and waits for client connections.
- The **client** connects to the server and initiates message exchange.
- Both client and server are capable of sending and receiving string messages.
- Messages are printed to the console on both ends upon receipt.

## Building the Application
The project uses CMake to build two separate executables for the client and server.

### Prerequisites
- Qt framework installed and configured.
- CMake build system.

### Build Instructions
1. Run `cmake` in the project directory to generate build files.
2. Use `make` or an equivalent build command to compile the executables.

## Running the Application
1. Open two terminals.
2. In the first terminal, run the server executable.
3. In the second terminal, run the client executable.
4. Observe the messages being exchanged and printed on both terminals.

## Files
- `server.cpp`: Contains the main function and logic for the server application.
- `client.cpp`: Contains the main function and logic for the client application.
- `CMakeLists.txt`: CMake configuration file to build the executables.

## Notes
- Default IP and port settings are used. Modify them in the source files as necessary for different network configurations.
- This application is intended for demonstration purposes and serves as a starting point for more complex client-server applications.
