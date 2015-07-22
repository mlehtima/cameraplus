// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

import QtQuick 2.0

Item {
    id: captureControl

    property bool capturePressed: false
    property bool captureKeyPressed: false
    property bool zoomPressed: false
    property bool proximityClosed: false
    property bool canceled: false
    property bool showCancelBanner: (zoomPressed || proximityClosed || captureKeyPressed) && state == "capturing"
    property bool enable: true
    property bool capturing: state == "capturing"

    signal startCapture
    signal cancelCapture

    states: [
        State {
            name: "idle"
            when: !capturePressed && !zoomPressed && !proximityClosed && !captureKeyPressed
        },
        State {
            name: "canceled"
            when: canceled
        },
        State {
            name: "capturing"
            when: capturePressed || zoomPressed || proximityClosed || captureKeyPressed
        }
    ]

    state: "idle"

    onStateChanged: {
        if (state == "idle") {
            captureControl.canceled = false
        }
    }

    onEnableChanged: captureControl.canceled = !enabled

    transitions: [
        Transition {
            from: "capturing"
            to: "idle"
            ScriptAction {
                script: captureControl.startCapture()
            }
        },
        Transition {
            from: "capturing"
            to: "canceled"
            ScriptAction {
                script: captureControl.cancelCapture()
            }
        }
    ]
}
