/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
 by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-14
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of University of the West of England, Bristol nor 
   the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF THE WEST OF ENGLAND, BRISTOL BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 This software makes use of third party libraries. For more information see:
 doc/license.txt included in the distribution.
 -------------------------------------------------------------------------------
 */

/*
 You must ensure you comply with the Juce license (either GPL or Commercial)
 to use this code, see http://juce.com
 */

#ifndef PLONK_JUCEAUDIOHOST_H
#define PLONK_JUCEAUDIOHOST_H

#include "JuceHeader.h" // assumes you're using a Jucer-created project with a JuceHeader.h file

BEGIN_PLONK_NAMESPACE

class JuceAudioHost :   public AudioHostBase<float>,
                        public juce::AudioIODeviceCallback
{
public:
    JuceAudioHost() throw();
    ~JuceAudioHost();
    
    Text getHostName() const throw();
    Text getNativeHostName() const throw();
    Text getInputName() const throw();
    Text getOutputName() const throw();
    double getCpuUsage() const throw();

    void startHost() throw();
    void stopHost() throw();
        
    void audioDeviceIOCallback (const float** inputs, int numInputs, 
                                float** outputs, int numOutputs, 
                                int blockSize) throw();
    void audioDeviceAboutToStart (juce::AudioIODevice* device) throw();
	void audioDeviceStopped() throw();
        
private:
    mutable juce::AudioDeviceManager audioDeviceManager;
};

END_PLONK_NAMESPACE

#endif  // PLONK_JUCEAUDIOHOST_H
