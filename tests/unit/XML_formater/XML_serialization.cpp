

#include <iostream>
#include "tinyxml2.h"
#include <catch2/catch_all.hpp>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void serialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElem) const = 0;
    virtual void deserialize(const tinyxml2::XMLElement* element) = 0;

    virtual Shape* deserializeObj(const tinyxml2::XMLElement* element) const = 0;
};

class Circle : public Shape {
private:
    float radius;
public:
    Circle(float r = 0.0f) : radius(r) {}

    void serialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElem) const override {
      tinyxml2::XMLElement* elem = doc.NewElement("Circle");
      elem->SetAttribute("radius", radius);
      parentElem->InsertEndChild(elem);
    }

    void deserialize(const tinyxml2::XMLElement* element) override {
      if (element != nullptr) {
        radius = element->FloatAttribute("radius");
      }
    }

    Circle* deserializeObj(const tinyxml2::XMLElement* element) const override {
      if (element == nullptr) return nullptr;
      return new Circle(element->FloatAttribute("radius"));
    }

    float getRadius() const { return radius; }
};

class Rectangle : public Shape {
private:
    float width;
    float height;
public:
    Rectangle(float w = 0.0f, float h = 0.0f) : width(w), height(h) {}

    void serialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElem) const override {
      tinyxml2::XMLElement* elem = doc.NewElement("Rectangle");
      elem->SetAttribute("width", width);
      elem->SetAttribute("height", height);
      parentElem->InsertEndChild(elem);
    }

    void deserialize(const tinyxml2::XMLElement* element) override {
      if (element != nullptr) {
        width = element->FloatAttribute("width");
        height = element->FloatAttribute("height");
      }
    }

    Rectangle* deserializeObj(const tinyxml2::XMLElement* element) const override {
      if (element == nullptr) return nullptr;
      return new Rectangle(element->FloatAttribute("width"), element->FloatAttribute("height"));
    }

    float getWidth() const { return width; }
    float getHeight() const { return height; }
};



TEST_CASE("Shapes are serialized to XML correctly", "[serialization]") {

  tinyxml2::XMLDocument doc;
  auto *root = doc.NewElement("Shapes");
  doc.InsertFirstChild(root);


  Circle circle(5.0f);
  Rectangle rectangle(10.0f, 2.0f);


  circle.serialize(doc, root);
  rectangle.serialize(doc, root);


  SECTION("Circle is serialized correctly") {
    tinyxml2::XMLElement *circleElement = root->FirstChildElement("Circle");
    REQUIRE(circleElement != nullptr);
    REQUIRE(circleElement->FloatAttribute("radius") == 5.0f);
  }

  SECTION("Rectangle is serialized correctly") {
    tinyxml2::XMLElement *rectangleElement = root->FirstChildElement("Rectangle");
    REQUIRE(rectangleElement != nullptr);
    REQUIRE(rectangleElement->FloatAttribute("width") == 10.0f);
    REQUIRE(rectangleElement->FloatAttribute("height") == 2.0f);
  }
}


TEST_CASE("Shapes are deserialized from XML correctly", "[deserialization]") {
  tinyxml2::XMLDocument doc;

  SECTION("Circle is deserialized correctly") {
    const char* xml = "<Circle radius='10' />";
    doc.Parse(xml);
    const tinyxml2::XMLElement* circleElement = doc.FirstChildElement("Circle");
    Circle circle;
    circle.deserialize(circleElement);

    REQUIRE(circle.getRadius() == Catch::Approx(10.0f));
  }

  SECTION("Rectangle is deserialized correctly") {
    const char* xml = "<Rectangle width='15' height='30' />";
    doc.Parse(xml);
    const tinyxml2::XMLElement* rectangleElement = doc.FirstChildElement("Rectangle");
    Rectangle rectangle;
    rectangle.deserialize(rectangleElement);

    REQUIRE(rectangle.getWidth() == Catch::Approx(15.0f));
    REQUIRE(rectangle.getHeight() == Catch::Approx(30.0f));
  }
}



