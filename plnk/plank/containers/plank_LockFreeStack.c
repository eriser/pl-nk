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
#include "plank_LockFreeStack.h"

PlankLockFreeStackRef pl_LockFreeStack_CreateAndInit()
{
    PlankLockFreeStackRef p;
    p = pl_LockFreeStack_Create();
    
    if (p != PLANK_NULL)
    {
        if (pl_LockFreeStack_Init (p) != PlankResult_OK)
            pl_LockFreeStack_Destroy (p);
        else
            return p;
    }
    
    return PLANK_NULL;
}

PlankLockFreeStackRef pl_LockFreeStack_Create()
{
    PlankMemoryRef m;
    PlankLockFreeStackRef p;
    
    m = pl_MemoryGlobal(); // OK, creation of the Stack isn't itself lock free
    p = (PlankLockFreeStackRef)pl_Memory_AllocateBytes (m, sizeof (PlankLockFreeStack));
    
    if (p != PLANK_NULL)
        pl_MemoryZero (p, sizeof (PlankLockFreeStack));
    
    return p;
}

PlankResult pl_LockFreeStack_Init (PlankLockFreeStackRef p)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    pl_MemoryZero (p, sizeof (PlankLockFreeStack));
    
    result = pl_AtomicPX_Init (&p->atom);
        
exit:
    return result;
}

PlankResult pl_LockFreeStack_DeInit (PlankLockFreeStackRef p)
{
    PlankResult result = PlankResult_OK;
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    if (pl_LockFreeStack_GetSize (p) != 0)
    {
        result = PlankResult_ContainerNotEmptyOnDeInit;
        goto exit;
    }
    
    result = pl_AtomicPX_DeInit (&p->atom);
    pl_MemoryZero (p, sizeof (PlankLockFreeStack));

exit:
    return result;    
}

PlankResult pl_LockFreeStack_Destroy (PlankLockFreeStackRef p)
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
    
    if ((result = pl_LockFreeStack_DeInit (p)) != PlankResult_OK)
        goto exit;
    
    result = pl_Memory_Free (m, p);
    
exit:
    return result;    
}

PlankResult pl_LockFreeStack_Clear (PlankLockFreeStackRef p)
{
    PlankResult result;    
    PlankLockFreeStackElementRef element;
    PlankP data;
    
    result = PlankResult_OK;
    
    if ((result = pl_LockFreeStack_Pop (p, &element)) != PlankResult_OK)
        goto exit;
    
    if (p->freeFunction != PLANK_NULL)
    {
        while (element != PLANK_NULL) 
        {
            data = pl_LockFreeStackElement_GetData (element);
            
            if (data != PLANK_NULL)
            {
                if ((result = (p->freeFunction) (data)) != PlankResult_OK)
                    goto exit;
            }
            
            if ((result = pl_LockFreeStackElement_Destroy (element)) != PlankResult_OK)
                goto exit;
            
            if ((result = pl_LockFreeStack_Pop (p, &element)) != PlankResult_OK)
                goto exit;
        }
    }
    else
    {
        while (element != PLANK_NULL) 
        {
            if ((result = pl_LockFreeStackElement_Destroy (element)) != PlankResult_OK)
                goto exit;
            
            if ((result = pl_LockFreeStack_Pop (p, &element)) != PlankResult_OK)
                goto exit;
        }
    }
    
exit:
    return result;    
}

PlankResult pl_LockFreeStack_SetFreeElementDataFunction (PlankLockFreeStackRef p, PlankLockFreeStackFreeElementDataFunction freeFunction)
{
    PlankResult result = PlankResult_OK;    
    p->freeFunction = freeFunction;
    return result;
}

PlankResult pl_LockFreeStack_Push (PlankLockFreeStackRef p, const PlankLockFreeStackElementRef element)
{
    PlankResult result;
    PlankP oldPtr, newPtr;
    PlankUL oldExtra, newExtra;
    PlankB success;
        
    result = PlankResult_OK;
    
	do {
		oldPtr = pl_AtomicPX_GetUnchecked (&p->atom);
        oldExtra = pl_AtomicPX_GetExtraUnchecked (&p->atom);
		
        pl_LockFreeStackElement_SetNext (element, oldPtr);
		
        newPtr = element;
        newExtra = oldExtra + 1;
        
        success = pl_AtomicPX_CompareAndSwap ((PlankAtomicPXRef)&(p->atom), oldPtr, oldExtra, newPtr, newExtra);
	} while (!success);
    
    pl_AtomicLL_Increment (&p->count);
    
    return result;
}

PlankResult pl_LockFreeStack_Pop (PlankLockFreeStackRef p, PlankLockFreeStackElementRef* element)
{
    PlankResult result;
    PlankP headPtr, nextPtr;
    PlankL headExtra, nextExtra;
    PlankB success;

    result = PlankResult_OK;
    
	do {
        headPtr = pl_AtomicPX_GetUnchecked (&p->atom);
        
		if (headPtr == PLANK_NULL)
        {
			*element = PLANK_NULL;
            goto exit;
        }
        
        headExtra = pl_AtomicPX_GetExtraUnchecked (&p->atom);

        nextPtr = pl_LockFreeStackElement_GetNext (headPtr);
        nextExtra = headExtra + 1;
        success = pl_AtomicPX_CompareAndSwap ((PlankAtomicPXRef)&(p->atom), 
                                              headPtr, headExtra, 
                                              nextPtr, nextExtra);
	} while (!success);
    
    *element = headPtr;
    pl_AtomicLL_Decrement (&p->count);

exit:
    return result;    
}

PlankLL pl_LockFreeStack_GetSize (PlankLockFreeStackRef p)
{
    return pl_AtomicLL_Get (&p->count);
}

