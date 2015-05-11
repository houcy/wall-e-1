#include "animated_button.h"
#include <QEvent>
#include <QMouseEvent>

#define DEFAULT_BUTTON_WIDTH 40
#define DEFAULT_BUTTON_HEIGHT 40

AnimatedButton::AnimatedButton(const QString &normalImagePath,
    const QString &hoveredImagePath, const QString &pressedImagePath,
    QWidget *parent) : QPushButton(parent),
    iconNormal(std::move(std::make_shared< QIcon >(normalImagePath))),
    iconHovered(std::move(std::make_shared< QIcon >(hoveredImagePath))),
    iconPressed(std::move(std::make_shared< QIcon >(pressedImagePath)))
{
    isMouseHover = false;
    isSetPressed = false;
    setButtonIcon(false);
    setIconSize(QSize(DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT));

    connect(this, SIGNAL(toggled(bool)), this, SLOT(setButtonIcon(bool)));
}

void AnimatedButton::setButtonIcon(bool pressed)
{
    std::shared_ptr< QIcon > icon;

    if (pressed || isSetPressed || isChecked())
        icon = iconPressed;
    else if (isMouseHover)
        icon = iconHovered;
    else
        icon = iconNormal;

    setIcon(*icon);
}

bool AnimatedButton::event(QEvent* e)
{
    if (e->type() == QEvent::Enter)
    {
        isMouseHover = true;
        setButtonIcon(false);
    }
    else if (e->type() == QEvent::Leave)
    {
        isMouseHover = false;
        setButtonIcon(false);
    }

    return QPushButton::event(e);
}

void AnimatedButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        setButtonIcon(true);

    QPushButton::mousePressEvent(e);
}

void AnimatedButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        setButtonIcon(false);

    QPushButton::mouseReleaseEvent(e);
}

void AnimatedButton::setDown(bool down)
{
    isSetPressed = down;
    setButtonIcon(down);

    QPushButton::setDown(down);
}
