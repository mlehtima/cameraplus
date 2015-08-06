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

#include "plugin.h"
#include "imagemode.h"
#include "videomode.h"
#include "camera.h"
#include "previewprovider.h"
#include "zoom.h"
#include "flash.h"
#include "scene.h"
#include "evcomp.h"
#include "videotorch.h"
#include "whitebalance.h"
#include "colortone.h"
#include "exposure.h"
#include "aperture.h"
#include "iso.h"
#include "noisereduction.h"
#include "flickerreduction.h"
#include "focus.h"
#include "autofocus.h"
#include "roi.h"
#include "videomute.h"
#include "metadata.h"
#include "imagesettings.h"
#include "resolutionmodel.h"
#include "videosettings.h"
#include "sounds.h"
#include "cameraconfig.h"
#include "videoplayer.h"
#include "viewfinder.h"
#include "capability.h"
#include "resolution.h"
#include "viewfinderhandler.h"
#include "viewfinderbufferhandler.h"
#include "viewfinderframehandler.h"
#if defined(QT4)
#include <QDeclarativeEngine>
#elif defined(QT5)
#include <QQmlEngine>
#endif

#define MAJOR 1
#define MINOR 0

DeclarativePlugin::DeclarativePlugin(QObject *parent) :
#if defined(QT4)
  QDeclarativeExtensionPlugin(parent) {
#elif defined(QT5)
  QQmlExtensionPlugin(parent) {
#endif

}

DeclarativePlugin::~DeclarativePlugin() {

}

#if defined(QT4)
void DeclarativePlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri) {
#elif defined(QT5)
void DeclarativePlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
#endif
  Q_UNUSED(uri);

  engine->addImageProvider("preview", new PreviewProvider);
}

void DeclarativePlugin::registerTypes(const char *uri) {
  Q_ASSERT(QLatin1String(uri) == QLatin1String("QtCamera"));

  qmlRegisterType<Camera>(uri, MAJOR, MINOR, "Camera");
  qmlRegisterType<ImageMode>(uri, MAJOR, MINOR, "ImageMode");
  qmlRegisterType<VideoMode>(uri, MAJOR, MINOR, "VideoMode");

  qmlRegisterUncreatableType<Zoom>(uri, MAJOR, MINOR, "Zoom", QObject::tr("Cannot create separate instance of Zoom"));
  qmlRegisterUncreatableType<Flash>(uri, MAJOR, MINOR, "Flash", QObject::tr("Cannot create separate instance of Flash"));
  qmlRegisterUncreatableType<Scene>(uri, MAJOR, MINOR, "Scene", QObject::tr("Cannot create separate instance of Scene"));
  qmlRegisterUncreatableType<EvComp>(uri, MAJOR, MINOR, "EvComp", QObject::tr("Cannot create separate instance of EvComp"));
  qmlRegisterUncreatableType<WhiteBalance>(uri, MAJOR, MINOR, "WhiteBalance", QObject::tr("Cannot create separate instance of WhiteBalance"));
  qmlRegisterUncreatableType<ColorTone>(uri, MAJOR, MINOR, "ColorTone", QObject::tr("Cannot create separate instance of ColorTone"));
  qmlRegisterUncreatableType<Exposure>(uri, MAJOR, MINOR, "Exposure", QObject::tr("Cannot create separate instance of Exposure"));
  qmlRegisterUncreatableType<Aperture>(uri, MAJOR, MINOR, "Aperture", QObject::tr("Cannot create separate instance of Aperture"));
  qmlRegisterUncreatableType<Iso>(uri, MAJOR, MINOR, "Iso", QObject::tr("Cannot create separate instance of Iso"));
  qmlRegisterUncreatableType<NoiseReduction>(uri, MAJOR, MINOR, "NoiseReduction", QObject::tr("Cannot create separate instance of NoiseReduction"));
  qmlRegisterUncreatableType<FlickerReduction>(uri, MAJOR, MINOR, "FlickerReduction", QObject::tr("Cannot create separate instance of FlickerReduction"));
  qmlRegisterUncreatableType<Focus>(uri, MAJOR, MINOR, "Focus", QObject::tr("Cannot create separate instance of Focus"));
  qmlRegisterUncreatableType<AutoFocus>(uri, MAJOR, MINOR, "AutoFocus", QObject::tr("Cannot create separate instance of AutoFocus"));
  qmlRegisterUncreatableType<Roi>(uri, MAJOR, MINOR, "Roi", QObject::tr("Cannot create separate instance of Roi"));

  qmlRegisterUncreatableType<VideoMute>(uri, MAJOR, MINOR, "VideoMute", QObject::tr("Cannot create separate instance of VideoMute"));
  qmlRegisterUncreatableType<VideoTorch>(uri, MAJOR, MINOR, "VideoTorch", QObject::tr("Cannot create separate instance of VideoTorch"));

  qmlRegisterType<MetaData>(uri, MAJOR, MINOR, "MetaData");
  qmlRegisterType<ImageSettings>(uri, MAJOR, MINOR, "ImageSettings");
  qmlRegisterType<VideoSettings>(uri, MAJOR, MINOR, "VideoSettings");
  qmlRegisterType<Sounds>(uri, MAJOR, MINOR, "Sounds");

  qmlRegisterUncreatableType<ResolutionModel>(uri, MAJOR, MINOR, "ResolutionModel",
			  "ResolutionModel can be obtained from the corresponding mode");

  qmlRegisterType<Mode>();
  qmlRegisterType<CameraConfig>(uri, MAJOR, MINOR, "CameraConfig");

  qmlRegisterType<VideoPlayer>("QtCameraExtras", MAJOR, MINOR, "VideoPlayer");
  qmlRegisterType<Viewfinder>(uri, MAJOR, MINOR, "Viewfinder");
  qmlRegisterType<Capability>();

  qmlRegisterUncreatableType<Resolution>(uri, MAJOR, MINOR, "Resolution",
					 "Resolution cannot be created manually");
  qmlRegisterType<ViewfinderBufferHandler>(uri, MAJOR, MINOR, "ViewfinderBufferHandler");
  qmlRegisterType<ViewfinderFrameHandler>(uri, MAJOR, MINOR, "ViewfinderFrameHandler");
  qmlRegisterType<ViewfinderHandler>();
}

#if defined(QT4)
Q_EXPORT_PLUGIN2(declarativeqtcamera, DeclarativePlugin);
#endif
