/*
 * QML file for the student data entry form in the client application.
 * Provides UI components for adding and editing student records.
 */



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
        property int currentId: -1
    }
    
    Connections {
        target: model
        
        function onStorageSuccess() {
            sync();
            successMsg.visible = true;
        }
    }

    onCurrentRowChanged: {
        successMsg.visible = false;
        sync();
    }
    
    function sync() {        
        if (currentRow > -1) {
            privateData.currentId = model.data(model.index(currentRow, 0));
            firstName.text = model.data(model.index(currentRow, 1));
            lastName.text = model.data(model.index(currentRow, 2));
            middleName.text = model.data(model.index(currentRow, 3));
            roll.text = model.data(model.index(currentRow, 4));
            className.text = model.data(model.index(currentRow, 5));
        } else {
            console.log("Clearing form.")

            privateData.currentId = -1;
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

        onClicked: {
            successMsg.visible = false;
            model.updateStudent(privateData.currentId, firstName.text,
                lastName.text, middleName.text, roll.text, className.text);
        }
    }
    
    Text {
        id: successMsg
        Layout.row: 6
        Layout.fillWidth: true
        visible: false
        text: "Successfully saved"
        color: "green"
        wrapMode: Text.Wrap
    }
    
    Text {
        id: errorMsg
        Layout.row: 7
        Layout.fillWidth: true
        visible: model.error
        text: model.errorString
        color: "red"
        wrapMode: Text.Wrap
    }
}
