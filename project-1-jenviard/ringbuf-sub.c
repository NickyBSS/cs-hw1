#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"

/*
 * author: Jenna
 * date: 3 sep 2025
 * pledge: this is my own work, unless otherwise noted
 *
 * update:
 */


/* static variables (zeroed) */

int g_debug = 0 ;
int is_verbose = 0 ;
struct RingBuf rb ;

int rb_ioctl(int op) {
	switch (op) {
		case RB_Q_SIZE:
			return RINGBUF_SIZE ;
		case RB_IS_EMPTY:
			if (rb.head == rb.tail && !rb.is_full) {
				return 1 ;
			}
			return 0 ;
		case RB_IS_FULL:
			return rb.is_full ;
		case RB_Q_COUNT:
			if (rb.is_full) {
				return RINGBUF_SIZE ;
			} else if (rb.head >= rb.tail) {
				return rb.head - rb.tail ;
			} else {
				return RINGBUF_SIZE - rb.tail + rb.head ;
			}
		default:
			;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
	if (rb.is_full) {
		return -1 ;
	}

	rb.ringbuf[rb.head] = (char)ele ;
	rb.head = (rb.head + 1) % RINGBUF_SIZE ;

	if (rb.head == rb.tail) {
		rb.is_full = 1 ;
	}

	return ele ;
}

int rb_dequeue(void) {
	if (rb.head == rb.tail && !rb.is_full) {
		return -1 ;
	}

	char ele = rb.ringbuf[rb.tail] ;
	rb.tail = (rb.tail + 1) % RINGBUF_SIZE ;
	rb.is_full = 0 ;

	return (int)ele ;
}