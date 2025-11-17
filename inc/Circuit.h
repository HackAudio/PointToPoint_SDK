//
//  Circuit.h
//
//  Base classes for circuits in the PointToPoint library
//  Created by Eric Tarr on 2/13/22.
//
//

#include "CircuitModel.h"

#pragma once

namespace PointToPoint {

//-------------------Don't change these base classes ---------
class CircuitProcessor
{
public:
    
    virtual ~CircuitProcessor() {};
    
    virtual void process(const float* input, float* output, int channel, int numSamples) = 0;
    
    virtual void processInPlace(float* buffer, int channel, int numSamples) = 0;
    
    virtual float processSample(float x, int channel) = 0;
    
    virtual void prepare(double sampleRate, int bufferSize) = 0;
    
    virtual std::vector<double> setParametersNoSmoothing(std::vector<double> parameters) = 0;
    
    virtual std::vector<double> setParametersWithSmoothing(std::vector<double> parameters) = 0;
    
    virtual void setParameterUpdateIntervalInSamples(int samples4Smooth) = 0;
    
    virtual void setSmoothResponseTimeMilliseconds(float milliseconds) = 0;
    
    virtual bool authenticateWithLicense(std::string license) = 0;

};


class Circuit : public CircuitProcessor
{
public:
    
    void process(const float * input, float * output, int channel, int numSamples)
    {
        circuit.process(input,output,channel,numSamples);
    }
    
    void processInPlace(float* buffer, int channel, int numSamples)
    {
        circuit.processInPlace(buffer,channel,numSamples);
    }
    
    float processSample(float x, int channel)
    {
        return circuit.processSample(x,channel);
    }
    
    void prepare(double sampleRate, int bufferSize) {
        circuit.prepare(sampleRate,bufferSize);
    }
    
    std::vector<double> setParametersNoSmoothing(std::vector<double> parameters){
        return circuit.setParametersNoSmoothing(parameters);
    }
    
    std::vector<double> setParametersWithSmoothing(std::vector<double> parameters){
        return circuit.setParametersWithSmoothing(parameters);
    }
    
    void setParameterUpdateIntervalInSamples(int samples4Smooth){
        circuit.setParameterUpdateIntervalInSamples(samples4Smooth);
    }
    
    void setSmoothResponseTimeMilliseconds(float milliseconds){
        circuit.setSmoothResponseTimeMilliseconds(milliseconds);
    }
    
    bool authenticateWithLicense(std::string license){
        return circuit.authenticateWithLicense(license);
    }
protected:
    
    CircuitModel::CircuitLayout layout;
    
    CircuitModel circuit {layout};
};




class CircuitChain : public CircuitProcessor
{
public:
    
    virtual ~CircuitChain() {};
    
    void process(const float * input, float * output, int channel, int numSamples) override {
        circuitChain[0]->process(input,output,channel,numSamples);
        float * pInput = output;
        for (int m = 1; m < circuitChain.size() ; ++m)
            circuitChain[m]->process(pInput,output,channel,numSamples);
    }
    
    void processInPlace(float* buffer, int channel, int numSamples) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            circuitChain[m]->processInPlace(buffer,channel,numSamples);
    }
    
    float processSample(float x, int channel) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            x = circuitChain[m]->processSample(x,channel);
        
        return x;
    }
    
    void prepare(double sampleRate, int bufferSize) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            circuitChain[m]->prepare(sampleRate,bufferSize);
    }
    
    std::vector<double> setParametersNoSmoothing(std::vector<double> parameters) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            parameters = circuitChain[m]->setParametersNoSmoothing(parameters);
        
        return parameters;
    }
    
    std::vector<double> setParametersWithSmoothing(std::vector<double> parameters) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            parameters = circuitChain[m]->setParametersWithSmoothing(parameters);
        
        return parameters;
    }
    
    void setParameterUpdateIntervalInSamples(int samples4Smooth) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            circuitChain[m]->setParameterUpdateIntervalInSamples(samples4Smooth);
    }
    
    void setSmoothResponseTimeMilliseconds(float milliseconds) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            circuitChain[m]->setSmoothResponseTimeMilliseconds(milliseconds);
    }
    
    bool authenticateWithLicense(std::string license) override {
        bool isLicensed;
        for (int m = 0; m < circuitChain.size() ; ++m){
            isLicensed = circuitChain[m]->authenticateWithLicense(license);
        }
        return isLicensed;
    }
    
protected:
    
    std::vector<std::shared_ptr<CircuitProcessor>> circuitChain;
};
//-------------^^^^ Don't change this "CircuitChain" base class ^^^^---------


// End of namespace
}



