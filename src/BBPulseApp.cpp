#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Callback.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"
#include "OSCListener.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BBPulseApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
    
    void audioCallback (uint64_t inSampleOffset, uint32_t ioSampleCount, audio::Buffer32f *buffer);
    
    float mFrequency;
    float mPhase, mPhaseAdd;
    vector<float> mOutput;
    
    float mModFrequency;
    float mModPhase, mModPhaseAdd;
    
    float mDelay;
    float mMix, mFeedback;
    vector<float> mDelayLine;
    int mDelayIndex;
    int mDelaySize;
    
};

void BBPulseApp::setup()
{
    mFrequency = 440.0f;
    mPhase = 0.0f;
    mPhaseAdd = 0.0f;
    
    mModFrequency = 0.0f;
    mModPhase = 0.0f;
    mModPhaseAdd = 0.0f;
    
    audio::Output::play( audio::createCallback( this, &BBPulseApp::audioCallback ) );
    
    mDelay = 200;
    mMix = 0.2f;
    mFeedback = 0.3f;
    
    mDelaySize = mDelay * 44.1f;
    for( int i=0; i<mDelaySize; i++ ){
        mDelayLine.push_back( 0.0f );
    }
    mDelayIndex = 0;
}

void BBPulseApp::mouseDown( MouseEvent event )
{
}

void BBPulseApp::update()
{
    float maxFrequency = 5000.0f;
    float targetFrequency = ( getMousePos().y / (float)getWindowHeight() ) * maxFrequency;
    mFrequency = math<float>::clamp( targetFrequency, 0.0f, maxFrequency );
    
    float maxModFrequency = 30.0f;
    float targetModFrequency = ( getMousePos().x / (float)getWindowWidth() ) * maxModFrequency;
    mModFrequency = math<float>::clamp( targetModFrequency, 0.0f, maxModFrequency );
}

void BBPulseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if( mOutput.size() > 0 ){
        Vec2f scale;
        scale.x = (float)getWindowWidth() / (float)mOutput.size();
        scale.y = 100.0f;
        float centerY = getWindowHeight() / 2.0f;
        
        gl::begin( GL_LINE_STRIP );
        for( int i=0; i<mOutput.size(); i++ ){
            float x = (float)i * scale.x;
            float y = mOutput[i] * scale.y + centerY;
            gl::vertex( x,  y );
        }
        
        gl::end();
        
        
    }
}


void BBPulseApp::audioCallback(uint64_t inSampleOffset, uint32_t ioSampleCount, audio::Buffer32f *buffer){
    if( mOutput.size() != ioSampleCount ){
        mOutput.resize( ioSampleCount );
    }
    mPhaseAdd += ( ( mFrequency / 44100.0f ) - mPhaseAdd ) * 0.1f;
    mModPhaseAdd += ( ( mModFrequency / 44100.0f ) - mModPhaseAdd ) * 0.1f;
    int numChannels = buffer->mNumberChannels;
    for( int i=0; i<ioSampleCount; i++ ){
        mPhase += mPhaseAdd;
        mModPhase += mModPhaseAdd;
        float output = math<float>::sin( mPhase * 2.0f * M_PI ) * math<float>::sin( mModPhase * 2.0f * M_PI );
        
        for( int j=0; j<numChannels; j++ ){
            buffer->mData[ i*numChannels + j ] = output;
        }
    }
    
    for( int i=0; i<ioSampleCount; i++ ){
        float output = buffer->mData[ i*numChannels ];
        
        int readIndex = mDelayIndex - mDelaySize + 1;
        if( readIndex < 0 ) readIndex += mDelaySize;
        
        float delay = mDelayLine[ readIndex * numChannels ];
        mDelayLine[ mDelayIndex ] = output + delay * mFeedback;
        
        if( ++mDelayIndex == mDelaySize ){
            mDelayIndex = 0;
        }
        
        output = math<float>::clamp( output + mMix * delay, -1.0f, 1.0f );
        mOutput[i] = output;
        
        for( int j=0; j<numChannels; j++ ){
            buffer->mData[ i*numChannels + j ] = output;
        }
        
    }
}


CINDER_APP_NATIVE( BBPulseApp, RendererGl )
