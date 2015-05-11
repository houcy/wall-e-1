#include "section.h"
#include <QPainter>

#define MIN_HEIGHT 20
#define BACKGROUND_COLOR "#dfdfdf"

#define LEFT_RECT_COLOR "#999999"
#define LEFT_RECT_WIDHT 5

#define LINE_COLOR "#999999"
#define LINE_WIDTH 1

#define PADDING 2

#define TEXT_SIZE 10
#define TEXT_OFFSET 5

Section::Section(QWidget *parent) : QFrame(parent)
{
    setMinimumHeight(MIN_HEIGHT);
}

Section::Section(const QString &text, QWidget *parent) : QFrame(parent),
    text(text)
{
    setMinimumHeight(MIN_HEIGHT);
}

void Section::drawRect(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(BACKGROUND_COLOR));

    painter.drawRect(rect());
}

void Section::drawLeftRect(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(LEFT_RECT_COLOR));

    painter.drawRect(PADDING, PADDING, LEFT_RECT_WIDHT, rect().height() -
        2 * PADDING);
}

void Section::drawBottomLine(QPainter &painter)
{
    painter.setPen(QPen(QBrush(LINE_COLOR), LINE_WIDTH));

    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
}

void Section::drawText(QPainter &painter)
{
    painter.setPen(Qt::black);

    QFont font;
    font.setBold(true);
    font.setPointSize(TEXT_SIZE);
    QFontMetrics fm(font);
    int textWidht = fm.width(text);
    int textHeight = fm.height();
    int textX = PADDING + LEFT_RECT_WIDHT + TEXT_OFFSET;
    int textY = rect().topLeft().y() + rect().height() / 2 - textHeight / 2;

    painter.drawText(QRect(textX, textY, textWidht, textHeight), text);
}

void Section::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawRect(painter);

    drawLeftRect(painter);

    drawBottomLine(painter);

    drawText(painter);
}
