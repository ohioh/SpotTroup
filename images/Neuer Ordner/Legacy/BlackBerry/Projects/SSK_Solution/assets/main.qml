import bb.cascades 1.2
import bb.multimedia 1.0

NavigationPane {
    id: navigationPane

    property variant proximity_colours: [ Color.Green, Color.Yellow, Color.Red ]
    property variant proximityPage

    function onRssi(proximity_indicator, rssi) {
        console.log("QQQQ onRssi called: rssi=" + rssi + ",proximity_band=" + proximity_indicator);
        proximityPage.setRssiLabel(rssi);
    }

    function onProximityChange(proximity_indicator) {
        console.log("QQQQ onProximityChange called:" + proximity_indicator);
        proximityPage.onProximityChange(proximity_indicator);
    }

    function linkEstablished() {
        console.log("QQQQ linkEstablished");
        proximityPage.disableConnect();
    }

    function lostLink() {
        console.log("QQQQ lostLink");
        setMessage("Link Lost");
        proximityPage.enableConnect();
        console.log("QQQQ Sounding Link Lost Alarm");
        if (audioPlayer.play() != MediaError.None) {
            console.log("XXXX error attempting to play sound");
        }
    }

    function setMessage(message) {
        console.log("QQQQ " + message);
        ui_message.text = message;
    }

    function sensitivity_text(sensitivity_value) {
        if (sensitivity_value < 4) {
            return "LOW";
        }
        if (sensitivity_value < 7) {
            return "MEDIUM";
        }
        return "HIGH";
    }

    function sendAlertOff() {
        _bt.sendAlertOff();
    }

    function sendAlertMid() {
        _bt.sendAlertMid();
    }

    function sendAlertHigh() {
        _bt.sendAlertHigh();
    }
    
    function sendAlertRequest() {
        _bt.sendAlertRequest();
    }
    
    function startSharing() {
        console.log("startSharing");
        _app.startSharing();
    }

    function stopSharing() {
        console.log("stopSharing")
        _app.stopSharing();
    }

    function monitorProximity() {
        _app.monitorProximity(selected_device_address.text,selected_device_name.text);
    }
    
    Page {
        actionBarVisibility: ChromeVisibility.Visible
        Container {
            layout: StackLayout {
            }
            verticalAlignment: VerticalAlignment.Fill
            Label {
                id: lblHeading_scan
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                }
                text: qsTr("Proximity Devices")
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                id: lblSelect
                text: "Please select a device"
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    fontFamily: "Courier"
                    lineHeight: 1.1
                }
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                id: selected_device_address
                visible: false
            }
            Label {
                id: selected_device_name
                visible: false
            }
            Label {
                id: ui_message
                visible: false
            }
            Container {
                layout: DockLayout {
                }
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                bottomPadding: 20
                ListView {
                    dataModel: _bt.deviceListing.model
                    listItemComponents: [
                        ListItemComponent {
                            type: "listItem"
                            StandardListItem {
                                title: ListItemData.deviceName
                            }
                        }
                    ]
                    onTriggered: {
                        var selectedItem = dataModel.data(indexPath);
                        proximityPage = proximityDefn.createObject();
                        selected_device_address.text = selectedItem.deviceAddress;
                        selected_device_name.text = selectedItem.deviceName;
                        _bt.setRemoteDevice(selected_device_address.text);
                        _app.monitorProximity(selected_device_address.text, selected_device_name.text);
                        console.log("QQQQ PUSHING PROXIMITY PAGE");
                        navigationPane.push(proximityPage);
                    }
                    function itemType(data, indexPath) {
                        if (indexPath.length == 1) {
                            return "header";
                        } else {
                            return "listItem";
                        }
                    }
                }
                ActivityIndicator {
                    id: activityIndicator
                    preferredWidth: 300
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                }
            }
        }

        attachedObjects: [
            ComponentDefinition {
                id: proximityDefn
                source: "proximity.qml"
            },
            MediaPlayer {
                id: audioPlayer
                sourceUrl: "asset:///sounds/beep.wav"
            }
        ]
    }
    onPopTransitionEnded: {
        page.destroy();
    }
}
