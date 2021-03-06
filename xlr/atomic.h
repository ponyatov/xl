#ifndef ATOMIC_H
#define ATOMIC_H
// ****************************************************************************
//  atomic.h                                                        XLR project
// ****************************************************************************
//
//   File Description:
//
//    The atomic operations we may need in XL
//
//
//
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License, with the
// following clarification and exception.
//
// Linking this library statically or dynamically with other modules is making
// a combined work based on this library. Thus, the terms and conditions of the
// GNU General Public License cover the whole combination.
//
// As a special exception, the copyright holders of this library give you
// permission to link this library with independent modules to produce an
// executable, regardless of the license terms of these independent modules,
// and to copy and distribute the resulting executable under terms of your
// choice, provided that you also meet, for each linked independent module,
// the terms and conditions of the license of that module. An independent
// module is a module which is not derived from or based on this library.
// If you modify this library, you may extend this exception to your version
// of the library, but you are not obliged to do so. If you do not wish to
// do so, delete this exception statement from your version.
//
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "base.h"

XL_BEGIN

// ============================================================================
//
//   Interface
//
// ============================================================================

struct OrderedAtomicMode;
struct AcquireAtomicMode;       // REVISIT: Not implemented yet
struct ReleaseAtomicMode;       // REVISIT: Not implemented yet


template <typename Value, typename Mode = OrderedAtomicMode>
struct Atomic
// ----------------------------------------------------------------------------
//   A value of type T with atomic properties
// ----------------------------------------------------------------------------
{
    typedef Value               value_t;
    typedef int                 diff_t;

public:
    Atomic()                    : value() {}
    Atomic(value_t v)           : value(v) {}
    Atomic(const Atomic &o)     : value(o.Get()) {}

    operator value_t() cons     { return value; }
    operator value_t&()         { return value; }

    bool  Get()                 { return value; }
    bool  Set(value_t from, value_t to);
    value_t Swap(value_t swap);
    value_t Add(diff_t delta);

    // Increment and decrement can be specialized for processors that
    // have special instructions with limited constant range (e.g. Itanium)
    template <int delta = 1>
    value_t Increment()         { return Add(delta); }
    template <int delta = 1>
    value_t Decrement()         { return Add(-delta); }

protected:
    volatile value_t            value;
};



#if defined(__x86_64__)
// ============================================================================
//
//   Implementation for x86-64
//
// ============================================================================

template<> inline
bool Atomic<int,OrderedAtomicMode>::Set(int from, int to)
// ----------------------------------------------------------------------------
//   Test that we have 'from' and set to 'to'
// ----------------------------------------------------------------------------
{
    unsigned char ret;
    asm volatile("lock\n"
                 "cmpxchgl %3,%2\n"
                 "sete %1\n"
                 : "=a" (to), "=qm" (ret), "+m" (value)
                 : "r" (to), "0" (from)
                 : "memory");
    return ret != 0;
}


template<> inline
int Atomic<int,OrderedAtomicMode>::Swap(int swap)
// ----------------------------------------------------------------------------
//   Atomically swap the value and return what was there before
// ----------------------------------------------------------------------------
{
    asm volatile("xchgl %0,%1"
                 : "=r" (swap), "+m" (value)
                 : "0" (swap)
                 : "memory");
    return swap;
}


template<> inline
int Atomic<int,OrderedAtomicMode>::Add(int delta)
// ----------------------------------------------------------------------------
//   Atomically increment the given value
// ----------------------------------------------------------------------------
{
    asm volatile("lock\n"
                 "xaddl %0,%1"
                 : "=r" (delta), "+m" (value)
                 : "0" (delta)
                 : "memory");
    return delta;
}


template<typename V> inline
bool Atomic<V *,OrderedAtomicMode>::Set(V *from, V *to)
// ----------------------------------------------------------------------------
//   Test that we have 'from' and set to 'to'
// ----------------------------------------------------------------------------
{
    unsigned char ret;
    asm volatile("lock\n"
                 "cmpxchgq %3,%2\n"
                 "sete %1\n"
                 : "=a" (to), "=qm" (ret), "+m" (value)
                 : "r" (to), "0" (from)
                 : "memory");
    return ret != 0;
}


template<typename V> inline
V Atomic<V *,OrderedAtomicMode>::Swap(V *swap)
// ----------------------------------------------------------------------------
//   Atomically swap the value and return what was there before
// ----------------------------------------------------------------------------
{
    asm volatile("xchgq %0,%1"
                 : "=r" (swap), "+m" (value)
                 : "0" (swap)
                 : "memory");
    return swap;
}


template<typename V> inline
V *Atomic<V*, OrderedAtomicMode>::Add(int delta)
// ----------------------------------------------------------------------------
//   Atomically increment the given value for pointers
// ----------------------------------------------------------------------------
{
    asm volatile("lock\n"
                 "xaddq %0,%1"
                 : "=r" (delta), "+m" (value)
                 : "0" (delta * sizeof(V))
                 : "memory");
    return reinterpret_cast<V> (delta);
}


template<> inline
int Atomic<int,OrderedAtomicMode>::Increment<1>()
// ----------------------------------------------------------------------------
//   If we increment by 1, use increment instead of add
// ----------------------------------------------------------------------------
{
    unsigned char ret;
    asm volatile("lock\n"
                 "incl %0"
                 : "=m" (value)
                 : "m" (value)
                 : "memory");
    return value;
}


template<> inline
int Atomic<int,OrderedAtomicMode>::Decrement<1>()
// ----------------------------------------------------------------------------
//   If we decrement by 1, use increment instead of add
// ----------------------------------------------------------------------------
{
    unsigned char ret;
    asm volatile("lock\n"
                 "decl %0"
                 : "=m" (value)
                 : "m" (value)
                 : "memory");
    return value;
    
}


#elif defined(__i386__)
// ============================================================================
// 
//   Implementation for "regular" x86
// 
// ============================================================================

#error "Not yet implemented"

#elif defined(__ppc__)
// ============================================================================
// 
//   Implementation for PowerPC
// 
// ============================================================================

#error "Not yet implemented"

#else
// ============================================================================
// 
//   Default implementation (pure C)
// 
// ============================================================================

#include <pthread.h>

#error "Not yet implemented"

#endif

XL_END

#endif // ATOMIC_H
