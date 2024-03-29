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

#ifndef PLONK_GRAPHFORWARDDECLARATIONS_H
#define PLONK_GRAPHFORWARDDECLARATIONS_H

#include "../maths/plonk_InlineBinaryOps.h"
#include "../maths/plonk_InlineUnaryOps.h"
#include "../maths/plonk_InlineMiscOps.h"
#include "../core/plonk_CoreForwardDeclarations.h"
#include "../containers/plonk_ContainerForwardDeclarations.h"
#include "../containers/variables/plonk_VariableForwardDeclarations.h"

// graph types
class ChannelInternalCore;
class BlockSize;
class SampleRate;
class ProcessInfo;
class ProcessInfoInternal;
class TimeStamp;
class InputDictionary;

// info
class IOKey;
class IOLimit;
class Measure;
class IOInfo;
class UnitInfo;
typedef ObjectArray<IOInfo>     IOInfos;
typedef ObjectArray<UnitInfo>   UnitInfos;

// core templated graph types
template<class SampleType>                                              class BusBuffer;
template<class SampleType>                                              class ChannelBase;
template<class SampleType>                                              class ChannelInternalBase;
template<class SampleType, class DataType>                              class ChannelInternal;
template<class SampleType>                                              class UnitBase;
template<class SampleType, class DataType>                              class ProxyOwnerChannelInternal;
template<class SampleType>                                              class ProxyChannelInternal;
template<class OwnerType>                                               struct ChannelData;
        
// common channels
template<class SampleType>                                              class ConstantChannelInternal;
template<class SampleType>                                              class NullChannelInternal;

template<class SampleType, PLONK_BINARYOPFUNCTION(SampleType, op)>      class BinaryOpChannelInternal;
template<class SampleType, PLONK_UNARYOPFUNCTION(SampleType, op)>       class UnaryOpChannelInternal;
template<class SampleType>                                              class MulAddChannelInternal;

// common units
template<class SampleType>                                              class MulAddUnit;
template<class SampleType>                                              class ReblockUnit;
template<class SampleType,Interp::TypeCode>                             class ResampleUnit;
template<class SampleType>                                              class MixerUnit;
template<class SampleType>                                              class OverlapMakeUnit;
template<class SampleType>                                              class OverlapMixUnit;
template<class SampleType>                                              class ParamUnit;
template<class SampleType>                                              class AtomicVariableUnit;
template<class SampleType,
         Interp::TypeCode InterpTypeCode = Interp::Linear>              class InputTaskUnit;
template<class SampleType,
         Interp::TypeCode InterpTypeCode = Interp::Linear>              class LagUnit;
template<class SampleType,
         Interp::TypeCode InterpTypeCode = Interp::Linear>              class DCUnit;
template<class SampleType>                                              class DiffUnit;

// type conversion units
template<class OutputSampleType, class InputSampleType>                 class TypeUnit;

typedef TypeUnit<float,int>     TypeInt2Float;
typedef TypeUnit<int,float>     TypeFloat2Int;
typedef TypeUnit<float,double>  TypeDouble2Float;
typedef TypeUnit<double,float>  TypeFloat2Double;
typedef TypeUnit<float,short>   TypeShort2Float;
typedef TypeUnit<short,float>   TypeFloat2Short;
typedef TypeUnit<float,Int24>   TypeInt242Float;
typedef TypeUnit<Int24,float>   TypeFloat2In24;

typedef TypeUnit<PLONK_TYPE_DEFAULT,float>      TypeFloat2Default;
typedef TypeUnit<PLONK_TYPE_DEFAULT,double>     TypeDouble2Default;
typedef TypeUnit<PLONK_TYPE_DEFAULT,char>       TypeChar2Default;
typedef TypeUnit<PLONK_TYPE_DEFAULT,int>        TypeInt2Default;
typedef TypeUnit<PLONK_TYPE_DEFAULT,short>      TypeShort2Default;
typedef TypeUnit<PLONK_TYPE_DEFAULT,Int24>      TypeInt242Default;

