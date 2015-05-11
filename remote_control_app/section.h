#ifndef SECTION_H
#define SECTION_H

#include <QFrame>

class Section : public QFrame
{
    Q_OBJECT

    QString text;

    void drawRect(QPainter &painter);
    void drawLeftRect(QPainter &painter);
    void drawBottomLine(QPainter &painter);
    void drawText(QPainter &painter);

protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit Section(QWidget *parent = 0);
    explicit Section(const QString &text, QWidget *parent = 0);
};

#endif // SECTION_H
