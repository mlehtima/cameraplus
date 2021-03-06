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

#ifndef CAMERA_RESOURCES_H
#define CAMERA_RESOURCES_H

#include <QObject>
#include <QThread>
#include <policy/resource-set.h>

class CameraResourcesWorker;

class CameraResources : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool acquired READ acquired NOTIFY acquiredChanged);
  Q_PROPERTY(bool hijacked READ hijacked NOTIFY hijackedChanged);
  Q_PROPERTY(bool scaleAcquired READ isScaleAcquired NOTIFY scaleAcquisitionChanged);

  Q_ENUMS(Mode);
  Q_ENUMS(ResourceType);

public:
  typedef enum {
    None,
    Image,
    Video,
    Recording,
    Player,
  } Mode;

  CameraResources(QObject *parent = 0);
  ~CameraResources();

  Q_INVOKABLE bool acquire(const Mode& mode);

  bool acquired() const;
  bool hijacked() const;
  bool isScaleAcquired() const;

signals:
  void acquiredChanged();
  void hijackedChanged();
  void updated();
  void scaleAcquisitionChanged();

private:
  bool isResourceGranted(const ResourcePolicy::ResourceType& resource) const;

  CameraResourcesWorker *m_worker;
  QThread m_thread;
};

class CameraResourcesWorker : public QObject {
  Q_OBJECT

public:
  CameraResourcesWorker(QObject *parent = 0);
  ~CameraResourcesWorker();

public slots:
  void acquire(bool *ok, const CameraResources::Mode& mode);
  void acquired(bool *ok);
  void hijacked(bool *ok);
  void isResourceGranted(bool *ok, int resource);

signals:
  void acquiredChanged();
  void hijackedChanged();
  void updated();

private slots:
  void init();

  void resourcesReleased();
  void lostResources();
  void resourcesGranted(const QList<ResourcePolicy::ResourceType>& optional);
  void resourcesDenied();

private:
  bool release();

  bool updateSet(const QList<ResourcePolicy::ResourceType>& required,
		 const QList<ResourcePolicy::ResourceType>& optional =
		 QList<ResourcePolicy::ResourceType>());

  QList<ResourcePolicy::ResourceType> listSet();

  void setAcquired(bool acquired);
  void setHijacked(bool hijacked);

  ResourcePolicy::ResourceSet *m_set;

  CameraResources::Mode m_mode;
  bool m_acquired;
  bool m_acquiring;
  bool m_hijacked;
};

#endif /* CAMERA_RESOURCES_H */
