#include "../header/canvas.h"
#include "../header/mainwindow.h"
#include <QApplication>
#include <QVector>
#include <iostream>

// Примеры использования move-semantics в проекте Paint Clone

void demonstrateMoveSemantics() {
    std::cout << "=== Move Semantics Examples ===\n\n";
    
    // 1. Пример с Canvas - создание и перемещение
    std::cout << "1. Canvas Move Constructor Example:\n";
    {
        Canvas originalCanvas(nullptr);
        originalCanvas.setColor(Qt::blue);
        originalCanvas.setWidth(5);
        
        // Используем move constructor
        Canvas movedCanvas = std::move(originalCanvas);
        
        std::cout << "   - Original canvas color: " << originalCanvas.getColor().name().toStdString() << "\n";
        std::cout << "   - Moved canvas color: " << movedCanvas.getColor().name().toStdString() << "\n";
        std::cout << "   - Original canvas width: " << originalCanvas.getWidth() << "\n";
        std::cout << "   - Moved canvas width: " << movedCanvas.getWidth() << "\n";
    }
    
    // 2. Пример с move assignment
    std::cout << "\n2. Canvas Move Assignment Example:\n";
    {
        Canvas canvas1(nullptr);
        Canvas canvas2(nullptr);
        
        canvas1.setColor(Qt::red);
        canvas1.setWidth(3);
        canvas2.setColor(Qt::green);
        canvas2.setWidth(7);
        
        // Используем move assignment
        canvas1 = std::move(canvas2);
        
        std::cout << "   - Canvas1 color after move: " << canvas1.getColor().name().toStdString() << "\n";
        std::cout << "   - Canvas1 width after move: " << canvas1.getWidth() << "\n";
    }
    
    // 3. Пример с векторами данных
    std::cout << "\n3. Vector Data Move Example:\n";
    {
        QVector<Canvas::LineData> lines1;
        lines1.append(Canvas::LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        lines1.append(Canvas::LineData(QLine(100, 100, 200, 200), Qt::blue, 3));
        
        QVector<Canvas::LineData> lines2;
        lines2.append(Canvas::LineData(QLine(0, 0, 50, 50), Qt::green, 1));
        
        std::cout << "   - Lines1 size before move: " << lines1.size() << "\n";
        std::cout << "   - Lines2 size before move: " << lines2.size() << "\n";
        
        // Используем move для эффективной передачи данных
        lines1 = std::move(lines2);
        
        std::cout << "   - Lines1 size after move: " << lines1.size() << "\n";
        std::cout << "   - Lines2 size after move: " << lines2.size() << "\n";
    }
    
    // 4. Пример с CanvasState
    std::cout << "\n4. CanvasState Move Example:\n";
    {
        Canvas::CanvasState state1;
        state1.line.append(Canvas::LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        state1.rectangle.append(Canvas::RectangleData(QRect(10, 10, 50, 50), Qt::blue, 1));
        
        Canvas::CanvasState state2;
        state2.ellipse.append(Canvas::EllipseData(QRect(20, 20, 60, 60), Qt::green, 3));
        
        std::cout << "   - State1 line count: " << state1.line.size() << "\n";
        std::cout << "   - State1 rectangle count: " << state1.rectangle.size() << "\n";
        std::cout << "   - State2 ellipse count: " << state2.ellipse.size() << "\n";
        
        // Используем move для объединения состояний
        state1 = std::move(state2);
        
        std::cout << "   - State1 ellipse count after move: " << state1.ellipse.size() << "\n";
        std::cout << "   - State2 ellipse count after move: " << state2.ellipse.size() << "\n";
    }
    
    // 5. Пример с MainWindow
    std::cout << "\n5. MainWindow Move Example:\n";
    {
        MainWindow window1(nullptr);
        MainWindow window2(nullptr);
        
        // Используем move constructor
        MainWindow movedWindow = std::move(window1);
        
        std::cout << "   - Window1 created successfully\n";
        std::cout << "   - Moved window created successfully\n";
    }
    
    std::cout << "\n=== Move Semantics Examples Completed ===\n";
}

// Функция для демонстрации производительности
void performanceComparison() {
    std::cout << "\n=== Performance Comparison ===\n";
    
    const int iterations = 10000;
    
    // Тест с копированием
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        QVector<Canvas::LineData> lines1;
        lines1.append(Canvas::LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        lines1.append(Canvas::LineData(QLine(100, 100, 200, 200), Qt::blue, 3));
        
        QVector<Canvas::LineData> lines2 = lines1; // Копирование
        lines2.append(Canvas::LineData(QLine(200, 200, 300, 300), Qt::green, 1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto copyTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Тест с move semantics
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        QVector<Canvas::LineData> lines1;
        lines1.append(Canvas::LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        lines1.append(Canvas::LineData(QLine(100, 100, 200, 200), Qt::blue, 3));
        
        QVector<Canvas::LineData> lines2 = std::move(lines1); // Move semantics
        lines2.append(Canvas::LineData(QLine(200, 200, 300, 300), Qt::green, 1));
    }
    end = std::chrono::high_resolution_clock::now();
    auto moveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Copy time: " << copyTime.count() << " microseconds\n";
    std::cout << "Move time: " << moveTime.count() << " microseconds\n";
    std::cout << "Performance improvement: " << (double)copyTime.count() / moveTime.count() << "x\n";
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    std::cout << "Paint Clone - Move Semantics Examples\n";
    std::cout << "=====================================\n\n";
    
    demonstrateMoveSemantics();
    performanceComparison();
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}
