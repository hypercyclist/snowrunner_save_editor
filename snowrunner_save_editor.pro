QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AppConfig.cpp \
    CompleteTasksTable.cpp \
    CompleteUpgradesTable.cpp \
    Database.cpp \
    GameAtlas.cpp \
    InformationWidget.cpp \
    Localization.cpp \
    Map.cpp \
    Region.cpp \
    SaveFile.cpp \
    TableFilterByRegionMap.cpp \
    Task.cpp \
    Truck.cpp \
    Upgrade.cpp \
    Utils.cpp \
    main.cpp \
    EditorWindow.cpp

HEADERS += \
    AppConfig.h \
    CompleteTasksTable.h \
    CompleteUpgradesTable.h \
    Database.h \
    EditorWindow.h \
    GameAtlas.h \
    InformationWidget.h \
    Localization.h \
    Map.h \
    Region.h \
    SaveFile.h \
    TableFilterByRegionMap.h \
    Task.h \
    Truck.h \
    Upgrade.h \
    Utils.h \
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
    rapidjson/writer.h \
    rapidxml/rapidxml.hpp \
    rapidxml/rapidxml_iterators.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml_utils.hpp

FORMS += \
    EditorWindow.ui \
    InformationWidget.ui \
    TableFilterByRegionMap.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc
