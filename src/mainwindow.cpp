#include "mainwindow.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>

#define BRUSH_MAX_WIDTH 20
#define BRUSH_MIN_WIDTH 1

    //Марк, пожалуйста, если не проебешь очередной день то сделай хотя бы:
    /*
    1) Добавить struct на все кисти
    2) Сделай возможность рисовать разными кистями раной толщины и длинны
    3) Сотри когда посчитаешь нужным или когда выполнишь все задания за день

    ПОКА НЕ СДЕЛАЕШЬ ЭТО, ТО ДАЖЕ НЕ ВЗДУМЙ ТРОГАТЬ ТУТ НИЧЕГО НАХУЙ!
     */

//Привет Марк, это я ты из будущего, я решил не стирать эти комментарии потому что понял что ты должен зайти сюда через пол года и посмеяться с этого
//Я исправил все баги, добавил move-semantics, теперь ты можешь запускать проект
// Удачи в дальнейшем, надеюсь ты снова сюда вернешься.

TitleBar::TitleBar(QWidget *parent) : QFrame(parent) {


    QPalette titlePalette;
    titlePalette.setColor(QPalette::Window, QColor(34, 34, 34));


    setFixedHeight(30);
    
    QPalette titlePalette;
    titlePalette.setColor(QPalette::Window, QColor(34, 34, 34));

    QLabel *title = new QLabel(" ", this);
    QPushButton *minBtn = new QPushButton("-", this);
    QPushButton *maxBtn = new QPushButton("□", this);
    QPushButton *closeBtn = new QPushButton("x", this);

    QPalette minPalette;
    QPalette maxPalette;
    QPalette closePalette;

    minPalette.setColor(QPalette::Button, QColor(51, 51, 51));
    minPalette.setColor(QPalette::ButtonText, Qt::white);

    maxPalette.setColor(QPalette::Button, QColor(51, 51, 51));
    maxPalette.setColor(QPalette::ButtonText, Qt::white);

    closePalette.setColor(QPalette::Button, QColor(187, 0, 0));
    closePalette.setColor(QPalette::ButtonText, Qt::white);

    minBtn->setFixedSize(30, 30);
    maxBtn->setFixedSize(30, 30);
    closeBtn->setFixedSize(30, 30);

    minBtn->setPalette(minPalette);
    maxBtn->setPalette(maxPalette);
    closeBtn->setPalette(closePalette);

    minBtn->setAutoFillBackground(1);
    maxBtn->setAutoFillBackground(1);
    closeBtn->setAutoFillBackground(1);

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

TitleBar::TitleBar(TitleBar&& other) noexcept
    : QFrame(std::move(other))
    , m_dragPosition(std::move(other.m_dragPosition))
{
}

TitleBar& TitleBar::operator=(TitleBar&& other) noexcept
{
    if (this != &other) {
        QFrame::operator=(std::move(other));
        m_dragPosition = std::move(other.m_dragPosition);
    }
    return *this;
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
    setMinimumSize(800, 600);
    setStyleSheet(R"(
        QWidget {
            background-color: #2b2b2b;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 10px;
        }
        QPushButton {
            background-color: #404040;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 6px 12px;
            color: #ffffff;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #505050;
            border-color: #666666;
        }
        QPushButton:pressed {
            background-color: #303030;
        }
        QToolButton {
            background-color: #404040;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px;
            color: #ffffff;
        }
        QToolButton:hover {
            background-color: #505050;
            border-color: #666666;
        }
        QToolButton:checked {
            background-color: #0078d4;
            border-color: #0078d4;
        }
        QLabel {
            color: #ffffff;
            font-weight: bold;
        }
        QFrame {
            background-color: #1e1e1e;
            border: none;
        }
    )");

    titleBar = new TitleBar(this);
    titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(titleBar, &TitleBar::minimizeClicked, this, &QWidget::showMinimized);
    connect(titleBar, &TitleBar::maximizeClicked, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(titleBar, &TitleBar::closeClicked, this, &QWidget::close);

    canvas = new Canvas(this);
    canvas->setFixedSize(600, 500);
    canvas->setColor(__color);
    canvas->setWidth(__width);

    QPushButton* plusBtn = new QPushButton("+");
    QPushButton* minusBtn = new QPushButton("-");
    plusBtn->setFixedSize(40, 30);
    minusBtn->setFixedSize(40, 30);
    plusBtn->setStyleSheet("QPushButton { font-size: 16px; font-weight: bold; }");
    minusBtn->setStyleSheet("QPushButton { font-size: 16px; font-weight: bold; }");

    connect(plusBtn, &QPushButton::clicked, this, &MainWindow::plus);
    connect(minusBtn, &QPushButton::clicked, this, &MainWindow::minus);

    QToolButton* triAngleBtn = new QToolButton();
    triAngleBtn->setIcon(QIcon(":/icons/triangle"));
    triAngleBtn->setToolTip("Triangle");
    triAngleBtn->setFixedSize(60, 35);
    triAngleBtn->setCheckable(true);

    QToolButton* rectAngleBtn = new QToolButton();
    rectAngleBtn->setIcon(QIcon(":/icons/rectangle"));
    rectAngleBtn->setToolTip("Rectangle");
    rectAngleBtn->setFixedSize(60, 35);
    rectAngleBtn->setCheckable(true);

    QToolButton* ellipseBtn = new QToolButton();
    ellipseBtn->setIcon(QIcon(":/icons/ellipse"));
    ellipseBtn->setToolTip("Ellipse");
    ellipseBtn->setFixedSize(60, 35);
    ellipseBtn->setCheckable(true);

    QToolButton* diamondBtn = new QToolButton();
    diamondBtn->setIcon(QIcon(":/icons/diamond"));
    diamondBtn->setToolTip("Diamond");
    diamondBtn->setFixedSize(60, 35);
    diamondBtn->setCheckable(true);

    QToolButton* lineBtn = new QToolButton();
    lineBtn->setText("╱");
    lineBtn->setToolTip("Line");
    lineBtn->setFixedSize(60, 35);
    lineBtn->setCheckable(true);
    lineBtn->setStyleSheet("QToolButton { font-size: 18px; font-weight: bold; }");

    QToolButton* moveFigureBtn = new QToolButton();
    moveFigureBtn->setText("✋");
    moveFigureBtn->setToolTip("Move");
    moveFigureBtn->setFixedSize(60, 35);
    moveFigureBtn->setCheckable(true);
    moveFigureBtn->setStyleSheet("QToolButton { font-size: 16px; }");

    QPushButton* undoBtn = new QPushButton("↶ Undo");
    undoBtn->setFixedSize(80, 35);
    undoBtn->setStyleSheet("QPushButton { font-size: 11px; }");

    QPushButton* redoBtn = new QPushButton("Redo ↷");
    redoBtn->setFixedSize(80, 35);
    redoBtn->setStyleSheet("QPushButton { font-size: 11px; }");

    connect(triAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(3); 
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(true);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(rectAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(4); 
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(true);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(ellipseBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(5); 
        moveFigureBtn->setChecked(false);
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(true);
        diamondBtn->setChecked(false);
        lineBtn->setChecked(false);
    });
    
    connect(diamondBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn, lineBtn, moveFigureBtn]() {
        canvas->setDrawingMode(6); 
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
    brushBtn->setToolTip("Brush");
    brushBtn->setCheckable(true);
    brushBtn->setFixedSize(60, 35);

    QToolButton* penBtn = new QToolButton();
    penBtn->setIcon(QIcon(":/icons/pen"));
    penBtn->setToolTip("Pen");
    penBtn->setCheckable(true);
    penBtn->setFixedSize(60, 35);

    QToolButton* pencilBtn = new QToolButton();
    pencilBtn->setIcon(QIcon(":/icons/pencil"));
    pencilBtn->setToolTip("Pencil");
    pencilBtn->setCheckable(true);
    pencilBtn->setFixedSize(60, 35);

    colorPickBtn = new QToolButton();
    colorPickBtn->setFixedSize(60, 35);

    QPalette colorChangePalette = colorPickBtn->palette();
    colorChangePalette.setBrush(QPalette::Base, __color);
    colorChangePalette.setBrush(QPalette::Button, __color);
    colorPickBtn->setPalette(colorChangePalette);

    QToolButton* colorBtn = new QToolButton();
    colorBtn->setText("Pick Color");
    colorBtn->setFixedSize(80, 35);

    connect(colorBtn, &QToolButton::clicked, this, color);
    
    connect(brushBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(0);
        brushBtn->setChecked(true);
        penBtn->setChecked(false);
        pencilBtn->setChecked(false);
        canvas->setWidth(5);
    });
    
    connect(penBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(1);
        brushBtn->setChecked(false);
        penBtn->setChecked(true);
        pencilBtn->setChecked(false);
        canvas->setWidth(2);
    });
    
    connect(pencilBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(2);
        brushBtn->setChecked(false);
        penBtn->setChecked(false);
        pencilBtn->setChecked(true);
        canvas->setWidth(1);
    });

    QVBoxLayout* brushPanel = new QVBoxLayout;
    brushPanel->setSpacing(10);
    brushPanel->setContentsMargins(15, 15, 15, 15);
    
    QLabel* toolsLabel = new QLabel("Drawing Tools");
    toolsLabel->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #ffffff; margin-bottom: 10px; }");
    brushPanel->addWidget(toolsLabel);
    
    QHBoxLayout* brushRow = new QHBoxLayout;
    QLabel* brushLabel = new QLabel("Brush");
    brushLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    brushLabel->setFixedWidth(60);
    brushRow->addWidget(brushLabel);
    brushRow->addWidget(brushBtn);
    brushRow->addStretch();
    brushPanel->addLayout(brushRow);

    QHBoxLayout* penRow = new QHBoxLayout;
    QLabel* penLabel = new QLabel("Pen");
    penLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    penLabel->setFixedWidth(60);
    penRow->addWidget(penLabel);
    penRow->addWidget(penBtn);
    penRow->addStretch();
    brushPanel->addLayout(penRow);

    QHBoxLayout* pencilRow = new QHBoxLayout;
    QLabel* pencilLabel = new QLabel("Pencil");
    pencilLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    pencilLabel->setFixedWidth(60);
    pencilRow->addWidget(pencilLabel);
    pencilRow->addWidget(pencilBtn);
    pencilRow->addStretch();
    brushPanel->addLayout(pencilRow); 

    QLabel* sizeLabel = new QLabel("Brush Size");
    sizeLabel->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #ffffff; margin-top: 15px; }");
    brushPanel->addWidget(sizeLabel);
    
    QHBoxLayout* sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(minusBtn);
    sizeLayout->addWidget(plusBtn);
    sizeLayout->addStretch();
    brushPanel->addLayout(sizeLayout);
    
    QLabel* colorLabel = new QLabel("Color");
    colorLabel->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #ffffff; margin-top: 15px; }");
    brushPanel->addWidget(colorLabel);
    
    brushPanel->addWidget(colorBtn);
    brushPanel->addWidget(colorPickBtn);
    brushPanel->addStretch();

    QHBoxLayout* figurePanel = new QHBoxLayout;
    figurePanel->setSpacing(8);
    figurePanel->setContentsMargins(15, 10, 15, 10);
    
    QLabel* figuresLabel = new QLabel("Shapes & Tools");
    figuresLabel->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #ffffff; margin-right: 15px; }");
    figurePanel->addWidget(figuresLabel);
    
    figurePanel->addWidget(triAngleBtn);
    figurePanel->addWidget(rectAngleBtn);
    figurePanel->addWidget(ellipseBtn);
    figurePanel->addWidget(diamondBtn);
    figurePanel->addWidget(lineBtn);
    figurePanel->addWidget(moveFigureBtn);
    
    figurePanel->addSpacing(20);
    
    figurePanel->addWidget(undoBtn);
    figurePanel->addWidget(redoBtn);
    figurePanel->addStretch();

    QVBoxLayout* centerPanel = new QVBoxLayout;
    centerPanel->setSpacing(0);
    centerPanel->setContentsMargins(0, 0, 0, 0);
    centerPanel->addLayout(figurePanel);
    centerPanel->addWidget(canvas, 1);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(brushPanel);
    mainLayout->addLayout(centerPanel, 1);
    mainLayout->addStretch();

    QVBoxLayout* windowLayout = new QVBoxLayout(this);
    windowLayout->setSpacing(0);
    windowLayout->setContentsMargins(0,0,0,0);
    windowLayout->addWidget(titleBar);
    windowLayout->addLayout(mainLayout, 1);
    setLayout(windowLayout);

    titleBar->setFixedWidth(this->width());
} 

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
        if (canvas->getDrawingMode() <= 2) {
            canvas->setWidth(__width);
        }
    }
}

void MainWindow::minus()
{
    if (__width > BRUSH_MIN_WIDTH) {
        --__width;
        if (canvas->getDrawingMode() <= 2) {
            canvas->setWidth(__width);
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (titleBar) {
        titleBar->setFixedWidth(this->width());
    }
}

MainWindow::MainWindow(MainWindow&& other) noexcept
    : QWidget(std::move(other))
    , __color(std::move(other.__color))
    , __width(other.__width)
    , colorPickBtn(other.colorPickBtn)
    , canvas(other.canvas)
    , titleBar(other.titleBar)
{
    other.colorPickBtn = nullptr;
    other.canvas = nullptr;
    other.titleBar = nullptr;
}

MainWindow& MainWindow::operator=(MainWindow&& other) noexcept
{
    if (this != &other) {
        QWidget::operator=(std::move(other));
        __color = std::move(other.__color);
        __width = other.__width;
        
        delete colorPickBtn;
        delete canvas;
        delete titleBar;
        
        colorPickBtn = other.colorPickBtn;
        canvas = other.canvas;
        titleBar = other.titleBar;
        
        other.colorPickBtn = nullptr;
        other.canvas = nullptr;
        other.titleBar = nullptr;
    }
    return *this;
}
