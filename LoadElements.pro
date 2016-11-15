QT += core
QT -= gui

CONFIG += c++14

TARGET = HiAsm_loadElements
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

#Информация о приложении
CONFIG += embed_manifest_exe
QMAKE_TARGET_COMPANY = "HiAsm community(c)"
QMAKE_TARGET_DESCRIPTION = "HiAsm loader"
QMAKE_TARGET_COPYRIGHT = "CriDos"
QMAKE_TARGET_PRODUCT = "HiAsm loader"
#RC_ICONS = "res/icon/icon.ico"
RC_LANG = "0x419"
VERSION = "1.0.0.0" #Версия приложения, ваш Кэп:)

DEFINES += APP_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
           APP_DESCRIPTION=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
           APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
           APP_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
           APP_VERSION=\"\\\"$$VERSION\\\"\"

SOURCES += $$files($$PWD/"entry/*.cpp", true)
HEADERS += $$files($$PWD/"entry/*.h", true)

#Подключаем HiAsm_engine
include(HiAsm_engine/HiAsm_engine.pri)
