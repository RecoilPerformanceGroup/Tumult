//
//  tumultVideoMixer.cpp
//  Tumult
//
//  Created by Johan Bichel Lindegaard on 01/10/14.
//
//

#include "tumultVideoMixer.h"

void VideoMixer::setup() {
    
    name = "VideoMixer Scene";
    oscAddress = "/vidmix";
    drawToPlanes = false;
    
    //vidplayer.loadMovie("/Users/recoil/Movies/00039.mov");
    happlayer.loadMovie("/Users/recoil/Movies/00039.mov");
    happlayer.play();
}

void VideoMixer::draw(int _surfaceId) {
    
}

void VideoMixer::flatDraw() {
    happlayer.draw(0, 0, 3840, 1080);
}

void VideoMixer::update() {
    vidplayer.update();
}