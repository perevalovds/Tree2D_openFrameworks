#include "Tree.h"
#include "pbTextureUtil.h"

TreeCommon treeCommon;

//--------------------------------------------------------
void TreeCommon::setup( ofParameterGroup &param )
{
    param.add( Enabled.set( "trEnabled", 1, 0, 1 ) );
    
    //param.add( BlobsAlpha.set( "trBlobsAlpha", 1, 0, 1 ) );
    //param.add( TreesAlpha.set( "trTreesAlpha", 1, 0, 1 ) );
    float slow1 = 0.3;
    float slow = 0.18;
    float slow2 = 0.02;
    int low = SmoothLowPass;
    
    BlobsAlpha.setup( param, "trBlobsAlpha", 1, 0, 1, slow1 );
    TreesAlpha.setup( param, "trTreesAlpha", 1, 0, 1, slow1 * 0.5 );
    
    param.add( Morph.set( "trMorph", 0.5, 0, 1 ) );
    param.add( Morph2.set( "trMorph2", 0.5, 0, 1 ) );
    
    param.add( Width.set( "trWidth", 0.5, 0, 1 ) );
    param.add( Decrease.set( "trDecrease", 0.6, 0, 1 ) );
    
    param.add( NoiseAmp.set( "trNoiseAmp", 0.1, 0, 1 ) );
    param.add( NoiseVel.set( "trNoiseVel", 0.1, 0, 1 ) );
    
    param.add( Branch.set( "trBranch", 0.4, 0, 1 ) );
    

    Scale1.setup( param, "trScale1", 0.4, 0, 1, slow2, low );
    Extend1.setup(param, "trExtend1", 0.3, 0, 1, slow, low);
    Spiral1.setup(param, "trSpiral1", 0.3, 0, 1, slow, low);
    ShiftA1.setup(param, "trShiftA1", 0.5, 0, 1, slow, low);
    ShiftB1.setup(param, "trShiftB1", 0.5, 0, 1, slow, low);

    Scale2.setup( param, "trScale2", 0.4, 0, 1, slow2, low );
    Extend2.setup(param, "trExtend2", 0.3, 0, 1, slow, low);
    Spiral2.setup(param, "trSpiral2", 0.3, 0, 1, slow, low);
    ShiftA2.setup(param, "trShiftA2", 0.5, 0, 1, slow, low);
    ShiftB2.setup(param, "trShiftB2", 0.5, 0, 1, slow, low);

    Scale3.setup( param, "trScale3", 0.4, 0, 1, slow2, low );
    Extend3.setup(param, "trExtend3", 0.3, 0, 1, slow, low);
    Spiral3.setup(param, "trSpiral3", 0.3, 0, 1, slow, low);
    ShiftA3.setup(param, "trShiftA3", 0.5, 0, 1, slow, low);
    ShiftB4.setup(param, "trShiftB4", 0.5, 0, 1, slow, low);

    Scale4.setup( param, "trScale4", 0.4, 0, 1, slow2, low );
    Extend4.setup( param, "trExtend4", 0.3, 0, 1, slow, low );
    Spiral4.setup( param, "trSpiral4", 0.3, 0, 1, slow, low );
    ShiftA4.setup( param, "trShiftA4", 0.5, 0, 1, slow, low );
    ShiftB3.setup( param, "trShiftB3", 0.5, 0, 1, slow, low );
    
}


void TreeCommon::update( float dt ) {
    BlobsAlpha.update( dt );
    TreesAlpha.update( dt );
    
    Scale1.update( dt );
    Scale2.update( dt );
    Scale3.update( dt );
    Scale4.update( dt );
    
    Extend1.update( dt );
    Extend2.update( dt );
    Extend3.update( dt );
    Extend4.update( dt );
    
    Spiral1.update( dt );
    Spiral2.update( dt );
    Spiral3.update( dt );
    Spiral4.update( dt );
    
    ShiftA1.update( dt );
    ShiftA2.update( dt );
    ShiftA3.update( dt );
    ShiftA4.update( dt );
    
    ShiftB1.update( dt );
    ShiftB2.update( dt );
    ShiftB3.update( dt );
    ShiftB4.update( dt );
}
//--------------------------------------------------------

