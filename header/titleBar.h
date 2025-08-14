#pragma once

#include <QFrame>
#include <QPoint>

class QMouseEvent;

class TitleBar : public QFrame {
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);
    
    // Move semantics
    TitleBar(TitleBar&& other) noexcept;
    TitleBar& operator=(TitleBar&& other) noexcept;
    
    // Delete copy constructor and assignment
    TitleBar(const TitleBar&) = delete;
    TitleBar& operator=(const TitleBar&) = delete;
    
    ~TitleBar();

signals:
    void minimizeClicked();
    void maximizeClicked();
    void closeClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPosition;
};
