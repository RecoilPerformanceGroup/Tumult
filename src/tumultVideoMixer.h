//
//  tumultVideoMixer.h
//  Tumult
//
//  Created by Johan Bichel Lindegaard on 01/10/14.
//
//

#include "ofMain.h"
#include "ContentScene.h"
#include "ofxHapPlayer.h"
#pragma once

class VideoMixer : public ContentScene  {
    
    void setup();
    void update();
    void draw(int _surfaceId);
    void flatDraw();
    
    ofVideoPlayer vidplayer;
    ofxHapPlayer happlayer;
};