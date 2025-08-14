#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPolygon>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_color(Qt::red)
    , m_width(1)
    , m_drawingMode(Brush)
    , m_drawing(false)
{
    setMinimumSize(400, 400);
    
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);
    
    // Add modern styling
    setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
        }
        QWidget:hover {
            border-color: #0078d4;
        }
    )");
    
    // Add shadow effect
    setGraphicsEffect(nullptr); // Remove any existing effects
}

// Move constructor
Canvas::Canvas(Canvas&& other) noexcept
    : QWidget(std::move(other))
    , m_color(std::move(other.m_color))
    , m_width(other.m_width)
    , m_drawingMode(other.m_drawingMode)
    , m_drawing(other.m_drawing)
    , m_lastPoint(std::move(other.m_lastPoint))
    , m_startPoint(std::move(other.m_startPoint))
    , m_lines(std::move(other.m_lines))
    , m_rectangles(std::move(other.m_rectangles))
    , m_ellipses(std::move(other.m_ellipses))
    , m_triangles(std::move(other.m_triangles))
    , m_diamonds(std::move(other.m_diamonds))
    , undoStack(std::move(other.undoStack))
    , redoStack(std::move(other.redoStack))
{
}

// Move assignment operator
Canvas& Canvas::operator=(Canvas&& other) noexcept
{
    if (this != &other) {
        QWidget::operator=(std::move(other));
        m_color = std::move(other.m_color);
        m_width = other.m_width;
        m_drawingMode = other.m_drawingMode;
        m_drawing = other.m_drawing;
        m_lastPoint = std::move(other.m_lastPoint);
        m_startPoint = std::move(other.m_startPoint);
        m_lines = std::move(other.m_lines);
        m_rectangles = std::move(other.m_rectangles);
        m_ellipses = std::move(other.m_ellipses);
        m_triangles = std::move(other.m_triangles);
        m_diamonds = std::move(other.m_diamonds);
        undoStack = std::move(other.undoStack);
        redoStack = std::move(other.redoStack);
    }
    return *this;
}

// Destructor
Canvas::~Canvas()
{
    // Qt will handle cleanup automatically
}

// Move methods for efficient data transfer
void Canvas::addLineData(QVector<LineData>&& lines)
{
    m_lines = std::move(lines);
    update();
}

void Canvas::addRectangleData(QVector<RectangleData>&& rectangles)
{
    m_rectangles = std::move(rectangles);
    update();
}

void Canvas::addEllipseData(QVector<EllipseData>&& ellipses)
{
    m_ellipses = std::move(ellipses);
    update();
}

void Canvas::addTriangleData(QVector<TriangleData>&& triangles)
{
    m_triangles = std::move(triangles);
    update();
}

void Canvas::addDiamondData(QVector<DiamondData>&& diamonds)
{
    m_diamonds = std::move(diamonds);
    update();
}

void Canvas::setColor(const QColor &color)
{
    m_color = color;
}

void Canvas::setWidth(int width)
{
    m_width = width;
}

void Canvas::setDrawingMode(int mode)
{
    m_drawingMode = mode;
}

int Canvas::getDrawingMode() const
{
    return m_drawingMode;
}

QColor Canvas::getColor() const
{
    return m_color;
}

int Canvas::getWidth() const
{
    return m_width;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    for (const LineData &data : m_lines) {
        painter.setPen(QPen(data.color, data.width));
        painter.drawLine(data.line);
    }
    
    for (const RectangleData &rect : m_rectangles) {
        painter.setPen(QPen(rect.color, rect.width));
        painter.drawRect(rect.rectangle);
    }
    
    for (const EllipseData &ellipse : m_ellipses) {
        painter.setPen(QPen(ellipse.color, ellipse.width));
        painter.drawEllipse(ellipse.ellipse);
    }
    
    for (const TriangleData &triangle : m_triangles) {
        painter.setPen(QPen(triangle.color, triangle.width));
        painter.drawPolygon(triangle.triangle);
    }
    
    for (const DiamondData &diamond : m_diamonds) {
        painter.setPen(QPen(diamond.color, diamond.width));
        painter.drawPolygon(diamond.diamond);
    }

    if (m_drawing) {
        QPointF currentPos = mapFromGlobal(QCursor::pos());
        
        switch (m_drawingMode) {
            case Brush:
            case Pen:
            case Pencil:
                painter.drawLine(m_lastPoint, currentPos);
                break;
            case Rectangle:
                painter.drawRect(QRect(m_startPoint.toPoint(), currentPos.toPoint()));
                break;
            case Ellipse:
                painter.drawEllipse(QRect(m_startPoint.toPoint(), currentPos.toPoint()));
                break;
            case Triangle:
                drawTriangle(painter, m_startPoint, currentPos);
                break;
            case Diamond:
                drawDiamond(painter, m_startPoint, currentPos);
                break;
            case Line:
                painter.drawLine(QLine(m_startPoint.toPoint(), currentPos.toPoint()));
                break;

        }
    }
}

