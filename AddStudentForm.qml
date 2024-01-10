import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    width: parent.width
    height: parent.height

    Column {
        anchors.centerIn: parent
        spacing: 10

        // Text Fields for student details
        TextField { id: firstNameField; placeholderText: "First Name" }
        TextField { id: lastNameField; placeholderText: "Last Name" }
        TextField { id: middleNameField; placeholderText: "Middle Name" }
        TextField { id: rollNumberField; placeholderText: "Roll Number" }
        TextField { id: classField; placeholderText: "Class" }
        TextField { id: marksField; placeholderText: "Marks" }

        // Button to add student
        Button {
            text: "Add"
            onClicked: {
                studentModel.addStudent({
                    "firstName": firstNameField.text,
                    "lastName": lastNameField.text,
                    "middleName": middleNameField.text,
                    "rollNumber": rollNumberField.text,
                    "class": classField.text,
                    "marks": marksField.text,
                });
                stackView.pop(); // Return to the previous view
            }
        }
    }
}
