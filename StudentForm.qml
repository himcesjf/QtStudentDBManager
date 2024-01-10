import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    visible: false

    property bool isEditMode: false
    property var currentStudent: null

    function openForAdd() {
        isEditMode = false; // Set isEditMode to false for adding
        currentStudent = null
        firstNameField.text = ""
        lastNameField.text = ""
        middleNameField.text = ""
        rollField.text = ""
        classField.text = ""
        visible = true
    }

    function openForEdit(student) {
        isEditMode = true
        currentStudent = student
        firstNameField.text = student.firstName
        lastNameField.text = student.lastName
        middleNameField.text = student.middleName
        rollField.text = student.roll
        classField.text = student.class
        visible = true
    }

    Column {
        spacing: 10
        TextField {
            id: firstNameField
            placeholderText: "First Name"
        }

        TextField {
            id: lastNameField
            placeholderText: "Last Name"
        }

        TextField {
            id: middleNameField
            placeholderText: "Middle Name"
        }

        TextField {
            id: rollField
            placeholderText: "Roll Number"
            inputMethodHints: Qt.ImhDigitsOnly // Only allow digits
        }

        TextField {
            id: classField
            placeholderText: "Class"
        }


        Button {
            text: isEditMode ? "Update" : "Add"
            onClicked: {
                if (isEditMode) {
                    studentModel.updateStudent(currentStudent.id, firstNameField.text, lastNameField.text,middleNameField.text, rollField.text, classField.text)
                } else {
                    studentModel.addStudent(firstNameField.text, lastNameField.text, middleNameField.text, rollField.text, classField.text )
                }
                visible = false
            }
        }

        Button {
            text: "Cancel"
            onClicked: {
                visible = false
            }
        }
    }
}
