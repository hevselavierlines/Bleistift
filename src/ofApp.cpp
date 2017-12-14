#include "ofApp.h"

using namespace YAMPE;
//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // repatable randomness
    ofSeedRandom(10);
    
    // simulation specific stuff goes here
    
    gui.setup();
    ImGui::GetIO().MouseDrawCursor = false;
    
    // load parameters from file
    // loadFromFile("settings.xml");
    
    // instantiate the ground
    ground.set(RANGE, RANGE);
    ground.rotate(90, 1,0,0);
    
    // lift camera to 'eye' level
    easyCam.setDistance(RANGE);
    float d = easyCam.getDistance();
    easyCam.setPosition(ofVec3f(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio))+easyCamTarget);
    easyCam.setTarget(easyCamTarget);
    
    // finally start everything off by resetting the simulation
    reset();
    
}

void ofApp::reset() {
    
    t = 0.0f;
    
    centrePoint.x = -4.0f * cos(ofDegToRad(-elevation));
    centrePoint.y = -4.0f * sin(ofDegToRad(-elevation)) + 1.0f + height;
    centrePoint.z = 0.0f;
    
    points.clear();
    
    centrePoint.z = length / 2;
    Particle::Ref partFront = Particle::Ref(new Particle());
    partFront->setPosition(centrePoint);
    points.push_back(partFront);
    
    centrePoint.z = -length / 2;
    Particle::Ref partBack = Particle::Ref(new Particle());
    partBack->setPosition(centrePoint);
    points.push_back(partBack);
    
    float theta = 2 * M_PI / sides;
    
    for(int i = 0; i < sides; i++) {
        float angle = theta * (i + 0.5f) + ofDegToRad(-elevation);
        
        ofVec3f position;
        position.x = centrePoint.x + (radius * cos(angle));
        position.y = centrePoint.y + (radius * sin(angle));
        position.z = length / 2;
        Particle::Ref particle = Particle::Ref(new Particle());
        if(i == 0) {
            particle->setBodyColor(ofColor::white);
            particle->setWireColor(ofColor::white);
        } else if(i == 1) {
            particle->setBodyColor(ofColor::red);
            particle->setWireColor(ofColor::red);
        } else if(i == 2) {
            particle->setBodyColor(ofColor::green);
            particle->setWireColor(ofColor::green);
        } else if(i == 3) {
            particle->setBodyColor(ofColor::blue);
            particle->setWireColor(ofColor::blue);
        }
        particle->setPosition(position);
        points.push_back(particle);
    }
    
    for(int i = 0; i < sides; i++) {
        float angle = theta * (i - 0.5f);
        
        ofVec3f position;
        position.x = centrePoint.x + (radius * cos(angle));
        position.y = centrePoint.y + (radius * sin(angle));
        position.z = -length / 2;
        Particle::Ref particle = Particle::Ref(new Particle());
        if(i == 0) {
            particle->setBodyColor(ofColor::red);
            particle->setWireColor(ofColor::red);
        } else if(i == 1) {
            particle->setBodyColor(ofColor::green);
            particle->setWireColor(ofColor::green);
        } else if(i == 2) {
            particle->setBodyColor(ofColor::blue);
            particle->setWireColor(ofColor::blue);
        }
        particle->setPosition(position);
        points.push_back(particle);
    }
}

void ofApp::update() {
    
    float dt = ofClamp(ofGetLastFrameTime(), 0.0, 0.02);
    if (!isRunning) return;
    t += dt;
    
    // TODO - simulation specific stuff goes here
    
}

