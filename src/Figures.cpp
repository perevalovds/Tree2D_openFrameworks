#include "Figures.h"
#include "pbTextureUtil.h"
#include "Tree.h"

FiguresCommon figuresCommon;


//--------------------------------------------------------
void FiguresCommon::setup( ofParameterGroup &param )
{
    
    float slow1 = 0.3;
    float slow = 0.18;
    float slow2 = 0.02;
    int low = SmoothLowPass;
    
    param.add( Enabled.set( "fEnabled", 0.1, 0, 1 ) );
    param.add( Burst1.set( "fBurst1", 0, 0, 1 ) );
    param.add( Burst2.set( "fBurst2", 0, 0, 1 ) );
    param.add( Burst3.set( "fBurst3", 0, 0, 1 ) );
    param.add( Burst4.set( "fBurst4", 0, 0, 1 ) );
    
    MoonAlpha.setup( param, "fMoonAlpha", 1, 0, 1, slow1 * 0.5 );
    Moon1.setup( param, "fMoon1", 0, 0, 1, slow2, low );
    Moon2.setup( param, "fMoon2", 1., 0, 1, slow2, low );
    Moon3.setup( param, "fMoon3", 0, 0, 1, slow2, low );
    Moon4.setup( param, "fMoon4", 0, 0, 1, slow2, low );
    
    
    ShiftA1.setup( param, "fShiftA1", 0.5, 0, 1, slow, low );
    ShiftA2.setup( param, "fShiftA2", 0.5, 0, 1, slow, low );
    ShiftA3.setup( param, "fShiftA3", 0.5, 0, 1, slow, low );
    ShiftA4.setup( param, "fShiftA4", 0.5, 0, 1, slow, low );
    
    ShiftB1.setup( param, "fShiftB1", 0.5, 0, 1, slow, low );
    ShiftB2.setup( param, "fShiftB2", 0.5, 0, 1, slow, low );
    ShiftB3.setup( param, "fShiftB3", 0.5, 0, 1, slow, low );
    ShiftB4.setup( param, "fShiftB4", 0.5, 0, 1, slow, low );
    
    ShiftC1.setup( param, "fShiftC1", 0.5, 0, 1, slow, low );
    ShiftC2.setup( param, "fShiftC2", 0.5, 0, 1, slow, low );
    ShiftC3.setup( param, "fShiftC3", 0.5, 0, 1, slow, low );
    ShiftC4.setup( param, "fShiftC4", 0.5, 0, 1, slow, low );
    
    ShiftD1.setup( param, "fShiftD1", 0.5, 0, 1, slow, low );
    ShiftD2.setup( param, "fShiftD2", 0.5, 0, 1, slow, low );
    ShiftD3.setup( param, "fShiftD3", 0.5, 0, 1, slow, low );
    ShiftD4.setup( param, "fShiftD4", 0.5, 0, 1, slow, low );
    
}

//--------------------------------------------------------
void FiguresCommon::update( float dt )
{
    MoonAlpha.update( dt );
    Moon1.update( dt );
    Moon2.update( dt );
    Moon3.update( dt );
    Moon4.update( dt );
    
    ShiftA1.update( dt );
    ShiftA2.update( dt );
    ShiftA3.update( dt );
    ShiftA4.update( dt );
    
    ShiftB1.update( dt );
    ShiftB2.update( dt );
    ShiftB3.update( dt );
    ShiftB4.update( dt );

    ShiftC1.update( dt );
    ShiftC2.update( dt );
    ShiftC3.update( dt );
    ShiftC4.update( dt );
    
    ShiftD1.update( dt );
    ShiftD2.update( dt );
    ShiftD3.update( dt );
    ShiftD4.update( dt );
}
    
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

