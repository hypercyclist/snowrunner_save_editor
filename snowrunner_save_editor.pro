QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    EditorWindow.cpp

HEADERS += \
    EditorWindow.h \
    rapidjson/allocators.h \
    rapidjson/cursorstreamwrapper.h \
    rapidjson/document.h \
    rapidjson/encodedstream.h \
    rapidjson/encodings.h \
    rapidjson/error/en.h \
    rapidjson/error/error.h \
    rapidjson/filereadstream.h \
    rapidjson/filewritestream.h \
    rapidjson/fwd.h \
    rapidjson/internal/biginteger.h \
    rapidjson/internal/clzll.h \
    rapidjson/internal/diyfp.h \
    rapidjson/internal/dtoa.h \
    rapidjson/internal/ieee754.h \
    rapidjson/internal/itoa.h \
    rapidjson/internal/meta.h \
    rapidjson/internal/pow10.h \
    rapidjson/internal/regex.h \
    rapidjson/internal/stack.h \
    rapidjson/internal/strfunc.h \
    rapidjson/internal/strtod.h \
    rapidjson/internal/swap.h \
    rapidjson/istreamwrapper.h \
    rapidjson/memorybuffer.h \
    rapidjson/memorystream.h \
    rapidjson/msinttypes/inttypes.h \
    rapidjson/msinttypes/stdint.h \
    rapidjson/ostreamwrapper.h \
    rapidjson/pointer.h \
    rapidjson/prettywriter.h \
    rapidjson/rapidjson.h \
    rapidjson/reader.h \
    rapidjson/schema.h \
    rapidjson/stream.h \
    rapidjson/stringbuffer.h \
    rapidjson/uri.h \
    rapidjson/writer.h

FORMS += \
    EditorWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc
