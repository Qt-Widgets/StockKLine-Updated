#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include "topbacktestingmenu.h"

TopBacktestingMenu::TopBacktestingMenu(QWidget* parent)
    : backTestConfig(BacktestingConfig::instance()), AutoGrid (parent, false)
{
    this->setFixedHeight(barHeight);

    QFont ft;
    ft.setPointSize(10);
    this->setFont(ft);

    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(50, 0, 0, 0);
    hbox->setSpacing(1);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);

    QLabel* productLabel = new QLabel(QStringLiteral("产品: "));
    productLabel->setPalette(palette);
    productLabel->setMaximumWidth(50);
    productLabel->setMinimumWidth(50);
    hbox->addWidget(productLabel);

    QLabel* enableCapitalAjdustmentLabel = new QLabel(QStringLiteral("调整: "));
    enableCapitalAjdustmentLabel->setPalette(palette);
    hbox->addWidget(enableCapitalAjdustmentLabel);
    enableCapitalAjdustmentCheckBox = new QCheckBox();
    enableCapitalAjdustmentCheckBox->setPalette(palette);
    enableCapitalAjdustmentCheckBox->setChecked(backTestConfig->enableCapitalAjdustment);
    hbox->addWidget(enableCapitalAjdustmentCheckBox);

    QLabel* avgIntervalLabel = new QLabel(QStringLiteral("   图中均线: "));
    avgIntervalLabel->setPalette(palette);
    hbox->addWidget(avgIntervalLabel);

    avgIntervalEdit = new QLineEdit();
    avgIntervalEdit->setStyleSheet("border: 1px solid red; background-color: black;");
    avgIntervalEdit->setMaximumWidth(50);
    avgIntervalEdit->setMaximumHeight(barHeight - 4);
    avgIntervalEdit->setText(QString::number(250));
    palette.setColor(QPalette::Text, Qt::white);
    avgIntervalEdit->setPalette(palette);
    hbox->addWidget(avgIntervalEdit);

    capitalPeriodEdit = appendLabelEdit(hbox, QStringLiteral("回测均线: "), palette);
    capitalPeriodEdit->setText(QString::number(backTestConfig->capitalPeriod));
    baseLotEdit = appendLabelEdit(hbox, QStringLiteral("基础单: "), palette);
    baseLotEdit->setText(QString::number(backTestConfig->baseLot));

    negThreshold3Edit = appendLabelEdit(hbox, QStringLiteral("负值3(%%): "), palette);
    negThreshold3Edit->setText(QString::number(backTestConfig->negThreshold3));
    negLotThreshold3Edit = appendEdit(hbox, palette);
    negLotThreshold3Edit->setText(QString::number(backTestConfig->negLotThreshold3));

    negThreshold2Edit = appendLabelEdit(hbox, QStringLiteral("负值2(%%): "), palette);
    negThreshold2Edit->setText(QString::number(backTestConfig->negThreshold2));
    negLotThreshold2Edit = appendEdit(hbox, palette);
    negLotThreshold2Edit->setText(QString::number(backTestConfig->negLotThreshold2));

    negThreshold1Edit = appendLabelEdit(hbox, QStringLiteral("负值1(%%): "), palette);
    negThreshold1Edit->setText(QString::number(backTestConfig->negThreshold1));
    negLotThreshold1Edit = appendEdit(hbox, palette);
    negLotThreshold1Edit->setText(QString::number(backTestConfig->negLotThreshold1));

    posThreshold1Edit = appendLabelEdit(hbox, QStringLiteral("正值1(%%): "), palette);
    posThreshold1Edit->setText(QString::number(backTestConfig->posThreshold1));
    posLotThreshold1Edit = appendEdit(hbox, palette);
    posLotThreshold1Edit->setText(QString::number(backTestConfig->posLotThreshold1));

    posThreshold2Edit = appendLabelEdit(hbox, QStringLiteral("正值2(%%): "), palette);
    posThreshold2Edit->setText(QString::number(backTestConfig->posThreshold2));
    posLotThreshold2Edit = appendEdit(hbox, palette);
    posLotThreshold2Edit->setText(QString::number(backTestConfig->posLotThreshold2));

    posThreshold3Edit = appendLabelEdit(hbox, QStringLiteral("正值3(%%): "), palette);
    posThreshold3Edit->setText(QString::number(backTestConfig->posThreshold3));
    posLotThreshold3Edit = appendEdit(hbox, palette);
    posLotThreshold3Edit->setText(QString::number(backTestConfig->posLotThreshold3));

    posThreshold4Edit = appendLabelEdit(hbox, QStringLiteral("正值4(%%): "), palette);
    posThreshold4Edit->setText(QString::number(backTestConfig->posThreshold4));
    posLotThreshold4Edit = appendEdit(hbox, palette);
    posLotThreshold4Edit->setText(QString::number(backTestConfig->posLotThreshold4));

    posThreshold5Edit = appendLabelEdit(hbox, QStringLiteral("正值5(%%): "), palette);
    posThreshold5Edit->setText(QString::number(backTestConfig->posThreshold5));
    posLotThreshold5Edit = appendEdit(hbox, palette);
    posLotThreshold5Edit->setText(QString::number(backTestConfig->posLotThreshold5));

    runTestButton = new QPushButton(QStringLiteral("运行回测"));
    hbox->addWidget(runTestButton);

    hbox->addStretch(1);
    this->setLayout(hbox);
}

