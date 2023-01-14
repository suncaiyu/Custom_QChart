import QtQuick 2.0

Item {
    property SkinBase current: light
    property SkinBase dark : dark
    property SkinBase light : light
    SkinDark {
        id :dark;
    }
    SkinLight{
        id :light
    }
}
