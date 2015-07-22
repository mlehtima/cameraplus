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
import CameraPlus 1.0

Item {
    id: focusKeyHandler
    property bool focusKeyPressed: false

    signal pressed()
    signal released()

    property bool active: rootWindow.active

    function handlePress() {
        if (!focusKeyHandler.active || focusKeyHandler.focusKeyPressed) {
            return
        }

        focusKeyHandler.focusKeyPressed = true
        focusKeyHandler.pressed()
    }

    function handleRelease() {
        if (!focusKeyHandler.active || !focusKeyHandler.focusKeyPressed) {
            return
        }

        focusKeyHandler.focusKeyPressed = false

        focusKeyHandler.released()
    }

    Connections {
        id: focusKeyConnection
        target: keys

        onActiveChanged: {
            if (!focusKeyConnection.active) {
                focusKeyHandler.focusKeyPressed = false
            }
        }

        onCameraFocusPressed: focusKeyHandler.handlePress()
        onCameraFocusReleased: focusKeyHandler.handleRelease()
    }

    Connections {
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active) {
                focusKeyHandler.focusKeyPressed = false
            }
        }
    }
}
