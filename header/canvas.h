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
    void undo();
    void redo();

private:
    void drawTriangle(QPainter &painter, const QPointF &start, const QPointF &end);
    void drawDiamond(QPainter &painter, const QPointF &start, const QPointF &end);

private:
    void setCanvasState();

private:
    QPolygon createTriangle(const QPointF &start, const QPointF &end);
    QPolygon createDiamond(const QPointF &start, const QPointF &end);
    QLine createLine(const QPointF &start, const QPointF &end);

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
        Diamond = 6,
        Line = 7,
        Move = 8
    };

private:
    QColor m_color;
    int m_width;
    int m_drawingMode;
    bool m_drawing;

private:
    struct LineData 
    {
        QLine line;
        QColor color;
        int width;
    };

    struct LinesData 
    {
        QLine lines;
        QColor color;
        int width;
    };

    struct TriangleData
    {
        QPolygon triangle;
        QColor color;
        int width;
    };

    struct DiamondData
    {
        QPolygon diamond;
        QColor color;
        int width;
    };

    struct RectangleData
    {
        QRect rectangle;
        QColor color;
        int width;
    };

    struct EllipseData
    {
        QRect ellipse;
        QColor color;
        int width;
    };

private:
    struct CanvasState 
    {
        QVector<LineData> line;
        QVector<EllipseData> ellipse;
        QVector<RectangleData> rectangle;
        QVector<TriangleData> triangle;
        QVector<DiamondData> diamond;
    };

private:
    QPointF m_lastPoint;
    QPointF m_startPoint;
    QVector<LineData> m_lines;
    QVector<RectangleData> m_rectangles;
    QVector<EllipseData> m_ellipses;
    QVector<TriangleData> m_triangles;
    QVector<DiamondData> m_diamonds;

private:
    QVector<CanvasState> undoStack;
    QVector<CanvasState> redoStack;
};
