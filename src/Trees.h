#pragma once

//Набор деревьев

#include "ofMain.h"

#include "ofxGui.h"
#include "Quad.h"
#include "Tree.h"




class Trees {
public:
    void setup( ofParameterGroup &param, float w, float h );
    void update( float dt, bool drawRect = false );
    void drawFbo();
    void drawMapped( bool drawRect = false );
    void drawMappedRects( ofColor color, bool vertices = false );
    
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
    
    
    int N;
    float w1, h1, m1;   //пропорции деревьев и отступа между ними
    
    vector<Quad> from;
    vector<Quad> to;
    
    Quad cropFrom;
    
    vector<TreeDyn> _tree;
    
    ofShader shaderRipple;
    
    
    //выделенные вершины для перемещения мышью
    QuadSelection _sel;
    ofPoint _mouse0;
    
};
