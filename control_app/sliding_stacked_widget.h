#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QEasingCurve>

class SlidingStackedWidget : public QStackedWidget
{
    Q_OBJECT

protected:
   QWidget *mainWindow;
   int speed;
   QEasingCurve::Type animationType;
   bool vertical;
   int now;
   int next;
   bool wrap;
   QPoint pNow;
   bool active;
   QList< QWidget * > blockedPageList;
   enum Direction : int;

   // This is used for internal purposes in the class engine
   void slideInWgt(QWidget *widget,
       SlidingStackedWidget::Direction direction = AUTOMATIC);

public:
    enum Direction : int
    {
        LEFT2RIGHT,
        RIGHT2LEFT,
        TOP2BOTTOM,
        BOTTOM2TOP,
        AUTOMATIC
    };
    explicit SlidingStackedWidget(QWidget *parent);
    SlidingStackedWidget(const SlidingStackedWidget &) = delete;
    SlidingStackedWidget &operator=(const SlidingStackedWidget &) = delete;
    bool isAnimationActive() const;

public slots:
    // Animation duration in milliseconds
    void setSpeed(int speed) noexcept;
    // Check out the QEasingCurve documentation for different styles
    void setAnimation(QEasingCurve::Type animationType) noexcept;
    void setVerticalMode(bool vertical = true) noexcept;
    // Wrapping is related to slideInNext/Prev; It defines the behaviour when
    // reaching last/first page
    void setWrap(bool wrap) noexcept;
    void slideInNext();
    void slideInPrev();
    void slideInIdx(int idx, Direction direction = AUTOMATIC);

signals:
    // This is used for internal purposes in the class engine
    void animationFinished();

protected slots:
   // This is used for internal purposes in the class engine
   void animationDoneSlot();
};

#endif // SLIDINGSTACKEDWIDGET_H
