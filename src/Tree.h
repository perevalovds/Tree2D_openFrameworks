#pragma once

//Растущее дерево

#include "ofMain.h"

#include "ofxGui.h"
#include "ParamFilter.h"


struct TreeCommon {
    void setup( ofParameterGroup &param );
    void update( float dt );
    
    ofParameter<float> Enabled;
 
    //ofParameter<float> BlobsAlpha;
    //ofParameter<float> TreesAlpha;
    
    ParamGui BlobsAlpha;
    ParamGui TreesAlpha;
    
    
    
    ofParameter<float> Morph;
    ofParameter<float> Morph2;
    
    ofParameter<float> Width;       //Толщина ствола
    ofParameter<float> Decrease;    //Скорость уменьшения ствола
    
    ofParameter<float> NoiseAmp;
    ofParameter<float> NoiseVel;

    ofParameter<float> Branch;

    ParamGui Scale1;
    ParamGui Scale2;
    ParamGui Scale3;
    ParamGui Scale4;
    
    ParamGui Extend1;
    ParamGui Extend2;
    ParamGui Extend3;
    ParamGui Extend4;

    
    ParamGui Spiral1;
    ParamGui Spiral2;
    ParamGui Spiral3;
    ParamGui Spiral4;

    float scale( int index ) {
        if ( index == 0 ) { return Scale1.value(); }
        if ( index == 1 ) { return Scale2.value(); }
        if ( index == 2 ) { return Scale3.value(); }
        if ( index == 3 ) { return Scale4.value(); }
        return 0;
    }
    
    float extend( int index ) {
        if ( index == 0 ) { return Extend1.value(); }
        if ( index == 1 ) { return Extend2.value(); }
        if ( index == 2 ) { return Extend3.value(); }
        if ( index == 3 ) { return Extend4.value(); }
        return 0;
    }
    
    float spiral( int index ) {
        if ( index == 0 ) { return Spiral1.value(); }
        if ( index == 1 ) { return Spiral2.value(); }
        if ( index == 2 ) { return Spiral3.value(); }
        if ( index == 3 ) { return Spiral4.value(); }
        return 0;
    }
    
    ParamGui ShiftA1;
    ParamGui ShiftA2;
    ParamGui ShiftA3;
    ParamGui ShiftA4;

    ParamGui ShiftB1;
    ParamGui ShiftB2;
    ParamGui ShiftB3;
    ParamGui ShiftB4;
    
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

};

extern TreeCommon treeCommon;



//Точки дерева
struct TPoint {
    TPoint() {
        parent = -1;
        //leaf = false;
        angle = 0;
        spiral = 0;
        move = false;
        energy = 0;
        width = 1.0;
    }
    
    int parent;     //номер родителя
    //bool leaf;       //это лист
    
    
    ofPoint p;      //точка
    ofPoint p1, p2; //точки для рисования
    
    float angle;    //угол
    float spiral;   //угол закручивания в спираль
    
    bool move;      //шевелится ли она
    
    float energy;
    float length;
    float width;
    
    ofPoint struc;      //положение структурного шума
    
    void updateAngleByNoise( float dt, float amp, float vel, float elast, float &noisePos ) {
        noisePos += dt * vel * elast;
        angle += ofSignedNoise( noisePos ) * amp;
    }
    
    void updatePoint( const ofPoint &parentP, float angle, float length, float width ) {
        ofPoint delta( cos( angle * DEG_TO_RAD ), sin( angle * DEG_TO_RAD ) );
        p = parentP + delta * length;
        
        p1 = ofPoint( p.x - width * delta.y, p.y + width * delta.x );
        p2 = ofPoint( p.x + width * delta.y, p.y - width * delta.x );
    }
};




class TreeDyn {
public:
    void setup( ofParameterGroup &param, float w, float h, int index );
    void update( float dt );
    void drawFbo();
    

    
    
    float w, h;
    ofFbo fbo;
    
    int _index;
    
    vector<TPoint> _p;
    vector<float> _noisePos;

    
};
