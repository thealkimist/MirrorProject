#include "particle.h"
#include "ofMain.h"

//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);
    
	damping = 0.01;
    
    color.set(0.0);
}

void Particle::init( ofPoint _pos, ofPoint _vel){
    realPoint = _pos;
    vel.set(_vel);
    this->set(_pos);
    
}

//------------------------------------------------------------
void Particle::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Particle::update(ofPoint pushing){
    
    vel += acc;
    //vel += realPoint - acc;
    //vel *= 1.0f - damping;
    *this += vel;
    //*this =( *realPoint + pushing );
    
    acc *= 0;
    
}

//------------------------------------------------------------
void Particle::draw(float changeAngle, float particleW, float particleH){
    ofPushMatrix();
    ofSetColor(color);
    float w = particleW;
    //float h = w * (1.0 + vel.length() * 2.0);
    float h = particleH;
    //float w = ofMap(size, 0, size, 5, 20), h = ofMap(size, 0, size, 5, 20);
    angle = ofLerp(angle,atan2(vel.y,vel.x) + HALF_PI, ofMap(damping, 0.01, 0.1, 0.05, 0.01)) * changeAngle;
    ofTranslate(x, y);
    ofRotateZ(ofRadToDeg(angle));
    ofRect(-w*0.2, -h*0.2, w, h);
    ofPopMatrix();
}

void Particle::infinitWalls(){
    
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
}

//------------------------------------------------------------
void Particle::addRepulsionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
    
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
        acc += diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
    
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle &p, float radius, float scale){
    
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
    
	// ----------- (2) calculate the difference & length
    
	ofVec2f diff	= (*this) - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		p.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle &p, float scale){
    
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
    
	// ----------- (2) calculate the difference & length
    
	ofVec2f diff	= (*this) - posOfForce;
	float length	= diff.length();
    float radius    = size + p.size ;
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1.0 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		p.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(Particle & p, float radius, float scale){
    
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
    
	// ----------- (2) calculate the difference & length
    
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
		p.acc += diff * scale * pct;
    }
    
}

//------------------------------------------------------------
void Particle::addClockwiseForce(Particle &p, float radius, float scale){
    
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
    
	// ----------- (2) calculate the difference & length
    
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x -= diff.y * scale * pct;
        acc.y += diff.x * scale * pct;
		p.acc.x += diff.y * scale * pct;
        p.acc.y -= diff.x * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addCounterClockwiseForce(Particle &p, float radius, float scale){
    
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
    
	// ----------- (2) calculate the difference & length
    
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
    
	// ----------- (3) check close enough
    
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x += diff.y * scale * pct;
        acc.y -= diff.x * scale * pct;
		p.acc.x -= diff.y * scale * pct;
        p.acc.y += diff.x * scale * pct;
        
    }
}

//------------------------------------------------------------
void Particle::bounceOffWalls(){
    
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
    
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
    
	if (x > maxx){
		x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (x < minx){
		x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
    
	if (y > maxy){
		y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (y < miny){
		y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
    
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
    
}
