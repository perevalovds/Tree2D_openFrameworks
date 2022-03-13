#include "MoonLine.h"
#include "pbTextureUtil.h"

//------------------------------------------------------------------------
void MoonLine::setup( float w0, float h0 )
{
    w = w0;
    h = h0;
    
    len = 1.0;

    
}

//------------------------------------------------------------------------
void MoonLine::update( float dt )
{

}

//------------------------------------------------------------------------
void MoonLine::setLength( float len0 )
{
    len = len0;
}

//------------------------------------------------------------------------
void MoonLine::draw( float alpha )
{
    ofFill();
    ofSetColor( 255, alpha * 255 );
    float w1 = 20;
    float h1 = h * len;
    ofRect( w/2 - w1/2, 0, w1, h1 );
    
}

//------------------------------------------------------------------------
