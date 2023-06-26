#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <random>

const double PI = 3.14159265359;

// Базовый абстрактный класс
class Curve3D {
public:
    virtual ~Curve3D() {}
    virtual double radius() const = 0;
    virtual double x(double t) const = 0;
    virtual double y(double t) const = 0;
    virtual double z(double t) const = 0;
    virtual double dx(double t) const = 0;
    virtual double dy(double t) const = 0;
    virtual double dz(double t) const = 0;
};

// Класс для круга
class Circle : public Curve3D {
private:
    double m_radius;

public:
    Circle(double radius) : m_radius(radius) {}

    double radius() const override {
        return m_radius;
    }

    double x(double t) const override {
        return m_radius * std::cos(t);
    }

    double y(double t) const override {
        return m_radius * std::sin(t);
    }

    double z(double /*t*/) const override {
        return 0.0;
    }

    double dx(double t) const override {
        return -m_radius * std::sin(t);
    }

    double dy(double t) const override {
        return m_radius * std::cos(t);
    }

    double dz(double /*t*/) const override {
        return 0.0;
    }
};

// Класс для эллипса
class Ellipse : public Curve3D {
private:
    double m_radiusX;
    double m_radiusY;

public:
    Ellipse(double radiusX, double radiusY) : m_radiusX(radiusX), m_radiusY(radiusY) {}

    double radius() const override {
        return std::max(m_radiusX, m_radiusY);
    }

    double x(double t) const override {
        return m_radiusX * std::cos(t);
    }

    double y(double t) const override {
        return m_radiusY * std::sin(t);
    }

    double z(double /*t*/) const override {
        return 0.0;
    }

    double dx(double t) const override {
        return -m_radiusX * std::sin(t);
    }

    double dy(double t) const override {
        return m_radiusY * std::cos(t);
    }

    double dz(double /*t*/) const override {
        return 0.0;
    }
};

// Класс для спирали
class Spiral : public Curve3D {
private:
    double m_radius;
    double m_step;

public:
    Spiral(double radius, double step) : m_radius(radius), m_step(step) {}

    double radius() const override {
        return m_radius;
    }

    double x(double t) const override {
        return m_radius * std::cos(t);
    }

    double y(double t) const override {
        return m_radius * std::sin(t);
    }

    double z(double t) const override {
        return m_step * t / (2.0 * PI);
    }

    double dx(double t) const override {
        return -m_radius * std::sin(t);
    }

    double dy(double t) const override {
        return m_radius * std::cos(t);
    }

    double dz(double /*t*/) const override {
        return m_step / (2.0 * PI);
    }
};

// Рандомайзер
double randomize(double min, double max) {
    std::random_device rd;                               // Генератор случайных чисел из аппаратных источников
    std::mt19937 gen(rd());                              // Инициализация генератора случайных чисел
    std::uniform_real_distribution<double> dist(min, max); // Распределение равномерно в диапазоне [min, max]

    return dist(gen);                                    // Генерация случайного числа
}

int main() {
    // Создаем контейнер для хранения кривых
    std::vector<std::unique_ptr<Curve3D>> curves;

    // Заполняем контейнер случайными кривыми
    curves.push_back(std::make_unique<Circle>(randomize(1.0, 5.0)));
    curves.push_back(std::make_unique<Ellipse>(randomize(1.0, 5.0), randomize(0.5, 3.0)));
    curves.push_back(std::make_unique<Spiral>(randomize(1.0, 5.0), randomize(0.1, 0.5)));

    // Выводим координаты точек и производные кривых в контейнере при t = PI/4
    for (const auto& curve : curves) {
        double t = PI / 4.0;
        std::cout << "Curve type: ";
        if (dynamic_cast<Circle*>(curve.get())) {
            std::cout << "Circle" << std::endl;
        }
        else if (dynamic_cast<Ellipse*>(curve.get())) {
            std::cout << "Ellipse" << std::endl;
        }
        else if (dynamic_cast<Spiral*>(curve.get())) {
            std::cout << "Spiral" << std::endl;
        }

        std::cout << "Point: (" << curve->x(t) << ", " << curve->y(t) << ", " << curve->z(t) << ")" << std::endl;
        std::cout << "Derivative: (" << curve->dx(t) << ", " << curve->dy(t) << ", " << curve->dz(t) << ")" << std::endl;
    }

    // Создаем второй контейнер и заполняем его кругами из первого контейнера
    std::vector<Circle*> circlesOnly;
    for (const auto& curve : curves) {
        if (Circle* circle = dynamic_cast<Circle*>(curve.get())) {
            circlesOnly.push_back(circle);
        }
    }

    // Сортируем второй контейнер по возрастанию радиусов окружностей
    std::sort(circlesOnly.begin(), circlesOnly.end(), [](Circle* a, Circle* b) {
        return a->radius() < b->radius();
        });

    // Выводим отсортированные окружности
    std::cout << "Sorted circles:" << std::endl;
    for (Circle* circle : circlesOnly) {
        std::cout << "Radius: " << circle->radius() << std::endl;
    }

    // Вычисляем общую сумму радиусов всех кривых во втором контейнере
    double totalRadius = 0.0;
    for (Circle* circle : circlesOnly) {
        totalRadius += circle->radius();
    }

    std::cout << "Total radius sum: " << totalRadius << std::endl;

    return 0;
}
