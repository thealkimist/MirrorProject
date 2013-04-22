#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "ofxFX.h"

#include "vectorField.h"
#include "particle.h"

class testApp : public ofBaseApp{
public:
    void    setup();
    void    update();
    void    draw();
    
    void    keyPressed  (int key);
    void    keyReleased(int key);
    void    mouseMoved(int x, int y );
    void    mouseDragged(int x, int y, int button);
    void    mousePressed(int x, int y, int button);
    void    mouseReleased(int x, int y, int button);
    void    windowResized(int w, int h);
    void    dragEvent(ofDragInfo dragInfo);
    void    gotMessage(ofMessage msg);
    
    void    init(int _width, int _height, int _screenWidth, int _screenHeight);
    void    preprocess(ofTexture &_text);
    
    ofTrueTypeFont		font;
    
    bool		bSendSerialMessage;			// a flag for sending serial
    char		bytesRead[3];				// data from serial, we will be trying to read 3
    char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?
    
    ofSerial	serial;
    
    string message;
    
    //  INPUT
    //
    ofVideoGrabber  video;
    ofImage         image;
    
    //  PREPROCESS
    //
    ofxGrayscale    grayscale;
    ofxGaussianBlur blur;
    ofxNormals      normals;
    
    //  PROCESS
    //
    ofPixels        pixels;
    vectorField     VF;
    vector<Particle*> particles;
    
    //  RENDER
    //
    ofFbo           fbo;
    
    float   noise;
    
    int     width, height,scale;
    
    bool    bImage;
    bool    bTrails;
    bool    bDrawField;
    bool    bPrintScreen;
    
    int blur2, dilate;
    int threshold;
    bool bGetBackground;
    
    //ofVideoGrabber cam;
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage bgImage;
    ofxCvGrayscaleImage diffImage;
    
    ofxCvContourFinder contourFinder;
    
    float   changeAngle, particleW, particleH, moving;
    int     quantity;
    ofPoint pushing;
};