QLineEdit* TopBacktestingMenu::appendLabelEdit(QHBoxLayout* hbox, QString labelString, QPalette& palette)
{
    QLabel* label = new QLabel(labelString);
    label->setPalette(palette);
    hbox->addWidget(label);

    QLineEdit* edit = appendEdit(hbox, palette);

    return edit;
}

QLineEdit* TopBacktestingMenu::appendEdit(QHBoxLayout* hbox, QPalette& palette)
{
    QLineEdit* edit = new QLineEdit();
    edit->setStyleSheet("border: 1px solid red; background-color: black;");
    edit->setMaximumWidth(30);
    edit->setMaximumHeight(barHeight - 4);
    palette.setColor(QPalette::Text, Qt::white);
    edit->setPalette(palette);
    hbox->addWidget(edit);

    return edit;
}

QPushButton *TopBacktestingMenu::getRunTestButton() const
{
    return runTestButton;
}

QCheckBox *TopBacktestingMenu::getEnableCapitalAjdustmentCheckBox() const
{
    return enableCapitalAjdustmentCheckBox;
}

QLineEdit *TopBacktestingMenu::getBaseLotEdit() const
{
    return baseLotEdit;
}

QLineEdit *TopBacktestingMenu::getNegLotThreshold3Edit() const
{
    return negLotThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getNegLotThreshold2Edit() const
{
    return negLotThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getNegLotThreshold1Edit() const
{
    return negLotThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getPosLotThreshold5Edit() const
{
    return posLotThreshold5Edit;
}

QLineEdit *TopBacktestingMenu::getPosLotThreshold4Edit() const
{
    return posLotThreshold4Edit;
}

QLineEdit *TopBacktestingMenu::getPosLotThreshold3Edit() const
{
    return posLotThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getPosLotThreshold2Edit() const
{
    return posLotThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getPosLotThreshold1Edit() const
{
    return posLotThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold3Edit() const
{
    return negThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold2Edit() const
{
    return negThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold1Edit() const
{
    return negThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold5Edit() const
{
    return posThreshold5Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold4Edit() const
{
    return posThreshold4Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold3Edit() const
{
    return posThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold2Edit() const
{
    return posThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold1Edit() const
{
    return posThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getCapitalPeriodEdit() const
{
    return capitalPeriodEdit;
}

QLineEdit* TopBacktestingMenu::getAvgIntervalEdit()
{
    return avgIntervalEdit;
}

TopBacktestingMenu::~TopBacktestingMenu()
{
    if (!avgIntervalEdit) {
        delete avgIntervalEdit;
    }
}

void TopBacktestingMenu::paintEvent(QPaintEvent* event)
{
    drawBottomLine();
}

void TopBacktestingMenu::drawBottomLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawLine(0, barHeight - 1, getGridWidth() + getMarginLeft() + getMarginRight(), barHeight - 1);
}
