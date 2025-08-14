# Move Semantics в Paint Clone

## Обзор

Этот проект демонстрирует грамотное использование move-semantics в C++ для оптимизации производительности при работе с графическими данными в приложении для рисования.

## Что такое Move Semantics?

Move semantics - это механизм в C++11+, который позволяет "перемещать" ресурсы между объектами вместо их копирования, что значительно повышает производительность при работе с большими объектами.

### Основные концепции:

1. **Move Constructor** - `ClassName(ClassName&& other)`
2. **Move Assignment Operator** - `ClassName& operator=(ClassName&& other)`
3. **Rvalue References** - `ClassName&&`
4. **std::move()** - функция для преобразования lvalue в rvalue

## Реализация в проекте

### 1. Canvas Class

```cpp
class Canvas : public QWidget {
public:
    // Move constructor
    Canvas(Canvas&& other) noexcept;
    
    // Move assignment operator
    Canvas& operator=(Canvas&& other) noexcept;
    
    // Delete copy operations to enforce move semantics
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;
    
    // Move methods for efficient data transfer
    void addLineData(QVector<LineData>&& lines);
    void addRectangleData(QVector<RectangleData>&& rectangles);
    // ... другие методы
};
```

### 2. Структуры данных с Move Semantics

```cpp
struct LineData {
    QLine line;
    QColor color;
    int width;
    
    // Move constructor
    LineData(LineData&& other) noexcept 
        : line(std::move(other.line))
        , color(std::move(other.color))
        , width(other.width) {}
        
    // Move assignment
    LineData& operator=(LineData&& other) noexcept {
        if (this != &other) {
            line = std::move(other.line);
            color = std::move(other.color);
            width = other.width;
        }
        return *this;
    }
    
    // Constructor with parameters
    LineData(const QLine& l, const QColor& c, int w) 
        : line(l), color(c), width(w) {}
};
```

### 3. Оптимизация методов Undo/Redo

```cpp
void Canvas::undo() {
    if (!undoStack.isEmpty()) {
        // Create current state and move it to redo stack
        CanvasState current;
        current.line = std::move(m_lines);
        current.ellipse = std::move(m_ellipses);
        // ... другие данные
        
        redoStack.append(std::move(current));
        
        // Move previous state from undo stack
        CanvasState prev = std::move(undoStack.takeLast());
        m_lines = std::move(prev.line);
        // ... восстановление состояния
    }
}
```

## Преимущества Move Semantics

### 1. Производительность
- **Без move semantics**: копирование всех данных при каждой операции
- **С move semantics**: перемещение указателей и освобождение ресурсов

### 2. Эффективность памяти
```cpp
// До оптимизации (копирование)
QVector<LineData> newLines = m_lines; // Копирует все элементы

// После оптимизации (перемещение)
QVector<LineData> newLines = std::move(m_lines); // Перемещает указатели
```

### 3. Оптимизация операций рисования
```cpp
// Создание фигур с move semantics
case Rectangle: {
    RectangleData rectangleData(QRect(m_startPoint.toPoint(), endPoint.toPoint()), m_color, m_width);
    m_rectangles.append(std::move(rectangleData));
    break;
}
```

## Примеры использования

### 1. Перемещение Canvas
```cpp
Canvas originalCanvas(nullptr);
originalCanvas.setColor(Qt::blue);
originalCanvas.setWidth(5);

// Используем move constructor
Canvas movedCanvas = std::move(originalCanvas);
```

### 2. Перемещение данных
```cpp
QVector<LineData> lines1;
lines1.append(LineData(QLine(0, 0, 100, 100), Qt::red, 2));

// Эффективная передача данных
QVector<LineData> lines2 = std::move(lines1);
```

### 3. Оптимизация состояний
```cpp
CanvasState state1;
state1.line.append(LineData(QLine(0, 0, 100, 100), Qt::red, 2));

CanvasState state2;
state2.ellipse.append(EllipseData(QRect(20, 20, 60, 60), Qt::green, 3));

// Объединение состояний с move semantics
state1 = std::move(state2);
```

## Лучшие практики

### 1. Всегда используйте `noexcept`
```cpp
Canvas(Canvas&& other) noexcept; // Правильно
Canvas(Canvas&& other);          // Неправильно
```

### 2. Проверяйте self-assignment
```cpp
Canvas& operator=(Canvas&& other) noexcept {
    if (this != &other) {  // Важно!
        // ... перемещение данных
    }
    return *this;
}
```

### 3. Используйте `std::move` для rvalue
```cpp
// Правильно
undoStack.append(std::move(current));

// Неправильно
undoStack.append(current); // Копирование вместо перемещения
```

### 4. Очищайте ресурсы после перемещения
```cpp
// В move constructor/assignment
other.colorPickBtn = nullptr;
other.canvas = nullptr;
other.titleBar = nullptr;
```

## Тестирование производительности

Проект включает примеры для сравнения производительности:

```cpp
void performanceComparison() {
    const int iterations = 10000;
    
    // Тест с копированием
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        QVector<LineData> lines1;
        lines1.append(LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        QVector<LineData> lines2 = lines1; // Копирование
    }
    auto copyTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Тест с move semantics
    for (int i = 0; i < iterations; ++i) {
        QVector<LineData> lines1;
        lines1.append(LineData(QLine(0, 0, 100, 100), Qt::red, 2));
        QVector<LineData> lines2 = std::move(lines1); // Move semantics
    }
    auto moveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Performance improvement: " << (double)copyTime.count() / moveTime.count() << "x\n";
}
```

## Заключение

Move semantics значительно улучшают производительность приложения для рисования, особенно при:

- Работе с большими объемами графических данных
- Операциях Undo/Redo
- Передаче данных между компонентами
- Создании и уничтожении объектов

Правильное использование move semantics делает код более эффективным и современным, следуя принципам C++11+.
