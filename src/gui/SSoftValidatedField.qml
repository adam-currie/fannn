import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

//todo: change underline color to red when text is invalid
STextField {
    id: field
    property string _prevText: ""
    property string _errorText
    property string _textBeforeEdit
    property bool _editing: false

    //overload me, return error text
    function set(v) { return ""; }

    onEditingFinished: {
        if (_errorText) {
            text = _textBeforeEdit
        }
        _editing = false
        _errorText = ""
    }

    onTextChanged: {
        if (activeFocus) {
            if (!_editing) {
                _editing = true;
                _textBeforeEdit = _prevText
            }
            _errorText = set(text)
        } else {
            //if we don't have activeFocus than we are setting this artificially,
            //so it shouldnt effect edit state
            let err = set(text)
            if (err) {
                text = _prevText
            }
        }

        _prevText = text
    }

    Warning {
        y: field.height
        topPadding: -9
        leftPadding: field.leftPadding
        text: _errorText
        visible: text !== ""
    }
}
