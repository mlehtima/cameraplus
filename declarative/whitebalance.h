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

#ifndef WHITE_BALANCE_H
#define WHITE_BALANCE_H

#include "capability.h"
#include "qtcamwhitebalance.h"

class QtCamDevice;

class WhiteBalance : public Capability {
  Q_OBJECT

  Q_PROPERTY(WhiteBalanceMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(WhiteBalanceMode);

public:
  typedef enum {
    Auto = QtCamWhiteBalance::Auto,
    Daylight = QtCamWhiteBalance::Daylight,
    Cloudy = QtCamWhiteBalance::Cloudy,
    Sunset = QtCamWhiteBalance::Sunset,
    Tungsten = QtCamWhiteBalance::Tungsten,
    Flourescent = QtCamWhiteBalance::Flourescent
  } WhiteBalanceMode;

  WhiteBalance(QtCamDevice *dev, QObject *parent = 0);
  ~WhiteBalance();

  WhiteBalanceMode value();
  void setValue(const WhiteBalanceMode& mode);

signals:
  void valueChanged();

private slots:
  void prepareForDeviceChange();

private:
  QtCamWhiteBalance *m_wb;
};

#endif /* WHITE_BALANCE_H */
