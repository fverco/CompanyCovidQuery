QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/forms/employeedialog.cpp \
    src/forms/surveydialog.cpp \
    src/objects/employeetablemodel.cpp \
    src/objects/survey.cpp \
    src/objects/surveydatabase.cpp \
    src/main.cpp \
    src/forms/mainwindow.cpp \
    src/objects/surveytablemodel.cpp

HEADERS += \
    src/forms/employeedialog.h \
    src/forms/surveydialog.h \
    src/objects/employeetablemodel.h \
    src/objects/survey.h \
    src/objects/surveydatabase.h \
    src/forms/mainwindow.h \
    src/objects/surveytablemodel.h

FORMS += \
    src/forms/employeedialog.ui \
    src/forms/surveydialog.ui \
    src/forms/mainwindow.ui

TRANSLATIONS += \
    lang/CompanyCovidQuery_af_ZA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