typedef TypeUnit<float,PLONK_TYPE_DEFAULT>      TypeDefault2Float;
typedef TypeUnit<double,PLONK_TYPE_DEFAULT>     TypeDefault2Double;
typedef TypeUnit<char,PLONK_TYPE_DEFAULT>       TypeDefault2Char;
typedef TypeUnit<int,PLONK_TYPE_DEFAULT>        TypeDefault2Int;
typedef TypeUnit<short,PLONK_TYPE_DEFAULT>      TypeDefault2Short;
typedef TypeUnit<Int24,PLONK_TYPE_DEFAULT>      TypeDefault2Int24;


// sampletype typedefs
typedef ChannelBase<float>              FloatChannel;
typedef ChannelBase<double>             DoubleChannel;
typedef ChannelBase<short>              ShortChannel;
typedef ChannelBase<int>                IntChannel;
typedef ChannelBase<Int24>              Int24Channel;
typedef ChannelBase<Long>               LongChannel;

typedef UnitBase<float>                 FloatUnit;
typedef UnitBase<double>                DoubleUnit;
typedef UnitBase<short>                 ShortUnit;
typedef UnitBase<char>                  CharUnit;
typedef UnitBase<int>                   IntUnit;
typedef UnitBase<Int24>                 Int24Unit;
typedef UnitBase<Long>                  LongUnit;

typedef BusBuffer<float>                FloatBus;
typedef BusBuffer<double>               DoubleBus;
typedef BusBuffer<char>                 CharBus;
typedef BusBuffer<int>                  IntBus;
typedef BusBuffer<Int24>                Int24Bus;
typedef BusBuffer<short>                ShortBus;
typedef BusBuffer<Long>                 LongBus;

#define PLONK_BUSARRAYBASETYPE NumericalArray
typedef PLONK_BUSARRAYBASETYPE<FloatBus>                    FloatBusses;
typedef PLONK_BUSARRAYBASETYPE<DoubleBus>                   DoubleBusses;
typedef PLONK_BUSARRAYBASETYPE<IntBus>                      IntBusses;
typedef PLONK_BUSARRAYBASETYPE<Int24Bus>                    Int24Busses;
typedef PLONK_BUSARRAYBASETYPE<ShortBus>                    ShortBusses;
typedef PLONK_BUSARRAYBASETYPE<LongBus>                     LongBusses;

#define PLONK_UNITBASETYPE NumericalArray
typedef PLONK_UNITBASETYPE<FloatUnit>                       FloatUnitRaw;
typedef PLONK_UNITBASETYPE<DoubleUnit>                      DoubleUnitRaw;
typedef PLONK_UNITBASETYPE<ShortUnit>                       ShortUnitRaw;
typedef PLONK_UNITBASETYPE<IntUnit>                         IntUnitRaw;
typedef PLONK_UNITBASETYPE<Int24Unit>                       Int24UnitRaw;
typedef PLONK_UNITBASETYPE<LongUnit>                        LongUnitRaw;

typedef NumericalArray2D<FloatChannel,FloatUnit>            FloatUnits;
typedef NumericalArray2D<DoubleChannel,DoubleUnit>          DoubleUnits;
typedef NumericalArray2D<ShortChannel,ShortUnit>            ShortUnits;
typedef NumericalArray2D<IntChannel,IntUnit>                IntUnits;
typedef NumericalArray2D<Int24Channel,Int24Unit>            Int24Units;
typedef NumericalArray2D<LongChannel,LongUnit>              LongUnits;

// default types
typedef ChannelBase<PLONK_TYPE_DEFAULT>                     Channel;
typedef UnitBase<PLONK_TYPE_DEFAULT>                        Unit;
typedef PLONK_UNITBASETYPE<UnitBase<PLONK_TYPE_DEFAULT> >   UnitRaw;
typedef NumericalArray2D<Channel,Unit>                      Units;
typedef BusBuffer<PLONK_TYPE_DEFAULT>                       Bus;
typedef PLONK_BUSARRAYBASETYPE<Bus>                         Busses;

// variable graph objects
typedef Variable< ChannelBase<float>& >                     FloatChannelVariable;
typedef Variable< ChannelBase<double>& >                    DoubleChannelVariable;
typedef Variable< ChannelBase<int>& >                       IntChannelVariable;
typedef Variable< ChannelBase<short>& >                     ShortChannelVariable;
typedef Variable< ChannelBase<Int24>& >                     Int24ChannelVariable;
typedef Variable< ChannelBase<Long>& >                      LongChannelVariable;
typedef Variable< ChannelBase<PLONK_TYPE_DEFAULT>& >        ChannelVariable;

