#pragma once

//Сглаживание значений параметров

#include "ofMain.h"
#include "ofxGui.h"


const int  SmoothLinear = 0;
const int  SmoothLowPass = 1;

struct ParamSmooth {
    void setup( float v0, float speed0, int type );
    void update( float dt );                    
    void updateValue( float v );
    void setValue( float v );
    
    float target;
    float value;
    float speed;
    
    int type;
};



struct ParamGui {
    void setup( ofParameterGroup &param, string oscName, float value0, float minV, float maxV, float speed0,
               int type = SmoothLinear );
    void update( float dt );
    float value();
    
    ofParameter<float> paramGui;
    ParamSmooth paramSmooth;
    
    
    
    
};