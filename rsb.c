//
// RetractableSerialBuffer
//	
//	Copyright (c) 2013 赵迤晨 (Zhao, Yichen) <max.zhaoyichen@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/**
 * @file
 * Retractable Serial Buffer
 * A fifo binary buffer that allows setting and restoring a single data marker
 *
 * @needs stdint.h
 * @needs stdbool.h
 * @needs stddef.h
 *
 * @author Zhao Yichen <max.zhaoyichen@gmail.com>
 * @copyright Copyright (c) 2013 赵迤晨 (Zhao, Yichen) <max.zhaoyichen@gmail.com>
 * @license MIT License
 * @version 0.1
 * 
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "rsb.h"

#define rsb_write_position(buffer) ((buffer->_read_position + buffer->length) % buffer->capacity)

void rsb_init(rsb_buffer_t* buffer) {
	buffer->buffer         = NULL;
	buffer->capacity       = 0;
	buffer->buffer_filled  = NULL;
	buffer->always_invoke  = false;
	buffer->_read_position = 0;
	buffer->length         = 0;
	buffer->_marker_length = -1;
}

_Bool rsb_write(rsb_buffer_t* buffer, uint8_t data) {
	if (rsb_space(buffer) <= 0){
		return false;
	}
	buffer->buffer[rsb_write_position(buffer)] = data;
	buffer->length ++;
	if (buffer->length == 1 //Just left the empty state
		|| buffer->always_invoke) {
		if (buffer->buffer_filled != NULL) buffer->buffer_filled();
	}
	return true;
}

int rsb_read(rsb_buffer_t* buffer) {
	int read_position;
	if (rsb_length(buffer) <= 0){
		return -1;
	}
	if (buffer->_marker_length > 0) {
		buffer->_marker_length --;
	}
	buffer->length --;
	read_position = buffer->_read_position;
	buffer->_read_position = (buffer->_read_position + 1) % buffer->capacity;
	return buffer->buffer[read_position];
}

int rsb_pop(rsb_buffer_t* buffer) {
	int read_position;
	if (rsb_length(buffer) <= 0) {
		return -1;
	}
	read_position = (buffer->_read_position + buffer->length - 1) % buffer->capacity;
	buffer->length --;
	if (buffer->_marker_length > buffer->length) {
		buffer->_marker_length = buffer->length;
	}
	return buffer->buffer[read_position];
}

void rsb_restore_marker(rsb_buffer_t* buffer) {
	if (buffer->_marker_length >= 0) {
		buffer->length = buffer->_marker_length;
	}
}