typedef Variable< UnitBase<float>& >                        FloatUnitVariable;
typedef Variable< UnitBase<double>& >                       DoubleUnitVariable;
typedef Variable< UnitBase<int>& >                          IntUnitVariable;
typedef Variable< UnitBase<short>& >                        ShortUnitVariable;
typedef Variable< UnitBase<Int24>& >                        Int24UnitVariable;
typedef Variable< UnitBase<Long>& >                         LongUnitVariable;
typedef Variable< UnitBase<PLONK_TYPE_DEFAULT>& >           UnitVariable;

typedef Variable< BusBuffer<float>& >                       FloatBusVariable;
typedef Variable< BusBuffer<double>& >                      DoubleBusVariable;
typedef Variable< BusBuffer<int>& >                         IntBusVariable;
typedef Variable< BusBuffer<short>& >                       ShortBusVariable;
typedef Variable< BusBuffer<Int24>& >                       Int24BusVariable;
typedef Variable< BusBuffer<Long>& >                        LongBusVariable;
typedef Variable< BusBuffer<PLONK_TYPE_DEFAULT>& >          BusVariable;

typedef Variable< FloatBusses& >                            FloatBussesVariable;
typedef Variable< DoubleBusses& >                           DoubleBussesVariable;
typedef Variable< IntBusses& >                              IntBussesVariable;
typedef Variable< ShortBusses& >                            ShortBussesVariable;
typedef Variable< Int24Busses& >                            Int24BussesVariable;
typedef Variable< LongBusses& >                             LongBussesVariable;
typedef Variable< Busses& >                                 BussesVariable;

typedef Variable< FloatUnits& >                             FloatUnitsVariable;
typedef Variable< DoubleUnits& >                            DoubleUnitsVariable;
typedef Variable< IntUnits& >                               IntUnitsVariable;
typedef Variable< ShortUnits& >                             ShortUnitsVariable;
typedef Variable< Int24Units& >                             Int24UnitsVariable;
typedef Variable< LongUnits& >                              LongUnitsVariable;
typedef Variable< Units& >                                  UnitsVariable;

typedef LockFreeQueue< UnitBase<float> >                    FloatUnitQueue;
typedef LockFreeQueue< UnitBase<double> >                   DoubleUnitQueue;
typedef LockFreeQueue< UnitBase<short> >                    ShortUnitQueue;
typedef LockFreeQueue< UnitBase<char> >                     CharUnitQueue;
typedef LockFreeQueue< UnitBase<int> >                      IntUnitQueue;
typedef LockFreeQueue< UnitBase<Int24> >                    Int24UnitQueue;
typedef LockFreeQueue< UnitBase<Long> >                     LongUnitQueue;
typedef LockFreeQueue< UnitBase<PLONK_TYPE_DEFAULT> >       UnitQueue;

template<class SampleType>                                  class QueueBufferBase;
typedef QueueBufferBase<float>                              FloatQueueBuffer;
typedef QueueBufferBase<double>                             DoubleQueueBuffer;
typedef QueueBufferBase<short>                              ShortQueueBuffer;
typedef QueueBufferBase<char>                               CharQueueBuffer;
typedef QueueBufferBase<int>                                IntQueueBuffer;
typedef QueueBufferBase<Int24>                              Int24QueueBuffer;
typedef QueueBufferBase<Long>                               LongQueueBuffer;
typedef QueueBufferBase<PLONK_TYPE_DEFAULT>                 QueueBuffer;

typedef LockFreeQueue< QueueBufferBase<float> >                    FloatBufferQueue;
typedef LockFreeQueue< QueueBufferBase<double> >                   DoubleBufferQueue;
typedef LockFreeQueue< QueueBufferBase<short> >                    ShortBufferQueue;
typedef LockFreeQueue< QueueBufferBase<char> >                     CharBufferQueue;
typedef LockFreeQueue< QueueBufferBase<int> >                      IntBufferQueue;
typedef LockFreeQueue< QueueBufferBase<Int24> >                    Int24BufferQueue;
typedef LockFreeQueue< QueueBufferBase<Long> >                     LongBufferQueue;
typedef LockFreeQueue< QueueBufferBase<PLONK_TYPE_DEFAULT> >       BufferQueue;


#endif // PLONK_GRAPHFORWARDDECLARATIONS_H