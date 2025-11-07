//
//  CircuitModel.hpp
//  CircuitModel
//
//  Created by Hack Audio on 1/20/22.
//

#ifndef CircuitModel_
#define CircuitModel_

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <vector>

#if defined(_MSC_VER)
  #include <memory>
#endif

#include "CircuitElement.h"


namespace PointToPoint {

class CircuitModel
{
    public:
    struct CircuitLayout
    {
        int numNodes = 0;
        CircuitElement::VoltageInput Vin {1,0};
        CircuitElement::VoltageOutput Vout {2,0};
        std::vector<CircuitElement::VoltageSource> Vs;
        std::vector<CircuitElement::Resistor> Rs;
        std::vector<CircuitElement::VariableResistor> VarRs;
        std::vector<CircuitElement::Potentiometer> Pots;
        std::vector<CircuitElement::Capacitor> Cs;
        std::vector<CircuitElement::OPAmp> OPAmps;
        std::vector<CircuitElement::Diode> Ds;
        std::vector<CircuitElement::BJT> BJTs;
        std::vector<CircuitElement::Tube12AX7> Tube12AX7s;
        bool hasDCBlocker = false;
        float Output = -1.f; 
    };
    
    CircuitModel(CircuitLayout layout);
    
    ~CircuitModel();
    
    CircuitModel(const CircuitModel& other);
    CircuitModel& operator=( CircuitModel rhs );
    
    void process(const float * input, float * output, int numSamples, int channel);
    
    void processInPlace(float * buffer, int numSamples, int channel);
    
    float processSample(float x, int channel);
    
    void prepare(double sampleRate, int bufferSize);
    
    void setParameterUpdateIntervalInSamples(int samples4Smooth);
    
    void setSmoothResponseTimeMilliseconds(float milliseconds);
    
    std::vector<double> setParametersNoSmoothing(std::vector<double> parameters);
    
    std::vector<double> setParametersWithSmoothing(std::vector<double> parameters);
    
    bool authenticateWithLicense(std::string license);
    
private:
    class CircuitModelImpl;
    std::unique_ptr<CircuitModelImpl> pImpl;
};

}
#pragma GCC visibility pop
#endif
