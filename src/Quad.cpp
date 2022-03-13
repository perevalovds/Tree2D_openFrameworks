#include "Quad.h"

#include "AppParam.h"

float QuadMouse = 8;                            //ПАРАМЕТР прилипания для точек мыши
float QuadMouseSqr = QuadMouse * QuadMouse;               
float QuadMouseDraw = 6;                            //ПАРАМЕТР прилипания для точек мыши


//--------------------------------------------------------
void Quad::setup( ofPoint p0, ofPoint p1, ofPoint p2, ofPoint p3 )
{
    p.resize( 4 );
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
}

//--------------------------------------------------------
void Quad::setup( ofRectangle r )
{
    setup( ofPoint( r.x, r.y ), ofPoint( r.x + r.width, r.y ),
          ofPoint( r.x + r.width, r.y + r.height ), ofPoint( r.x, r.y + r.height ) );
    
}

//--------------------------------------------------------
void Quad::draw( ofColor color, bool vertices )
{
    ofSetColor( color );
    int n = p.size();
    for (int i=0; i<n; i++) {
        ofLine( p[i], p[ (i+1) % n ]);
    }
    if ( vertices  ) {
        //вершины
        ofNoFill();
        float size = 2*QuadMouseDraw + 1;
        for (int i=0; i<n; i++){
            ofRect( p[i].x - QuadMouseDraw, p[i].y - QuadMouseDraw, size, size );
            int j = ( i + 1 ) % n;
            ofPoint edg = ( p[i] + p[j] ) * 0.5;
            ofRect( edg.x - QuadMouseDraw, edg.y - QuadMouseDraw, size, size );
        }
        ofPoint c = center();
        ofRect( c.x - QuadMouseDraw, c.y - QuadMouseDraw, size, size );
        ofSetColor( 0 );
        ofRect( c.x - QuadMouseDraw-1, c.y - QuadMouseDraw-1, size+2, size+2 );
    }
    
}

//--------------------------------------------------------
vector<ofPoint> Quad::shifted( float topX, float bottomX )   //сдвинутый Quad
{
    vector<ofPoint> q = p;
    q[0].x += topX;
    q[1].x += topX;
    q[2].x += bottomX;
    q[3].x += bottomX;
    return q;
}

//--------------------------------------------------------
void Quad::drawFilled()
{
    ofFill();
    ofTriangle( p[0], p[1], p[2] );
    ofTriangle( p[0], p[2], p[3] );
    
}

//--------------------------------------------------------
void Quad::drawFilled( vector<ofPoint> &p0 )
{
    ofFill();
    ofTriangle( p0[0], p0[1], p0[2] );
    ofTriangle( p0[0], p0[2], p0[3] );
}


//--------------------------------------------------------
void Quad::save( const string &xmlName )
{
    string s = "";
    for (int i=0; i<p.size(); i++) {
        if ( i > 0 ) {
            s += ";";
        }
        s += ofToString( p[i].x ) + ";" + ofToString( p[i].y );
    }
    ParamStringSave( xmlName, s );
    
    
}

//--------------------------------------------------------
void Quad::load( const string &xmlName )
{
    string s = ParamString( xmlName );
    vector<string> list = ofSplitString( s, ";" );
    int n = p.size();
    if ( list.size() >= 2 * n ) {
        int k = 0;
        for ( int i=0; i<n; i++ ) {
            p[i].x = ofToFloat( list[ k++ ] );
            p[i].y = ofToFloat( list[ k++ ] );
        }
    }
}


//--------------------------------------------------------
ofPoint Quad::center()
{
    ofPoint c;
    int n = p.size();
    if ( n > 0 ) {
        for ( int i=0; i<p.size(); i++) {
            c += p[i];
        }
        c /= n;
    }
    return c;
}

//--------------------------------------------------------
QuadSelection Quad::getSelection( const ofPoint &mouse )  //выдает выбранные вершины
{
    int n = p.size();
    vector<int> mask( n, 0 );
    //центр фигуры
    if ( center().distanceSquared( mouse ) <= QuadMouseSqr ) {
        fill( mask.begin(), mask.end(), 1 );
    }
    for (int i=0; i<n; i++) {
        //сама вершина
        if ( p[i].distanceSquared( mouse ) <= QuadMouseSqr ) {
            fill( mask.begin(), mask.end(), 0 );
            mask[i] = 1;
            break;  //выходим чтоб можно было разлепить вершины
        }
        //середина со следующей вершиной
        int j = ( i + 1 ) % n;
        if ( ( (p[i] + p[ j ]) * 0.5 ).distanceSquared( mouse ) <= QuadMouseSqr ) {
            mask[i] = mask[j] = 1;
        }
        
    }
    QuadSelection sel;
    sel.mask = mask;
    sel.exists = ( sel.count() > 0 );
    return sel;
}

//--------------------------------------------------------
void Quad::move( QuadSelection &sel, ofPoint shift ) //сдвиг выбранных вершин
{
    if ( sel.exists ) {
        vector<int> &mask = sel.mask;
        for (int i=0; i<p.size(); i++) {
            if ( mask[i] ) {
                p[ i ] += shift;
            }
        }
    }
}


//--------------------------------------------------------
void quadMapping( Quad &from, Quad &to, ofTexture &tex )
{
    to.draw( ofColor( 255, 0, 0 ) );
    
}

//--------------------------------------------------------