void Canvas::setCanvasState()
{
    // Create state with move semantics for better performance
    CanvasState state;
    state.line = std::move(m_lines);
    state.ellipse = std::move(m_ellipses);
    state.rectangle = std::move(m_rectangles);
    state.triangle = std::move(m_triangles);
    state.diamond = std::move(m_diamonds);
    
    undoStack.append(std::move(state));
    redoStack.clear();
    
    // Recreate empty vectors for new drawing
    m_lines.clear();
    m_ellipses.clear();
    m_rectangles.clear();
    m_triangles.clear();
    m_diamonds.clear();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drawing = true;
        m_startPoint = event->position();
        m_lastPoint = event->position();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawing) {
        if (m_drawingMode == Brush || m_drawingMode == Pen || m_drawingMode == Pencil) {
            LineData data(QLine(m_lastPoint.toPoint(), event->position().toPoint()), m_color, m_width);
            m_lines.append(std::move(data));
            m_lastPoint = event->position();
        }
        update(); 
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        m_drawing = false;
        
        QPointF endPoint = event->position();
       
        switch (m_drawingMode) {
            case Brush:
            case Pen:
            case Pencil:
                break;
            case Rectangle: {
                RectangleData rectangleData(QRect(m_startPoint.toPoint(), endPoint.toPoint()), m_color, m_width);
                m_rectangles.append(std::move(rectangleData));
                break;
            }
            case Ellipse: {
                EllipseData ellipseData(QRect(m_startPoint.toPoint(), endPoint.toPoint()), m_color, m_width);
                m_ellipses.append(std::move(ellipseData));
                break;
            }
            case Triangle: {
                TriangleData triangleData(createTriangle(m_startPoint, endPoint), m_color, m_width);
                m_triangles.append(std::move(triangleData));
                break;
            }
            case Diamond: {
                DiamondData diamondData(createDiamond(m_startPoint, endPoint), m_color, m_width);
                m_diamonds.append(std::move(diamondData));
                break;
            }
            case Line: {
                LineData lineData(createLine(m_startPoint, endPoint), m_color, m_width);
                m_lines.append(std::move(lineData));
                break;
            }
        } 
        update();
        setCanvasState();
    }
}

void Canvas::drawTriangle(QPainter &painter, const QPointF &start, const QPointF &end)
{
    QRectF rect(start, end);
    QPolygon triangle;
    triangle << QPoint(rect.center().x(), rect.top())
             << QPoint(rect.left(), rect.bottom())
             << QPoint(rect.right(), rect.bottom());
    painter.drawPolygon(triangle);
}

void Canvas::drawDiamond(QPainter &painter, const QPointF &start, const QPointF &end)
{
    QRectF rect(start, end);
    QPolygon diamond;
    diamond << QPoint(rect.center().x(), rect.top())
            << QPoint(rect.right(), rect.center().y())
            << QPoint(rect.center().x(), rect.bottom())
            << QPoint(rect.left(), rect.center().y());
    painter.drawPolygon(diamond);
}

QPolygon Canvas::createTriangle(const QPointF &start, const QPointF &end)
{
    QRectF rect(start, end);
    QPolygon triangle;
    triangle << QPoint(rect.center().x(), rect.top())
             << QPoint(rect.left(), rect.bottom())
             << QPoint(rect.right(), rect.bottom());
    return triangle;
}

QPolygon Canvas::createDiamond(const QPointF &start, const QPointF &end)
{
    QRectF rect(start, end);
    QPolygon diamond;
    diamond << QPoint(rect.center().x(), rect.top())
            << QPoint(rect.right(), rect.center().y())
            << QPoint(rect.center().x(), rect.bottom())
            << QPoint(rect.left(), rect.center().y());
    return diamond;
}
// void drawTriangle(QPainter &painter, const QPointF &start, const QPointF &end);
// void drawDiamond(QPainter &painter, const QPointF &start, const QPointF &end);
// void drawLine(QPainter &painter, const QPointF &start, const QPointF &end);

QLine Canvas::createLine(const QPointF &start, const QPointF &end)
{
    return QLine(start.toPoint(), end.toPoint());
}


// setCanvasState()
// undo()
// redo()


// struct CanvasState 
//     {
//         QVector<LineData> line;
//         QVector<EllipseData> ellipse;
//         QVector<RectangleData> rectangle;
//         QVector<TriangleData> triangle;
//         QVector<DiamondData> diamond;
//     };

void Canvas::undo()
{
    if (!undoStack.isEmpty())
    {
        // Create current state and move it to redo stack
        CanvasState current;
        current.line = std::move(m_lines);
        current.ellipse = std::move(m_ellipses);
        current.rectangle = std::move(m_rectangles);
        current.triangle = std::move(m_triangles);
        current.diamond = std::move(m_diamonds);
        redoStack.append(std::move(current));

        // Move previous state from undo stack
        CanvasState prev = std::move(undoStack.takeLast());
        m_lines = std::move(prev.line);
        m_ellipses = std::move(prev.ellipse);
        m_rectangles = std::move(prev.rectangle);
        m_triangles = std::move(prev.triangle);
        m_diamonds = std::move(prev.diamond);
        update();
    }
}

void Canvas::redo()
{
    if (!redoStack.isEmpty()) 
    {
        // Create current state and move it to undo stack
        CanvasState current;
        current.line = std::move(m_lines);
        current.ellipse = std::move(m_ellipses);
        current.rectangle = std::move(m_rectangles);
        current.triangle = std::move(m_triangles);
        current.diamond = std::move(m_diamonds);
        undoStack.append(std::move(current));

        // Move next state from redo stack
        CanvasState next = std::move(redoStack.takeLast());
        m_lines = std::move(next.line);
        m_ellipses = std::move(next.ellipses);
        m_rectangles = std::move(next.rectangle);
        m_triangles = std::move(next.triangle);
        m_diamonds = std::move(next.diamond); 
        update();
    }
}






