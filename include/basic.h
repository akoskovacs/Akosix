#ifndef BASIC_H
#define BASIC_H

#define NULL ((void *)0)

#define __inline __attribute__((__inline__))
#define __unused __attribute__((unused))
#define __packed __attribute__((packed))

#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#define va_copy(d,s)	__builtin_va_copy(d,s)

#endif // BASIC_H
