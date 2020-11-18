/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.2

Page {

    function setRssiLabel(rssi) {
        console.log("QQQQ setting RSSI label");
        lbl_rssi.text = "RSSI: " + rssi;
    }
    
    function onProximityChange(proximity_indicator) {
        console.log("XXXX onProximityChange called:" + proximity_indicator);
        proximity_band_indicator.background = proximity_colours[proximity_indicator];
    }
    
    function enableConnect() {
        btn_connect.enabled = true;
    }

    function disableConnect() {
        btn_connect.enabled = false;
    }

    Container {
        topPadding: 20
        bottomPadding: 20
        leftPadding: 10
        rightPadding: 10
        Label {
            text: selected_device_address.text + " " + selected_device_name.text
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            id: lbl_alert_level
            text: "Alert Level: 0"
            textStyle.base: SystemDefaults.TextStyles.BodyText
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            text: "PROXIMITY"
            textStyle.base: SystemDefaults.TextStyles.BodyText
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            id: lbl_rssi
            text: "RSSI: "
            textStyle.base: SystemDefaults.TextStyles.BodyText
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            horizontalAlignment: HorizontalAlignment.Center
        }

        Container {
            id: proximity_band_indicator
            layoutProperties: StackLayoutProperties {
                spaceQuota: 3
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            background: Color.Green
            opacity: 0.6
        }

        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 5
            }
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Button {
                text: "Low"
                onClicked: {
                    sendAlertOff();
                    lbl_alert_level.text = "Alert Level: 0";
                }
            }
            Button {
                text: "Mid"
                onClicked: {
                    sendAlertMid();
                    lbl_alert_level.text = "Alert Level: 1";
                }
            }
            Button {
                text: "High"
                onClicked: {
                    sendAlertHigh();
                    lbl_alert_level.text = "Alert Level: 2";
                }
            }
        }
        Container {
            layoutProperties: StackLayoutProperties {
            }
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                text: "Share"
                textStyle.base: SystemDefaults.TextStyles.BodyText
            }
            ToggleButton {
                horizontalAlignment: HorizontalAlignment.Center
                onCheckedChanged: {
                    if (checked) {
                        startSharing();
                    } else {
                        stopSharing();
                    }
                }
            }
        }
        Label {
            id: lbl_ui_message
            text: ui_message.text
            textStyle.base: SystemDefaults.TextStyles.BodyText
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        Button {
            text: "Make a Noise"
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                sendAlertRequest();
            }
        }
        Button {
            id: btn_connect
            text: "Connect"
            horizontalAlignment: HorizontalAlignment.Center
            enabled: false
            onClicked: {
                monitorProximity();
            }
        }
    }

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Back"
            onTriggered: {
                console.log("QQQQ disconnecting then going back");
                _bt.disconnect();
                navigationPane.pop();
            }
        }
    }
}
