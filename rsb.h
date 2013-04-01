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
 * Retractable Serial Buffer Header
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
#ifndef RSB_H
#define RSB_H

typedef struct {
	uint8_t* buffer;
	int capacity;
	void (*buffer_filled) ();
	_Bool always_invoke;
	int _read_position;
	int length;
	int _marker_length;
} rsb_buffer_t, rsb_buffer;

/**
 * Initialize an rsb_buffer_t structure
 * @param buffer [description]
 */
void rsb_init(rsb_buffer_t* buffer);
#define rsb_init_array(buff, array) { rsb_init(buff); (buff)->buffer = array; (buff)->capacity = sizeof(array); }
#define rsb_init_pointer(buff, pointer, size) { rsb_init(buff); (buff)->buffer = pointer; (buff)->capacity = size; }

/**
 * Write a byte into the specified buffer
 * @param  buffer [description]
 * @param  data   [description]
 * @return        true if succeed, otherwise false (buffer overflow)
 */
_Bool rsb_write(rsb_buffer_t* buffer, uint8_t data);
#define rsb_push rsb_write

/**
 * Read a byte from the front of the buffer
 * @param  buffer [description]
 * @return        the byte if the buffer is not empty. Otherwise, -1.
 */
int rsb_read(rsb_buffer_t* buffer);

/**
 * Pop a byte from the end of the buffer
 * @param  buffer [description]
 * @return        the byte if the buffer is not empty. Otherwise, -1.
 */
int rsb_pop(rsb_buffer_t* buffer);

/**
 * Return the empty room in a buffer
 * @param  length [description]
 * @return        [description]
 */
#define rsb_space(buffer) ((buffer)->capacity - (buffer)->length)

/**
 * Checks if the rsb_buffer is free
 */
#define rsb_is_free(buffer) (rsb_space(buffer) > 0)

/**
 * Checks if the rsb_buffer is empty
 */
#define rsb_is_empty(buffer) (rsb_length(buffer) == 0)

/**
 * Return the filled length of a buffer
 * @param  length [description]
 * @return        [description]
 */
#define rsb_length(buffer) ((buffer)->length)

/**
 * Empty a buffer
 * @param  buffer [description]
 * @return        [description]
 */
#define rsb_clear(buffer) { (buffer)->length = 0; }

/**
 * Set the restore marker of the given buffer (at the current byte)
 * @param  buffer [description]
 * @return        [description]
 */
#define rsb_set_marker(buffer) { (buffer)->_marker_length = (buffer)->length; }

/**
 * Clear the restore marker of the given buffer
 * @param  buffer [description]
 * @return        [description]
 */
#define rsb_clear_marker(buffer) { (buffer)->_marker_length = -1; }

void rsb_restore_marker(rsb_buffer_t* buffer);

#endif