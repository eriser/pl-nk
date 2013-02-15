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

#include "plank_StandardHeader.h"
#include "plank_NeuralNode.h"
#include "../../maths/vectors/plank_Vectors.h"
#include "../../random/plank_RNG.h"


static const float NeuralFE1 = 2.7182818284590452354f;



PlankResult pl_NeuralNodeF_Init (PlankNeuralNodeFRef p)
{
    return pl_NeuralNodeF_InitWithNumWeightsAndRange (p, 1, 0.1f);
}

PlankResult pl_NeuralNodeF_InitWithNumWeights (PlankNeuralNodeFRef p, const int numWeights)
{
    return pl_NeuralNodeF_InitWithNumWeightsAndRange (p, numWeights, 0.1f);
}

PlankResult pl_NeuralNodeF_InitWithNumWeightsAndRange (PlankNeuralNodeFRef p, const int numWeights, const float range)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    pl_MemoryZero (p, sizeof (PlankNeuralNodeF));
    
    if ((result = pl_DynamicArray_InitWithItemSizeAndSize (&p->weightVector, sizeof (PlankF), pl_MaxI (1, numWeights), PLANK_TRUE)) != PlankResult_OK)
        goto exit;
        
    if ((result = pl_NeuralNodeF_Randomise (p, range)) != PlankResult_OK)
        goto exit;
    
exit:
    return result;
}

PlankResult pl_NeuralNodeF_DeInit (PlankNeuralNodeFRef p)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    result = pl_DynamicArray_DeInit (&p->weightVector);
    pl_MemoryZero (p, sizeof (PlankNeuralNodeF));
    
exit:
    return result;
}

int pl_NeuralNodeF_GetNumWeights (PlankNeuralNodeFRef p)
{
    return pl_DynamicArray_GetSize (&p->weightVector);
}

PlankResult pl_NeuralNodeF_Set (PlankNeuralNodeFRef p, const float* weights, const float threshold)
{
    PlankResult result;
    result = PlankResult_OK;
    
    if ((result = pl_NeuralNodeF_SetWeights (p, weights)) != PlankResult_OK) goto exit;
    if ((result = pl_NeuralNodeF_SetThreshold (p, threshold)) != PlankResult_OK) goto exit;

exit:
    return result;    
}

PlankResult pl_NeuralNodeF_SetThreshold (PlankNeuralNodeFRef p, const float threshold)
{
    p->threshold = threshold;
    return PlankResult_OK;
}

PlankResult pl_NeuralNodeF_SetWeights (PlankNeuralNodeFRef p, const float* weights)
{
    PlankResult result;
    result = PlankResult_OK;

    pl_VectorMoveF_NN ((PlankF*)pl_DynamicArray_GetArray (&p->weightVector),
                       weights,
                       pl_DynamicArray_GetSize (&p->weightVector));
    
    return result;
}

PlankResult pl_NeuralNodeF_SetWeight (PlankNeuralNodeFRef p, const int index, const float weight)
{
    PlankResult result;
    float* weightVectorPtr;
    int size;
    
    result = PlankResult_OK;
    size = pl_DynamicArray_GetSize (&p->weightVector);
    
    if ((index < 0) || (index >= size))
    {
        result = PlankResult_IndexOutOfRange;
        goto exit;
    }
    
    weightVectorPtr = (float*)pl_DynamicArray_GetArray (&p->weightVector);
    weightVectorPtr[index] = weight;
  
exit:
    return result;
}

PlankResult pl_NeuralNodeF_Get (PlankNeuralNodeFRef p, float* weights, float* threshold)
{
    PlankResult result;
    result = PlankResult_OK;
    
    if ((result = pl_NeuralNodeF_GetWeights (p, weights)) != PlankResult_OK)
        goto exit;

    *threshold = pl_NeuralNodeF_GetThreshold (p);
    
exit:
    return result;
}

float pl_NeuralNodeF_GetThreshold (PlankNeuralNodeFRef p)
{
    return p->threshold;
}

PlankResult pl_NeuralNodeF_GetWeights (PlankNeuralNodeFRef p, float* weights)
{
    PlankResult result;
    result = PlankResult_OK;
    
    pl_VectorMoveF_NN (weights,
                       (const float*)pl_DynamicArray_GetArray (&p->weightVector),
                       pl_DynamicArray_GetSize (&p->weightVector));
    
    return result;
}

const float* pl_NeuralNodeF_GetWeightsPtr (PlankNeuralNodeFRef p)
{
    return (const float*)pl_DynamicArray_GetArray (&p->weightVector);
}

PlankResult pl_NeuralNodeF_Reset (PlankNeuralNodeFRef p, const float amount)
{
    PlankRNGRef r;
    float amount2;
    float* weightVectorPtr;
    int size, i;
    
    weightVectorPtr = (float*)pl_DynamicArray_GetArray (&p->weightVector);
    size = pl_DynamicArray_GetSize (&p->weightVector);
    r = pl_RNGGlobal();
    
    amount2 = amount * 2.f;
    
	for (i = 0; i < size; ++i)
		weightVectorPtr[i] = pl_RNG_NextFloat (r) * amount2 - amount;
    
	return PlankResult_OK;
}

PlankResult pl_NeuralNodeF_Randomise (PlankNeuralNodeFRef p, const float amount)
{
    PlankRNGRef r;
    float amount2;
    float* weightVectorPtr;
    int size, i;
    
    weightVectorPtr = (float*)pl_DynamicArray_GetArray (&p->weightVector);
    size = pl_DynamicArray_GetSize (&p->weightVector);
    r = pl_RNGGlobal();
    
    amount2 = amount * 2.f;
    
	for (i = 0; i < size; ++i)
		weightVectorPtr[i] += pl_RNG_NextFloat (r) * amount2 - amount;
    
	return PlankResult_OK;
}

float pl_NeuralNodeF_Propogate (PlankNeuralNodeFRef p, const float* inputVector)
{
	float input;
    const float* weightVectorPtr;
    int size, i;
    
    input = 0.f;
    weightVectorPtr = (const float*)pl_DynamicArray_GetArray (&p->weightVector);
    size = pl_DynamicArray_GetSize (&p->weightVector);
    
    // can vectorise this..
    
	for (i = 0; i < size; ++i)
		input += inputVector[i] * weightVectorPtr[i];

	p->output = 1.f / (1.f + pl_PowF (NeuralFE1, -(input + p->threshold)));
	
	return p->output;
}

PlankResult pl_NeuralNodeF_BackProp (PlankNeuralNodeFRef p, const float* inputVector, const float error, const float actFuncOffset, const float learnRate, float* adjustVector)
{
	float output, adjust, learn;
	float* weightVectorPtr;
    int size, i;
    
    output = p->output;
    adjust = error * (actFuncOffset + (output * (1.f - output)));
    learn = adjust * learnRate;

	weightVectorPtr = (float*)pl_DynamicArray_GetArray (&p->weightVector);

	size = pl_DynamicArray_GetSize (&p->weightVector);
    
    // can vectorise this..
    
	for (i = 0; i < size; ++i)
	{
		weightVectorPtr[i] += inputVector[i] * learn;
		adjustVector[i] += weightVectorPtr[i] * adjust;
	}
	
	p->threshold += learn;
    
    return PlankResult_OK;
}

float pl_NeuralNodeF_GetOutput (PlankNeuralNodeFRef p)
{
    return p->output;
}
