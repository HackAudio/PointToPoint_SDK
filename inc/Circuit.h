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
    
    virtual void process(const float* input, float* output, int c, int numSamples) = 0;
    
    virtual void processInPlace(float* buffer, int c, int numSamples) = 0;
    
    virtual float processSample(float x, int c) = 0;
    
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
    
    void process(const float * input, float * output, int numSamples, int channel)
    {
        circuit.process(input,output,numSamples,channel);
    }
    
    void processInPlace(float* buffer, int numSamples, int channel)
    {
        circuit.processInPlace(buffer,numSamples,channel);
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
    
    void process(const float * input, float * output, int c, int numSamples) override {
        circuitChain[0]->process(input,output,c,numSamples);
        float * pInput = output;
        for (int m = 1; m < circuitChain.size() ; ++m)
            circuitChain[m]->process(pInput,output,c,numSamples);
    }
    
    void processInPlace(float* buffer, int c, int numSamples) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            circuitChain[m]->processInPlace(buffer,c,numSamples);
    }
    
    float processSample(float x, int c) override {
        for (int m = 0; m < circuitChain.size() ; ++m)
            x = circuitChain[m]->processSample(x,c);
        
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



