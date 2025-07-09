#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QList>
#include <QVector>
#include <QToolButton>
#include <QDebug>
#include <QIcon>
#include <QBrush>
#include <QColorDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "canvas.h"
#include <QFrame>

class TitleBar : public QFrame {
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

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

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void plus();
    void minus();
    void color();

private:
    QColor __color;
    int __width;
    QToolButton* colorPickBtn;
    Canvas* canvas;
    TitleBar* titleBar;
};
