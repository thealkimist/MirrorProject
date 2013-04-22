#include "testApp.h"
#define STRINGIFY(A) #A
#include "Poco/RegularExpression.h"

using Poco::RegularExpression;


vector < string > getMatchedStrings (string stringToParse, string regex );
vector < string > getMatchedStrings (string contents, string regex, int & lastPos ){
    
    vector < string > results;
    RegularExpression regEx(regex);
    RegularExpression::Match match;
    
    lastPos = 0;
    while(regEx.match(contents, match) != 0) {
        
        // we get the sub string from the content
        // and then trim the content so that we
        // can continue to search
        string foundStr = contents.substr(match.offset, match.length);
        contents = contents.substr(match.offset + match.length);
        
        
        lastPos += match.offset + match.length;
        
        results.push_back(foundStr);
        
    }
    return results;
}

// comparison routine for sort...
bool comparisonFunction(  Particle * a, Particle * b ) {
	return a->x < b->x;
}
bool sorting;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	
	bSendSerialMessage = false;
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 64);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	//serial.setup(5, 9600); //open the first device
	//serial.setup("COM4"); // windows example
	serial.setup("/dev/tty.usbmodemfd131",9600); // mac osx example
	//serial.setup("/dev/ttyUSB0", 9600); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
    
    
    sorting = true;
    changeAngle = 1.0f;
    particleW = 10;
    particleH = 10;
    moving = 0.8;
    pushing.set(ofRandom(-10,10), ofRandom(-10,10));
    
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    /*bGetBackground = true;
     threshold = 20;
     blur2 = 3;
     dilate = 2;
     
     colorImage.allocate(640, 480);
     grayImage.allocate(640, 480);
     bgImage.allocate(640, 480);
     diffImage.allocate(640, 480);*/
    
    //init(ofGetScreenWidth(),ofGetScreenHeight(),1024,768); //------------not sure what it does
    video.initGrabber(640, 480);
    init(640,480,1024,768);
    bImage = false;
    
    noise = 0.0f;
    
    bTrails = false;
}

void testApp::init(int _width, int _height, int _screenWidth, int _screenHeight){
    width = _width;
    height = _height;
    scale = 10;
    quantity = 10;
    
    //  Setup
    //
    ofSetWindowShape(_screenWidth,_screenHeight);
    
    //  Pre-Process
    //
    grayscale.allocate(width,height);
    blur.allocate(width, height);
    blur.setRadius(10);
    blur.setPasses(10);
    normals.allocate(width, height);
    
    //  Process
    //
    VF.clear();
    VF.setupField(width/scale,height/scale,_screenWidth,_screenHeight);
    //pixels.allocate(width, height, 4); <--------------------What for?
    for (int i = 0; i < particles.size(); i++){
        delete particles[i];
    }
    particles.clear();
    for (int x = 0; x < width/scale; x++){
        for(int y = 0; y < height/scale; y++){
            Particle *myParticle = new Particle();
            myParticle->init(ofPoint(ofMap(x,0, width/scale,0, _screenWidth),
                                     ofMap(y,0,height/scale,0, _screenHeight),
                                     0.0),
                             ofPoint(0,0));
            myParticle->size = quantity;
            particles.push_back(myParticle);
        }
    }
    
    //  Render
    //
    /*
     fbo.allocate(_screenWidth, _screenHeight);
     fbo.begin();
     ofClear(0,0);
     fbo.end();*/
    
    bDrawField  = false;
    bPrintScreen = false;
}

//--------------------------------------------------------------
void testApp::preprocess(ofTexture &_text){
    grayscale.setTexture( _text );
    grayscale.update();
    blur << grayscale;
    blur.update();
    normals << blur;
    normals.update();
    
    ofPixels normPixels;
    normPixels.allocate(normals.getWidth(), normals.getHeight(), 4);
    normals.getTextureReference().readToPixels(normPixels);
    _text.readToPixels(pixels);
    /*int scaledWidth = width/scale;
     int scaledHeight = height/scale;
     for(int x = 0; x <= scaledWidth; x++){
     for(int y = 0; y <= scaledHeight; y++){
     
     int scaledX = ofClamp(x*scale,0,width-1);
     int scaledY = ofClamp(y*scale,0,height-1);
     ofFloatColor normalColor = normPixels.getColor(scaledX, scaledY);
     //ofSetColor(255);
     int index = x + y * scaledWidth;
     ofPoint norm = ofPoint((normalColor.r - 0.5) * 2.0,
     (normalColor.g - 0.5) * 2.0, 0.0);
     
     //float pct = 0.5;
     //VF[index] = VF[index]*(1.0-pct) + norm * pct;
     }
     }*/
}

