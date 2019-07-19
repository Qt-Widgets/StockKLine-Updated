#ifndef MARKETDATASPLITTER_H
#define MARKETDATASPLITTER_H

#include <QSplitter>

class MarketDataSplitter : public QSplitter
{
    Q_OBJECT

public:
    MarketDataSplitter(QWidget* parent) : QSplitter(Qt::Vertical, parent) {}

    void childKeyPressEvent(QKeyEvent* event);
    void childMouseMoveEvent(QMouseEvent* event);
    void childMousePressEvent(QMouseEvent* event);
    void childMouseReleaseEvent(QMouseEvent* event);

signals:
    void childKeyPressed(QKeyEvent* event);
    void childMouseMoved(QMouseEvent* event);
    void childMousePressed(QMouseEvent* event);
    void childMouseReleased(QMouseEvent* event);
};

#endif // MARKETDATASPLITTER_H
