#include "Trees.h"
#include "pbTextureUtil.h"


//--------------------------------------------------------
void Trees::setup( ofParameterGroup &param, float w0, float h0 )
{
    N = 4;
    
    
    w = w0;
    h = h0;
 
    treeCommon.setup( param );
    _tree.resize( N );
    for (int i=0; i<N; i++) {
        _tree[i].setup( param, w, h, i );
    }
    fbo.allocate( w, h );
    fboSyphon.allocate( w, h );
    


    h1 = 1.0; //1.5;
    w1 = 0.5;
    m1 = 0.2;
    
    from.resize( N );
    to.resize( N );
    
    float W2 = w1 * N + m1 * (N-1);
    float scl = min( w / W2, h / h1 );
    float w2 = w1 * scl;
    float h2 = h1 * scl;
    for (int i=0; i<N; i++) {
        float x = ofMap( i + 0.5, 0, N, 0, w );
        float y = h / 2;
        from[i].setup( ofRectangle( x - w2/2, y - h2/2, w2, h2 ) );
 
        //переворот по y
        to[i] = from[i];
        to[i].p[0] = from[i].p[3];
        to[i].p[1] = from[i].p[2];
        to[i].p[2] = from[i].p[1];
        to[i].p[3] = from[i].p[0];
        
    }
    //to = from;
    
    //crop
    float scl3 = min( w / w1, h / h1 );
    float w3 = w1 * scl3;
    float h3 = h1 * scl3;
    cropFrom.setup( ofRectangle( w/2 - w3/2, h/2 - h3 / 2, w3, h3 ));
    
    load();
    
    //считывание xml
    //to[0].p[1].y += 100;
    //to[0].p[0].x += 100;
    
    shaderRipple.load("shaders/rippleVert.c", "shaders/rippleFrag.c");
}

//--------------------------------------------------------



void Trees::update( float dt, bool drawRect )
{
    TreeCommon &com = treeCommon;
    com.update( dt );

    
    if ( !com.Enabled ) { return; }
    
    for (int i=0; i<N; i++) {
        _tree[i].update( dt );
        _tree[i].drawFbo();
    }
    drawFbo();
    drawMapped( drawRect );

}

//--------------------------------------------------------

void Trees::drawFbo()
{
    TreeCommon &com = treeCommon;
    if ( !com.Enabled ) { return; }
    
    
    fbo.begin();
    ofBackground( 0, 0, 0, 0 );
    
    shaderRipple.begin();		//Enable the shader
    
	float time = ofGetElapsedTimef();
	shaderRipple.setUniform1f( "time", time );	//Pass float parameter "time" to shader
    
    for (int i=0; i<N; i++){

        shaderRipple.setUniform1f( "noiseShift", i * 4.23 );
        shaderRipple.setUniform1f( "blobsAlpha", com.BlobsAlpha.value() );
        
        ofSetColor( 255 );
        pbTextureUtil::drawQuads( from[i].p, cropFrom.p, _tree[i].fbo.getTextureReference() );
        
        //from[i].draw( ofColor( 255, 0, 0 ) );
    }
    shaderRipple.end();
    
    //cropFrom.draw( ofColor( 255, 255, 255 ) );
    
    
    //ofSetColor( 255 );
    //pbTextureUtil::drawQuads( cropFrom.p, cropFrom.p,  input );
    
    
    fbo.end();
 
}

//--------------------------------------------------------


void Trees::drawMapped( bool drawRect )
{
    TreeCommon &com = treeCommon;
    if ( !com.Enabled ) { return; }
    
    fboSyphon.begin();
    ofBackground( 0, 0, 0, 0 );
    ofSetColor( 255 );
    
    const float mappingRange = 100;

    for (int i=0; i<N; i++){
        float tx = ofMap( com.shiftA( i ), 0, 1, -mappingRange, mappingRange );
        float bx = ofMap( com.shiftB( i ), 0, 1, -mappingRange, mappingRange );
        vector<ofPoint> quad = to[i].shifted( tx, bx );
        pbTextureUtil::drawQuads( quad, from[i].p, fbo.getTextureReference() );
    }
    
    if ( drawRect ) {
        drawMappedRects( ofColor( 255 ), false );
    }
    
    fboSyphon.end();
}

//--------------------------------------------------------
void Trees::drawMappedRects( ofColor color, bool vertices )
{
    for (int i=0; i<N; i++){        
        to[i].draw( color, vertices );
    }
}

//--------------------------------------------------------
void Trees::move( int index, int vertex, ofPoint shift )
{
    Quad &q = to[index];
    for (int i=0; i<q.p.size(); i++) {
        if ( i == vertex || vertex == -1 ) {
            q.p[i] += shift;
        }
    }
    save( index );
}

//--------------------------------------------------------
void Trees::reset()
{
    for (int i=0; i<N; i++) {
        to[i] = from[i];
    }
    
}

//--------------------------------------------------------
void Trees::save( int index )
{
    for (int i=0; i<N; i++) {
        if ( i == index || index == -1 ) {
            to[i].save( "Tree" + ofToString( i + 1 ) );
        }
    }
}

//--------------------------------------------------------
void Trees::load( int index )
{
    for (int i=0; i<N; i++) {
        if ( i == index || index == -1 ) {
            to[i].load( "Tree" + ofToString( i + 1 ) );
        }
    }

}

//--------------------------------------------------------------
void Trees::mousePressed(int x, int y, int button){
    if ( button != 2 ) { return; }  //Правая кнопка мыши
    
    ofPoint mouse( x, y );
    for (int i=0; i<N; i++) {
        QuadSelection sel = to[i].getSelection( mouse );
        if ( sel.exists ) {
            sel.id1 = i;
            _sel = sel;
            _mouse0 = mouse;
            break;
        }
    }
    
}

//--------------------------------------------------------------
void Trees::mouseDragged(int x, int y, int button){
    if ( _sel.exists ) {
        ofPoint mouse( x, y );
        ofPoint delta = (mouse - _mouse0) * 1.0; //0.3;    //Скорость сдвига меньше, чтоб точнее ставить
        to[ _sel.id1 ].move( _sel, delta );
        
        _mouse0 = mouse;
    }
}


//--------------------------------------------------------------
void Trees::mouseReleased(int x, int y, int button){
    if ( _sel.exists ) {
        save( _sel.id1 );
        _sel.clear();
    }
}

//--------------------------------------------------------


