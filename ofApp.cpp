#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(1280, 720);
	ofSetFrameRate(this->cap.get(CV_CAP_PROP_FPS));

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame, src, blend_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		return;
	}
	cv::resize(cap_frame, src, this->cap_size);
	cv::flip(src, src, 1);
	cv::cvtColor(src, src, CV_RGB2BGR);

	this->frame = this->frame.zeros(this->frame.size(), this->frame.type());

	for (int x = 0; x < src.cols; x++) {

		for (int y = 0; y < src.rows; y++) {

			ofVec2f point(x, y);

			if (x > 390 && x < 890 && y > 110 && y < 610) {

				int group_x = (x - 390) / 100;
				int group_y = (y - 110) / 100;
				int value = ofNoise(group_x * 0.05, group_y * 0.05, ofGetFrameNum() * 0.005) * 4;
				int angle = value * 90;

				ofVec2f center = ofVec2f(group_x * 100 + 390 + 50, group_y * 100 + 110 + 50);
				point = point - center;
				point = point.getRotated(angle);
				point = point + center;
			}

			int tmp_x = (int)point.x;
			int tmp_y = (int)point.y;

			if (tmp_x >= 0 && tmp_x < src.cols && tmp_y >= 0 && tmp_y < src.rows) {
				
				this->frame.at<cv::Vec3b>(tmp_y, tmp_x) = src.at<cv::Vec3b>(y, x);
			}
		}
	}

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}