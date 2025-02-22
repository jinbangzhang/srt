#ifndef SRT_SYNC_ATOMIC_MSVC_H_
#define SRT_SYNC_ATOMIC_MSVC_H_

// Define which functions we need (don't include <intrin.h>).
extern "C" {
short _InterlockedIncrement16(short volatile*);
long _InterlockedIncrement(long volatile*);
__int64 _InterlockedIncrement64(__int64 volatile*);

short _InterlockedDecrement16(short volatile*);
long _InterlockedDecrement(long volatile*);
__int64 _InterlockedDecrement64(__int64 volatile*);

char _InterlockedExchange8(char volatile*, char);
short _InterlockedExchange16(short volatile*, short);
long __cdecl _InterlockedExchange(long volatile*, long);
__int64 _InterlockedExchange64(__int64 volatile*, __int64);

char _InterlockedCompareExchange8(char volatile*, char, char);
short _InterlockedCompareExchange16(short volatile*, short, short);
long __cdecl _InterlockedCompareExchange(long volatile*, long, long);
__int64 _InterlockedCompareExchange64(__int64 volatile*, __int64, __int64);
};

// Define which functions we want to use as inline intriniscs.
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedIncrement16)

#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedDecrement16)

#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange8)
#pragma intrinsic(_InterlockedCompareExchange16)

#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchange8)
#pragma intrinsic(_InterlockedExchange16)

#if defined(_M_X64)
#pragma intrinsic(_InterlockedIncrement64)
#pragma intrinsic(_InterlockedDecrement64)
#pragma intrinsic(_InterlockedCompareExchange64)
#pragma intrinsic(_InterlockedExchange64)
#endif  // _M_X64

namespace srt {
namespace sync {
namespace msvc {
template <typename T, size_t N = sizeof(T)>
struct interlocked {
};

template <typename T>
struct interlocked<T, 1> {
  static inline T increment(T volatile* x) {
    // There's no _InterlockedIncrement8().
    char old_val, new_val;
    do {
      old_val = static_cast<char>(*x);
      new_val = old_val + static_cast<char>(1);
    } while (_InterlockedCompareExchange8(reinterpret_cast<volatile char*>(x),
                                          new_val,
                                          old_val) != old_val);
    return static_cast<T>(new_val);
  }

  static inline T decrement(T volatile* x) {
    // There's no _InterlockedDecrement8().
    char old_val, new_val;
    do {
      old_val = static_cast<char>(*x);
      new_val = old_val - static_cast<char>(1);
    } while (_InterlockedCompareExchange8(reinterpret_cast<volatile char*>(x),
                                          new_val,
                                          old_val) != old_val);
    return static_cast<T>(new_val);
  }

  static inline T compare_exchange(T volatile* x,
                                   const T new_val,
                                   const T expected_val) {
    return static_cast<T>(
        _InterlockedCompareExchange8(reinterpret_cast<volatile char*>(x),
                                     static_cast<const char>(new_val),
                                     static_cast<const char>(expected_val)));
  }

  static inline T exchange(T volatile* x, const T new_val) {
    return static_cast<T>(_InterlockedExchange8(
        reinterpret_cast<volatile char*>(x), static_cast<const char>(new_val)));
  }
};

template <typename T>
struct interlocked<T, 2> {
  static inline T increment(T volatile* x) {
    return static_cast<T>(
        _InterlockedIncrement16(reinterpret_cast<volatile short*>(x)));
  }

  static inline T decrement(T volatile* x) {
    return static_cast<T>(
        _InterlockedDecrement16(reinterpret_cast<volatile short*>(x)));
  }

  static inline T compare_exchange(T volatile* x,
                                   const T new_val,
                                   const T expected_val) {
    return static_cast<T>(
        _InterlockedCompareExchange16(reinterpret_cast<volatile short*>(x),
                                      static_cast<const short>(new_val),
                                      static_cast<const short>(expected_val)));
  }

  static inline T exchange(T volatile* x, const T new_val) {
    return static_cast<T>(
        _InterlockedExchange16(reinterpret_cast<volatile short*>(x),
                               static_cast<const short>(new_val)));
  }
};

template <typename T>
struct interlocked<T, 4> {
  static inline T increment(T volatile* x) {
    return static_cast<T>(
        _InterlockedIncrement(reinterpret_cast<volatile long*>(x)));
  }

  static inline T decrement(T volatile* x) {
    return static_cast<T>(
        _InterlockedDecrement(reinterpret_cast<volatile long*>(x)));
  }

  static inline T compare_exchange(T volatile* x,
                                   const T new_val,
                                   const T expected_val) {
    return static_cast<T>(
        _InterlockedCompareExchange(reinterpret_cast<volatile long*>(x),
                                    static_cast<const long>(new_val),
                                    static_cast<const long>(expected_val)));
  }

  static inline T exchange(T volatile* x, const T new_val) {
    return static_cast<T>(_InterlockedExchange(
        reinterpret_cast<volatile long*>(x), static_cast<const long>(new_val)));
  }
};

template <typename T>
struct interlocked<T, 8> {
  static inline T increment(T volatile* x) {
#if defined(_M_X64)
    return static_cast<T>(
        _InterlockedIncrement64(reinterpret_cast<volatile __int64*>(x)));
#else
    // There's no _InterlockedIncrement64() for 32-bit x86.
    __int64 old_val, new_val;
    do {
      old_val = static_cast<__int64>(*x);
      new_val = old_val + static_cast<__int64>(1);
    } while (_InterlockedCompareExchange64(
                 reinterpret_cast<volatile __int64*>(x), new_val, old_val) !=
             old_val);
    return static_cast<T>(new_val);
#endif  // _M_X64
  }

  static inline T decrement(T volatile* x) {
#if defined(_M_X64)
    return static_cast<T>(
        _InterlockedDecrement64(reinterpret_cast<volatile __int64*>(x)));
#else
    // There's no _InterlockedDecrement64() for 32-bit x86.
    __int64 old_val, new_val;
    do {
      old_val = static_cast<__int64>(*x);
      new_val = old_val - static_cast<__int64>(1);
    } while (_InterlockedCompareExchange64(
                 reinterpret_cast<volatile __int64*>(x), new_val, old_val) !=
             old_val);
    return static_cast<T>(new_val);
#endif  // _M_X64
  }

  static inline T compare_exchange(T volatile* x,
                                   const T new_val,
                                   const T expected_val) {
    return static_cast<T>(_InterlockedCompareExchange64(
        reinterpret_cast<volatile __int64*>(x),
        static_cast<const __int64>(new_val),
        static_cast<const __int64>(expected_val)));
  }

  static inline T exchange(T volatile* x, const T new_val) {
#if defined(_M_X64)
    return static_cast<T>(
        _InterlockedExchange64(reinterpret_cast<volatile __int64*>(x),
                               static_cast<const __int64>(new_val)));
#else
    // There's no _InterlockedExchange64 for 32-bit x86.
    __int64 old_val;
    do {
      old_val = static_cast<__int64>(*x);
    } while (_InterlockedCompareExchange64(
                 reinterpret_cast<volatile __int64*>(x), new_val, old_val) !=
             old_val);
    return static_cast<T>(old_val);
#endif  // _M_X64
  }
};
}  // namespace msvc
}  // namespace sync
}  // namespace srt

#endif  // ATOMIC_ATOMIC_MSVC_H_