void testApp::update(){
    
    int howMany = serial.available();
    
    vector <string> parsedMsg;
    
    if (howMany > 0){
        
        unsigned char bytes[howMany];
        serial.readBytes(bytes, howMany);
        //cout << "numBytes recvd: " << ofToString(howMany) << endl;
        
        for (int i = 0; i < howMany; i++){
            message += (bytes[i]);
        }
        
        //cout << message << endl;
        int foundHash = 0;
        foundHash = message.find("#");
        
        if (foundHash > 0){
            cout << message << endl;
            parsedMsg = ofSplitString(message, ",");
            
            cout << "val 1: " << parsedMsg[0] << endl;
            cout << "val 2: " << parsedMsg[1] << endl;
            cout << "val 3: " << parsedMsg[2] << endl;
            cout << "val 4: " << parsedMsg[3] << endl;
            cout << "val 5: " << parsedMsg[4] << endl;
            cout << "val 6: " << parsedMsg[5] << endl;
            cout << "val 7: " << parsedMsg[6] << endl;
            cout << "val 8: " << parsedMsg[7] << endl;
            cout << "val 9: " << parsedMsg[8] << endl;
            cout << "val 10: " << parsedMsg[9] << endl;
            cout << "val 11: " << parsedMsg[10] << endl;
            
            cout << "---------" << endl;
            message.clear();
        }
        
        howMany = 0;
    }
    
    //  INPUT + PREPROCESS
    //
    if (!bImage){
        video.update();
        if ( video.isFrameNew() ){
            /*
             colorImage.setFromPixels(video.getPixelsRef());
             colorImage.mirror(false, true);
             grayImage = colorImage;
             if(bGetBackground)
             {
             bgImage = grayImage;
             bGetBackground = false;
             }
             diffImage.absDiff(bgImage, grayImage);
             diffImage.blur( blur2*2 + 1 );
             diffImage.threshold(threshold);
             
             for(int i=0; i<dilate; i++)
             diffImage.dilate();*/
            
            preprocess(video.getTextureReference());
        }
    } else {
        preprocess(image.getTextureReference());
    }
    /*ofSetColor(255);
     video.draw(10, 10);
     colorImage.draw(340, 10);
     grayImage.draw(670, 10);
     bgImage.draw(10, 260);*/
    //diffImage.draw(340, 260);
    //colorImage.draw(0, 0);
    //  PROCESS
    //
    
    //  Sort all the particle
    //
    if(sorting){
        sort( particles.begin(), particles.end(), comparisonFunction );}
    
    //  Particle - Particle Interaction
    //
    for (int i = 0; i < particles.size(); i++){
		for (int j = i-1; j >= 0; j--){
            if ( fabs(particles[j]->x - particles[i]->x) > 20) break;
            particles[i]->addRepulsionForce( *particles[j], moving);
		}
	}
    
    //  Particle - VF
    //
    //VF.noiseField( noise, 0.01, 3, true);
    for (int i = 0; i < particles.size(); i++){
        
        //  Position
        //
        int xOnVideo = ofMap(particles[i]->x, 0, ofGetWidth(), 0, width-1,true);
        int yOnVideo = ofMap(particles[i]->y, 0, ofGetHeight(), 0, height-1,true);
        
        particles[i]->color.lerp( pixels.getColor(xOnVideo, yOnVideo) , 0.01);
        float  brigtness = particles[i]->color.getBrightness();
        
        particles[i]->size = (0.1 + (1.0 - brigtness)*0.9 ) * (scale*2.0);
        //particles[i]->damping = ofMap( brigtness ,0.0,1.0,0.1,0.01);
        
        //ofPoint force = VF.getForceFromPos( *particles[i] );
        //particles[i]->addForce( force );
        particles[i]->bounceOffWalls();
        particles[i]->update(pushing);
	}
    
    ofSetWindowTitle(" noise: " + ofToString(noise) + " threshold: " + ofToString(threshold) + " rotateAngle: " + ofToString(changeAngle) + " particleSize: " + ofToString(particleH) + " moving: " + ofToString(moving) + " pushing: " + ofToString(pushing));
    //"fps: "+ofToString(ofGetFrameRate()) +
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    if (bTrails){
        
        //fbo.begin();
        for (int i = 0; i < particles.size(); i++){
            ofSetColor(0);
            particles[i]->draw(changeAngle, particleW, particleH);
        }
        //fbo.end();
        //ofSetColor(255);
        //fbo.draw(0, 0);
        
        if (bPrintScreen){
            ofSaveScreen("screenshot-"+ofGetTimestampString()+".png");
            bPrintScreen = false;
        }
    }
    
    if (bPrintScreen){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    for (int i = 0; i < particles.size(); i++){
        ofSetColor(0);
        particles[i]->draw(changeAngle, particleW, particleH);
    }
    
    if (bPrintScreen){
        ofEndSaveScreenAsPDF();
        bPrintScreen = false;
    }
    
    
    if (bDrawField){
        ofSetColor(255,100);
        normals.draw(0,0,ofGetWidth(),ofGetHeight());
        ofDisableSmoothing();
        ofSetColor(100);
        VF.draw();
        ofEnableSmoothing();
    }
    
    if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
		ofSetColor(0);
	} else {
		ofSetColor(220);
	}
	string msg;
	msg += "click to test serial:\n";
	msg += "nBytes read " + ofToString(nBytesRead) + "\n";
	msg += "nTimes read " + ofToString(nTimesRead) + "\n";
	msg += "read: " + ofToString(bytesReadString) + "\n";
	msg += "(at time " + ofToString(readTime, 3) + ")";
	font.drawString(msg, 50, 100);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        bGetBackground = true;
        
        for (int i = 0; i < particles.size(); i++){
            delete particles[i];
        }
        particles.clear();
        
        for (int x = 0; x < width/scale; x++){
            for(int y = 0; y < height/scale; y++){
                Particle *myParticle = new Particle();
                //                myParticle->init(ofPoint(ofMap(x,0, width/scale,0, ofGetWidth()),ofMap(y,0,height/scale,0,ofGetHeight()),0.0),ofPoint(0,0));
                myParticle->init(ofPoint(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),0.0),ofPoint(0,0));
                myParticle->size = 2;
                particles.push_back(myParticle);
            }
        }
        
        fbo.begin();
        ofClear(0,0);
        fbo.end();
        
    } else if ( key == 'f'){
        bDrawField = !bDrawField;
	} else if ( key == 'v'){
        bImage = false;
        init(video.getWidth(),video.getHeight(),1024,768);
	} else if ( key == 'p'){
        bPrintScreen = true;
    } else if ( key == 't'){
        bTrails = !bTrails;
    }
    
    if(key == '2')
        threshold ++;
    if(key == '1')
        threshold --;
    if(key == 's'){
        changeAngle += 0.1f;
        if(changeAngle >= 1)changeAngle = 1;
    }
    if(key == 'a'){
        changeAngle -= 0.1f;
        if(changeAngle <= 0)changeAngle = 0;
    }
    if(key == 'z'){
        particleW -= 1.0f;
        particleH -= 1.0f;
        if(particleW <= 0 || particleH <= 0){particleW = 0;particleH = 0;}
    }
    if(key == 'x'){
        particleW += 1.0f;
        particleH += 1.0f;
    }
    /*if(key == 'q'){
     pushing-= ofRandom(-1,1);
     if(pushing.x <= 0)pushing.set(0,0);;
     }
     if(key == 'w')
     pushing += 1;*/
    if(key == 'k')
        moving -= 0.1; if(moving <= 0) moving = 0;
    if(key == 'l')
        moving += 0.1;
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //noise   = ofMap(y, 0, ofGetHeight(), 0.1, 0.5);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if(dragInfo.files.size() > 0){
        bImage = image.loadImage(dragInfo.files[0]);
        if (bImage){
            image.update();
            init(image.getWidth(),image.getHeight(),image.getWidth()*1.2,image.getHeight()*1.2);
            preprocess(image.getTextureReference());
        }
    }
}
