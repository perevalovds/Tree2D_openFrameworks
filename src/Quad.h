#pragma once

//Рендер 4-угольников

#include "ofMain.h"

//выделение точек квада
struct QuadSelection {
    QuadSelection() { clear(); }
    void clear() { exists = false; id1 = -1; id2 = -1; }

    bool exists;
    int id1, id2;   //идентификатор квада
    vector<int> mask; //маска выделенных точек
    int count() {
        int s = 0;
        for (int i=0; i<mask.size(); i++) {
            s += mask[i];
        }
        return s;
    }
    
};



struct Quad {
    vector<ofPoint> p;
    void setup( ofPoint p0, ofPoint p1, ofPoint p2, ofPoint p3 );
    void setup( ofRectangle r );
    void draw( ofColor color, bool vertices = false );
    void drawFilled();
    void drawFilled( vector<ofPoint> &p0 );
    
    void save( const string &xmlName );
    void load( const string &xmlName );
    
    ofPoint center();
    
    QuadSelection getSelection( const ofPoint &mouse );  //выдает выбранные вершины
    void move( QuadSelection &sel, ofPoint shift ); //сдвиг выбранных вершин
    
    vector<ofPoint> shifted( float topX, float bottomX );   //сдвинутый Quad

    float widthTop() { return p[1].x - p[0].x; }
    float heightLeft() { return p[3].y - p[0].y; }
    ofPoint leftTop() { return p[0]; }
};

void quadMapping( Quad &from, Quad &to, ofTexture &tex );

