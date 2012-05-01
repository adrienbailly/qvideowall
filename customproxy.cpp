#include "customproxy.h"

#include <QtGui>

CustomProxy::CustomProxy(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent, wFlags)
{
    timeLine = new QTimeLine(250, this);
    connect(timeLine, SIGNAL(valueChanged(qreal)),
            this, SLOT(updateStep(qreal)));
    connect(timeLine, SIGNAL(finished()),
            this, SLOT(resetZValue()));

    //we never cache the video
    setCacheMode(NoCache);
}

QRectF CustomProxy::boundingRect() const
{
    return QGraphicsProxyWidget::boundingRect().adjusted(-40, -40, 40, 40);
}

void CustomProxy::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget)
{
    QRectF br = boundingRect();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 128));
    painter->drawPolygon(QPolygonF(br.adjusted(10, 10, 10, 10)).intersected(br));

    painter->setPen( Qt::white );
    painter->drawRect( boundingRect() );

    QFont font("Courier New");
    font.setWeight( QFont::DemiBold );
    font.setPixelSize( 30 );
    painter->setFont( font );

    QRectF text( br );
    text.setTop( text.bottom()-40.0 );
    painter->drawText( text, mTitle, QTextOption(Qt::AlignCenter) );
    painter->drawRect( text );

    QGraphicsProxyWidget::paintWindowFrame(painter, option, widget);
}

void CustomProxy::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverEnterEvent(event);
    scene()->setActiveWindow(this);
    setZValue(1);
    if (timeLine->direction() != QTimeLine::Forward)
        timeLine->setDirection(QTimeLine::Forward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();

    emit focusChanged( false );
    //qDebug() << mTitle << "focusChanged( true )";
}
        
void CustomProxy::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverLeaveEvent(event);
    if (timeLine->direction() != QTimeLine::Backward)
        timeLine->setDirection(QTimeLine::Backward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();

    emit focusChanged( true );
    //qDebug() << mTitle << "focusChanged( false )";
}
        
void CustomProxy::updateStep(qreal step)
{
    QRectF r = boundingRect();
    ///widget()->setWindowOpacity(1. - step/3.);
    setTransform(QTransform()
                 .translate(r.width() / 2, r.height() / 2)
                 .rotate(step * 30, Qt::XAxis)
                 .rotate(step * 10, Qt::YAxis)
                 .rotate(step * 5, Qt::ZAxis)
                 .scale(1 + step, 1 + step)
                 .translate(-r.width() / 2, -r.height() / 2));
}

void CustomProxy::resetZValue()
{
    if (timeLine->direction() == QTimeLine::Backward)
        setZValue(0);
}
