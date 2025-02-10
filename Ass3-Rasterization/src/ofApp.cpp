#include "ofApp.h"
#include "matf4x4.h"
#include"utilsFn.h"
// cpp_compiler_options_openmp.cpp
#include <omp.h>


//--------------------------------------------------------------
void ofApp::setup(){
    w = ofGetWidth();//1024
    h = ofGetHeight();//768
    ArcballFlag = false;
    colorPixels.allocate(w, h, OF_PIXELS_RGB);
    texColor.allocate(colorPixels);
    _PhongExp = 32;
    _ambient = f3Vector(0.05, 0.05, 0.05);
    _TestColor = f3Vector(0.5, 0, 0);
    _ShadingTYPE = 0;//initialize as the normal shading
    _ModelSelect = 0;//0 for beacon 1 for teapot
    ModelChangeFlage = false;
    _LightTensity= 1;
    StringInfo.load("vag.ttf", 10);
    ofLoadImage(_Mypixels, "texture.jpg");
    int wt = _Mypixels.getWidth();
    int ht= _Mypixels.getHeight();
    cout << "_Mypixels.getWidth();=" << wt; cout << "_Mypixels.getWidth();" << ht;

    _Mytexture.allocate(_Mypixels);

    //1.Load the model
    _basepath ="C:\\Users\\LID0E\\source\\OpenFramework\\of_v0.12.0_vs_release\\apps\\myApps\\Assignment3_Rasterization\\bin\\data\\";
    //_basepath = "D:\\OpenFramework\\apps\\myApps\\ASSIGNMENT3\\bin\\data\\";
    _TeapotCen = f3Vector(0.217, 1.575, 0);
    _BeaconCen = f3Vector(125, 125, 125);
    FragData = FragRes(w, h);
    if (_ModelSelect == 0) {
        _TestColor = f3Vector(0.5, 0.5, 0.5);
        _objname = "KAUST_Beacon.obj";
        _filename = _basepath + _objname;
        loadModel(_filename.c_str(), _basepath.c_str(), false);
        RasterData = RasResources(OBJData.vectices.size());
        setupScene(_BeaconCen, _BeaconCen, f3Vector(0, 0, 0), f3Vector(125, 125, 350), f3Vector(-200, -200, -50), f3Vector(200, 200, -300), f3Vector(125, 300, 350));// take care for perspective
    }
    else if (_ModelSelect == 1) {
        _TestColor = f3Vector(0.5, 0, 0);
        _objname = "teapot.obj";
        _filename = _basepath + _objname;
        loadModel(_filename.c_str(), _basepath.c_str(), false);
        RasterData = RasResources(OBJData.vectices.size());
        setupScene(_TeapotCen, _TeapotCen, f3Vector(0, 0, 0), f3Vector(0.217, 1.575, 4), f3Vector(-4, -4, -2), f3Vector(4, 4, -6), f3Vector(0.217, 10, 4));
    }
    else if (_ModelSelect == 10) {
        //for testing
        _TestColor = f3Vector(0.5, 0, 0);
        _objname = "test_triangle.obj";
        _filename = _basepath + _objname;
        _TeapotCen = f3Vector(5, 4, 1.5);
        loadModel(_filename.c_str(), _basepath.c_str(), false);
        RasterData = RasResources(OBJData.vectices.size());
        setupScene(_TeapotCen, _TeapotCen, f3Vector(0, 0, 0), f3Vector(5, 4, 4), f3Vector(-4, -4, -2), f3Vector(4, 4, -6), f3Vector(5, 10, 4));
    }
         
    //string objname = "test_triangle.obj"; 
    //string objname = "teapot.obj";


    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //ofClear(0);
    if (ModelChangeFlage) {//reload the data
        w = ofGetWidth();//1024
        h = ofGetHeight();//768
        ArcballFlag = false;
        colorPixels.allocate(w, h, OF_PIXELS_RGB);
        texColor.allocate(colorPixels);
        _PhongExp = 32;
        _ambient = f3Vector(0.05, 0.05, 0.05);
        
        _ShadingTYPE = 0;//initialize as the normal shading
        //_ModelSelect = 0;//0 for beacon 1 for teapot
        ModelChangeFlage = false;
        _LightTensity = 1;
        StringInfo.load("vag.ttf", 10);
        //1.Load the model
        //_basepath ="C:\\Users\\LID0E\\source\\OpenFramework\\of_v0.12.0_vs_release\\apps\\myApps\\Assignment3_Rasterization\\bin\\data\\";
        //_basepath = "D:\\OpenFramework\\apps\\myApps\\ASSIGNMENT3\\bin\\data\\";
        _TeapotCen = f3Vector(0.217, 1.575, 0);
        _BeaconCen = f3Vector(125, 125, 125);
        FragData = FragRes(w, h);
        if (_ModelSelect == 0) {
            _TestColor = f3Vector(0.5, 0.5, 0.5);
            _objname = "KAUST_Beacon.obj";
            _filename = _basepath + _objname;
            loadModel(_filename.c_str(), _basepath.c_str(), false);
            RasterData = RasResources(OBJData.vectices.size());
            setupScene(_BeaconCen, _BeaconCen, f3Vector(0, 0, 0), f3Vector(125, 125, 350), f3Vector(-200, -200, -50), f3Vector(200, 200, -300), f3Vector(125, 300, 350));// take care for perspective
        }
        else if (_ModelSelect == 1) {
            _TestColor = f3Vector(0.5, 0, 0);
            _objname = "teapot.obj";
            _filename = _basepath + _objname;
            loadModel(_filename.c_str(), _basepath.c_str(), false);
            RasterData = RasResources(OBJData.vectices.size());
            setupScene(_TeapotCen, _TeapotCen, f3Vector(0, 0, 0), f3Vector(0.217, 1.575, 4), f3Vector(-4, -4, -2), f3Vector(4, 4, -6), f3Vector(0.217, 10, 4));
        }
        else if (_ModelSelect == 10) {
            //for testing
            _TestColor = f3Vector(0.5, 0, 0);
            _objname = "test_triangle.obj";
            _filename = _basepath + _objname;
            _TeapotCen = f3Vector(5, 4, 1.5);
            loadModel(_filename.c_str(), _basepath.c_str(), false);
            RasterData = RasResources(OBJData.vectices.size());
            setupScene(_TeapotCen, _TeapotCen, f3Vector(0, 0, 0), f3Vector(5, 4, 4), f3Vector(-4, -4, -2), f3Vector(4, 4, -6), f3Vector(5, 10, 4));
        }
        ModelChangeFlage = false;
    }
    double start = omp_get_wtime();
    VerticesShader();
    FragmentShader();
    FragmentProcess();
    double finish = omp_get_wtime();
    _Telapsed = finish - start;
    _FPS = int(1 / _Telapsed);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    texColor.draw(0, 0, w, h);//draws the texture at a given point (x,y), with a given width (w) and height (h). Draws the texture at the x, y and w, h.
    _infoString = "FPS:" + ofToString(_FPS) + " \nelapsed: " + ofToString(_Telapsed) + " s" + "\n Shading Choose: " + ofToString(_ShadingTYPE)+"\n Model Selected:"+_objname;
    StringInfo.drawString(_infoString, w - 300, 10);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    cout << "key=" << key << endl;
    if (key == 32) {
        //key is 'space',change the animation
        _ShadingTYPE = (_ShadingTYPE + 1) % 4;
    }
    if (key == 57358) {
        _ModelSelect = (_ModelSelect + 1) % 2;
        ModelChangeFlage=true;
    }
    if (key == 57357) {
        _Texture = (_Texture+1)%2;
    }
    if (key == 57359) {
        _ProjectionTYPE = (_ProjectionTYPE + 1) % 2;
    }
    //57358 for -> key  57356 ×ó¼ü 57357ÉÏ¼ü 57358ÓÒ¼ü 57359ÏÂ¼ü
    //13 for enter key
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    cout << "_startP" << _startP.u << ", " << _startP.v << endl;
    cout << "mouseDragged" << x << ", " << y << endl;
    //Here record the finshed point of transformation and update the transfomation matrix
    if (ArcballFlag) {
        _c_AcrTFmat = ArcballMat(_startP.u, _startP.v, x, y);
        /*
        Recording the transform!!
        */
    }

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    cout << "mousePressed" << x <<", " << y << endl;
    ArcballFlag = true;
    _startP.u = x;
    _startP.v = y;
    //here record the start point of transformation
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    cout << "mouseReleased" << x << ", " << y << endl;
    _p_AcrTFmat = _c_AcrTFmat* _p_AcrTFmat;//previous transformation first!
    _c_AcrTFmat = matf4x4();//reset as I, otherwise it will be calculated twice!!!
    ArcballFlag = false;
    //here record the transformation matrix for concatenate 
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
