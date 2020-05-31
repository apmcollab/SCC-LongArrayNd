/*
 * SCC_LongArray1d.h
 *
 *  Created on: Jun 7, 2016
 *      Author: anderson
 *
 *
 *  A minimal 1d long array class with move semantic implementation.
 *
*/
/*
#############################################################################
#
# Copyright 2015 Chris Anderson
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# For a copy of the GNU General Public License see
# <http://www.gnu.org/licenses/>.
#
#############################################################################
*/


#include <cmath>
#include <functional>
#include <iostream>

#ifdef  _DEBUG
#include <cstdio>
#else
#ifndef NDEBUG
#define NDEBUG
#endif
#endif
#include <cassert>

#undef VERBOSE_OPS

#ifndef SCC_LONG_ARRAY_1D_
#define SCC_LONG_ARRAY_1D_

namespace SCC
{
class LongArray1d
{
	public:

    LongArray1d()
	{
	dataPtr    = nullptr;
	index1Size = 0;
	}

	LongArray1d(long n)
	{
	dataPtr     = new long[n];
	index1Size = n;
	}

    LongArray1d(const LongArray1d& V)
    {
      #ifdef VERBOSE_OPS
      std::cout << "Standard Copy " << std::endl;
      #endif

      if(V.dataPtr == nullptr)
      {dataPtr = nullptr; index1Size = 0; return;}

      dataPtr     = new long[V.index1Size];
      index1Size = V.index1Size;
#ifdef _MSC_VER
      std::memcpy(dataPtr, V.dataPtr, (sizeof(long))*index1Size);
#else
      std::copy(V.dataPtr, V.dataPtr + index1Size, dataPtr);
#endif
   }

    LongArray1d(LongArray1d&& V)
    {
      #ifdef VERBOSE_OPS
      std::cout << "Move Copy " << std::endl;
      #endif

      dataPtr      = V.dataPtr;
      index1Size   = V.index1Size;
      V.dataPtr    = nullptr;
      V.index1Size = 0;;
    }

   	virtual ~LongArray1d()
	{
	if(dataPtr != nullptr) delete [] dataPtr;
	}

	void initialize()
	{
	if(dataPtr != nullptr) delete [] dataPtr;
	dataPtr    = nullptr;
	index1Size = 0;
	}

	void initialize(long n)
	{
      if(index1Size != n)
      {
	  if(dataPtr != nullptr) delete [] dataPtr;
      dataPtr     = new long[n];
      index1Size = n;
      }
	}

    void initialize(const LongArray1d& V)
    {
      if(V.dataPtr == nullptr)
      {
      if(dataPtr != nullptr) delete [] dataPtr;
      dataPtr = nullptr;
      index1Size = 0;
      return;
      }

      if(index1Size != V.index1Size)
      {
	  if(dataPtr != nullptr) delete [] dataPtr;
      dataPtr     = new long[V.index1Size];
      index1Size = V.index1Size;
      }

#ifdef _MSC_VER
      std::memcpy(dataPtr, V.dataPtr, (sizeof(long))*index1Size);
#else
      std::copy(V.dataPtr, V.dataPtr + index1Size, dataPtr);
#endif
     }

    void initialize(LongArray1d&& V)
    {
      if(V.dataPtr == nullptr)
      {
      if(dataPtr != nullptr) delete [] dataPtr;
      dataPtr = nullptr;
      index1Size = 0;
      return;
      }

      if(dataPtr != nullptr) delete [] dataPtr;

      dataPtr      = V.dataPtr;
      index1Size   = V.index1Size;
      V.dataPtr    = 0;
      V.index1Size = 0;
    }

    // Assignment operators : Being careful with nullptr instances

