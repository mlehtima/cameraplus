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

#ifndef QT_CAM_MODE_P_H
#define QT_CAM_MODE_P_H

#include <QSize>
#include <QFileInfo>
#include <QDir>
#include "qtcamdevice_p.h"
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>
#include "qtcamgstmessagehandler.h"

#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

#if GST_CHECK_VERSION(1,0,0)
#define PREVIEW_CAPS "video/x-raw, format = (string)BGRx, width = (int) %1, height = (int) %2"
#else
#define PREVIEW_CAPS "video/x-raw-rgb, width = (int) %1, height = (int) %2, bpp = (int) 32, depth = (int) 24, red_mask = (int) 65280, green_mask = (int) 16711680, blue_mask = (int) -16777216"
#endif

#define CAPS_NO_FPS "%s, width=(int)%d,height=(int)%d"
#define CAPS_FPS "%s, width=(int)%d,height=(int)%d,framerate=(fraction)[%d/%d,%d/%d]"

class QtCamDevicePrivate;
class PreviewImageHandler;
class DoneHandler;

class QtCamModePrivate {
public:
  QtCamModePrivate(QtCamDevicePrivate *d) : id(-1), dev(d) {}
  virtual ~QtCamModePrivate() {}

  void init(DoneHandler *handler) {
    doneHandler = handler;
  }

  int modeId(const char *mode) {
    if (!dev->cameraBin) {
      return -1;
    }

    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(dev->cameraBin),
						     "mode");
    if (!pspec) {
      return -1;
    }

    if (!G_IS_PARAM_SPEC_ENUM(pspec)) {
      return -1;
    }

    GParamSpecEnum *e = G_PARAM_SPEC_ENUM(pspec);
    GEnumClass *klass = e->enum_class;

    for (unsigned x = 0; x < klass->n_values; x++) {
      if (QLatin1String(mode) == QLatin1String(klass->values[x].value_nick)) {
	return klass->values[x].value;
      }
    }

