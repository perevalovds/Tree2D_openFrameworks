#include "ParamFilter.h"


//--------------------------------------------------------
void ParamSmooth::setup( float v0, float speed0, int type0 )
{
    target = v0;
    value = v0;
    speed = speed0;
    type = type0;
}

//--------------------------------------------------------
void ParamSmooth::update( float dt )
{
    if ( type == SmoothLinear ) {
        float d = target - value;
        float maxD = dt * speed;
        d = ofClamp( d, -maxD, maxD );
        value += d;
    }
    if ( type == SmoothLowPass ) {
        value = ( 1 - speed ) * value + speed * target;
    }
}

//--------------------------------------------------------
void ParamSmooth::updateValue( float v )
{
    target = v;
}

//--------------------------------------------------------
void ParamSmooth::setValue( float v )
{
    target = value = v;
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
void ParamGui::setup( ofParameterGroup &param, string oscName, float value0, float minV, float maxV, float speed0,
                     int type0 )
{
    param.add( paramGui.set( oscName, value0, minV, maxV ) );
    paramSmooth.setup( value0, speed0, type0 );
    
}

//--------------------------------------------------------
void ParamGui::update( float dt )
{
    float v = paramGui;
    paramSmooth.updateValue( v );
    paramSmooth.update( dt );
}

//--------------------------------------------------------
float ParamGui::value()
{
    return paramSmooth.value;
}

//--------------------------------------------------------
