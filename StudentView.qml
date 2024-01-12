import QtQuick
import QtQuick.Controls

Item {
    id: view

    property QtObject selectionModel: selectionModel

    property alias currentRow: table.currentRow
    property alias model: table.model

    HorizontalHeaderView {
        id: header
        syncView: table
        clip: true
    }

    TableView {
        id: table

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        clip: true
        interactive: true

        rowSpacing: 1
        columnSpacing: 1

        columnWidthProvider: function(col) {
            return width / columns;
        }

        delegate: Rectangle {
            implicitWidth: table.width / table.columns
            implicitHeight: 30

            required property bool current
            required property bool selected
            color: row === table.currentRow ? palette.highlight : palette.base
            
            Label {
                x: 4
                anchors.verticalCenter: parent.verticalCenter
                text: display 
            }
        }

        selectionBehavior: TableView.SelectionDisabled
        selectionMode: TableView.SingleSelection
        selectionModel: ItemSelectionModel { id: selectionModel }

        TapHandler {
            id: tapHandler
            enabled: table.currentRow !== -1
            onTapped: (eventPoint, button) =>  {
                if (!table.childAt(eventPoint.x, eventPoint.y)) {
                    table.selectionModel.clear();
                }     
            }
        }
    }
}