    return -1;
  }

  GstEncodingProfile *loadProfile(const QString& path, const QString& name) {
    GError *error = NULL;
    QString targetPath;
    QFileInfo info(path);
    if (!info.isAbsolute()) {
      targetPath = dev->conf->lookUp(path);
    }
    else {
      targetPath = info.filePath();
    }

    GstEncodingTarget *target = gst_encoding_target_load_from_file(targetPath.toUtf8().constData(),
								   &error);
    if (!target) {
      qCritical() << "Failed to load encoding target from" << path << error->message;
      g_error_free(error);
      return 0;
    }

    GstEncodingProfile *profile = gst_encoding_target_get_profile(target, name.toUtf8().data());
    if (!profile) {
      qCritical() << "Failed to load encoding profile from" << path;
      gst_encoding_target_unref(target);
      return 0;
    }

    gst_encoding_target_unref(target);

    return profile;
  }

  void resetCaps(const char *property) {
    if (!dev->cameraBin) {
      return;
    }

    g_object_set(dev->cameraBin, property, NULL, NULL);
  }

  bool inNightMode() {
    if (!dev->cameraBin) {
      return false;
    }

    int val = 0;

    g_object_get(dev->videoSource, "scene-mode", &val, NULL);

    return val == GST_PHOTOGRAPHY_SCENE_MODE_NIGHT;
  }

  void setCaps(const char *property, const QSize& resolution, int fps) {
    QString mediaType = dev->conf->mediaType(property);
    QByteArray arr = mediaType.toLatin1();
    const gchar *media = arr.isEmpty() ? NULL : arr.constData();

    QString format = dev->conf->mediaFourcc(property);
    QByteArray mediaArr = format.toLatin1();

#if GST_CHECK_VERSION(1,0,0)
    const gchar *fourcc = mediaArr.isEmpty() ? NULL : mediaArr.constData();
#else
    unsigned long fourcc = GST_STR_FOURCC(mediaArr.constData());
#endif

    if (!dev->cameraBin) {
      return;
    }

    if (resolution.width() <= 0 || resolution.height() <= 0) {
      return;
    }

    GstCaps *caps = 0;

    if (fps <= 0) {
      gchar *tpl = g_strdup_printf (CAPS_NO_FPS, media,
				    resolution.width(),
				    resolution.height());
      caps = gst_caps_from_string (tpl);
      g_free (tpl);
    }
    else {
      gchar *tpl = g_strdup_printf (CAPS_FPS, media,
				    resolution.width(),
				    resolution.height(),
				    fps - 1, 1, fps + 1, 1);
      caps = gst_caps_from_string (tpl);
      g_free (tpl);
    }

    if (fourcc) {
#if GST_CHECK_VERSION(1,0,0)
      gst_caps_set_simple (caps, "format", G_TYPE_STRING, fourcc, NULL);
#else
      gst_caps_set_simple (caps, "format", GST_TYPE_FOURCC, fourcc, NULL);
#endif
    }

    GstCaps *old = 0;

    g_object_get(dev->cameraBin, property, &old, NULL);

    if (gst_caps_is_equal(caps, old)) {
      gst_caps_unref(old);
      gst_caps_unref(caps);

      return;
    }

    g_object_set(dev->cameraBin, property, caps, NULL);

    if (old) {
      gst_caps_unref(old);
    }
  }

  void setPreviewSize(const QSize& size) {
    if (!dev->cameraBin) {
      return;
    }

    if (size.width() <= 0 && size.height() <= 0) {
      g_object_set(dev->cameraBin, "preview-caps", NULL, "post-previews", FALSE, NULL);
    }
    else {
      QString preview = QString(PREVIEW_CAPS).arg(size.width()).arg(size.height());

      GstCaps *caps = gst_caps_from_string(preview.toLatin1());

      if (!dev->conf->isPreviewSupported()) {
	qWarning() << "Cannot set preview caps. Preview not supported";
	gst_caps_unref(caps);
	return;
      }

      g_object_set(dev->cameraBin, "preview-caps", caps, "post-previews", TRUE, NULL);

      gst_caps_unref(caps);
    }
  }

  void setFileName(const QString& file) {
    fileName = file;
  }

  void setTempFileName(const QString& file) {
    tempFileName = file;
  }

  void enableViewfinderFilters() {
    if (dev->viewfinderFilters) {
      if (!dev->conf->viewfinderFiltersUseAnalysisBin()) {
	qWarning() << "Cannot enable viewfinder filters without a controlling bin";
	return;
      }

      g_object_set(dev->viewfinderFilters, "bypass", FALSE, NULL);
    }
  }

  void disableViewfinderFilters() {
    if (dev->viewfinderFilters) {
      if (!dev->conf->viewfinderFiltersUseAnalysisBin()) {
	qWarning() << "Cannot disable viewfinder filters without a controlling bin";
	return;
      }

      g_object_set(dev->viewfinderFilters, "bypass", TRUE, NULL);
    }
  }

  int id;
  QtCamMode *q_ptr;
  QtCamDevicePrivate *dev;
  PreviewImageHandler *previewImageHandler;
  DoneHandler *doneHandler;
  QString fileName;
  QString tempFileName;
};

class DoneHandler : public QtCamGstMessageHandler {
public:
  DoneHandler(QtCamModePrivate *m, const char *done, QObject *parent = 0) :
    QtCamGstMessageHandler(done, parent) {
    mode = m;
  }

  virtual ~DoneHandler() { }

  virtual void handleMessage(GstMessage *message) {
    // If we have a temp file then we rename it:
    if (!mode->tempFileName.isEmpty() && !mode->fileName.isEmpty()) {
      if (!QFile::rename(mode->tempFileName, mode->fileName)) {
	qCritical() << "Failed to rename" << mode->tempFileName << "to" << mode->fileName;
      }
    }

    QString fileName;
    const GstStructure *s = gst_message_get_structure(message);
    if (gst_structure_has_field(s, "filename")) {
      const char *str = gst_structure_get_string(s, "filename");
      if (str) {
	fileName = QString::fromUtf8(str);
      }
    }

    if (fileName.isEmpty()) {
      fileName = mode->fileName;
    }

    QMetaObject::invokeMethod(mode->q_ptr, "saved", Qt::QueuedConnection,
			      Q_ARG(QString, fileName));
  }

  QtCamModePrivate *mode;
};

#endif /* QT_CAM_MODE_P_H */
