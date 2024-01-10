import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    width: parent.width
    height: parent.height

    ListView {
        id: studentsList
        anchors.fill: parent
        model: studentModel

        delegate: Item {
            width: parent.width
            height: 40

            Row {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                Text { text: model.firstName + " " + model.lastName }
                Button {
                    text: "Edit"
                    onClicked: stackView.push({ item: "UpdateStudentForm.qml", properties: { studentId: model.id }})
                }
            }
        }
    }
}
