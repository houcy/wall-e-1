#include "sliding_stacked_widget.h"
#include "log.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

SlidingStackedWidget::SlidingStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    // Parent should not be 0; not tested for any other case yet!
    if (parent)
        mainWindow = parent;
    else
    {
        mainWindow = this;
        warning("Untested mainwindow case !");
    }

    vertical = false;
    speed = 500;
    //check out the QEasingCurve documentation for different styles
    animationType = QEasingCurve::OutBack;
    now = 0;
    next = 0;
    wrap = false;
    pNow = QPoint(0,0);
    active = false;
}

void SlidingStackedWidget::setVerticalMode(bool vertical) noexcept
{
    this->vertical = vertical;
}

void SlidingStackedWidget::setSpeed(int speed) noexcept
{
    this->speed = speed;
}

void SlidingStackedWidget::setAnimation(QEasingCurve::Type animationType)
    noexcept
{
    this->animationType = animationType;
}

void SlidingStackedWidget::setWrap(bool wrap) noexcept
{
    this->wrap = wrap;
}

void SlidingStackedWidget::slideInNext()
{
    int now = currentIndex();

    if (wrap || (now < count() - 1))
        slideInIdx(now + 1);
}


void SlidingStackedWidget::slideInPrev()
{
    int now = currentIndex();

    if (wrap || (now > 0))
        slideInIdx(now - 1);
}

void SlidingStackedWidget::slideInIdx(int idx, Direction direction)
{
    if (idx > count() - 1)
    {
        direction = vertical ? TOP2BOTTOM : RIGHT2LEFT;
        idx = idx % count();
    }
    else if (idx < 0)
    {
        direction = vertical ? BOTTOM2TOP: LEFT2RIGHT;
        idx = (idx + count()) % count();
    }
    slideInWgt(widget(idx), direction);
}


void SlidingStackedWidget::slideInWgt(QWidget * newWidget, Direction direction)
{
    if (active)
    {
        // At the moment, do not allow re-entrance before an animation is
        // completed. Other possibility may be to finish the previous animation
        // abrupt, or to revert the previous animation with a counter
        // animation, before going ahead or to revert the previous animation
        // abrupt and all those only, if the newwidget is not the same as that
        // of the previous running animation.
        return;
    }
    else
        active=true;

    Direction directionHint;
    int now = currentIndex();
    int next = indexOf(newWidget);
    if (now == next)
    {
        active = false;
        return;
    }
    else if (now < next)
        directionHint = vertical ? TOP2BOTTOM : RIGHT2LEFT;
    else
        directionHint=vertical ? BOTTOM2TOP : LEFT2RIGHT;

    if (direction == AUTOMATIC)
        direction = directionHint;

    int offsetX = frameRect().width();
    int offsetY = frameRect().height();

    // The following is important, to ensure that the new widget
    // has correct geometry information when sliding in first time
    widget(next)->setGeometry(0, 0, offsetX, offsetY);

    if (direction == BOTTOM2TOP)
    {
        offsetX = 0;
        offsetY = -offsetY;
    }
    else if (direction == TOP2BOTTOM)
        offsetX = 0;
    else if (direction == RIGHT2LEFT)
    {
        offsetX = -offsetX;
        offsetY = 0;
    }
    else if (direction == LEFT2RIGHT)
        offsetY = 0;

    // Re-position the next widget outside/aside of the display area
    QPoint pNext = widget(next)->pos();
    QPoint pNow = widget(now)->pos();
    pNow = pNow;

    widget(next)->move(pNext.x() - offsetX, pNext.y() - offsetY);
    widget(next)->show();
    widget(next)->raise();

    // Animate both, the now and next widget to the side, using animation framework
    QPropertyAnimation *animNow = new QPropertyAnimation(widget(now), "pos");

    animNow->setDuration(speed);
    animNow->setEasingCurve(animationType);
    animNow->setStartValue(QPoint(pNow.x(), pNow.y()));
    animNow->setEndValue(QPoint(offsetX + pNow.x(), offsetY + pNow.y()));
    QPropertyAnimation *animNext = new QPropertyAnimation(widget(next), "pos");
    animNext->setDuration(speed);
    animNext->setEasingCurve(animationType);
    animNext->setStartValue(QPoint(-offsetX + pNext.x(), offsetY + pNext.y()));
    animNext->setEndValue(QPoint(pNext.x(), pNext.y()));

    QParallelAnimationGroup *animGroup = new QParallelAnimationGroup;

    animGroup->addAnimation(animNow);
    animGroup->addAnimation(animNext);

    QObject::connect(animGroup, SIGNAL(finished()), this, SLOT(animationDoneSlot()));
    this->next = next;
    this->now = now;
    active = true;
    animGroup->start();

    // Note: the rest is done via a connect from the animation ready;
    // animation->finished() provides a signal when animation is done;
    // so we connect this to some post processing slot,
    // that we implement here below in animationDoneSlot.
}

void SlidingStackedWidget::animationDoneSlot()
{
    // When ready, call the QStackedWidget slot setCurrentIndex(int)
    setCurrentIndex(next);
    // Then hide the outshifted widget now, and  (may be done already
    // implicitely by QStackedWidget)
    widget(now)->hide();
    // Then set the position of the outshifted widget now back to its original
    widget(now)->move(pNow);

    active = false;
    emit animationFinished();
}

bool SlidingStackedWidget::isAnimationActive() const
{
    return active;
}
