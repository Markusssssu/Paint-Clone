#include "mainwindow.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>

#define BRUSH_MAX_WIDTH 20
#define BRUSH_MIN_WIDTH 1

    //Марк, пожалуйста, если не проебешь очередной день то сделай хотя бы:
    /*
    1) Окно и добавь на него все элементы
    2) Сделай возможность рисовать, так же добавь разную ширину под кисти
    3) Сотри когда посчитаешь нужным или когда выполнишь все задания за день

    ПОКА НЕ СДЕЛАЕШЬ ЭТО, ТО ДАЖЕ НЕ ВЗДУМЙ ТРОГАТЬ ТУТ НИЧЕГО НАХУЙ!
     */

TitleBar::TitleBar(QWidget *parent) : QFrame(parent) {
    setFixedHeight(30);
    setStyleSheet("background: #222; color: white;");

    QLabel *title = new QLabel("Graphics App", this);
    QPushButton *minBtn = new QPushButton("-", this);
    QPushButton *maxBtn = new QPushButton("□", this);
    QPushButton *closeBtn = new QPushButton("x", this);

    minBtn->setFixedSize(30, 30);
    maxBtn->setFixedSize(30, 30);
    closeBtn->setFixedSize(30, 30);
    minBtn->setStyleSheet("background: #333; color: white; border: none;");
    maxBtn->setStyleSheet("background: #333; color: white; border: none;");
    closeBtn->setStyleSheet("background: #b00; color: white; border: none;");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(title);
    layout->addStretch();
    layout->addWidget(minBtn);
    layout->addWidget(maxBtn);
    layout->addWidget(closeBtn);
    layout->setContentsMargins(5, 0, 5, 0);
    setLayout(layout);

    connect(minBtn, &QPushButton::clicked, this, &TitleBar::minimizeClicked);
    connect(maxBtn, &QPushButton::clicked, this, &TitleBar::maximizeClicked);
    connect(closeBtn, &QPushButton::clicked, this, &TitleBar::closeClicked);
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        parentWidget()->move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

MainWindow::MainWindow(QWidget *parent)
        : QWidget(parent),
        __color(Qt::red),
        __width(BRUSH_MIN_WIDTH)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    titleBar = new TitleBar(this);
    connect(titleBar, &TitleBar::minimizeClicked, this, &QWidget::showMinimized);
    connect(titleBar, &TitleBar::maximizeClicked, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(titleBar, &TitleBar::closeClicked, this, &QWidget::close);

    canvas = new Canvas(this);
    canvas->setFixedSize(500, 500);
    canvas->setColor(__color);
    canvas->setWidth(__width);

    QPushButton* plusBtn = new QPushButton("+");
    QPushButton* minusBtn = new QPushButton("-");

    connect(plusBtn, &QPushButton::clicked, this, &MainWindow::plus);
    connect(minusBtn, &QPushButton::clicked, this, &MainWindow::minus);

    QToolButton* triAngleBtn = new QToolButton();
    triAngleBtn->setIcon(QIcon(":/icons/triangle"));
    triAngleBtn->setToolTip("Triangle");
    triAngleBtn->setFixedSize(50, 25);
    triAngleBtn->setCheckable(1);

    QToolButton* rectAngleBtn = new QToolButton();
    rectAngleBtn->setIcon(QIcon(":/icons/rectangle"));
    rectAngleBtn->setToolTip("Rectangle");
    rectAngleBtn->setFixedSize(50, 25);
    rectAngleBtn->setCheckable(1);

    QToolButton* ellipseBtn = new QToolButton();
    ellipseBtn->setIcon(QIcon(":/icons/ellipse"));
    ellipseBtn->setToolTip("Ellipse");
    ellipseBtn->setFixedSize(50, 25);
    ellipseBtn->setCheckable(1);

    QToolButton* diamondBtn = new QToolButton();
    diamondBtn->setIcon(QIcon(":/icons/diamond"));
    diamondBtn->setToolTip("Diamond");
    diamondBtn->setFixedSize(50, 25);
    diamondBtn->setCheckable(1);

    QToolButton* lineBtn = new QToolButton();
    lineBtn->setText("/");
    lineBtn->setToolTip("Line");
    lineBtn->setFixedSize(50, 25);
    lineBtn->setCheckable(1);

    QToolButton* moveFigureBtn = new QToolButton();
    moveFigureBtn->setText("#");
    moveFigureBtn->setToolTip("Move");
    moveFigureBtn->setFixedSize(50, 25);
    moveFigureBtn->setCheckable(1);

    QPushButton* undoBtn = new QPushButton("Undo <--");
    undoBtn->setFixedSize(50, 25);

    QPushButton* redoBtn = new QPushButton("Reod -->");
    redoBtn->setFixedSize(50, 25);

    connect(triAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(3); // Triangle
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(true);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(rectAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(4); // Rectangle
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(true);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(ellipseBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(5); // Ellipse
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(true);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(diamondBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(6); // Diamond
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(true);
        lineBtn->setChecked(false);
    });

    connect(lineBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(7);
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(true);
    });

    connect(moveFigureBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(8);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
        moveFigureBtn->setChecked(true);
    });

    connect(undoBtn, &QPushButton::clicked, canvas, &Canvas::undo);
    connect(redoBtn, &QPushButton::clicked, canvas, &Canvas::redo);

    QToolButton* brushBtn = new QToolButton();
    brushBtn->setIcon(QIcon(":/icons/brush"));
    brushBtn->setToolTip("brush");
    brushBtn->setCheckable(true);

    QToolButton* penBtn = new QToolButton();
    penBtn->setIcon(QIcon(":/icons/pen"));
    penBtn->setToolTip("pen");
    penBtn->setCheckable(1);

    QToolButton* pencilBtn = new QToolButton();
    pencilBtn->setIcon(QIcon(":/icons/pencil"));
    pencilBtn->setToolTip("pencil");
    pencilBtn->setCheckable(1);

    colorPickBtn = new QToolButton();
    colorPickBtn->setFixedSize(50, 20);

    QPalette colorChangePalette = colorPickBtn->palette();
    colorChangePalette.setBrush(QPalette::Base, __color);
    colorChangePalette.setBrush(QPalette::Button, __color);
    colorPickBtn->setPalette(colorChangePalette);

    QToolButton* colorBtn = new QToolButton();
    colorBtn->setText("Color");

    connect(colorBtn, &QToolButton::clicked, this, color);
    
    connect(brushBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(0);
        brushBtn->setChecked(true);
        penBtn->setChecked(false);
        pencilBtn->setChecked(false);
<<<<<<< HEAD:src/mainwindow.cpp
        canvas->setWidth(5);
    });
    
    connect(penBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(1);
        brushBtn->setChecked(false);
        penBtn->setChecked(true);
        pencilBtn->setChecked(false);
        canvas->setWidth(2);
=======
        canvas->setWidth(5); 
    });
    
    connect(penBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(1); 
        brushBtn->setChecked(false);
        penBtn->setChecked(true);
        pencilBtn->setChecked(false);
        canvas->setWidth(2); 
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    });
    
    connect(pencilBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(2);
        brushBtn->setChecked(false);
        penBtn->setChecked(false);
        pencilBtn->setChecked(true);
        canvas->setWidth(1);
    });

