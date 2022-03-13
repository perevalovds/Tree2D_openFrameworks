#pragma once

//Взлетающие острые фигуры

#include "ofMain.h"

#include "ofxGui.h"
#include "Quad.h"
#include "Figure.h"
#include "ParamFilter.h"
#include "MoonLine.h"

struct FiguresCommon {
    void setup( ofParameterGroup &param );
    void update( float dt );
    
    ofParameter<float> Enabled;
    
    ofParameter<float> Burst1;
    ofParameter<float> Burst2;
    ofParameter<float> Burst3;
    ofParameter<float> Burst4;
    float burst( int index ) {
        if ( index == 0 ) { return Burst1; }
        if ( index == 1 ) { return Burst2; }
        if ( index == 2 ) { return Burst3; }
        if ( index == 3 ) { return Burst4; }
        return 0;
    }
    
    ParamGui MoonAlpha;
    ParamGui Moon1;
    ParamGui Moon2;
    ParamGui Moon3;
    ParamGui Moon4;
    float moon( int index ) {
        if ( index == 0 ) { return Moon1.value(); }
        if ( index == 1 ) { return Moon2.value(); }
        if ( index == 2 ) { return Moon3.value(); }
        if ( index == 3 ) { return Moon4.value(); }
        return 0;
    }
    float moonAlpha() { return MoonAlpha.value(); }
    
    
    ParamGui ShiftA1;
    ParamGui ShiftA2;
    ParamGui ShiftA3;
    ParamGui ShiftA4;
    
    ParamGui ShiftB1;
    ParamGui ShiftB2;
    ParamGui ShiftB3;
    ParamGui ShiftB4;
    
    ParamGui ShiftC1;
    ParamGui ShiftC2;
    ParamGui ShiftC3;
    ParamGui ShiftC4;
    
    ParamGui ShiftD1;
    ParamGui ShiftD2;
    ParamGui ShiftD3;
    ParamGui ShiftD4;
    
    float shiftA( int index ) {
        if ( index == 0 ) { return ShiftA1.value(); }
        if ( index == 1 ) { return ShiftA2.value(); }
        if ( index == 2 ) { return ShiftA3.value(); }
        if ( index == 3 ) { return ShiftA4.value(); }
        return 0;
    }
    
    float shiftB( int index ) {
        if ( index == 0 ) { return ShiftB1.value(); }
        if ( index == 1 ) { return ShiftB2.value(); }
        if ( index == 2 ) { return ShiftB3.value(); }
        if ( index == 3 ) { return ShiftB4.value(); }
        return 0;
    }
    
    float shiftC( int index ) {
        if ( index == 0 ) { return ShiftC1.value(); }
        if ( index == 1 ) { return ShiftC2.value(); }
        if ( index == 2 ) { return ShiftC3.value(); }
        if ( index == 3 ) { return ShiftC4.value(); }
        return 0;
    }
    
    float shiftD( int index ) {
        if ( index == 0 ) { return ShiftD1.value(); }
        if ( index == 1 ) { return ShiftD2.value(); }
        if ( index == 2 ) { return ShiftD3.value(); }
        if ( index == 3 ) { return ShiftD4.value(); }
        return 0;
    }
};

extern FiguresCommon figuresCommon;

class Figures {
    public:
    void setup( ofParameterGroup &param, float w, float h );
    void update( float dt, bool drawRect = false );
    void drawFbo();
    void drawMapped( bool drawRect = false );
    void drawMappedRects( ofColor color, bool vertices = false );
    
    void updateMask();
    
    void move( int i, int vertex, ofPoint shift );
    void reset();
    void save( int i = -1 );
    void load( int i = -1 );
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    float w, h;
    ofFbo fbo;
    ofFbo fboSyphon;
    ofFbo fboMask;      //маска
    
    int N;      //число полос
    int S; //на сколько частей дробить
    
    float w1, h1, m1;   //пропорции деревьев и отступа между ними
    
    vector<Quad> from;
    
    int M;
    vector<Quad> from1; //дробления
    vector<Quad> to1;
    
    Quad cropFrom;
    
    //ofShader shaderRipple;
    
    
    //выделенные вершины для перемещения мышью
    QuadSelection _sel;
    ofPoint _mouse0;
    
    vector<Figure> fig; //фигурки
    
    vector<MoonLine> moon;  //лунная дорожка
    
};
