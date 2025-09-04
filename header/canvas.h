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
    
    Canvas(Canvas&& other) noexcept;
    Canvas& operator=(Canvas&& other) noexcept;
    
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;
    
    ~Canvas();
    
    void setColor(const QColor &color);
    void setWidth(int width);
    void setDrawingMode(int mode);
    int getWidth() const;
    int getDrawingMode() const;
    QColor getColor() const;
    void undo();
    void redo();
    
    void addLineData(QVector<LineData>&& lines);
    void addRectangleData(QVector<RectangleData>&& rectangles);
    void addEllipseData(QVector<EllipseData>&& ellipses);
    void addTriangleData(QVector<TriangleData>&& triangles);
    void addDiamondData(QVector<DiamondData>&& diamonds);

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
        
        LineData(LineData&& other) noexcept 
            : line(std::move(other.line))
            , color(std::move(other.color))
            , width(other.width) {}
            
        LineData& operator=(LineData&& other) noexcept {
            if (this != &other) {
                line = std::move(other.line);
                color = std::move(other.color);
                width = other.width;
            }
            return *this;
        }
        
        LineData() = default;
        
        LineData(const QLine& l, const QColor& c, int w) 
            : line(l), color(c), width(w) {}
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
        
        TriangleData(TriangleData&& other) noexcept 
            : triangle(std::move(other.triangle))
            , color(std::move(other.color))
            , width(other.width) {}
            
        TriangleData& operator=(TriangleData&& other) noexcept {
            if (this != &other) {
                triangle = std::move(other.triangle);
                color = std::move(other.color);
                width = other.width;
            }
            return *this;
        }
        
        TriangleData() = default;
        
        TriangleData(const QPolygon& t, const QColor& c, int w) 
            : triangle(t), color(c), width(w) {}
    };

    struct DiamondData
    {
        QPolygon diamond;
        QColor color;
        int width;
        
        DiamondData(DiamondData&& other) noexcept 
            : diamond(std::move(other.diamond))
            , color(std::move(other.color))
            , width(other.width) {}
            
        DiamondData& operator=(DiamondData&& other) noexcept {
            if (this != &other) {
                diamond = std::move(other.diamond);
                color = std::move(other.color);
                width = other.width;
            }
            return *this;
        }
        
        DiamondData() = default;
        
    
        DiamondData(const QPolygon& d, const QColor& c, int w) 
            : diamond(d), color(c), width(w) {}
    };

    struct RectangleData
    {
        QRect rectangle;
        QColor color;
        int width;
        
        RectangleData(RectangleData&& other) noexcept 
            : rectangle(std::move(other.rectangle))
            , color(std::move(other.color))
            , width(other.width) {}
            
        RectangleData& operator=(RectangleData&& other) noexcept {
            if (this != &other) {
                rectangle = std::move(other.rectangle);
                color = std::move(other.color);
                width = other.width;
            }
            return *this;
        }
        
        // Default constructor
        RectangleData() = default;
        
        // Constructor with parameters
        RectangleData(const QRect& r, const QColor& c, int w) 
            : rectangle(r), color(c), width(w) {}
    };

    struct EllipseData
    {
        QRect ellipse;
        QColor color;
        int width;
        
        // Move constructor for EllipseData
        EllipseData(EllipseData&& other) noexcept 
            : ellipse(std::move(other.ellipse))
            , color(std::move(other.color))
            , width(other.width) {}
            
        // Move assignment for EllipseData
        EllipseData& operator=(EllipseData&& other) noexcept {
            if (this != &other) {
                ellipse = std::move(other.ellipse);
                color = std::move(other.color);
                width = other.width;
            }
            return *this;
        }
        
        // Default constructor
        EllipseData() = default;
        
        // Constructor with parameters
        EllipseData(const QRect& e, const QColor& c, int w) 
            : ellipse(e), color(c), width(w) {}
    };

private:
    struct CanvasState 
    {
        QVector<LineData> m_line;
        QVector<EllipseData> m_ellipse;
        QVector<RectangleData> m_rectangle;
        QVector<TriangleData> m_triangle;
        QVector<DiamondData> m_diamond;
        
        CanvasState(CanvasState&& other) noexcept 
            : line(std::move(other.line))
            , ellipse(std::move(other.ellipse))
            , rectangle(std::move(other.rectangle))
            , triangle(std::move(other.triangle))
            , diamond(std::move(other.diamond)) {}
            
        CanvasState& operator=(CanvasState&& other) noexcept {
            if (this != &other) {
                line = std::move(other.line);
                ellipse = std::move(other.ellipse);
                rectangle = std::move(other.rectangle);
                triangle = std::move(other.triangle);
                diamond = std::move(other.diamond);
            }
            return *this;
        }
        
        CanvasState() = default;
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
    QVector<CanvasState> m_undoStack;
    QVector<CanvasState> m_redoStack;
};
