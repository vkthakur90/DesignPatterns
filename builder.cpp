#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <string>
#include <concepts>
#include <type_traits>

template <std::floating_point FloatType>
constexpr FloatType PI = std::atan2(static_cast<FloatType>(0), static_cast<FloatType>(-1));

template <std::floating_point FloatType>
struct IfaceShape {
    virtual FloatType getArea() const = 0;
    virtual FloatType getPerimeter() const = 0;
    virtual ~IfaceShape() = default;
};

template <std::floating_point FloatType>
using Shape = std::unique_ptr<IfaceShape<FloatType>>;

// Concrete Circle implementing shape and builder
template <std::floating_point FloatType>
class CircleBuilder : public IfaceShape<FloatType> {
public:
    void setRadius(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Circle radius must be positive, got: " + std::to_string(val));
        }
        radius = val;
    }

    void compute() {
        area = PI<FloatType> * radius * radius;
        perimeter = static_cast<FloatType>(2) * PI<FloatType> * radius;
    }
    
    FloatType getArea() const override final {
        return area;
    }

    FloatType getPerimeter() const override final {
        return perimeter;
    }

private:
    FloatType radius{0};
    FloatType area{0};
    FloatType perimeter{0};    
};

// Concrete Rectangle implementing shape and builder
template <std::floating_point FloatType>
class RectangleBuilder : public IfaceShape<FloatType> {
public:
    void setLength(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Rectangle length must be positive, got: " + std::to_string(val));
        }
        length = val;
    }
    
    void setBreadth(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Rectangle breadth must be positive, got: " + std::to_string(val));
        }
        breadth = val;
    }

    void compute() {
        area = length * breadth;
        perimeter = static_cast<FloatType>(2) * (length + breadth);
    }
    
    FloatType getArea() const override final {
        return area;
    }
    
    FloatType getPerimeter() const override final {
        return perimeter;
    }
    
private:
    FloatType length{0};
    FloatType breadth{0}; 
    FloatType area{0};
    FloatType perimeter{0};    
};

// Concrete Triangle implementing shape and builder
template <std::floating_point FloatType>
class TriangleBuilder : public IfaceShape<FloatType> {
public:
    void setSideA(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Triangle side A must be positive, got: " + std::to_string(val));
        }
        a = val;
    }
    
    void setSideB(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Triangle side B must be positive, got: " + std::to_string(val));
        }
        b = val;
    }
    
    void setSideC(FloatType val) {
        if (val <= static_cast<FloatType>(0)) {
            throw std::invalid_argument("Triangle side C must be positive, got: " + std::to_string(val));
        }
        c = val;
    }

    void compute() {
        // Optional: validate triangle inequality
        if (a + b <= c || a + c <= b || b + c <= a) {
            throw std::domain_error("Triangle sides do not satisfy the triangle inequality");
        }
        FloatType s = (a + b + c) / static_cast<FloatType>(2);
        area = std::sqrt(s * (s - a) * (s - b) * (s - c));
        perimeter = a + b + c;
    }
    
    FloatType getArea() const override final {
        return area;
    }
    
    FloatType getPerimeter() const override final {
        return perimeter;
    }
    
private:
    FloatType a{0};
    FloatType b{0};
    FloatType c{0};
    FloatType area{0};
    FloatType perimeter{0};
};

template <std::floating_point FloatType>
struct ShapeFactory {
    static Shape<FloatType> createCircle(FloatType r){
        auto obj = std::make_unique<CircleBuilder<FloatType>>();
        obj->setRadius(r);
        obj->compute();
        
        return obj;
    }
    
    static Shape<FloatType> createRectangle(FloatType l, FloatType b){
        auto obj = std::make_unique<RectangleBuilder<FloatType>>();
     
        obj->setLength(l);
        obj->setBreadth(b);
        obj->compute();

        return obj;        
    }
    
    static Shape<FloatType> createTriangle(FloatType a, FloatType b, FloatType c){
        auto obj = std::make_unique<TriangleBuilder<FloatType>>();
        
        obj->setSideA(a);
        obj->setSideB(b);
        obj->setSideC(c);
        obj->compute();
        
        return obj;
    }
    
    static Shape<FloatType> createSquare(FloatType side){
        auto obj = std::make_unique<RectangleBuilder<FloatType>>();
        
        obj->setLength(side);
        obj->setBreadth(side);
        obj->compute();
        
        return obj;
    }
    
    static Shape<FloatType> createIsoscalesTriangle(FloatType equal_side, FloatType other_side){
        auto obj = std::make_unique<TriangleBuilder<FloatType>>();
        
        obj->setSideA(equal_side);
        obj->setSideB(equal_side);
        obj->setSideC(other_side);
        obj->compute();
        
        return obj;
    }

    static Shape<FloatType> createEquilateralTriangle(FloatType side) {
        auto obj = std::make_unique<TriangleBuilder<FloatType>>();
        
        obj->setSideA(side);
        obj->setSideB(side);
        obj->setSideC(side);
        obj->compute();
        
        return obj;
    }    
};

int main() {
    // 1. Create a vector of shapes (using double precision)
    std::vector< Shape<double> > shapes;
    
    // 2. Fill it using the factory
    shapes.push_back(ShapeFactory<double>::createCircle(3.5));
    shapes.push_back(ShapeFactory<double>::createRectangle(4.0, 2.5));
    shapes.push_back(ShapeFactory<double>::createSquare(5.0));
    shapes.push_back(ShapeFactory<double>::createTriangle(3.0, 4.0, 5.0));
    shapes.push_back(ShapeFactory<double>::createIsoscalesTriangle(6.0, 4.0));
    shapes.push_back(ShapeFactory<double>::createEquilateralTriangle(2.5));

    // 3. Print header
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Shape #\tArea\t\tPerimeter\n";
    std::cout << "-------\t----\t\t---------\n";

    // 4. Iterate and print
    for (size_t i = 0; i < shapes.size(); ++i) {
        double area      = shapes[i]->getArea();
        double perimeter = shapes[i]->getPerimeter();
        std::cout << (i+1)
                  << "\t" << area
                  << "\t\t" << perimeter
                  << "\n";
    }

    return 0;
}

