#ifndef CUSTOMPROXY_H
#define CUSTOMPROXY_H

#include <QtCore/qtimeline.h>
#include <QtGui/qgraphicsproxywidget.h>

class CustomProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    CustomProxy(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

    QRectF boundingRect() const;
    void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget);                          

    void setTitle( const QString& title ) {
        mTitle = title;
    }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                                          
private slots:
    void updateStep(qreal step);
    void resetZValue();

signals:
    void focusChanged(bool focus);

private:
    QTimeLine *timeLine;
    QString mTitle;
};

#endif
