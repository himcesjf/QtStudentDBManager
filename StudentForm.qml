import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GridLayout {
    rowSpacing: 4
    columnSpacing: 2

    property int currentRow: -1
    property QtObject model: null

    QtObject {
        id: privateData
        property bool syncToCurrentRow: true
    }

    onCurrentRowChanged: {
        if (!privateData.syncToCurrentRow) {
            return;
        }

        if (currentRow > -1) {
            console.log("Syncing form to row:", currentRow);

            firstName.text = model.data(model.index(currentRow, 1));
            lastName.text = model.data(model.index(currentRow, 2));
            middleName.text = model.data(model.index(currentRow, 3));
            roll.text = model.data(model.index(currentRow, 4));
            className.text = model.data(model.index(currentRow, 5));
        } else {
            console.log("Clearing form.")

            firstName.text = "";
            lastName.text = "";
            middleName.text = "";
            roll.text = "";
            className.text = "";     
        }
    }

    Label { Layout.row: 0; Layout.column: 0; text: "First name:" }

    TextField {
        id: firstName
        Layout.row: 0
        Layout.column: 1
        Layout.fillWidth: true
        placeholderText: "Enter a first name ..."
    }

    Label { Layout.row: 1; Layout.column: 0; text: "Middle name:" }

    TextField {
        id: middleName
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
        placeholderText: "Enter a middle name ..."
    }

    Label { Layout.row: 2; Layout.column: 0; text: "Last name:" }

    TextField {
        id: lastName
        Layout.row: 2
        Layout.column: 1
        Layout.fillWidth: true
        placeholderText: "Enter a last name ..."
    }

    Label { Layout.row: 3; Layout.column: 0; text: "Roll:" }

    TextField {
        id: roll
        Layout.row: 3
        Layout.column: 1
        Layout.fillWidth: true
        placeholderText: "Enter a roll ..."
    }
    
    Label { Layout.row: 4; Layout.column: 0; text: "Class:" }

    TextField {
        id: className
        Layout.row: 4
        Layout.column: 1
        Layout.fillWidth: true
        placeholderText: "Enter a class ..."
    }

    Button {
        Layout.row: 5
        Layout.columnSpan: 2
        Layout.fillWidth: true
        text: form.currentRow !== -1 ? "Update" : "Add"

        onClicked: form.sync()
    }

    function sync() {
        if (currentRow < 0) {
            privateData.syncToCurrentRow = false;
            model.insertRows(model.rowCount(), 1);
        }
            
        model.setData(model.index(currentRow, 1), firstName.text);
        model.setData(model.index(currentRow, 2), lastName.text);
        model.setData(model.index(currentRow, 3), middleName.text);
        model.setData(model.index(currentRow, 4), roll.text);
        model.setData(model.index(currentRow, 5), className.text);

        model.submit();

        privateData.syncToCurrentRow = true;
    }
}