void Figures::setup( ofParameterGroup &param, float w0, float h0 )
{
    N = 4;
    S = 4; //на сколько частей дробить
    M = N * S;
    
    w = w0;
    h = h0;
 
    figuresCommon.setup( param );

    fbo.allocate( w, h );
    fboSyphon.allocate( w, h );    
    fboMask.allocate( w, h );
    
    h1 = 1.0; //1.5;
    w1 = 0.2;
    m1 = (1.0 - N * w1) / (N-1);
    
    
    from.resize( N );
    from1.resize( M );
    to1.resize( M );
    
    float W2 = w1 * N + m1 * (N-1);
    float scl = min( w / W2, h / h1 );
    float w2 = w1 * scl;
    float h2 = h1 * scl;
    for (int i=0; i<N; i++) {
        float x = ofMap( i + 0.5, 0, N, 0, w );
        float y = h / 2;
        
        float x0 = x - w2/2;
        float y0 = y - h2/2;
        float w0 = w2;
        float h0 = h2;
        from[i].setup( ofRectangle( x0, y0, w0, h0 ) );
 
        for ( int k=0; k<S; k++ ) {
            float yA = ofMap( k, 0, S, y0, y0+h0);
            float yB = ofMap( k+1, 0, S, y0, y0+h0);
            from1[ k + i * S ].setup( ofRectangle( x0, yA, w0, yB - yA ) );
        }
        //переворот по y
        //to[i] = from[i];
        //to[i].p[0] = from[i].p[3];
        //to[i].p[1] = from[i].p[2];
        //to[i].p[2] = from[i].p[1];
        //to[i].p[3] = from[i].p[0];
        
    }
    to1 = from1;
    
    load();
    updateMask();
    
    //лунная дорожка
    moon.resize( N );
    for (int i=0; i<N; i++) {
        moon[i].setup(from[i].widthTop(), from[i].heightLeft() );
    }
    
    //shaderRipple.load("shaders/rippleVert.c", "shaders/rippleFrag.c");
}

//--------------------------------------------------------


void Figures::update( float dt, bool drawRect )
{
    FiguresCommon &com = figuresCommon;
    
    com.update( dt );

    if ( !com.Enabled ) { return; }
    
    
    //Удаление отработавших фигурок
	int i=0;
	while (i < fig.size()) {
		if ( !fig[i].live ) {
			fig.erase( fig.begin() + i );
		}
		else {
			i++;
		}
	}
    
    //Рождение фигурок
    float burstRate = 10;
    for (int i=0; i<N; i++) {
        float x0 = from[i].p[0].x;
        float x1 = from[i].p[1].x;
        float b = com.burst( i );
        int n = b * burstRate;                 
        for ( int i=0; i<n; i++ ) {
            Figure f;
            f.setup( w, h, x0, x1 );
            fig.push_back( f );
        }
    }
    
    //Обновление фигурок
    for (int i=0; i<fig.size(); i++) {
        fig[i].update( dt );
    }
    
    //Лунная дорожка
    for (int i=0; i<N; i++) {
        moon[i].setLength( com.moon( i ) );
        moon[i].update( dt );
    }
    
    //Обновление маски //TODO не надо бы всегда делать
    updateMask();

    //Рисование
    drawFbo();
    drawMapped( drawRect );    

}

//--------------------------------------------------------

void Figures::drawFbo()
{
    FiguresCommon &com = figuresCommon;
    if ( !com.Enabled ) { return; }
    
    
    ofEnableAlphaBlending();
    
    fbo.begin();
    ofBackground( 0, 0, 0, 0 );

    //Лунная дорожка
    for (int i=0; i<N; i++) {
        ofPushMatrix();
        ofTranslate( from[i].leftTop() );
        moon[i].draw( com.moonAlpha() );
        
        ofPopMatrix();
    }
    
    
    //Фигурки
    for (int i=0; i<fig.size(); i++) {
        fig[i].draw();
    }
    
    //Прямоугольники from
    //for (int i=0; i<N; i++){
    //    ofSetColor( 255 );
    //    from[i].draw( ofColor( 255, 0, 0 ) );
    //}

    
    fbo.end();
 
}

//--------------------------------------------------------
const float mappingRange = 100;


void Figures::drawMapped( bool drawRect )
{
    FiguresCommon &com = figuresCommon;
    if ( !com.Enabled ) { return; }
    
    TreeCommon &tcom = treeCommon;
    
    fboSyphon.begin();
    ofBackground( 0, 0, 0, 0 );
    ofSetColor( 255 );
    
    
    float x[5];
    for (int i=0; i<N; i++){
        x[0] = ofMap( tcom.shiftB( i ), 0, 1, -mappingRange, mappingRange );
        x[1] = ofMap( com.shiftA( i ), 0, 1, -mappingRange, mappingRange );
        x[2] = ofMap( com.shiftB( i ), 0, 1, -mappingRange, mappingRange );
        x[3] = ofMap( com.shiftC( i ), 0, 1, -mappingRange, mappingRange );
        x[4] = ofMap( com.shiftD( i ), 0, 1, -mappingRange, mappingRange );
        for ( int k = 0; k < S; k++ ) {
            int ind = k + i * S;
            vector<ofPoint> quad = to1[ind].shifted( x[k], x[k+1] );
            pbTextureUtil::drawQuads( quad, from1[ind].p, fbo.getTextureReference() );
        }
    }
    
//    for (int i=0; i<M; i++){
//        pbTextureUtil::drawQuads( to1[i].p, from1[i].p, fbo.getTextureReference() );
//    }
    
    if ( drawRect ) {
        drawMappedRects( ofColor( 255 ), false );
    }
    
    fboSyphon.end();
}

