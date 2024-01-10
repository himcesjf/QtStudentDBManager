import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    property int studentId: 0
    width: parent.width
    height: parent.height

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField { id: firstNameField; placeholderText: "First Name" }
        TextField { id: lastNameField; placeholderText: "Last Name" }

        Button {
            text: "Update"
            onClicked: {
                studentModel.updateStudent(studentId, firstNameField.text, lastNameField.text);
                stackView.pop(); // Return to the previous view
            }
        }
    }

    Component.onCompleted: {
        // Load student data based on studentId
        var student = studentModel.getStudentById(studentId);
        if (student) {
            firstNameField.text = student.firstName;
            lastNameField.text = student.lastName;
        }
    }
}
