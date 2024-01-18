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

## Summary
The application facilitates a continuous data flow between the database and the GUI, mediated by the client and server over a network. 
It emphasizes careful data serialization/deserialization handling and robust error management.
