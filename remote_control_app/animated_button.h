#ifndef HEADLIGHTS_BUTTON_H
#define HEADLIGHTS_BUTTON_H

#include <QPushButton>
#include <memory>

class QMouseEvent;

class AnimatedButton : public QPushButton
{
    Q_OBJECT

    bool isMouseHover;
    bool isSetPressed;
    std::shared_ptr< QIcon > iconNormal;
    std::shared_ptr< QIcon > iconHovered;
    std::shared_ptr< QIcon > iconPressed;

protected:
    bool event(QEvent* e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

public:
    AnimatedButton(const QString &normalImagePath,
        const QString &hoveredImagePath, const QString &pressedImagePath,
        QWidget *parent = 0);
    ~AnimatedButton() = default;
    AnimatedButton(const AnimatedButton &) = delete;
    AnimatedButton &operator=(const AnimatedButton &) = delete;

    void setDown(bool);

public slots:
    void setButtonIcon(bool pressed);
};

#endif // HEADLIGHTS_BUTTON_H