//--------------------------------------------------------
void TreeDyn::setup( ofParameterGroup &param, float w0, float h0, int index )
{
    w = w0;
    h = h0;
    _index = index;
        
    fbo.allocate( w, h );
    
    
}

//--------------------------------------------------------


void TreeDyn::update( float dt )
{
    TreeCommon &com = treeCommon;
    
   
    float morph = com.Morph * 30;  //положение шума
    float morph2 = com.Morph2 * 1;  //положение шума
    
    float lineWidth = ofMap( com.Width, 0, 1, 0.1, 20 );
    float lineDecrease = ofMap( com.Decrease, 0, 1, 0.93, 0.99 );
    
    float energy = 2000;        //длина максимальной линии
    float length0 = ofMap( com.scale( _index ), 0, 1, 0.01, 20 );         //длина первого сегмента
    
    float extend = com.extend( _index ) * 3;      //разброс углов
    float spiralValue = com.spiral( _index ) * 2; //закручивание в спираль
    
    float noiseAmp = com.NoiseAmp * 50.0;
    float noiseVel = com.NoiseVel * 2.5;
    
    int maxN = 10000;       //максимальное число вершин
    
    
    float energyThresh = 10;    //порог, когда еще можно расти
    
    //Первая точка - постоянна
    if ( _p.size() == 0 ) {
        TPoint p;
        p.p = ofPoint( w/2, h * 1 );
        p.move = false;
        p.energy = energy;
        p.angle = -90;
        _p.push_back( p );
        
        _noisePos.resize( maxN );
        for ( int i=0; i<maxN; i++ ) {
            _noisePos[i] = ofRandom( 0, 1000 );
        }
    }
    else {
        _p.resize( 1 );
    }
    TPoint &p0 = _p[0];
    p0.energy = energy;
    p0.width = lineWidth;
    p0.p1 = p0.p + ofPoint( lineWidth, 0 );
    p0.p2 = p0.p - ofPoint( lineWidth, 0 );
    p0.length = length0;
    p0.struc.y = morph;
    p0.struc.z = morph2;
    
    float EnergyDec = 8;
    
    
    
    //Строим остальные точки
    int i = 0;
    while ( i < _p.size() && _p.size() < maxN ) {
		TPoint p = _p[i];
		float en = p.energy;
		//если есть куда расти - растем
		if ( en > energyThresh ) {
            //рост
			float kShorterMin = 0.8;		//на сколько укорачивать
			float kShorterMax = 1.1;		//на сколько укорачивать
			float shorter = ofMap( ofNoise( p.struc.x + i*15.232, p.struc.y, p.struc.z + 55.53 ),
                                  0, 1, kShorterMin, kShorterMax );
            
			float length = p.length; //c.length * shorter;
			float width = p.width * lineDecrease;
            
			float elastic = ofMap( width, 0, lineWidth, 1.0, 0.1 );	//коэфф. эластичности
            
			float kMaxAngle = 20 * extend;
			float angleD = kMaxAngle * elastic;
			float angle = ofMap( ofNoise( p.struc.x + i*44.2, p.struc.y, p.struc.z + 24.92 ),
                                0, 1, -angleD, angleD );
			
			float energy1 = en - EnergyDec;
            
			float kGrowAngle = 2 * spiralValue * elastic;
			float spiral = p.spiral
            + ofMap( ofNoise( p.struc.x + i*83.685, p.struc.y, p.struc.z + 334.52 ),
                    0, 1,  -kGrowAngle, kGrowAngle );
			
            
			TPoint p1;
            p1.parent = i;
            p1.angle = angle + p.angle + spiral;
            p1.length = length;
            p1.energy = energy1;
            p1.move = false;
            p1.spiral = spiral;
            p1.width = width;
            
            p1.struc = p.struc;
            
            p1.updatePoint( p.p, p1.angle, length, width );
            /*ofPoint delta( cos( p1.angle * DEG_TO_RAD ), sin( p1.angle * DEG_TO_RAD ) );
             p1.p = p.p + delta * length;
             
             p.p1 = ofPoint( p.p.x - width * delta.y, p.p.y + width * delta.x );
             p.p2 = ofPoint( p.p.x + width * delta.y, p.p.y - width * delta.x );
             */
            
            _p.push_back( p1 );
            
            
            //ветвление
            //при этом ближе к концу - чаще разветвляемся
            float e = energy1 / energy;
            float chance = 0.00;
            if ( e < 0.90 ) chance = 0.15; //0.15;
            if ( e < 0.6 ) chance = 0.20; //0.20;
            if ( e < 0.15 ) chance = 0.30;
            float r = ofNoise( p.struc.x + i*27.834, p.struc.y, p.struc.z + 580.42 );
            if ( r <= chance ) {
                //cout << "branch" << endl;
                int dir = (ofNoise( p.struc.x + i*95.361, p.struc.y, p.struc.z + 105.683 ) > 0.5) ? -1 : 1;
                
                float kSpiralMin = 5 * spiralValue;
                float kSpiralMax = 10 * spiralValue;
                float spiral = dir * ofMap( ofNoise( p.struc.x + i * 73.483, p.struc.y, p.struc.z + 22.33 ),
                                           0, 1, kSpiralMin, kSpiralMax );
                
                //уменьшение энергии
                float dec = ofMap(ofNoise( p.struc.x + i*39.594, p.struc.y, p.struc.z + 39.695 ),
                                  0, 1, //0.8, 1);
                                  0.5, 1 );
                                  
                float energy2 = dec * en;
                //float width2 = dec * width;
                
                float angle2 = dir * ofMap(ofNoise( p.struc.x + i*395.485, p.struc.y, p.struc.z + 937.573 ),
                                           0, 1, 30, 40 ) * extend;
                
                TPoint p2;
                p2.parent = i;
                p2.angle = angle2 + p.angle + spiral;
                p2.length = length;
                p2.energy = energy2;
                p2.move = true;
                p2.spiral = spiral;
                p2.width = width;
                
                p2.struc = p.struc;
                
                p2.updatePoint( p.p, p2.angle, p2.length, width );
                /*float dx = cos( p2.angle * DEG_TO_RAD );
                 float dy = sin( p2.angle * DEG_TO_RAD );
                 p2.p = p.p + ofPoint( dx, dy ) * p2.length;
                 */
                _p.push_back( p2 );
                
                //шевелим эту и предыдущую точку
                int indexA = _p.size() - 2;
                int indexB = _p.size() - 1;
                
                TPoint &A = _p[ indexA ];
                TPoint &B = _p[ indexB ];
                float elast = 1.0;
                A.updateAngleByNoise( dt, noiseAmp, noiseVel, elast, _noisePos[ indexA ] );
                B.updateAngleByNoise( dt, noiseAmp, noiseVel, elast, _noisePos[ indexB ] );
                
                //c1.setup( i, angle + dir * angle1, length, width1, Grow( energy1, growAngle * dir, elastic ),
                //         true //точка ветвления, поэтому noised
                //         );
                //_p.push_back( p2 );
            }
        }
        i++;
    }
    
    
}

