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
    
    painter.setPen(QPen(m_color, m_width));
    painter.setBrush(QBrush(m_color));
    
    for (const QLine &line : m_lines) {
        painter.drawLine(line);
    }
    
    for (const QRect &rect : m_rectangles) {
        painter.drawRect(rect);
    }
    
    for (const QRect &ellipse : m_ellipses) {
        painter.drawEllipse(ellipse);
    }
    
    for (const QPolygon &triangle : m_triangles) {
        painter.drawPolygon(triangle);
    }
    
    for (const QPolygon &diamond : m_diamonds) {
        painter.drawPolygon(diamond);
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
        }
    }
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
            // Для кистей рисуем линии при движении
            QLine line(m_lastPoint.toPoint(), event->position().toPoint());
            m_lines.append(line);
            m_lastPoint = event->position();
        }
        update(); // Перерисовываем для показа текущего элемента
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
            case Rectangle:
                m_rectangles.append(QRect(m_startPoint.toPoint(), endPoint.toPoint()));
                break;
            case Ellipse:
                m_ellipses.append(QRect(m_startPoint.toPoint(), endPoint.toPoint()));
                break;
            case Triangle:
                m_triangles.append(createTriangle(m_startPoint, endPoint));
                break;
            case Diamond:
                m_diamonds.append(createDiamond(m_startPoint, endPoint));
                break;
        }
        
        update();
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
