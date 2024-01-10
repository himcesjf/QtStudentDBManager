import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    width: 800
    height: 600

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField { id: firstNameField; placeholderText: "First Name" }
        TextField { id: lastNameField; placeholderText: "Last Name" }
        TextField { id: middleNameField; placeholderText: "Middle Name" }
        TextField { id: rollField; placeholderText: "Roll Number" }
        TextField { id: classField; placeholderText: "Class" }
        TextField { id: marksField; placeholderText: "Marks" }

        Button {
            text: "Add"
            onClicked: {
                    studentModel.addStudent(
                    firstNameField.text,
                    lastNameField.text,
                    middleNameField.text,
                    parseInt(rollField.text),
                    classField.text,
                    parseInt(marksField.text)
                );

                console.log("Adding student with:", firstNameField.text, lastNameField.text);


                stackView.pop(); // Return to the previous view
            }
        }
    }
}
