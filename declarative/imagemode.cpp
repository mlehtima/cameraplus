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

#include "imagemode.h"
#include "qtcamimagemode.h"
#include "qtcamdevice.h"
#include "camera.h"
#include "resolution.h"

ImageMode::ImageMode(QObject *parent) :
  Mode(parent),
  m_image(0),
  m_fastCaptureEnabled(false) {

}

ImageMode::~ImageMode() {
  m_image = 0;
}


bool ImageMode::capture(const QString& fileName) {
  return m_image ? m_image->capture(fileName) : false;
}

void ImageMode::preChangeMode() {
  if (m_image) {
    QObject::disconnect(m_image, SIGNAL(captureStarted()), this, SIGNAL(captureStarted()));
    QObject::disconnect(m_image, SIGNAL(captureEnded()), this, SIGNAL(captureEnded()));
  }

  m_image = 0;
}

void ImageMode::postChangeMode() {
  m_image = m_cam->device()->imageMode();

  if (m_image) {
    QObject::connect(m_image, SIGNAL(captureStarted()), this, SIGNAL(captureStarted()));
    QObject::connect(m_image, SIGNAL(captureEnded()), this, SIGNAL(captureEnded()));
  }
}

void ImageMode::changeMode() {
  m_mode = m_cam->device()->imageMode();
}

bool ImageMode::enableFastCapture() {
  if (m_fastCaptureEnabled) {
    return true;
  }

  if (m_image) {
    m_fastCaptureEnabled = m_image->enableFastCapture();
  } else {
    m_fastCaptureEnabled = false;
  }

  emit fastCaptureEnabledChanged();

  return m_fastCaptureEnabled;
}

void ImageMode::disableFastCapture() {
  if (!m_fastCaptureEnabled) {
    return;
  }

  if (m_image) {
    m_image->disableFastCapture();
  }

  m_fastCaptureEnabled = false;

  emit fastCaptureEnabledChanged();
}

bool ImageMode::isFastCaptureEnabled() const {
  return m_fastCaptureEnabled;
}

Resolution *ImageMode::resolution() {
  if (m_cam && m_cam->device()) {
    return new Resolution(m_cam->device()->imageMode()->currentResolution());
  }

  return 0;
}
