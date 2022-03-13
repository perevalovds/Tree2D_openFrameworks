#pragma once

//Физика фигурок

#include "ofMain.h"

#include "ofxGui.h"


class Figure {
public:
    Figure() { live = false; }
    
    void setup( float w, float h, float x0, float x1 );
    
    void update( float dt );
    void draw();
    
    bool live;
    
private:
    float w, h, x0, x1;
    
    float posX; // от шума
    float posY;
    
    float speed;
    
    vector<ofPoint> p;  //вершины
    ofColor color;      //цвет
    bool filled;
};