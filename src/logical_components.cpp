#include "logical_components.hpp"

InputComponent::InputComponent(double in){
    m_in = in >= 2.5 ? 1 : 0; 
}
double InputComponent::voltage() const{
    return m_in;
}
double ANDComponent::voltage() const{
    return (m_in1->voltage() && m_in2->voltage());
}
double ORComponent::voltage() const{
    return (m_in1->voltage()|| m_in2->voltage());
}
double XORComponent::voltage() const{
    return m_in1->voltage() == m_in2->voltage() ? 0.0 : 1.0; 
}
double NOTComponent::voltage() const{
    return m_in1->voltage() == 1.0 ? 0.0 : 1.0;
}
double NANDComponent::voltage() const{
    return !(m_in1->voltage() && m_in2->voltage());
}
double NORComponent::voltage() const{
    return !(m_in1->voltage() || m_in2->voltage());
}
double NXORComponent::voltage() const{
    return m_in1->voltage() == m_in2->voltage() ? 1.0 : 0.0;
}
