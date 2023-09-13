#pragma once

#pragma warning(push)
#pragma warning(disable: 26819)
#pragma warning(disable: 26800)
#pragma warning(disable: 26495)
#pragma warning(disable: 26451)
#pragma warning(disable: 4244)
#pragma warning(disable: 6001)

#include <vector>
#include <array>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include <uuid_v4.h>
#include <srtparser.h>

#include <nlohmann/json.hpp>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <QSettings>
#include <QFileInfo>
#include <QVideoSink>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QtWidgets/QDialog>
#include <QGraphicsVideoItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtAdvancedStylesheet.h>

#pragma warning(pop)