    LongArray1d& operator=(const LongArray1d& V)
    {
      #ifdef VERBOSE_OPS
      std::cout << "Standard Assignment" << std::endl;
      #endif

      if (this != &V)
      {
         if((dataPtr == nullptr)&&(V.dataPtr != nullptr))
         {
         index1Size  = V.index1Size;
         dataPtr     = new long[index1Size];
#ifdef _MSC_VER
         std::memcpy(dataPtr, V.dataPtr, (sizeof(long))*index1Size);
#else
         std::copy(V.dataPtr, V.dataPtr + index1Size, dataPtr);
#endif
         }
         else if((dataPtr == nullptr)&&(V.dataPtr == nullptr)){dataPtr = nullptr; index1Size = 0; return *this;}
         else
         {
         assert(sizeCheck(this->index1Size,V.index1Size));
#ifdef _MSC_VER
         std::memcpy(dataPtr, V.dataPtr, (sizeof(long))*index1Size);
#else
         std::copy(V.dataPtr, V.dataPtr + index1Size, dataPtr);
#endif
         }
      }
      return *this;
    }

	LongArray1d& operator=(LongArray1d&& V)
	{
    #ifdef VERBOSE_OPS
    std::cout << "Move Assignment" << std::endl;
    #endif

	if((dataPtr == nullptr)&&(V.dataPtr != nullptr))
    {
      dataPtr      = V.dataPtr;
      index1Size   = V.index1Size;
      V.dataPtr    = nullptr;
      V.index1Size = 0;
    }
    else if((dataPtr == nullptr)&&(V.dataPtr == nullptr)){dataPtr = nullptr; index1Size = 0; return *this;}
    else
    {
      assert(sizeCheck(this->index1Size,V.index1Size));

      // Remove existing data

      delete [] dataPtr;

      dataPtr      = V.dataPtr;
      index1Size   = V.index1Size;
      V.dataPtr    = nullptr;
      V.index1Size = 0;
    }
    return *this;
    }

/*!  Returns the dimension of the vector */

	virtual long getDimension()
	{
    return index1Size;
	}

#ifndef NDEBUG
    long&  operator()(long i1)
    {
    assert(boundsCheck(i1, 0, index1Size-1,1));
    return *(dataPtr +  i1);
    };

    const long&  operator()(long i1) const
    {
    assert(boundsCheck(i1, 0, index1Size-1,1));
    return *(dataPtr +  i1);
    };

#else
    inline long&  operator()(long i1)
    {
    return *(dataPtr + i1);
    };

    inline const long&  operator()(long i1) const
    {
    return *(dataPtr + i1);
    };
#endif


/*!  Sets all values of the vector to d. */

    void setToValue(long d)
    {
	for(long i = 0; i < index1Size; i++)
	{dataPtr[i] =  d;}
    }

    void addValue(long d)
	{
	for(long i = 0; i < index1Size; i++)
	{dataPtr[i] += d;}
	}


 /*!  Outputs vector values to a stream. */

friend std::ostream& operator<<(std::ostream& outStream, const LongArray1d& V)
{

	    long i;
	    for(i = 0; i <  V.index1Size; i++)
	    {
	      outStream << V(i) << " ";
	      outStream << std::endl;
	    }
	    return outStream;
}


    long getSize()         const {return index1Size;}
    long getIndex1Size()  const {return index1Size;}

    long* getDataPointer(){return dataPtr;}
    const  long* getDataPointer()  const  {return dataPtr;}

	long*   dataPtr;
	long index1Size;


//###################################################################
//                      Bounds Checking
//###################################################################
//
#ifdef _DEBUG
        bool boundsCheck(long i, long begin, long end, int coordinate) const
        {
        if((i < begin)||(i  > end))
        {
        std::cerr << "SCC::LongArray1d index " << coordinate << " out of bounds " << std::endl;
        std::cerr << "Offending index value : " << i << " Acceptable Range [" << begin << "," << end << "]" << std::endl;
        return false;
        }
        return true;
        }
#else
        bool boundsCheck(long, long, long, int) const {return true;}
#endif

#ifdef _DEBUG
    bool sizeCheck(long size1, long size2)
    {
    if(size1 != size2)
    {
    std::cerr << "SCC::LongArray1d sizes are incompatible : " << size1 << " != " << size2;
    return false;
    }
    return true;
    }

    bool sizeCheck(long size1, long size2) const
    {
    if(size1 != size2)
    {
    std::cerr << "SCC::LongArray1d sizes are incompatible : " << size1 << " != " << size2;
    return false;
    }
    return true;
    }
#else
    bool sizeCheck(long, long) {return true;}
    bool sizeCheck(long, long) const{return true;}
#endif

};
}

#endif /* SCC_LongArray1d_ */
