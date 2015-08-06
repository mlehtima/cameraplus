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

#ifndef QT_CAM_VIEWFINDER_RENDERER_P_H
#define QT_CAM_VIEWFINDER_RENDERER_P_H

#include <QSize>
#include <QMutex>
#include "qtcamviewfinderframe.h"

class QtCamViewfinderRendererBufferInterface {
public:
  virtual void handleData(unsigned char *data,
			  const QSize& size,
			  const QtCamViewfinderFrame::Format& format) = 0;
};

class QtCamViewfinderRendererPrivate {
public:
  QtCamViewfinderRendererPrivate() :
    angle(0),
    flipped(false),
    iface(0) {

  }

  void setInterface(QtCamViewfinderRendererBufferInterface *i) {
    m_lock.lock();
    iface = i;
    m_lock.unlock();
  }

  int angle;
  bool flipped;

  QMutex m_lock;
  QtCamViewfinderRendererBufferInterface *iface;
};

#endif /* QT_CAM_VIEWFINDER_RENDERER_P_H */
