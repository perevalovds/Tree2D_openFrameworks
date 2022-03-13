#include "Figure.h"
#include "pbTextureUtil.h"

//------------------------------------------------------------------------
void Figure::setup( float w0, float h0, float x00, float x10 )
{
    w = w0;
    h = h0;
    x0 = x00;
    x1 = x10;
    
//    posX = (x0 + x1) / 2;
    float indent = 20;
    posX = ofRandom( x0 + indent, x1 - indent );
    posY = h;
    
    float speed0 = 100; //200;
    float speed1 = 250; //500;
    speed = ofRandom( speed0, speed1 );
    
    live = true;
    
    //задание контура
    int n = ofRandom( 4, 9 );
    float jumpY = 0.5;

    float rad = ofRandom( 10, 20 );

    p.resize( n );
    p[0] = ofPoint( ofRandom( -rad, rad ), -rad );
    for (int i=1; i<n; i++) {
        float x = ofMap( i, 1, n-1, -rad/2, rad/2 );
        float y = ofRandom( -jumpY * rad, jumpY * rad ) + rad;
        p[i] = ofPoint( x, y );
    }
    
    filled = ( speed > ( speed0 + speed1 ) * 0.5 );
    if ( ofRandom(0, 1) < 0.1 ) { filled = !filled; }
    //( ofRandom( 0, 1 ) < 0.5 );
    
    if ( ofRandom( 0, 1 ) < 0.5 ) {
        color = ofColor( 255 );
    }
    else {
        color = ofColor( 255, 0, 0 );
        color.setHue( ofRandom( 0, 360 ));
        color.setSaturation( 100 );
    }
    
}

//------------------------------------------------------------------------
void Figure::update( float dt )
{
    if ( !live ) { return; }
    
    
    posY += -dt * speed;

    if ( posY <= -10 ) { live = false; }
}

//------------------------------------------------------------------------
void Figure::draw()
{
    if ( !live ) { return; }
    
    ofSetColor( color );
    
    int n = p.size();

    ofPoint pos( posX, posY );
    
/*    if ( !filled ) {
        for (int i=0; i<n-1; i++) {
            ofLine( p[i] + pos, p[i+1] + pos );
        }
        ofLine( p[n-1] + pos, p[0] + pos );
    }
    else {
        ofFill();
        for (int i=1; i<n-1; i++) {
            ofTriangle( p[0] + pos, p[i] + pos, p[i+1] + pos );
        }
    }*/

    if ( filled ) { ofFill(); }
    else { ofNoFill(); }
    ofRect( posX-10, posY-10, 20, 20 );

    
}

//------------------------------------------------------------------------
