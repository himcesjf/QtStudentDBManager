import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    title: "Student DB Manager"
    width: 800
    height: 600
    visible: true

    // ListView for browsing students
    ListView {
        id: listView
        anchors.fill: parent
        model: studentModel

        delegate: Item {
            width: listView.width
            height: 50
            RowLayout {
                Text { text: "ID: " + studentId }
                Text { text: "Name: " + firstName + " " + lastName }

                Button {
                    text: "Edit"
                    onClicked: {
                        studentForm.openForEdit(model.studentId)
                    }
                }
            }
        }
    }

    // Button to open the form for adding a new student
    Button {
        text: "Add Student"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: {
            studentForm.openForAdd()
        }
    }

    // Student form for adding/editing students
    StudentForm {
        id: studentForm
        anchors.fill: parent
        visible: false
    }
}
