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

#ifndef QT_CAM_GST_MESSAGE_HANDLER_H
#define QT_CAM_GST_MESSAGE_HANDLER_H

#include <QObject>

class QtCamGstMessageHandlerPrivate;
typedef struct _GstMessage GstMessage;

class QtCamGstMessageHandler : public QObject {
  Q_OBJECT

public:
  QtCamGstMessageHandler(const QString& messageName, QObject *parent = 0);
  virtual ~QtCamGstMessageHandler();

  QString messageName() const;

  virtual void handleMessage(GstMessage *message);

signals:
  void messageSent(GstMessage *message);

private:
  QtCamGstMessageHandlerPrivate *d_ptr;
};

#endif /* QT_CAM_GST_MESSAGE_HANDLER_H */
