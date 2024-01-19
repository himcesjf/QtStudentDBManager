# Client-Server Application for Student Record Management

## Overview
- This project builds upon the [QtStudentDBManager network_basic project](https://github.com/himcesjf/QtStudentDBManager/tree/network_basic), taking a step up in demonstrating an advnaced client-server architecture using Qt 6.
- This also integrates the GUI of [QtStudentDBManager_gui_using_QSqlTableModel project](https://github.com/himcesjf/QtStudentDBManager/tree/gui_using_QSqlTableModel)
- This is designed to manage Student records, interfacing with a PostgreSQL database and a graphical user interface (GUI).

## Components
The application consists of two main components:
1. **Server Program**
2. **Client Program**

### Server Program
#### Purpose
- Manages Student records by saving and retrieving them from a PostgreSQL database.

#### Configuration
- Reads settings such as the listening port from an INI configuration file.

#### Network Listening
- Listens for incoming network connections.

#### Data Handling
- Receives and reads Qt serialized array of Student objects sent by the client.
- Deserializes this data into an in-memory array.
- Saves the Student records into the PostgreSQL database.

#### Separation of Concerns
- Maintains a separation between network communication and database operations.
- Interacts within these components using Qt's signal and slot mechanism.

#### Response to Client
- Sends a confirmation message ("OK") back to the client after successful data handling.

### Client Program
#### Purpose
- Sends Student data to the server for saving and awaits a response.

#### User Interface
- Utilizes a GUI - [QtStudentDBManager_gui_using_QSqlTableModel project](https://github.com/himcesjf/QtStudentDBManager/tree/gui_using_QSqlTableModel)

#### Data Sending
- Serializes Student data and sends it to the server.
- Configuration settings like server address and port are read from an INI file.

#### Response Handling
- Displays "Successfully saved" upon receiving an "OK" response from the server.
- If there's an error or a problem in connecting with the server, it shows: "Failed to save, please contact administrator".

## PostgreSQL
- This is user::password - myuser::myuser
- Database name - studentsdb

- Make sure in your postgresql.conf you enable the listen address and port
`$ sudo nano /var/lib/pgsql/data/postgresql.conf`

- And, in your pg_hga.conf make sure all the lines that say "ident" are changed to "md5"
`$ sudo nano /var/lib/pgsql/data/pg_hba.conf`

- Most imporantantly, make sure that the studentsdb is created under myuser as the table owner else it will return 42501 privileges error code. Solutions to 42601 also lies in privileges.
`$ createdb studentsdb -O myuser`

- Then restart it:
`$ sudo systemctl reload postgresql`


## Data Flow and Processing
### From Database to GUI
1. **Database to Server**: The server retrieves and serializes Student records from the database.
2. **Server to Client**: Serialized data is sent over the network to the client.
3. **Client to GUI**: The client deserializes the data and displays it in the GUI.

### From GUI to Database
1. **GUI to Client**: GUI captures and serializes user-entered or modified Student data.
2. **Client to Server**: The client sends this serialized data to the server.
3. **Server to Database**: The server deserializes and saves the data to the database.

## Additional Features
- **Listening for Connections**: The server continuously listens for incoming client connections.
- **Bidirectional Communication**: Essential for keeping the database and GUI synchronized.
- **Serialization/Deserialization**: Ensures efficient and correct data transmission over the network.
- **Error Handling**: Robust error management for network issues, database errors, and data format problems.

### Description of each Component

## Shared code:
# Settings.h:
Convenience singleton class to read/write settings values from the .ini file. Quite basic, could use some improvement like getters that fall back to defaults if stuff in the file is missing or invalid, rather than writing defaults to the file.

# Student.h/cpp
Class to store student data in memory. Basically just a data struct, but inherits from QObject for property support. Would be nicer as a Q_GADGET instead as it doesn't need all QObject features. Supports serialization/deserialization to QDataStream for network transfer. The id field defaults to -1 which the server understands as a new student not yet in the db.

# NetworkMessage.h/cpp:
Another data struct with serialization/deserialization support for network transfer. This is what client and server now exchange. It has a type field to allow them to distinguish what they are receiving and act accordingly. StatusMessage is used for messages, StorageConfirmation is used to confirm db inserts and respond with the id (important for new rows, when the client must be told the id by the server after db insert) and StudentObject is used to send student data. The payload field is a string (message), or an int (id) or a byte array (serialized student object). So, basically it's a wrapper over some data, with some metadata.

## Server code:
# main_server.cpp
The main, just instanciates a QApp, creates instances of the db and network server classes and makes signal/slot connections between them (as per the requirements).

# StudentDatabase.h/cpp:
Does the QSql database bits, like creating the table, retrieving students, inserting, updating

# StudentServer.h/cpp
TCP server, exchanges NetworkMessages with client. Still sends out students on connect, now also receives them and sends back confirmation. The confirmation contains the id of the student inserted or altered, so that the client can update its model. If the server gets an id -1 student it knows it has to insert it new and generate a new id for the client to adopt.

## Client code:
# main_client.cpp:
The main, sets up QML engine and initiates GUI

# main.qml:
Main window layout, sets up table, sidebar, etc.

# StudentView.qml:
Table view

# StudentForm.qml
Sidebar editing form

# StudentModel.h/cpp
Connects to server, grabs data, maps it to model API for consumption by GUI. Also has a slot to add/update students (and them to the server), where id -1 means a new student. Also has error and success props/signals for use by the GUI.

## Summary
The application facilitates a continuous data flow between the database and the GUI, mediated by the client and server over a network. 
It emphasizes careful data serialization/deserialization handling and robust error management.
