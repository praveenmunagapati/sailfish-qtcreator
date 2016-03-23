TEMPLATE = aux

STATIC_BASE = $$PWD

# files/folders that are conditionally "deployed" to the build directory
DATA_DIRS = \
    welcomescreen \
    examplebrowser \
    snippets \
    templates \
    themes \
    designer \
    schemes \
    styles \
    rss \
    debugger \
    qmldesigner \
    qmlicons \
    qml \
    qml-type-descriptions \
    modeleditor \
    generic-highlighter \
    glsl \
    cplusplus
macx: DATA_DIRS += scripts

for(data_dir, DATA_DIRS) {
    files = $$files($$PWD/$$data_dir/*, true)
    # Info.plist.in are handled below
    for(file, files):!contains(file, ".*/Info\\.plist\\.in$"):!exists($$file/*): \
        STATIC_FILES += $$file
}

include(static.pri)

SRCRESOURCEDIR = $$IDE_SOURCE_TREE/src/share/qtcreator/
defineReplace(stripSrcResourceDir) {
    win32 {
        !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
    } else {
        !contains(1, ^/.*):1 = $$OUT_PWD/$$1
    }
    out = $$clean_path($$1)
    out ~= s|^$$re_escape($$SRCRESOURCEDIR)||$$i_flag
    return($$out)
}

# files that are to be unconditionally "deployed" to the build dir from src/share to share
DATA_DIRS = \
    externaltools
DATA_FILES_SRC = \
    externaltools/lrelease.xml \
    externaltools/lupdate.xml \
    externaltools/sort.xml \
    externaltools/qmlviewer.xml \
    externaltools/qmlscene.xml
unix {
    macx:DATA_FILES_SRC += externaltools/vi_mac.xml
    else:DATA_FILES_SRC += externaltools/vi.xml
} else {
    DATA_FILES_SRC += externaltools/notepad_win.xml
}
for(file, DATA_FILES_SRC):DATA_FILES += $${SRCRESOURCEDIR}$$file
unconditionalCopy2build.input = DATA_FILES
unconditionalCopy2build.output = $$IDE_DATA_PATH/${QMAKE_FUNC_FILE_IN_stripSrcResourceDir}
isEmpty(vcproj):unconditionalCopy2build.variable_out = PRE_TARGETDEPS
win32:unconditionalCopy2build.commands = $$QMAKE_COPY \"${QMAKE_FILE_IN}\" \"${QMAKE_FILE_OUT}\"
unix:unconditionalCopy2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
unconditionalCopy2build.name = COPY ${QMAKE_FILE_IN}
unconditionalCopy2build.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += unconditionalCopy2build

!macx {
    for(data_dir, DATA_DIRS) {
        $${data_dir}.files = $$IDE_DATA_PATH/$$data_dir
        $${data_dir}.path = $$INSTALL_DATA_PATH
        $${data_dir}.CONFIG += no_check_exist
        INSTALLS += $$data_dir
    }
}
