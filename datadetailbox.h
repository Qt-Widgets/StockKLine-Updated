#ifndef DATADETAILBOX_H
#define DATADETAILBOX_H

#include <QWidget>

class DataDetailBox : public QWidget
{
    Q_OBJECT

public:
    explicit DataDetailBox(QWidget* parent);
    void virtual paintEvent(QPaintEvent* event);

private:
    int marginTop = 100;

    int height = 100;
    int width = 70;

};

#endif // DATADETAILBOX_H
