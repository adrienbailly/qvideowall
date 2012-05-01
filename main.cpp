#include "customproxy.h"

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/videowidget.h>
#include <phonon/seekslider.h>
#include <phonon/effect.h>

#include <QtGui>
#include <QAbstractFileEngine>

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(videowall);
    QApplication app(argc, argv);
    app.setApplicationName( "Embedded Dialogs Demo" );

    QGraphicsScene scene;

    int dimension = 3;
    QFileInfoList files;
    QDir dir;

    if (argc > 1 ) {
        dir = QDir( app.arguments().at(1) );
    } else {
        dir = QDir(".");
    }

    if (dir.exists()) {
        files = dir.entryInfoList(QStringList() << "*.wmv" << "*.mov" << "*.avi" << "*.mp4" );
    } else {
        files << app.arguments().at(1);
    }
    if (argc > 2) {
        bool ok = false;
        int newDimension = app.arguments().at(2).toInt(&ok);
        if (ok) {
            dimension = newDimension;
        }
    }

    // Degug
    qDebug() << "List of files:";
    for(int k=0; k<files.size(); ++k) {
        qDebug() << "\t" << k << ":" << files.at(k).fileName();
    }

    if (files.isEmpty())
        dimension = 0;

    int index = 0;
    for (int y = 1; y <= dimension && index < files.size(); ++y) {
        for (int x = 1; x <= dimension && index < files.size(); ++x) {
            Phonon::MediaObject *media = new Phonon::MediaObject();
            media->setCurrentSource(files.at(index).absoluteFilePath());

            for(int k=0; k<10; k++) {
                media->enqueue( Phonon::MediaSource(files.at(index).absoluteFilePath()) );
            }

            Phonon::VideoWidget *video = new Phonon::VideoWidget();
            Phonon::createPath(media, video);

            Phonon::AudioOutput *audio = new Phonon::AudioOutput();
            Phonon::createPath(media, audio);
            audio->setMuted( true );

            // Video rectangle
            ///CustomProxy *proxy = new CustomProxy(0, Qt::Window);
            CustomProxy *proxy = new CustomProxy();
            proxy->setTitle( files.at(index).baseName() );
            proxy->setWidget(video);
            QObject::connect(proxy, SIGNAL(focusChanged(bool)), audio, SLOT(setMuted(bool)));

            QRectF rect = proxy->boundingRect();
            proxy->setPos(x * rect.width() * 1.05, y * rect.height() * 1.05);
            proxy->show();
            scene.addItem(proxy);

            // Play video
            media->play();

            ////index = (index + 1) % files.size();
            index++;
        }
    }

    scene.setSceneRect(scene.itemsBoundingRect());

    QGraphicsView view(&scene);
#ifndef QT_NO_OPENGL
    view.setViewport(new QGLWidget);
#endif
    view.scale(1. / qreal(dimension), 1. / qreal(dimension));
    view.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //view.setBackgroundBrush(QPixmap(":/No-Ones-Laughing-3.jpg"));
    view.setBackgroundBrush( Qt::darkGray );
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.show();
    view.setWindowTitle("Embedded Dialogs Demo");

    return app.exec();
}
