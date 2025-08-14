#include "titleBar.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>

// signals:
//     void minimizeClicked();
//     void maximizeClicked();
//     void closeClicked();

// protected:
//     void mousePresstEvent(QMouseEvent *event) override;
//     void mouseMoveEvent(QMouseEvent *event) override;

// private:
//     QPoint m_dragPosition;


TitleBar::TitleBar(QWidget *parent) : QFrame(parent) {
    setFixedHeight(40);
    setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2d2d2d, stop:1 #1e1e1e);
            border-bottom: 2px solid #0078d4;
        }
        QPushButton {
            background-color: transparent;
            border: none;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 12px;
            font-weight: bold;
            padding: 8px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.2);
        }
        QLabel {
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
            font-weight: bold;
            padding: 8px;
        }
    )");

    QLabel *title = new QLabel("Paint Clone", this);
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    QPushButton *minBtn = new QPushButton("−", this);
    QPushButton *maxBtn = new QPushButton("□", this);
    QPushButton *closeBtn = new QPushButton("✕", this);
    
    minBtn->setFixedSize(40, 40);
    maxBtn->setFixedSize(40, 40);
    closeBtn->setFixedSize(40, 40);
    
    minBtn->setToolTip("Minimize");
    maxBtn->setToolTip("Maximize");
    closeBtn->setToolTip("Close");
    
    // Special styling for close button
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 16px;
            font-weight: bold;
            padding: 8px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #e81123;
        }
        QPushButton:pressed {
            background-color: #c50e1f;
        }
    )");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(title);
    layout->addStretch();
    layout->addWidget(minBtn);
    layout->addWidget(maxBtn);
    layout->addWidget(closeBtn);
    layout->setContentsMargins(15, 0, 5, 0);
    layout->setSpacing(2);
    setLayout(layout);

    connect(minBtn, &QPushButton::clicked, this, &TitleBar::minimizeClicked);
    connect(maxBtn, &QPushButton::clicked, this, &TitleBar::maximizeClicked);
    connect(closeBtn, &QPushButton::clicked, this, &TitleBar::closeClicked);
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
