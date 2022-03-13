#include "ofApp.h"

#include "Tree.h"
#include "Trees.h"
#include "Figures.h"
#include "AppParam.h"


int w = 1024;
int h = 768;

bool useOSC = true;
            //false;
int oscInPort = 12346;

int SceneTrees = 1;
int SceneFigures = 2;

int SceneEmpty = 3;
int scene = SceneFigures;


Trees trees;
Figures figures;

bool bigScreen = //false;
            true;

bool drawRect = false;

int edit = 0;       //tree number
int editV = 0;      //vertex number
float editSpeed = 10;

//--------------------------------------------------------------
void testApp::setup(){
    string title = "Inversia Trees 1.17";
    if ( !useOSC ) { title += " - OSC DISABLED!"; }
    else {
        title += " - Port " + ofToString( oscInPort );
    }
    ofSetWindowTitle( title );

    //syphonTrees.setName( "Trees" );
    //syphonFigures.setName( "Figures" );
    //syphonMask.setName( "Mask" );

    params.setName( "params" );
    
    params_trees.setName("Trees");
    params_fig.setName("Figures");
    trees.setup(params_trees, w, h );
    figures.setup(params_fig, w, h );
    
    params.add(params_trees);
    params.add(params_fig);

    gui.setup( params );
    
    //fbo.allocate( w, h, GL_RGBA );
	
	if ( useOSC ) {
        sync.setup((ofParameterGroup&)gui.getParameter(), oscInPort, "localhost", 12346);
    }
    
	ofSetVerticalSync(false);
    ofSetFrameRate( 30 );
    
    setBigScreen( bigScreen );
    
    ParamSetup();
    
    scene = ParamInt( "interface:scene", scene );
}

//--------------------------------------------------------------
void testApp::exit() {
    
}

//--------------------------------------------------------------
float _time0 = 0;
void testApp::update(){
	//OSC
    if ( useOSC ) {
        sync.update();
    }
    
    //Scenes
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - _time0, 0, 1.0 / 25.0 );
    _time0 = time;
    

    trees.update( dt, drawRect );
    figures.update( dt, drawRect );
    
    
    //Syphon
    //syphonTrees.publishTexture( &trees.fboSyphon.getTextureReference() );
    //syphonFigures.publishTexture( &figures.fboSyphon.getTextureReference() );
    //syphonMask.publishTexture( &figures.fboMask.getTextureReference() );
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground( 0 );
            
    ofColor color = ( drawRect ) ? ofColor( 255, 0, 0 ) : ofColor( 255 );

    ofDisableAlphaBlending();
    if ( scene == SceneTrees ) {
        ofSetColor( 255 );
        //trees.fbo.draw( 0, 0 );
        trees.fboSyphon.draw( 0, 0 );
        trees.drawMappedRects( color, true );
    }
    if ( scene == SceneFigures ) {
        ofSetColor( 255 );
        //figures.fbo.draw( 0, 0 );
        figures.fboSyphon.draw( 0, 0 );
        figures.drawMappedRects( color, true );
        
        //ofSetColor( 255 );
        //figures.fboMask.draw( 0, 0 );
    }
    ofEnableAlphaBlending();
    
    if ( bigScreen ) {
        gui.draw();
    }
    
    ofSetColor( 255 );
    float x = ( bigScreen ) ? ( ofGetWidth() / 2 ) : 10;
    ofDrawBitmapString( "s-save, 1,2-scenes, RBM,Space - mapping, -/= screen", x, 15 );
	//ofSetColor(color);
	//for(int i=0;i<number;i++){
	//	ofCircle(ofGetWidth()*.5-size*((number-1)*0.5-i), ofGetHeight()*.5, size);
	//}
}

//--------------------------------------------------------------
void testApp::setBigScreen( bool v )
{
    bigScreen = v;
    if ( bigScreen ) {
        ofSetWindowShape( w, h );
    }
    else {
        ofSetWindowShape( w*1/2, h*1/2 );
    }
}

//--------------------------------------------------------------
void setScene( int s )
{
    scene = s;
    ParamIntSave( "interface:scene", scene );

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == '-' ) {
        setBigScreen( false );
    }
    if ( key == '=' ) {
        setBigScreen( true );
    }
    if ( key == '1' ) { setScene( SceneTrees ); }
    if ( key == '2' ) { setScene( SceneFigures ); }
//    if ( key == '3' ) { scene = SceneEmpty; }
    
    if ( key == ' ' ) { drawRect = !drawRect; }
    
    if ( key == 'R' ) {
        if ( scene == SceneTrees ) { trees.reset(); }
        if ( scene == SceneFigures ) { figures.reset(); }
    }
    
    //Tree
    /*if ( key == '1' ) { edit = 0; }
    if ( key == '2' ) { edit = 1; }
    if ( key == '3' ) { edit = 2; }
    if ( key == '4' ) { edit = 3; }
    
    //Vertex
    if ( key == 'q' ) { editV = 0; }
    if ( key == 'w' ) { editV = 1; }
    if ( key == 'e' ) { editV = 2; }
    if ( key == 'r' ) { editV = 3; }
    
    if ( key == 'a' ) { editSpeed = 0.5; }
    if ( key == 's' ) { editSpeed = 10; }

    //Shift
    ofPoint shift;
    if ( key == OF_KEY_LEFT ) { shift.x = -1; }
    if ( key == OF_KEY_RIGHT ) { shift.x = 1; }
    if ( key == OF_KEY_UP ) { shift.y = 1; }
    if ( key == OF_KEY_DOWN ) { shift.y = -1; }
    if ( shift.x != 0 || shift.y != 0 ) {
        trees.move( edit, editV, shift * editSpeed );
    }
    */
        
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if ( scene == SceneTrees ) {    trees.mousePressed( x, y, button ); }
    if ( scene == SceneFigures ) {    figures.mousePressed( x, y, button ); }
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if ( scene == SceneTrees ) {    trees.mouseDragged( x, y, button ); }
    if ( scene == SceneFigures ) {    figures.mouseDragged( x, y, button ); }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if ( scene == SceneTrees ) {    trees.mouseReleased( x, y, button ); }
    if ( scene == SceneFigures ) {    figures.mouseReleased( x, y, button ); }

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
