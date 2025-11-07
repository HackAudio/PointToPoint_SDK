//
//  CircuitElement.h
//
//  Collection of the available circuit elements
//  in the PointToPoint library
//  Created by Eric Tarr on 11/4/21.
//
//

#pragma once
#include <cmath>

namespace PointToPoint {

// Class definition
class CircuitElement
{

public:
	
    //---------------------------------------------------
    
    class VoltageInput
    {
    public:
        VoltageInput (int node1,int node2):
                    node1(node1),node2(node2) {};
        
        int getNode1() {return node1;}
        int getNode2() {return node2;}
    private:
        int node1;
        int node2;
    };
    
    //---------------------------------------------------
    
    class VoltageOutput
    {
    public:
        VoltageOutput (int node1,int node2):
                    node1(node1),node2(node2) {};
        
        int getNode1() {return node1;}
        int getNode2() {return node2;}
    private:
        int node1;
        int node2;
    };
    
    //---------------------------------------------------
    
    class VoltageSource
    {
    public:
        VoltageSource (double voltage,int node):
                    voltage(voltage),node(node) {};
        
        int getVoltage() {return voltage;}
        int getNode() {return node;}
    private:
        double voltage;
        int node;
    };
    
    //---------------------------------------------------
    
    class Resistor
    {
    public:
        Resistor (double value,int node1,int node2):
                    value(value),node1(node1),node2(node2) {};
        
        double getValue() {return value;}
        int getNode1() {return node1;}
        int getNode2() {return node2;}
    private:
        double value;
        int node1;
        int node2;
    };
    
    enum ParamDirection {PROPORTIONAL,INVPROPORTIONAL};
    enum Skew {LINEAR,LOG};
    
    class VariableResistor
    {
    public:
        
        VariableResistor (double maxValue,
                       int parameterNumber,
                       ParamDirection direction,
                       Skew skew,
                       int node1, int node2,
                       double initValue = 1.0):
            maxValue(maxValue),
            parameterNumber(parameterNumber-1),
            direction(direction),
            skew(skew),
            node1(node1),
        node2(node2){setValue(initValue);};
        
        double getMaxValue() {return maxValue;}
        int getParameterNumber() {return parameterNumber;}
        void setValue(double normValue) {setNormValue(normValue);}
        double getValue() {return calculateValue();}
        double getNode1() {return node1;}
        double getNode2() {return node2;}
        void setParamDirection(ParamDirection paramDirection) {direction = paramDirection;}
        
    private:
        double maxValue;
        double value = 1.0;
        
        int parameterNumber;
        ParamDirection direction;
        Skew skew;
        
        int node1;
        int node2;
        
        void setNormValue(double normValue)
        {
            if (normValue < 0.0)
                value = 0.0;
            else if(normValue > 1.0)
                value = 1.0;
            else
                value = normValue;
        }
        
        double calculateValue(){
            double tempValue = value;
            if (direction == ParamDirection::INVPROPORTIONAL)
                tempValue = 1.0 - value;
            
            if (skew == Skew::LOG)
                tempValue = std::pow(tempValue,10.0);
            
            if (tempValue < 0.001)
                tempValue = 0.001;
            
            tempValue *= maxValue;
            
            return tempValue;
        }
    };
    
    //---------------------------------------------------
    
    class Potentiometer
    {
    public:
        
        Potentiometer (double maxValue,
                       int paramNum,
                       ParamDirection direction,
                       Skew skew,
                       int node1, int node2, int node3,
                       double initValue = 1.0):
            maxValue(maxValue),
            parameterNumber(paramNum-1),
            direction(direction),
            skew(skew),
            node1(node1),
        node2(node2),
        node3(node3),
        variableResistor1(maxValue, paramNum, direction,
                          skew, node1, node2, initValue),
        variableResistor2(maxValue, paramNum, direction,
                          skew, node2, node3, initValue)
        {
            setValue(initValue);
            if (direction == ParamDirection::PROPORTIONAL){
                variableResistor1.setParamDirection(ParamDirection::PROPORTIONAL);
                variableResistor2.setParamDirection(ParamDirection::INVPROPORTIONAL);
            }
            else {
                variableResistor1.setParamDirection(ParamDirection::INVPROPORTIONAL);
                variableResistor2.setParamDirection(ParamDirection::PROPORTIONAL);
            }
        };

        VariableResistor getVariableResistor1() {return variableResistor1;}
        VariableResistor getVariableResistor2() {return variableResistor2;}
        
        double getMaxValue() {return maxValue;}
        int getParameterNumber() {return parameterNumber;}
        void setValue(double normValue) {setNormValue(normValue);}
        double getValue() {return calculateValue();}
        double getNode1() {return node1;}
        double getNode2() {return node2;}
        double getNode3() {return node3;}
        

    private:
        double maxValue;
        double value = 1.0;
        
        int parameterNumber;
        ParamDirection direction;
        Skew skew;
        
        int node1;
        int node2;
        int node3;
        
        VariableResistor variableResistor1;
        VariableResistor variableResistor2;
        
