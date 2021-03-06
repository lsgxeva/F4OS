/*
 * Copyright (C) 2013, 2014 F4OS Authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/fault.h>

void *memchr(const void *ptr, int value, size_t num) {
    const unsigned char *p = ptr;

    while (num--) {
        if (*p == value) {
            return (void *) p;
        }
        p++;
    }

    return NULL;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num) {
    const unsigned char *p1 = ptr1;
    const unsigned char *p2 = ptr2;

    while (num--) {
        if (*p1 != *p2) {
            if (*p1 > *p2) {
                return 1;
            }
            else {
                return -1;
            }
        }

        p1++;
        p2++;
    }

    return 0;
}

/* Set size bytes to value from p */
void memset32(void *p, int32_t value, uint32_t size) {
    uint32_t *end = (uint32_t *) ((uintptr_t) p + size);

    /* Disallowed unaligned addresses */
    if ( (uintptr_t) p % 4 ) {
        panic_print("Attempt to memset unaligned address (0x%x).", p);
    }

    while ( (uint32_t*) p < end ) {
        *((uint32_t*)p) = value;
        p++;
    }
}

/* Set size bytes to value from p */
void memset(void *p, uint8_t value, uint32_t size) {
    uint8_t *end = (uint8_t *) ((uintptr_t) p + size);

    while ( (uint8_t*) p < end ) {
        *((uint8_t*)p) = value;
        p++;
    }
}

void memcpy(void *dst, const void *src, int n) {
    const uint8_t *s = src;
    uint8_t *d = dst;

    while(n--) {
        *d++ = *s++;
    }
}

// Overlap-safe memcpy
void memmove(void *dst, const void *src, size_t n) {
    const char *s = src;
    char *d = dst;
    if ((uintptr_t)s < (uintptr_t)d)
        while(n--) d[n] = s[n];
    else
        while(n--) *d++ = *s++;
}

char *strchr(const char *s, int c) {
    while (*s != c && *s) {
        s++;
    }

    if (*s == c) {
        return (char *) s;
    }

    return NULL;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

size_t strnlen(const char *s, int n) {
    size_t len = 0;
    while (*s++ && len < n) {
        len++;
    }
    return len;
}

void strreverse(char *s) {
    char *begin = s;

    if (!s || !*s) {
        return;
    }

    while (*++s);
    s--;

    while (s > begin) {
        char temp = *begin;
        *begin++ = *s;
        *s-- = temp;
    }
}

int strcmp(const char *s, const char *p) {
    while (*s == *p && *s != '\0' && *p != '\0') {
        s++;
        p++;
    }

    if (*s == *p) {
        return 0;
    }
    else if (*s > *p) {
        return 1;
    }
    else {
        return -1;
    }
}

int strncmp(const char *s, const char *p, uint32_t n) {
    while (*s == *p && *s != '\0' && *p != '\0' && n) {
        s++;
        p++;
        n--;
    }

    if (!n || *s == *p) {
        return 0;
    }
    else if (*s > *p) {
        return 1;
    }
    else {
        return -1;
    }
}

char *strncpy(char *destination, const char *source, int num) {
    char *ret = destination;

    while (*source && num-- > 0) {
        *destination++ = *source++;
    }

    while (num-- > 0) {
        *destination++ = '\0';
    }

    return ret;
}

// Find if a character is in a list
int chrnlst(char c, const char *l) {
    for (int i = 0; l[i] != '\0'; i++) {
        if (c == l[i])
            return 1;
    }
    return 0;
}
