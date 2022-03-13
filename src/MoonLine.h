#pragma once

//Лунная дорожка

#include "ofMain.h"

#include "ofxGui.h"


class MoonLine {
public:
    MoonLine() { }
    
    void setup( float w, float h );
    
    void update( float dt );
    void draw( float alpha );
    
    void setLength( float len );
    
    
private:
    float w, h;
    
    float len;
    
    
    
};