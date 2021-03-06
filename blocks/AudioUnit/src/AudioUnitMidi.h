/*
 This is part of a block for Audio Unit integration in Cinder (http://libcinder.org)
 Copyright (c) 2013, Adam Carlucci. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "AudioUnitTypes.h"
#include <CoreMIDI/CoreMIDI.h>
#include <string>
#include <vector>

namespace cinder { namespace audiounit {

struct Midi
{
	static unsigned long getNumberOfSources(){return MIDIGetNumberOfSources();}
	static std::vector<std::string> getSourceNames();
	static void printSourceNames();
};

class MidiReceiver
{
	MIDIClientRef   _client;
	MIDIEndpointRef _endpoint;
	MIDIPortRef     _port;
	AudioUnitRef    _unit;
	
public:
	MidiReceiver(const std::string &clientName = "Cinder");
	~MidiReceiver();
	
	bool createMidiDestination(const std::string &portName);
	bool connectToMidiSource(unsigned long midiSourceIndex);
	bool connectToMidiSource(const std::string &midiSourceName);
	void disconnectFromMidiSource(unsigned long midiSourceIndex);
	void routeMidiTo(GenericUnit &unitToRouteTo);
};

} } // namespace cinder::audiounit
