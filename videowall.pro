SOURCES += main.cpp
SOURCES += customproxy.cpp
HEADERS += customproxy.h

QT += phonon opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_DEMOS]/videowall
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.png *.pro *.html *.doc images
sources.path = $$[QT_INSTALL_DEMOS]/videowall
INSTALLS += target sources
