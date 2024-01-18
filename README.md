# QtPostgresApp

## Overview
QtPostgresApp is a simple Qt 6 command-line interface (CLI) application that demonstrates basic interactions with a PostgreSQL database. It creates a table, inserts data into it, and then queries and prints the data.

## Requirements
- Qt 6
- PostgreSQL
- A PostgreSQL driver compatible with Qt (QPSQL)

## Configuration
Before running the application, ensure you have a PostgreSQL database accessible and configure the following details in `main.cpp`:
- Host (default: localhost)
- Port (default: 5432)
- Database Name
- Username
- Password

## Functionality
The application performs the following operations:
1. Connects to the specified PostgreSQL database.
2. Creates a table named `greetings`.
3. Inserts sample data ('Hello', 'World') into the `greetings` table.
4. Queries and prints all rows from the `greetings` table.

## Troubleshooting
If you encounter permission issues related to database operations, ensure that the PostgreSQL user specified in the application has the necessary privileges to create tables and insert data in the specified database.
