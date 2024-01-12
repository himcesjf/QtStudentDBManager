# Client-Server Application for Student Display

## Overview
- This project builds upon the [QtStudentDBManager network_minimal project](https://github.com/himcesjf/QtStudentDBManager/tree/network_minimal), taking a step up in demonstrating a basic client-server architecture using Qt 6.
- Focusing on network communication and GUI display, it showcases a networked data exchange where a server sends serialized student data to a client for visualization. 
- Highlights key concepts in Qt network programming, dynamic memory management, and model-view design patterns. 

## Components
- **Server Application**: Manages a collection of `Student` objects and transmits them to connected clients.
- **Client Application**: Receives `Student` data, deserializes it, and displays it in a GUI using `QTableView`.

## Data Flow and Processing
1. **Server Side**:
   - Holds `Student` data within a `QVector`.
   - Serializes `Student` objects into a byte stream for network transmission using `QDataStream`.
   - Sends this data to connected client applications.
2. **Client Side**:
   - Connects to the server and requests data.
   - Receives and deserializes the byte stream back into `Student` objects in a `QVector`.
   - Utilizes `StudentModel`, a subclass of `QAbstractTableModel`, to manage and display data
   - Displays the data in a `QTableView`.

## Additional Features
- **Memory Management**: Uses dynamic allocation for `Student` objects to optimize memory usage.
- **Configurable Settings**: Server IP and port can be adjusted in the `config.ini` file, enhancing flexibility.
- **Custom Serialization**: Implements custom methods for serializing and deserializing `Student` objects, demonstrating data handling in a networked environment.