//--------------------------------------------------------
vector<ofPoint> _treeTemp;

void TreeDyn::drawFbo()
{
    TreeCommon &com = treeCommon;
    int size = com.Branch * _p.size();        //длина максимальной линии

    if ( size > 0 ) {
        fbo.begin();
        ofBackground( 0, 0, 0, 0 );
   
        //ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
        float alpha = com.TreesAlpha.value();
        ofSetColor( 255, 255, 255, alpha * 255 );
        
        ofFill();
    
        vector<ofPoint> &p = _treeTemp;
        p.resize( size * 4 );
        int k = 0;
    
        //glBegin( GL_QUADS );
        for ( int i=1; i<size; i++ ) {
            TPoint &p1 = _p[i];
            TPoint &p0 = _p[ p1.parent ];
            
            p[ k++ ] = p0.p2;
            p[ k++ ] = p0.p1;
            p[ k++ ] = p1.p1;
            p[ k++ ] = p1.p2;
            
            //ofLine( p0.p, p1.p );
            //glVertex2f(p0.p2.x, p0.p2.y);
            //glVertex2f(p0.p1.x, p0.p1.y);
            //glVertex2f(p1.p1.x, p1.p1.y);
            //glVertex2f(p1.p2.x, p1.p2.y);
    
        }
        //glEnd();
        
        pbTextureUtil::drawQuads( p );
        
        fbo.end();
    }
}

//--------------------------------------------------------



