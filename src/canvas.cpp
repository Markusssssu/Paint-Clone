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
    CanvasState state = {m_lines, m_ellipses, m_rectangles, m_triangles, m_diamonds};
    undoStack.append(state);
    redoStack.clear();
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
            LineData data;
            data.line = QLine(m_lastPoint.toPoint(), event->position().toPoint());
            data.color = m_color;
            data.width = m_width;
            m_lines.append(data);
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
                RectangleData rectangleData;
                rectangleData.rectangle = QRect(m_startPoint.toPoint(), endPoint.toPoint());
                rectangleData.color = m_color;
                rectangleData.width = m_width;
                m_rectangles.append(rectangleData);
                break;
            }
            case Ellipse: {
                EllipseData ellipseData;
                ellipseData.ellipse = QRect(m_startPoint.toPoint(), endPoint.toPoint());
                ellipseData.color = m_color;
                ellipseData.width = m_width;
                m_ellipses.append(ellipseData);
                break;
            }
            case Triangle: {
                TriangleData triangleData;
                triangleData.triangle = createTriangle(m_startPoint, endPoint);
                triangleData.color = m_color;
                triangleData.width = m_width;
                m_triangles.append(triangleData);
                break;
            }
            case Diamond: {
                DiamondData diamondData;
                diamondData.diamond = createDiamond(m_startPoint, endPoint);
                diamondData.color = m_color;
                diamondData.width = m_width;
                m_diamonds.append(diamondData);
                break;
            }
            case Line: {
                LineData lineData;
                lineData.line = createLine(m_startPoint, endPoint);
                lineData.color = m_color;
                lineData.width = m_width;
                m_lines.append(lineData);
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
        CanvasState current = {m_lines, m_ellipses, m_rectangles, m_triangles, m_diamonds};
        redoStack.append(current);

        CanvasState prev = undoStack.takeLast();
        m_lines = prev.line;
        m_ellipses = prev.ellipse;
        m_rectangles = prev.rectangle;
        m_triangles = prev.triangle;
        m_diamonds = prev.diamond;
        update();
    }
}

void Canvas::redo()
{
    if (!redoStack.isEmpty()) 
    {
        CanvasState current = {m_lines, m_ellipses, m_rectangles, m_triangles, m_diamonds};
        undoStack.append(current);

        CanvasState next = redoStack.takeLast();
        m_lines = next.line;
        m_ellipses = next.ellipse;
        m_rectangles = next.rectangle;
        m_triangles = next.triangle;
        m_diamonds = next.diamond; 
        update();
    }
}






