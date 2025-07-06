#include "mainwindow.h"

#define BRUSH_MAX_WIDTH 20
#define BRUSH_MIN_WIDTH 1

    //Марк, пожалуйста, если не проебешь очередной день то сделай хотя бы:
    /*
    1) Окно и добавь на него все элементы
    2) Сделай возможность рисовать, так же добавь разную ширину под кисти
    3) Сотри когда посчитаешь нужным или когда выполнишь все задания за день

    ПОКА НЕ СДЕЛАЕШЬ ЭТО, ТО ДАЖЕ НЕ ВЗДУМЙ ТРОГАТЬ ТУТ НИЧЕГО НАХУЙ!
     */

MainWindow::MainWindow(QWidget *parent)
        : QWidget(parent),
        __color(Qt::red),
        __width(BRUSH_MIN_WIDTH)
{

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

    // Подключаем кнопки фигур к режимам рисования
    connect(triAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn]() {
        canvas->setDrawingMode(3); // Triangle
        triAngleBtn->setChecked(true);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
    });
    
    connect(rectAngleBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn]() {
        canvas->setDrawingMode(4); // Rectangle
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(true);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(false);
    });
    
    connect(ellipseBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn]() {
        canvas->setDrawingMode(5); // Ellipse
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(true);
        diamondBtn->setChecked(false);
    });
    
    connect(diamondBtn, &QToolButton::clicked, [this, triAngleBtn, rectAngleBtn, ellipseBtn, diamondBtn]() {
        canvas->setDrawingMode(6); // Diamond
        triAngleBtn->setChecked(false);
        rectAngleBtn->setChecked(false);
        ellipseBtn->setChecked(false);
        diamondBtn->setChecked(true);
    });

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
        canvas->setDrawingMode(0); // Brush
        brushBtn->setChecked(true);
        penBtn->setChecked(false);
        pencilBtn->setChecked(false);
        canvas->setWidth(5); // Широкая кисть
    });
    
    connect(penBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(1); // Pen
        brushBtn->setChecked(false);
        penBtn->setChecked(true);
        pencilBtn->setChecked(false);
        canvas->setWidth(2); // Средняя ширина
    });
    
    connect(pencilBtn, &QToolButton::clicked, [this, brushBtn, penBtn, pencilBtn]() {
        canvas->setDrawingMode(2); // Pencil
        brushBtn->setChecked(false);
        penBtn->setChecked(false);
        pencilBtn->setChecked(true);
        canvas->setWidth(1); // Тонкая линия
    });

    // --- Новый layout ---
    // Панель кистей (слева)
    QVBoxLayout* brushPanel = new QVBoxLayout;

    // Brush
    QHBoxLayout* brushRow = new QHBoxLayout;
    QLabel* brushLabel = new QLabel("Brush");
    brushLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    brushRow->addWidget(brushLabel);
    brushRow->addWidget(brushBtn);
    brushPanel->addLayout(brushRow);

    // Pen
    QHBoxLayout* penRow = new QHBoxLayout;
    QLabel* penLabel = new QLabel("Pen");
    penLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    penRow->addWidget(penLabel);
    penRow->addWidget(penBtn);
    brushPanel->addLayout(penRow);

    // Pencil
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

    // Панель фигур (сверху по центру)
    QHBoxLayout* figurePanel = new QHBoxLayout;
    figurePanel->addWidget(triAngleBtn);
    figurePanel->addWidget(rectAngleBtn);
    figurePanel->addWidget(ellipseBtn);
    figurePanel->addWidget(diamondBtn);
    figurePanel->addStretch();

    // Центральная панель: сверху фигуры, ниже canvas
    QVBoxLayout* centerPanel = new QVBoxLayout;
    centerPanel->addLayout(figurePanel);
    centerPanel->addWidget(canvas, 1);

    // Основной layout: кисти слева, центр (фигуры+canvas) по центру
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(brushPanel);
    mainLayout->addLayout(centerPanel, 1);
    mainLayout->addStretch();

    setLayout(mainLayout);
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
        // Обновляем ширину только если выбран режим кисти
        if (canvas->getDrawingMode() <= 2) { // Brush, Pen, Pencil
            canvas->setWidth(__width);
        }
    }
}

void MainWindow::minus()
{
    if (__width > BRUSH_MIN_WIDTH) {
        --__width;
        // Обновляем ширину только если выбран режим кисти
        if (canvas->getDrawingMode() <= 2) { // Brush, Pen, Pencil
            canvas->setWidth(__width);
        }
    }
}