void ofApp::draw() {
    
    ofEnableDepthTest();
    ofBackgroundGradient(ofColor(128), ofColor(0), OF_GRADIENT_BAR);
    
    ofPushStyle();
    easyCam.begin();
    
    ofDrawGrid(RANGE/(2*8), 8, false, isXGridVisible, isYGridVisible, isZGridVisible);
    
    if (isAxisVisible) ofDrawAxis(1);
    
    if (isGroundVisible) {
        ofPushStyle();
        ofSetHexColor(0xB87333);
        ground.draw();
        ofPopStyle();
    }
    
    for(Particle::Ref p : points) {
        p->draw();
    }
    ofPushMatrix();
    ofSetColor(0, 255, 0, 128);
    ofRotate(-elevation, 0, 0, 1.0f);
    ofDrawBox(0, 0, 0, 10, 0.1f, 10);
    ofPopMatrix();
    easyCam.end();
    ofPopStyle();
    
    // draw gui elements
    gui.begin();
    drawAppMenuBar();
    drawMainWindow();
    drawLoggingWindow();
    gui.end();
    
}


void ofApp::drawAppMenuBar() {
    
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "")) quit();
            ImGui::EndMenu();
        }
        
        float d = easyCam.getDistance();
        
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem(isAxisVisible?"Hide Unit Axis":"Show Unit Axis", "")) isAxisVisible = !isAxisVisible;
            if (ImGui::MenuItem(isGroundVisible?"Hide Ground":"Show Ground", "")) isGroundVisible = !isGroundVisible;
            if (ImGui::MenuItem(isXGridVisible?"Hide Grid (X)":"Show Grid (X)", "")) isXGridVisible = !isXGridVisible;
            if (ImGui::MenuItem(isYGridVisible?"Hide Grid (Y)":"Show Grid (Y)", "")) isYGridVisible = !isYGridVisible;
            if (ImGui::MenuItem(isZGridVisible?"Hide Grid (Z)":"Show Grid (Z)", "")) isZGridVisible = !isZGridVisible;
            ImGui::Separator();
            if (ImGui::MenuItem("Align camera above X axis ", "")) {
                easyCam.setPosition(ofVec3f(d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio), cameraHeightRatio*d, 0)+easyCamTarget);
                easyCam.setTarget(easyCamTarget);
            }
            if (ImGui::MenuItem("Align camera above Z axis ", "")) {
                easyCam.setPosition(ofVec3f(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio))+easyCamTarget);
                easyCam.setTarget(easyCamTarget);
                cout <<"here";
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Align camera along X axis ", "")) {
                easyCam.setPosition(ofVec3f(d, 0, 0)+easyCamTarget);
                easyCam.setTarget(easyCamTarget);
            }
            if (ImGui::MenuItem("Align camera along Y axis ", "")) {
                easyCam.setPosition(ofVec3f(0.001, d, 0)+easyCamTarget);
                easyCam.setTarget(easyCamTarget);
            }
            if (ImGui::MenuItem("Align camera along Z axis ", "")) {
                easyCam.setPosition(ofVec3f(0, 0, d)+easyCamTarget);
                easyCam.setTarget(easyCamTarget);
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Application")) {
            if (ImGui::MenuItem("Add application specific menu items here", "")) {
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}


void ofApp::drawMainWindow() {
    
    
    ImGui::SetNextWindowPos(ImVec2(0,20), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("Main")) {
        
        if (ImGui::CollapsingHeader("3D")) {
            if(ImGui::Button("Reset##CameraTarget")) {
                easyCamTarget = ofVec3f(0,5,0);
                easyCam.setTarget(easyCamTarget);
            }
            
            ImGui::SameLine();
            if (ImGui::InputFloat3("Camera Target", &easyCamTarget.x)) {
                easyCam.setTarget(easyCamTarget);
            }
            if (ImGui::SliderFloat("Camera Height Ratio", &cameraHeightRatio, 0.0f, 1.0f))
                cameraHeightRatioChanged(cameraHeightRatio);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        
        ImGui::SliderFloat("Elevation", &elevation, 0, 90, "%2.1f");
        ImGui::Checkbox("Sticky", &sticky);
        ImGui::Text("Bleistift");
        ImGui::SliderInt("Sides", &sides, 3, 20);
        ImGui::SliderFloat("Height", &height, 0, 1, "%1.2f");
        
        if(ImGui::Button("Reset")) reset();
        ImGui::SameLine();
        if(ImGui::Button(isRunning?"Stop":" Go ")) isRunning = !isRunning;
        ImGui::SameLine();
        ImGui::Text("   Time = %8.1f", t);
        if(ImGui::Button("Quit")) quit();
        
        if (ImGui::CollapsingHeader("Numerical Output")) {
            // TODO - numeric output goes here
        }
        
        if (ImGui::CollapsingHeader("Graphical Output")) {
            // TODO - graphical output goes here
        }
    }
    
    // store window size so that camera can ignore mouse clicks
    mainWindowRectangle.setPosition(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
    mainWindowRectangle.setSize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    ImGui::End();
    
}


void ofApp::drawLoggingWindow() {
    ImGui::SetNextWindowSize(ImVec2(290,300), ImGuiSetCond_Always);
    ImGui::SetNextWindowPos(ImVec2(ofGetWindowWidth()-300,20), ImGuiSetCond_Always);
    
    if (ImGui::Begin("Logging")) {
    }
    // store window size so that camera can ignore mouse clicks
    loggingWindowRectangle.setPosition(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
    loggingWindowRectangle.setSize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    ImGui::End();
}

//--------------------------------------------------------------
// GUI events and listeners
//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
    
    float d = easyCam.getDistance();
    
    switch (key) {
            
            //        case 'h':                               // toggle GUI/HUD
            //           isGuiVisible = !isGuiVisible;
            //            break;
            //        case 'b':                               // toggle debug
            //            isDebugVisible = !isDebugVisible;
            //            break;
            //        case 'a':                               // toggle axis unit vectors
            //            isAxisVisible = !isAxisVisible;
            //            break;
            //        case 'g':                               // toggle ground
            //            isGroundVisible = !isGroundVisible;
            //            break;
            //        case 'u':                               // set the up vecetor to be up (ground to be level)
            //            easyCam.setTarget(ofVec3f::zero());
            //            break;
            //
            //        case 'S' :                              // save gui parameters to file
            //            gui.saveToFile("settings.xml");
            //
            //            break;
            //        case 'L' :                              // load gui parameters
            //            gui.loadFromFile("settings.xml");
            //            break;
            //
        case 'z':
            easyCam.setPosition(ofVec3f(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio))+easyCamTarget);
            easyCam.setTarget(easyCamTarget);
            break;
        case 'Z':
            easyCam.setPosition(0, 0, d);
            easyCam.setTarget(ofVec3f::zero());
            break;
        case 'x':
            easyCam.setPosition(ofVec3f(d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio), cameraHeightRatio*d, 0)+easyCamTarget);
            easyCam.setTarget(easyCamTarget);
            break;
        case 'X':
            easyCam.setPosition(ofVec3f(d, 0, 0)+easyCamTarget);
            easyCam.setTarget(easyCamTarget);
            break;
        case 'Y':
            easyCam.setPosition(ofVec3f(0.001, d, 0)+easyCamTarget);
            easyCam.setTarget(easyCamTarget);
            break;
            
        case 'f':                               // toggle fullscreen
            // BUG: window size is not preserved
            isFullScreen = !isFullScreen;
            if (isFullScreen) {
                ofSetFullscreen(false);
            } else {
                ofSetFullscreen(true);
            }
            break;
            
            // simulation specific stuff goes here
            
    }
}


void ofApp::cameraHeightRatioChanged(float & cameraHeightRatio) {
    
    float d = easyCam.getDistance();
    easyCam.setPosition(ofVec3f(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio))+easyCamTarget);
    easyCam.setTarget(easyCamTarget);
}


void ofApp::quit() {
    ofExit();
}

//--------------------------------------------------------------
// Unused
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y ) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {
    // easy camera should ignore GUI mouse clicks
    if (mainWindowRectangle.inside(x,y)||loggingWindowRectangle.inside(x,y))
        easyCam.disableMouseInput();
    else
        easyCam.enableMouseInput();
}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}