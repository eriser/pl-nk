/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-13
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

#ifndef PLANK_JSON_H
#define PLANK_JSON_H

#include "../../files/plank_File.h"

#define PLANK_JSON_TYPE         "type"
#define PLANK_JSON_VERSION      "vers"
#define PLANK_JSON_FORMAT       "fmt"
#define PLANK_JSON_FORMATBINARY "bin"
#define PLANK_JSON_FORMATZIP    "zip"
#define PLANK_JSON_FORMATTEXT   "txt"

PLANK_BEGIN_C_LINKAGE

typedef struct PlankJSON* PlankJSONRef;

static PlankJSONRef pl_JSON_Object();
static PlankJSONRef pl_JSON_Array();
static PlankJSONRef pl_JSON_String (const char* string);
static PlankJSONRef pl_JSON_Int(const int value);
static PlankJSONRef pl_JSON_Float (const float value);
static PlankJSONRef pl_JSON_Double (const double value);
static PlankJSONRef pl_JSON_Bool (const PlankB state);
static PlankJSONRef pl_JSON_Null();


PlankJSONRef pl_JSON_FromFile (PlankFileRef f);
PlankResult pl_JSON_WriteToFile (PlankJSONRef p, PlankFileRef f);

static PlankB pl_JSON_IsObject (PlankJSONRef p);
static PlankB pl_JSON_IsArray (PlankJSONRef p);
static PlankB pl_JSON_IsString (PlankJSONRef p);
static PlankB pl_JSON_IsInt(PlankJSONRef p);
static PlankB pl_JSON_IsFloat (PlankJSONRef p);
static PlankB pl_JSON_IsDouble (PlankJSONRef p);
static PlankB pl_JSON_IsBool (PlankJSONRef p);
static PlankB pl_JSON_IsNull (PlankJSONRef p);

static PlankJSONRef pl_JSON_IncrementRefCount (PlankJSONRef p);
static void pl_JSON_DecrementRefCount (PlankJSONRef p);

static PlankL pl_JSON_ObjectGetSize (PlankJSONRef p);
static PlankJSONRef pl_JSON_ObjectAtKey (PlankJSONRef p, const char* key);
static void pl_JSON_ObjectPutKey (PlankJSONRef p, const char* key, const PlankJSONRef value);

static PlankL pl_JSON_ArrayGetSize (PlankJSONRef p);
static PlankJSONRef pl_JSON_ArrayAt (PlankJSONRef p, const PlankL index);
static void pl_JSON_ArrayPut (PlankJSONRef p, const PlankL index, const PlankJSONRef value);
static void pl_JSON_ArrayAppend (PlankJSONRef p, const PlankJSONRef value);

static double pl_JSON_DoubleGet (PlankJSONRef p);
static float pl_JSON_FloatGet (PlankJSONRef p);
static int pl_JSON_IntGet (PlankJSONRef p);
static const char* pl_JSON_StringGet (PlankJSONRef p);

//

PlankJSONRef pl_JSON_VersionString (const PlankUC ex, const PlankUC major, const PlankUC minor, const PlankUC micro);
PlankUI pl_JSON_VersionCode (const PlankUC ex, const PlankUC major, const PlankUC minor, const PlankUC micro);
PlankUI pl_JSON_VersionGet (PlankJSONRef p);


void pl_JSON_ObjectSetType (PlankJSONRef p, const char* type);
void pl_JSON_ObjectSetVersionString (PlankJSONRef p, const PlankUC ex, const PlankUC major, const PlankUC minor, const PlankUC micro);
void pl_JSON_ObjectSetVersionCode (PlankJSONRef p, const PlankUC ex, const PlankUC major, const PlankUC minor, const PlankUC micro);
PlankUI pl_JSON_ObjectGetVersion (PlankJSONRef p);

void pl_JSON_ObjectSetFormatBinary (PlankJSONRef p);
void pl_JSON_ObjectSetFormatText (PlankJSONRef p);
void pl_JSON_ObjectSetFormatZip (PlankJSONRef p);

PlankB pl_JSON_IsFormatBinary (PlankJSONRef p);
PlankB pl_JSON_IsFormatIsText (PlankJSONRef p);
PlankB pl_JSON_IsFormatZip (PlankJSONRef p);

PlankB pl_JSON_IsObjectType (PlankJSONRef p, const char* type);

PlankB pl_JSON_IsObjectFormatBinary (PlankJSONRef p);
PlankB pl_JSON_IsObjectFormatText (PlankJSONRef p);
PlankB pl_JSON_IsObjectFormatZip (PlankJSONRef p);

PLANK_END_C_LINKAGE

#define PLANK_INLINING_FUNCTIONS 1
#include "plank_JSONInline.h"
#undef PLANK_INLINING_FUNCTIONS


#endif // PLANK_JSON_H
