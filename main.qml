import QtQuick 2.12
import QtQuick.Controls 2.12
import com.mycompany.student 1.0

ApplicationWindow {
    title: "Student Management System"
    width: 800
    height: 600
    visible: true

    StudentModel {
        id: studentModel
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "StudentsListView.qml"
    }

    Button {
        text: "Add Student"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        onClicked: stackView.push("AddStudentForm.qml")
    }
}
