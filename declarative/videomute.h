// -*- c++ -*-

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

#ifndef VIDEO_MUTE_H
#define VIDEO_MUTE_H

#include <QObject>

class QtCamVideoMute;
class QtCamDevice;

class VideoMute : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY stateChanged);

public:
  VideoMute(QtCamDevice *dev, QObject *parent = 0);
  ~VideoMute();

  bool isEnabled() const;
  void setEnabled(bool enabled);

signals:
  void stateChanged();

private slots:
  void prepareForDeviceChange();

private:
  QtCamVideoMute *m_mute;
};

#endif /* VIDEO_MUTE_H */
