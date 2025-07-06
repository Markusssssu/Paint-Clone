#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QVector>
#include <QLine>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    
    void setColor(const QColor &color);
    void setWidth(int width);
    void setDrawingMode(int mode);
    int getWidth() const;
    int getDrawingMode() const;
    QColor getColor() const;

private:
    void drawTriangle(QPainter &painter, const QPointF &start, const QPointF &end);
    void drawDiamond(QPainter &painter, const QPointF &start, const QPointF &end);
    QPolygon createTriangle(const QPointF &start, const QPointF &end);
    QPolygon createDiamond(const QPointF &start, const QPointF &end);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    enum DrawingMode {
        Brush = 0,
        Pen = 1,
        Pencil = 2,
        Triangle = 3,
        Rectangle = 4,
        Ellipse = 5,
        Diamond = 6
    };

private:
    QColor m_color;
    int m_width;
    int m_drawingMode;
    bool m_drawing;
    QPointF m_lastPoint;
    QPointF m_startPoint;
    QVector<QLine> m_lines;
    QVector<QRect> m_rectangles;
    QVector<QRect> m_ellipses;
    QVector<QPolygon> m_triangles;
    QVector<QPolygon> m_diamonds;
};
