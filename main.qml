import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.student.manager 1.0

ApplicationWindow {
    id: window

    width: 800
    height: 600

    visible: true

    title: "Student Manager"

    StudentView {
        id: view

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: parent.width - drawer.width

        model: StudentModel { 
            id: studentModel 

            onRowsInserted: view.selectionModel.setCurrentIndex(index(rowCount() - 1, 0),
                ItemSelectionModel.SelectCurrent)
        }
    }

    Drawer {
        id: drawer

        width: parent.width * 0.33
        height: parent.height

        visible: true
        edge: Qt.RightEdge
        interactive: false
        modal: false

        ColumnLayout {
            y: 4
            width: parent.width

            StudentForm {
                id: form

                currentRow: view.currentRow
                model: view.model

                Layout.fillWidth: true
                Layout.leftMargin: parent.y
                Layout.rightMargin: parent.y
            }
        }
    }
}
