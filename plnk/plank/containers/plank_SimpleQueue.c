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

#include "../core/plank_StandardHeader.h"
#include "plank_SimpleQueue.h"

PlankSimpleQueueRef pl_SimpleQueue_CreateAndInit()
{
    PlankSimpleQueueRef p;
    p = pl_SimpleQueue_Create();
    
    if (p != PLANK_NULL)
    {
        if (pl_SimpleQueue_Init (p) != PlankResult_OK)
            pl_SimpleQueue_Destroy (p);
        else
            return p;
    }
    
    return PLANK_NULL;
}

PlankSimpleQueueRef pl_SimpleQueue_Create()
{
    PlankMemoryRef m;
    PlankSimpleQueueRef p;
    
    m = pl_MemoryGlobal();
    p = (PlankSimpleQueueRef)pl_Memory_AllocateBytes (m, sizeof (PlankSimpleQueue));
    
    if (p != PLANK_NULL)
        pl_MemoryZero (p, sizeof (PlankSimpleQueue));
    
    return p;
}

PlankResult pl_SimpleQueue_Init (PlankSimpleQueueRef p)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    pl_MemoryZero (p, sizeof (PlankSimpleQueue));
                
exit:
    return result;
}

PlankResult pl_SimpleQueue_DeInit (PlankSimpleQueueRef p)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
        
    if (pl_SimpleQueue_GetSize (p) != 0)
        result = PlankResult_ContainerNotEmptyOnDeInit;
    
    pl_MemoryZero (p, sizeof (PlankSimpleQueue));

exit:
    return result;    
}

PlankResult pl_SimpleQueue_Destroy (PlankSimpleQueueRef p)
{
    PlankResult result;
    PlankMemoryRef m;
    
    result = PlankResult_OK;
    m = pl_MemoryGlobal();
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    if ((result = pl_SimpleQueue_DeInit (p)) != PlankResult_OK)
        goto exit;
    
    result = pl_Memory_Free (m, p);
    
exit:
    return result;    
}


PlankResult pl_SimpleQueue_Clear (PlankSimpleQueueRef p)
{
    PlankResult result = PlankResult_OK;
    PlankSimpleQueueElementRef element;
    PlankP data;
    
    if ((result = pl_SimpleQueue_Pop (p, &element)) != PlankResult_OK)
        goto exit;
    
    if (p->freeFunction != PLANK_NULL)
    {
        while (element != PLANK_NULL) 
        {
            data = pl_SimpleQueueElement_GetData (element);
            
            if (data != PLANK_NULL)
            {
                if ((result = (p->freeFunction) (data)) != PlankResult_OK)
                    goto exit;
            }
            
            if ((result = pl_SimpleQueueElement_Destroy (element)) != PlankResult_OK)
                goto exit;
            
            if ((result = pl_SimpleQueue_Pop (p, &element)) != PlankResult_OK)
                goto exit;
        }
    }
    else
    {
        while (element != PLANK_NULL) 
        {
            if ((result = pl_SimpleQueueElement_Destroy (element)) != PlankResult_OK)
                goto exit;
            
            if ((result = pl_SimpleQueue_Pop (p, &element)) != PlankResult_OK)
                goto exit;
        }
    }
    
exit:
    return result;    
}

PlankResult pl_SimpleQueue_SetFreeElementDataFunction (PlankSimpleQueueRef p, 
                                                       PlankSimpleQueueFreeElementDataFunction freeFunction)
{
    PlankResult result = PlankResult_OK;
    p->freeFunction = freeFunction;
    return result;
}

PlankResult pl_SimpleQueue_Push (PlankSimpleQueueRef p, const PlankSimpleQueueElementRef element)
{
    PlankResult result = PlankResult_OK;
    
    pl_SimpleQueueElement_SetNext (element, PLANK_NULL);

    if (p->tail == PLANK_NULL)
        p->head = element;
    else
        pl_SimpleQueueElement_SetNext (p->tail, element);
    
    p->tail = element;
    p->count++;
    
    return result;
}

PlankResult pl_SimpleQueue_Pop (PlankSimpleQueueRef p, PlankSimpleQueueElementRef* element)
{
    PlankResult result = PlankResult_OK;
    PlankSimpleQueueElementRef headElement, nextElement;
    
    headElement = p->head;
    
    if (headElement == PLANK_NULL)
    {
        *element = PLANK_NULL;
        goto exit;
    }
    
    nextElement = pl_SimpleQueueElement_GetNext (headElement);
    
    if (headElement == p->tail)
        p->tail = nextElement;
    else
        p->head = nextElement;
    
    p->count--;
    
    if (p->count == 0)
    {
        p->head = PLANK_NULL;
        p->tail = PLANK_NULL;
    }
    
    *element = headElement;
    
exit:
    return result;    
}

PlankLL pl_SimpleQueue_GetSize (PlankSimpleQueueRef p)
{
    return p->count;
}

