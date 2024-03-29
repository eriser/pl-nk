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

#include "plank_StandardHeader.h"
#include "plank_Result.h"

const char* pl_ResultString (PlankResult result)
{
    static const char* strings[] = {
        "OK",                                   //PlankResult_OK = 0,
        "Unknown error",                        //PlankResult_UnknownError,
        "Invalid result code",                  //PlankResult_ResultInvalid,
        
        "Memory error",                         //PlankResult_MemoryError,
        
        "A null pointer was passed to a function where this is invalid",   //PlankResult_NullPointerError
        "Error in a parameter to an array function",                        //PlankResult_ArrayParameterError
        
        "File mode invalid",                    //PlankResult_FileModeInvalid,
        "File open failed",                     //PlankResult_FileOpenFailed,
        "File close failed",                    //PlankResult_FileCloseFailed,
        "File already open",                    //PlankResult_FileAlreadyOpen,
        "File invalid",                         //PlankResult_FileInvalid,
        "File filepath invalid",                //PlankResult_FilePathInvalid,
        "File read error",                      //PlankResult_FileReadError,
        "File write error",                     //PlankResult_FileWriteError,
        "File end-of-file reached",             //PlankResult_FileEOF,
        "File seek failed",                     //PlankResult_FileSeekFailed,
        "File remove failed",                   //PlankResult_FileRemoveFailed
        "Failed to make a directory",           //PlankResult_FileMakeDirectoryFailed
        
        "The specified chunk ID was not found",                                         //PlankResult_IffFileReaderChunkNotFound
        
        "Invalid data while accessing an audio file",                                   //PlankResult_AudioFileInavlidType
        "A potentially valid audio file but not yet supported",                         //PlankResult_AudioFileUnsupportedType
        "The file is not ready for this operation",                                     //PlankResult_AudioFileNotReady
        "The data chunks appears to be invalid",                                        //PlankResult_AudioFileDataChunkInvalid
        "A file operation was attempted when the file was at an invalid position",      //PlankResult_AudioFileInvalidFilePosition
        "The audio file frame format changed mid-stream.",                              //PlankResult_AudioFileFrameFormatChanged
        "The source audio file changed but not the frame format.",                      //PlankResult_AudioFileChanged
        
        "Setting the Thread function failed",                                           //PlankResult_ThreadSetFunctionFailed
        "Setting the Thread user data failed",                                          //PlankResult_ThreadSetUserDataFailed
        "An attempt to start a Thread failed because it is already running",            //PlankResult_ThreadAlreadyRunning
        "An attempt to start a Thread failed for some reason",                          //PlankResult_ThreadStartFailed
        "An operation on an invalid (possibly null) thread was attempted",              //PlankResult_ThreadInvalid
        "Cancelling a Thread failed",                                                   //PlankResult_ThreadCancelFailed
        "Waiting a Thread failed",                                                      //PlankResult_ThreadWaitFailed
        "The Thread function is invalid",                                               //PlankResult_ThreadFunctionInvalid
        "A thread was signalled to exit that has already set set to exit",              //PlankResult_ThreadShouldExitAlreadySet
        "Setting the thread's priority failed",                                         //PlankResult_ThreadSetPriorityFailed
        "Setting the thread's affinity failed",                                         //PlankResult_ThreadSetAffinityFailed
        "The thread deleted itself",                                                    //PlankResult_ThreadWasDeleted
        
        "One or more callback functions were null",                                     //PlankResult_FunctionsInvalid
        
        "An index for a list, array etc was out of range",                                      //PlankResult_IndexOutOfRange
        "An item count was invalid (e.g., 0 or too small for the context)",                     //PlankResult_ItemCountInvalid
        "A container (e.g., list, queue, stack) is being de-initialised but is non-empty",      //PlankResult_ContainerNotEmptyOnDeInit

        "The maximum number of identifiers for thread-local storage has been reached",          //PlankResult_ThreadLocalStorageMaximumIdentifiersReached
        "A generic JSON error occurred",                                                        //PlankResult_JSONError
        "A JSON error occurred with a file",                                                    //PlankResult_JSONFileError
        "A generic Zip error occurred",                                                         //PlankResult_ZipError
        "An error with mismatching types for share pointers occurred",                          //PlankResult_SharedPtrTypeError,

        ""
    };
    
    if ((result < 0) || (result >= PlankNumResults))
        result = PlankResult_ResultInvalid;
    
    return strings[result];
}
