#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BBPulseApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void BBPulseApp::setup()
{
}

void BBPulseApp::mouseDown( MouseEvent event )
{
}

void BBPulseApp::update()
{
}

void BBPulseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( BBPulseApp, RendererGl )
