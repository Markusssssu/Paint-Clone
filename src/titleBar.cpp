#include "titleBar.h"

// signals:
//     void minimizeClicked();
//     void maximizeClicked();
//     void closeClicked();

// protected:
//     void mousePresstEvent(QMouseEvent *event) override;
//     void mouseMoveEvent(QMouseEvent *event) override;

// private:
//     QPoint m_dragPosition;


TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent)
{
    QPushButton *minimizeBtn = new QPushButton("-", this);
    QPushButton *maximizeBtn = new QPushButton("□", this);
    QPushButton *closeBtn = new QPushButton("x", this);

    connect(minimizeBtn, &QPushButton::clicked, this, minimizeClicked());
    connect(maximizeBtn, &QPushButton::clicked, this, maximizeClicked());
    connect(closeBtn, &QPushButton::clicked, this, closeClicked());

    QHBoxLayout *titleBarLayout = new QHBoxLayout;
    titleBarLayout->addWidget(minimizeBtn);
    titleBarLayout->addWidget(maximizeBtn);
    titleBarLayout->addWidget(closeBtn);
}

void TitleBar::minimizeClicked()
{
    if (parent = this->window()) {
        if (window->isMinimized()) {
            window->showNormal();
        } else
        {
            window->showMinimized();
        }
    }
}

void TitleBar::maximizeClicked()
{
     if (parent = this->window()) {
        if (window->isMaximized()) {
            window->showNormal();
        } else
        {
            window->showMaximized();
        }
    }
}

void TitleBar::closeClicked()
{
    if (parent = this->window()) {
        window->close();
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}