//--------------------------------------------------------
void Figures::drawMappedRects( ofColor color, bool vertices )
{
    for (int i=0; i<M; i++){
        to1[i].draw( color, vertices );
    }
}

//--------------------------------------------------------
void Figures::updateMask()
{
    FiguresCommon &com = figuresCommon;    
    TreeCommon &tcom = treeCommon;
    
    fboMask.begin();
    ofBackground( 0 );
    ofSetColor( 255 );
    
    float x[5];
    for (int i=0; i<N; i++){
        x[0] = ofMap( tcom.shiftB( i ), 0, 1, -mappingRange, mappingRange );
        x[1] = ofMap( com.shiftA( i ), 0, 1, -mappingRange, mappingRange );
        x[2] = ofMap( com.shiftB( i ), 0, 1, -mappingRange, mappingRange );
        x[3] = ofMap( com.shiftC( i ), 0, 1, -mappingRange, mappingRange );
        x[4] = ofMap( com.shiftD( i ), 0, 1, -mappingRange, mappingRange );
        for ( int k = 0; k < S; k++ ) {
            int ind = k + i * S;
            vector<ofPoint> quad = to1[ind].shifted( x[k], x[k+1] );
            to1[ind].drawFilled( quad );
        }
    }
    
//    for (int i=0; i<M; i++) {
//        to1[i].drawFilled();
//    }
    
    fboMask.end();
}

//--------------------------------------------------------
void Figures::move( int index, int vertex, ofPoint shift )
{
    Quad &q = to1[index];
    for (int i=0; i<q.p.size(); i++) {
        if ( i == vertex || vertex == -1 ) {
            q.p[i] += shift;
        }
    }
    save( index );
}

//--------------------------------------------------------
void Figures::reset()
{
    to1 = from1;
    updateMask();
    
}

//--------------------------------------------------------
void Figures::save( int index )
{
    for (int i=0; i<M; i++) {
        if ( i == index || index == -1 ) {
            to1[i].save( "Figures" + ofToString( i + 1 ) );
        }
    }
}

//--------------------------------------------------------
void Figures::load( int index )
{
    for (int i=0; i<M; i++) {
        if ( i == index || index == -1 ) {
            to1[i].load( "Figures" + ofToString( i + 1 ) );
        }
    }
    updateMask();

}

//--------------------------------------------------------------
void Figures::mousePressed(int x, int y, int button){
    if ( button != 2 ) { return; }  //Правая кнопка мыши
    
    ofPoint mouse( x, y );
    for (int i=0; i<M; i++) {
        QuadSelection sel = to1[i].getSelection( mouse );
        if ( sel.exists ) {
            sel.id1 = i;
            _sel = sel;
            _mouse0 = mouse;
            break;
        }
    }
    
}

//--------------------------------------------------------------
void Figures::mouseDragged(int x, int y, int button){
    if ( _sel.exists ) {
        ofPoint mouse( x, y );
        ofPoint delta = (mouse - _mouse0) * 1.0; //0.3;    //Скорость сдвига меньше, чтоб точнее ставить
        to1[ _sel.id1 ].move( _sel, delta );
        
        //двигаем связанный Quad
        int ind = _sel.id1;
        int x = ind / S;
        int y = ind % S;
        if ( y < S - 1 ) {
            vector<int> mask( 4, 0 );
            vector<int> &mask0 = _sel.mask;
            mask[ 0 ] = mask0[ 3 ];
            mask[ 1 ] = mask0[ 2 ];
            if ( mask[0] || mask[1] ) {
                QuadSelection sel = _sel;
                sel.mask = mask;
                
                int index = y + 1 + S * x;
                to1[ index ].move( sel, delta );
                
            }
        }
        if ( y > 0 ) {
            vector<int> mask( 4, 0 );
            vector<int> &mask0 = _sel.mask;
            mask[ 2 ] = mask0[ 1 ];
            mask[ 3 ] = mask0[ 0 ];
            if ( mask[2] || mask[3] ) {
                QuadSelection sel = _sel;
                sel.mask = mask;
                
                int index = y - 1 + S * x;
                to1[ index ].move( sel, delta );
                
            }
        }
        
        _mouse0 = mouse;
        
        updateMask();
        
    }
}


//--------------------------------------------------------------
void Figures::mouseReleased(int x, int y, int button){
    if ( _sel.exists ) {
        save();
        _sel.clear();
    }
}

//--------------------------------------------------------