        void setNormValue(double normValue)
        {
            if (normValue < 0.0)
                value = 0.0;
            else if(normValue > 1.0)
                value = 1.0;
            else
                value = normValue;
            
            variableResistor1.setValue(value);
            variableResistor2.setValue(value);
        }
        
        double calculateValue(){
            double tempValue = value;
            if (direction == ParamDirection::INVPROPORTIONAL)
                tempValue = 1.0 - value;
            
            if (skew == Skew::LOG)
                tempValue = std::pow(tempValue,10.0);
            
            if (tempValue < 0.001)
                tempValue = 0.001;
            
            tempValue *= maxValue;
            
            return tempValue;
        }
    };
    //---------------------------------------------------
    
    class Capacitor
    {
    public:
        Capacitor (double value,int node1,int node2):
                    value(value),node1(node1),node2(node2) {};
        
        double getValue() {return value;}
        int getNode1() {return node1;}
        int getNode2() {return node2;}
    private:
        double value;
        int node1;
        int node2;
    };
    
    //---------------------------------------------------
    
    class OPAmp
    {
    public:
        OPAmp (int nonInvertingNode,
               int invertingNode,
               int outputNode,
               int nodeConnectedToGround = 0):
                    nonInvertingNode(nonInvertingNode),
                    invertingNode(invertingNode),
                    outputNode(outputNode){};
        
        int getNonInvertingNode() {return nonInvertingNode;}
        int getInvertingNode() {return invertingNode;}
        int getOutputNode() {return outputNode;}
        
    private:
        int nonInvertingNode;
        int invertingNode;
        int outputNode;
    };
    
    //---------------------------------------------------
    
    class Diode
    {
    public:
        enum Semiconductor {SILICON,GERMANIUM};
        enum Configuration {SINGLE,MATCHEDPAIR};
        
        Diode (Semiconductor type,
               Configuration config,
                       int node1, int node2):
            type(type),
            config(config),
            node1(node1),
        node2(node2){
            if (type == Semiconductor::SILICON){
                Is = 1.0e-12;
                eta = 1.0;
            }
            else{ // GERMANIUM
                Is = 1.0e-6;
                eta = 1.68;
            }
            Vt = 26.0e-3;
        };
        
        double getSaturationCurrent() {return Is;}
        void setSaturationCurrent(double saturationCurrent){Is = saturationCurrent;}
        double getThermalVoltage() {return Vt;}
        void setThermalVoltage(double thermalVoltage) {Vt = thermalVoltage;}
        double getEmissionCoeff() {return eta;}
        void setEmissionCoeff(double emissionCoeff) {eta = emissionCoeff;}
        
        Configuration getConfiguration() {return config;}
        
        double getNode1() {return node1;}
        double getNode2() {return node2;}
    private:
        
        Semiconductor type;
        Configuration config;
        
        int node1;
        int node2;
        
        double Is;
        double Vt;
        double eta;
        
    };
    
    //---------------------------------------------------
    
    class BJT
    {
    public:
        enum Doping { NPN , PNP };
        enum Semiconductor { SILICON , GERMANIUM };
        
        
        BJT (Doping doping, Semiconductor type,
                       int nodeBase, int nodeCollector, int nodeEmitter):
            doping(doping),
            type(type),
            nodeBase(nodeBase),
            nodeCollector(nodeCollector),
            nodeEmitter(nodeEmitter){
            if (type == Semiconductor::SILICON){
                Is = 1.0e-12;
                eta = 1.0;
            }
            else{ // GERMANIUM
                Is = 1.0e-6;
                eta = 1.68;
            }
            Vt = 26.0e-3;
        };
        
        double getSaturationCurrent() {return Is;}
        void setSaturationCurrent(double saturationCurrent){Is = saturationCurrent;}
        double getThermalVoltage() {return Vt;}
        void setThermalVoltage(double thermalVoltage) {Vt = thermalVoltage;}
        double getEmissionCoeff() {return eta;}
        void setEmissionCoeff(double emissionCoeff) {eta = emissionCoeff;}
        
        Doping getDoping() {return doping;}
        
        double getNodeBase() {return nodeBase;}
        double getNodeCollector() {return nodeCollector;}
        double getNodeEmitter() {return nodeEmitter;}
    private:
        
        Doping doping;
        Semiconductor type;
        
        
        int nodeBase;
        int nodeCollector;
        int nodeEmitter;
        
        double Is;
        double Vt;
        double eta;
        
    };
    //---------------------------------------------------
    
    class Tube12AX7
    {
    public:
        enum Model12AX7 { EXH , RSD1 , RSD2 };
        
        Tube12AX7 (Model12AX7 model,
                       int nodeGrid, int nodeAnode, int nodeCathode):
            model(model),
            nodeGrid(nodeGrid),
            nodeAnode(nodeAnode),
            nodeCathode(nodeCathode){
        };
        
        double getNodeGrid() {return nodeGrid;}
        double getNodeAnode() {return nodeAnode;}
        double getNodeCathode() {return nodeCathode;}
        
        Model12AX7 getModel() {return model;}
    private:
        
        Model12AX7 model;

        int nodeGrid;
        int nodeAnode;
        int nodeCathode;
        
    };
};

}

