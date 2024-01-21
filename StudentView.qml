/*
 * QML file defining the student data view in the client application.
 * Presents student data in a tabular format and handles user interactions.
 */


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

    Connections {
        target: model

        function onErrorChanged() {
            if (model.error) {
                table.selectionModel.clear();
            }
        }
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
            // Hide Version column.
            if (col === 1) {
                return 0;
            }

            return width / (columns - 1);
        }

        delegate: Rectangle {
            implicitWidth: table.width / table.columns
            implicitHeight: 30

            required property bool current
            required property bool selected
            color: {
                if (row === table.currentRow) {
                    return palette.highlight;
                } else if (model.decoration) {
                    return "green";
                }

                return palette.base;
            }

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
