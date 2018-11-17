#ifndef LOGICAL_COMPONENTS_HPP
#define LOGICAL_COMPONENTS_HPP 
#include <memory>
class Component {
public:
    virtual double voltage() const = 0;
};
class InputComponent : public Component {
public:
    InputComponent(double in);
    double voltage() const;
private:
    double m_in;
};
// You can make Component from other Components, or from doubles which will
// instance InputComponents and add them into tree
class LOGComponent : public Component {
public:
  LOGComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2 = nullptr)
      :m_in1(in1), m_in2(in2)
  {}
  LOGComponent(double in1 = 0, double in2 = 0)
      :m_in1(std::make_shared<InputComponent>(in1)),m_in2(std::make_shared<InputComponent>(in2))
  {}


protected:
  std::shared_ptr<Component>  m_in1, m_in2;
};

class ANDComponent : public LOGComponent {
public:
    ANDComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
class ORComponent : public LOGComponent {
public:
    ORComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
class XORComponent : public LOGComponent {
public:
    XORComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
class NOTComponent : public LOGComponent {
public:
    NOTComponent(std::shared_ptr<Component> in)
        : LOGComponent(in)
    {}

    double voltage() const;

};
class NANDComponent : public LOGComponent {
public:
    NANDComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
class NORComponent : public LOGComponent {
public:
    NORComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
class NXORComponent : public LOGComponent {
public:
    NXORComponent(std::shared_ptr<Component> in1, std::shared_ptr<Component> in2)
        : LOGComponent(in1, in2)
    {}
    double voltage() const;
};
#endif /* ifndef LOGICAL_COMPONENTS_HPP */
