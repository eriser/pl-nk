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

#ifndef PLONK_FILTERCOEFFS3PARAM_H
#define PLONK_FILTERCOEFFS3PARAM_H

#include "../channel/plonk_ChannelInternalCore.h"
#include "plonk_FilterForwardDeclarations.h"



/** Filter coefficient generator from two control parameters. */
template<class ShapeType>
class FilterCoeffs3ParamChannelInternal 
:   public ProxyOwnerChannelInternal<typename ShapeType::SampleDataType, 
                                     typename ShapeType::Data>
{
public:
    typedef typename ShapeType::SampleDataType                  SampleType;
    typedef typename ShapeType::Data                            Data;
    typedef typename ShapeType::FormType                        FormType;
    
    typedef ChannelBase<SampleType>                             ChannelType;
    typedef ObjectArray<ChannelType>                            ChannelArrayType;
        
    typedef ProxyOwnerChannelInternal<SampleType,Data>          Internal;
    typedef UnitBase<SampleType>                                UnitType;
    typedef InputDictionary                                     Inputs;
    typedef NumericalArray<SampleType>                          Buffer;
        
    FilterCoeffs3ParamChannelInternal (Inputs const& inputs, 
                                       Data const& data, 
                                       BlockSize const& blockSize,
                                       SampleRate const& sampleRate,
                                       ChannelArrayType& channels) throw()
    :   Internal (FormType::NumCoeffs, inputs, data, blockSize, sampleRate, channels),
        inputKeys (ShapeType::getInputKeys())
    {
        plonk_assert (ShapeType::NumParams == 3);

        SampleRate& filterSampleRate = this->getInputAsSampleRate (IOKey::FilterSampleRate);
        filterSampleRate.addReceiver (this);
        
        updateFilterSampleRateInData();
    }
    
    ~FilterCoeffs3ParamChannelInternal()
    {
        SampleRate& filterSampleRate = this->getInputAsSampleRate (IOKey::FilterSampleRate);
        filterSampleRate.removeReceiver (this);
    }
            
    Text getName() const throw()
    {
        return "Filter Coeffs 3 Param (" + ShapeType::getFormName() + " / " + ShapeType::getShapeName() + ")";
    }       
    
    IntArray getInputKeys() const throw()
    {
        return inputKeys;
    }    
        
    void initChannel (const int channel) throw()
    {        
        plonk_assert (ShapeType::NumParams == 3);

        UnitType& param0Unit = this->getInputAsUnit (inputKeys.atUnchecked (0));
        UnitType& param1Unit = this->getInputAsUnit (inputKeys.atUnchecked (1));
        UnitType& param2Unit = this->getInputAsUnit (inputKeys.atUnchecked (2));
        
        plonk_assert (param0Unit.getOverlap (0) == Math<DoubleVariable>::get1());
        plonk_assert (param1Unit.getOverlap (0) == Math<DoubleVariable>::get1());
        plonk_assert (param2Unit.getOverlap (0) == Math<DoubleVariable>::get1());

        if ((channel % this->getNumChannels()) == 0)
        {
            this->setBlockSize (BlockSize::decide (param0Unit.getBlockSize (0),
                                                   this->getBlockSize()));
            this->setSampleRate (SampleRate::decide (param0Unit.getSampleRate (0),
                                                     this->getSampleRate()));
            
            Data& data = this->getState();
            data.params[0] = param0Unit.getValue (0);
            data.params[1] = param1Unit.getValue (0);
            data.params[2] = param2Unit.getValue (0);
            
            ShapeType::calculate (data);
            
            for (int i = 0; i < FormType::NumCoeffs; ++i)
                this->initProxyValue (i, data.coeffs[i]);            
        }
    }    
    
    void changed (DoubleVariable::Sender const& source, Text const& message, Dynamic const& payload) throw()
    {        
        SampleRate sampleRateSource = static_cast<SampleRate> (source);
        
        if (sampleRateSource == this->getInputAsSampleRate (IOKey::FilterSampleRate))
        {
            this->updateFilterSampleRateInData();
            return;
        }
                
        Internal::changed (source, message, payload); // process others
    }    
    
    void updateFilterSampleRateInData() throw()
    {
        const SampleRate& filterSampleRate = this->getInputAsSampleRate (IOKey::FilterSampleRate);
        Data& data = this->getState();

        data.filterSampleRate = filterSampleRate.getValue();
        data.filterSampleDuration = 1.0 / data.filterSampleRate;
    }
    
    void process (ProcessInfo& info, const int /*channel*/) throw()
    {   
        int i, j;

        Data& data = this->getState();
                            
        const int outputBufferLength = this->getOutputBuffer (0).length();
            
        UnitType& param0Unit = this->getInputAsUnit (inputKeys.atUnchecked (0));
        UnitType& param1Unit = this->getInputAsUnit (inputKeys.atUnchecked (1));
        UnitType& param2Unit = this->getInputAsUnit (inputKeys.atUnchecked (2));
        
        const Buffer& param0Buffer (param0Unit.process (info, 0));
        const Buffer& param1Buffer (param1Unit.process (info, 0));
        const Buffer& param2Buffer (param2Unit.process (info, 0));
        const SampleType* const param0Samples = param0Buffer.getArray();
        const SampleType* const param1Samples = param1Buffer.getArray();
        const SampleType* const param2Samples = param2Buffer.getArray();
        
        const int param0BufferLength = param0Buffer.length();
        const int param1BufferLength = param1Buffer.length();
        const int param2BufferLength = param2Buffer.length();
        
        if (outputBufferLength == param0BufferLength)
        {
            if ((outputBufferLength == param1BufferLength) &&
                (outputBufferLength == param2BufferLength))
            {
                // NNN
                for (i = 0; i < outputBufferLength; ++i)
                {
                    data.params[0] = param0Samples[i];
                    data.params[1] = param1Samples[i];
                    data.params[2] = param2Samples[i];
                    
                    ShapeType::calculate (data);
                    
                    for (j = 0; j < FormType::NumCoeffs; ++j)
                        this->getOutputSamples (j) [i] = data.coeffs[j];
                }            
            }
            else if (param1BufferLength == 1)
            {
                if (outputBufferLength == param2BufferLength)
                {   
                    // N1N
                    data.params[1] = param1Samples[0];

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        data.params[0] = param0Samples[i];
                        data.params[2] = param2Samples[i];
                        
                        ShapeType::calculate (data);
                        
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                else if (param2BufferLength == 1)
                {
                    // N11
                    data.params[1] = param1Samples[0];
                    data.params[2] = param2Samples[0];

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        data.params[0] = param0Samples[i];
                        
                        ShapeType::calculate (data);
                        
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                else goto fallback; // Nnn
            }
            else if (param2BufferLength == 1)
            {
                if (outputBufferLength == param1BufferLength)
                {
                    // NN1
                    data.params[2] = param2Samples[0];

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        data.params[0] = param0Samples[i];
                        data.params[1] = param1Samples[i];
                        
                        ShapeType::calculate (data);
                        
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                else goto fallback; // Nn1
            }
            else goto fallback; //?
        }
        else if (param0BufferLength == 1)
        {
            if ((outputBufferLength == param1BufferLength) &&
                (outputBufferLength == param2BufferLength))
            {
                // 1NN
                data.params[0] = param0Samples[0];

                for (i = 0; i < outputBufferLength; ++i)
                {
                    data.params[1] = param1Samples[i];
                    data.params[2] = param2Samples[i];
                    
                    ShapeType::calculate (data);
                    
                    for (j = 0; j < FormType::NumCoeffs; ++j)
                        this->getOutputSamples (j) [i] = data.coeffs[j];
                }            
            }
            else if (param1BufferLength == 1)
            {
                if (param2BufferLength == outputBufferLength)
                {
                    // 11N
                    data.params[0] = param0Samples[0];
                    data.params[1] = param1Samples[0];

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        data.params[2] = param2Samples[i];
                        
                        ShapeType::calculate (data);
                        
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                else if (param2BufferLength == 1)
                {
                    // 111
                    data.params[0] = param0Samples[0];
                    data.params[1] = param1Samples[0];
                    data.params[2] = param2Samples[0];
                    
                    ShapeType::calculate (data);

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                else goto fallback; // 11n
            }
            else if (param2BufferLength == 1)
            {
                if (param1BufferLength == outputBufferLength)
                {
                    // 1N1
                    data.params[0] = param0Samples[0];
                    data.params[2] = param2Samples[0];

                    for (i = 0; i < outputBufferLength; ++i)
                    {
                        data.params[1] = param1Samples[i];
                        
                        ShapeType::calculate (data);
                        
                        for (j = 0; j < FormType::NumCoeffs; ++j)
                            this->getOutputSamples (j) [i] = data.coeffs[j];
                    }            
                }
                goto fallback; // 1n1
            }
            else goto fallback; // 1nn
        }
        else goto fallback; // nnn
        
        return;
        
    fallback:
        {
            double param0Position = 0.0;
            const double param0Increment = double (param0BufferLength) / double (outputBufferLength);
            double param1Position = 0.0;
            const double param1Increment = double (param1BufferLength) / double (outputBufferLength);
            double param2Position = 0.0;
            const double param2Increment = double (param2BufferLength) / double (outputBufferLength);
            
            for (i = 0; i < outputBufferLength; ++i)
            {
                data.params[0] = param0Samples[int (param0Position)];
                data.params[1] = param1Samples[int (param1Position)];
                data.params[2] = param2Samples[int (param2Position)];
                
                ShapeType::calculate (data);
                
                for (j = 0; j < FormType::NumCoeffs; ++j)
                    this->getOutputSamples (j) [i] = data.coeffs[j];
                
                param0Position += param0Increment;
                param1Position += param1Increment;
                param2Position += param2Increment;
            }        
        }
    }
    
private:
    IntArray inputKeys;
};





/** Filter coefficients generator for an IIR filter using three control parameters. 
 @ingroup FilterUnits ControlUnits */
template<class ShapeType>
class FilterCoeffs3ParamUnit
{
public:        
    typedef typename ShapeType::SampleDataType              SampleType;
    typedef typename ShapeType::Data                        Data;
    typedef typename ShapeType::FormType                    FormType;
        
    typedef FilterCoeffs3ParamChannelInternal<ShapeType>    FilterCoeffsInternal;
    typedef ChannelBase<SampleType>                         ChannelType;
    typedef ChannelInternal<SampleType,Data>                Internal;
    typedef UnitBase<SampleType>                            UnitType;
    typedef InputDictionary                                 Inputs;
    
    static PLONK_INLINE_LOW UnitInfos getInfo() throw()
    {
        const double sampleRate = SampleRate::getDefault().getValue();
        
        return UnitInfo ("Filter Coeffs 3 Param", "Filter coefficients generator for an IIR filter using three control parameters.",
                         
                         // output
                         FormType::NumCoeffs, 
                         IOKey::Coeffs,             Measure::Coeffs,    0.0,                IOLimit::None,
                         IOKey::End,
                         
                         // inputs
                         IOKey::Generic,            Measure::Unknown,   IOInfo::NoDefault,  IOLimit::None,
                         IOKey::Generic,            Measure::Unknown,   IOInfo::NoDefault,  IOLimit::None,
                         IOKey::Generic,            Measure::Unknown,   IOInfo::NoDefault,  IOLimit::None,
                         IOKey::FilterSampleRate,   Measure::Hertz,     sampleRate,         IOLimit::Minimum,   Measure::Hertz,             0.0,
                         IOKey::BlockSize,          Measure::Samples,   0.0,                IOLimit::Minimum,   Measure::Samples,           1.0,
                         IOKey::SampleRate,         Measure::Hertz,     -1.0,               IOLimit::Minimum,   Measure::Hertz,             0.0,
                         IOKey::End);
    }
    
    
    /** Filter coefficients from three control parameters. 
     This will generally be for some of the second order filters (e.g., peak notch or the shelving filters). */
    static UnitType ar (UnitType const& param0,
                        UnitType const& param1,
                        UnitType const& param2,
                        SampleRates const filterSampleRates = SampleRate::getDefault(),
                        BlockSize const& preferredBlockSize = BlockSize::noPreference(),
                        SampleRate const& preferredSampleRate = SampleRate::noPreference()) throw()
    {                
        const IntArray inputKeys = ShapeType::getInputKeys();
        
        plonk_assert (inputKeys.length() == 3);
        
        const int numInputChannels = plonk::max (param0.getNumChannels(), param1.getNumChannels(), param2.getNumChannels());
        const int numSampleRates = filterSampleRates.length();
        const int numChannels = filterSampleRates.areAllEqual() ? numInputChannels : plonk::max (numInputChannels, numSampleRates);
        UnitType result (UnitType::emptyWithAllocatedSize (numChannels * FormType::NumCoeffs));
        
        Data data;
        Memory::zero (data);
        data.base.sampleRate = -1.0;
        data.base.sampleDuration = -1.0;

        for (int i = 0; i < numChannels; ++i)
        {
            Inputs inputs;
            inputs.put (inputKeys.atUnchecked (0), param0[i]);
            inputs.put (inputKeys.atUnchecked (1), param1[i]);
            inputs.put (inputKeys.atUnchecked (2), param2[i]);
            inputs.put (IOKey::FilterSampleRate, filterSampleRates.wrapAt (i));
            
            result.add (UnitType::template proxiesFromInputs<FilterCoeffsInternal> (inputs, 
                                                                                    data, 
                                                                                    preferredBlockSize, 
                                                                                    preferredSampleRate));
        }
        
        return result;
    }
};




#endif // PLONK_FILTERCOEFFS3PARAM_H