<<<<<<< HEAD:src/mainwindow.cpp
    QVBoxLayout* brushPanel = new QVBoxLayout;

=======
   
    QVBoxLayout* brushPanel = new QVBoxLayout;

    
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    QHBoxLayout* brushRow = new QHBoxLayout;
    QLabel* brushLabel = new QLabel("Brush");
    brushLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    brushRow->addWidget(brushLabel);
    brushRow->addWidget(brushBtn);
    brushPanel->addLayout(brushRow);

<<<<<<< HEAD:src/mainwindow.cpp
=======
  
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    QHBoxLayout* penRow = new QHBoxLayout;
    QLabel* penLabel = new QLabel("Pen");
    penLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    penRow->addWidget(penLabel);
    penRow->addWidget(penBtn);
    brushPanel->addLayout(penRow);

<<<<<<< HEAD:src/mainwindow.cpp
=======

>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    QHBoxLayout* pencilRow = new QHBoxLayout;
    QLabel* pencilLabel = new QLabel("Pencil");
    pencilLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    pencilRow->addWidget(pencilLabel);
    pencilRow->addWidget(pencilBtn); 
    brushPanel->addLayout(pencilRow); 

    brushPanel->addWidget(plusBtn);
    brushPanel->addWidget(minusBtn);
    brushPanel->addWidget(colorBtn);
    brushPanel->addWidget(colorPickBtn);
    brushPanel->addStretch();

    QHBoxLayout* figurePanel = new QHBoxLayout;
    figurePanel->addWidget(triAngleBtn);
    figurePanel->addWidget(rectAngleBtn);
    figurePanel->addWidget(ellipseBtn);
    figurePanel->addWidget(diamondBtn);
    figurePanel->addWidget(lineBtn);
    figurePanel->addWidget(moveFigureBtn);
    figurePanel->addWidget(undoBtn);
    figurePanel->addWidget(redoBtn);
    figurePanel->addStretch();
<<<<<<< HEAD:src/mainwindow.cpp

=======
    
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    QVBoxLayout* centerPanel = new QVBoxLayout;
    centerPanel->addLayout(figurePanel);
    centerPanel->addWidget(canvas, 1);

<<<<<<< HEAD:src/mainwindow.cpp
    QHBoxLayout* mainLayout = new QHBoxLayout();
=======
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
    mainLayout->addLayout(brushPanel);
    mainLayout->addLayout(centerPanel, 1);
    mainLayout->addStretch();

    QVBoxLayout* windowLayout = new QVBoxLayout(this);
    windowLayout->setSpacing(0);
    windowLayout->setContentsMargins(0,0,0,0);
    windowLayout->addWidget(titleBar);
    windowLayout->addLayout(mainLayout, 1);
    setLayout(windowLayout);
} // Закрытие конструктора MainWindow

void MainWindow::color()
{
    QColor color = QColorDialog::getColor(__color, this, "Pick color");

    if (color.isValid())
    {
        __color = color;
        canvas->setColor(__color);
        
        QPalette colorChangePalette = colorPickBtn->palette();
        colorChangePalette.setBrush(QPalette::Base, __color);
        colorChangePalette.setBrush(QPalette::Button, __color);
        colorPickBtn->setPalette(colorChangePalette);
    }
    else
    {
        QMessageBox::information(this,
                                 "Information",
                                 "You cancle pick color"
                                 );
    }
}

void MainWindow::plus()
{
    if (__width < BRUSH_MAX_WIDTH) {
        ++__width;
<<<<<<< HEAD:src/mainwindow.cpp
        if (canvas->getDrawingMode() <= 2) {
=======
        if (canvas->getDrawingMode() <= 2) { 
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
            canvas->setWidth(__width);
        }
    }
}

void MainWindow::minus()
{
    if (__width > BRUSH_MIN_WIDTH) {
        --__width;
<<<<<<< HEAD:src/mainwindow.cpp
        if (canvas->getDrawingMode() <= 2) {
=======
        if (canvas->getDrawingMode() <= 2) { 
>>>>>>> 89f969b662a9a82f4e6d9ab54ae2ffc7adfaf023:mainwindow.cpp
            canvas->setWidth(__width);
        }
    }
}


