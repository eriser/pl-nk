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

#ifndef PLANK_AUDIOFILEREADER_H
#define PLANK_AUDIOFILEREADER_H

#include "plank_AudioFileCommon.h"

PLANK_BEGIN_C_LINKAGE

/** An audio file reader.
  
 @defgroup PlankAudioFileReaderClass Plank AudioFileReader class
 @ingroup PlankClasses
 @{
 */

/** An opaque reference to the <i>Plank AudioFileReader</i> object. */
typedef struct PlankAudioFileReader* PlankAudioFileReaderRef; 

/** Create and initialise a <i>Plank AudioFileReader</i> object and return an oqaque reference to it.
 @return A <i>Plank AudioFileReader</i> object as an opaque reference or PLANK_NULL. */
PlankAudioFileReaderRef pl_AudioFileReader_CreateAndInit();

/** Create a <i>Plank AudioFileReader</i> object and return an oqaque reference to it.
 @return A <i>Plank AudioFileReader</i> object as an opaque reference or PLANK_NULL. */
PlankAudioFileReaderRef pl_AudioFileReader_Create();

/** Initialise a <i>Plank AudioFileReader</i> object. 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_Init (PlankAudioFileReaderRef p);

/** Deinitialise a <i>Plank AudioFileReader</i> object. 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_DeInit (PlankAudioFileReaderRef p);

/** Destroy a <i>Plank AudioFileReader</i> object. 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_Destroy (PlankAudioFileReaderRef p);

/** Gets the underlying <i>Plank %File</i> object. 
 This is the raw file object the is performing the fundamental file access operations.
 @param p The <i>Plank AudioFileReader</i> object.
 @return The <i>Plank %File</i> object. */
PlankFileRef pl_AudioFileReader_GetFile (PlankAudioFileReaderRef p);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_Open (PlankAudioFileReaderRef p, const char* filepath);

PlankResult pl_AudioFileReader_OpenWithMetaData (PlankAudioFileReaderRef p, const char* filepath);

PlankResult pl_AudioFileReader_OpenInternal (PlankAudioFileReaderRef p, const char* filepath, const PlankAudioFileMetaDataIOFlags metaDataIOFlags);

/** Open from a file object.
 The AudioFileReader takes ownership of the file and zeros the incomming file object. */
PlankResult pl_AudioFileReader_OpenWithFile (PlankAudioFileReaderRef p, PlankFileRef file, const PlankAudioFileMetaDataIOFlags metaDataIOFlags);

PlankResult pl_AudioFileReader_OpenWithAudioFileArray (PlankAudioFileReaderRef p, PlankDynamicArrayRef array, PlankB ownArray, const int multiMode, int* indexRef);

typedef PlankResult (*PlankAudioFileReaderCustomNextFunction)(PlankP, PlankAudioFileReaderRef, PlankAudioFileReaderRef*);
typedef PlankResult (*PlankAudioFileReaderCustomFreeFunction)(PlankP);
typedef PlankResult (*PlankAudioFileReaderCustomSetFrameFunction)(PlankAudioFileReaderRef, const PlankLL frameIndex);
typedef PlankResult (*PlankAudioFileReaderCustomGetFrameFunction)(PlankAudioFileReaderRef, PlankLL *);


PlankResult pl_AudioFileReader_OpenWithCustomNextFunction (PlankAudioFileReaderRef p,
                                                           PlankAudioFileReaderCustomNextFunction nextFunction,
                                                           PlankAudioFileReaderCustomFreeFunction freeFunction,
                                                           PlankAudioFileReaderCustomSetFrameFunction setFrameFunction,
                                                           PlankAudioFileReaderCustomGetFrameFunction getFrameFunction,
                                                           PlankP ref);

PlankResult pl_AudioFileReader_OpenWithRegion (PlankAudioFileReaderRef p, PlankAudioFileReaderRef original, PlankAudioFileRegionRef region);

//PlankResult pl_AudioFileReader_OpenCopy (PlankAudioFileReaderRef p, PlankAudioFileReaderRef original);
//PlankResult pl_AudioFileReader_OpenCopyWithoutMetaData (PlankAudioFileReaderRef p, PlankAudioFileReaderRef original);


/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_Close (PlankAudioFileReaderRef p);


const PlankAudioFileFormatInfo* pl_AudioFileReader_GetFormatInfoReadOnly (PlankAudioFileReaderRef p);


/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetFormat (PlankAudioFileReaderRef p, int *format);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetEncoding (PlankAudioFileReaderRef p, int *encoding);

/** 
 The result of this will be invalid if the file is a compressed format.
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetBitsPerSample (PlankAudioFileReaderRef p, int *bitsPerSample);

/** 
 The result of this will be invalid if the file is a compressed format.
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetBytesPerFrame (PlankAudioFileReaderRef p, int *bytesPerFrame);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetNumChannels (PlankAudioFileReaderRef p, int *numChannels);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetSampleRate (PlankAudioFileReaderRef p, double *sampleRate);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetNumFrames (PlankAudioFileReaderRef p, PlankLL *numFrames);

PlankB pl_AudioFileReader_IsPositionable (PlankAudioFileReaderRef p);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_SetFramePosition (PlankAudioFileReaderRef p, const PlankLL frameIndex);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_ResetFramePosition (PlankAudioFileReaderRef p);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_GetFramePosition (PlankAudioFileReaderRef p, PlankLL *frameIndex);

/** 
 @param p The <i>Plank AudioFileReader</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_AudioFileReader_ReadFrames (PlankAudioFileReaderRef p, const PlankB convertByteOrder, const int numFrames, void* data, int* framesRead);

PlankAudioFileMetaDataRef pl_AudioFileReader_GetMetaData (PlankAudioFileReaderRef p);

PlankResult pl_AudioFileReader_SetName (PlankAudioFileReaderRef p, const char* text);
const char* pl_AudioFileReader_GetName (PlankAudioFileReaderRef p);

PlankResult pl_AudioFileReader_GetChannelItentifier (PlankAudioFileReaderRef p, const int channel, PlankChannelIdentifier* identifier);
PlankResult pl_AudioFileReader_GetChannelLayout (PlankAudioFileReaderRef p, PlankChannelLayout* layout);


/** @} */

PLANK_END_C_LINKAGE

typedef struct PlankAudioFileReaderCustom* PlankAudioFileReaderCustomRef;

#if !DOXYGEN
typedef struct PlankAudioFileReader
{
    PlankP peer;
    PlankAudioFileFormatInfo formatInfo;
    PlankC format;

    PlankLL numFrames;
    PlankLL dataPosition;
    
    PlankAudioFileMetaDataIOFlags metaDataIOFlags;
    PlankAudioFileMetaDataRef metaData;
    PlankDynamicArray name;
    
    PlankM readFramesFunction;
    PlankM setFramePositionFunction;
    PlankM getFramePositionFunction;
} PlankAudioFileReader;

typedef struct PlankAudioFileReaderCustom
{
    PlankAudioFileReaderCustomNextFunction nextFunction;
    PlankAudioFileReaderCustomFreeFunction freeFunction;
    PlankP ref;
    PlankAudioFileReaderRef currentAudioFile;
} PlankAudioFileReaderCustom;
#endif

#endif // PLANK_AUDIOFILEREADER_H
