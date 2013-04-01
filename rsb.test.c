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
 * Sample RSB Test
 *
 * @author Zhao Yichen <max.zhaoyichen@gmail.com>
 * @copyright Copyright (c) 2013 赵迤晨 (Zhao, Yichen) <max.zhaoyichen@gmail.com>
 * @license MIT License
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "rsb.h"
#include <stdio.h>
#include <windows.h>

unsigned char data_buffer[10];
rsb_buffer_t buffer;
rsb_buffer_t* buffer_ptr;

_Bool keep_running = true;

DWORD WINAPI ReaderThread(LPVOID dummy) {
	while (keep_running) {
		int result = rsb_read(&buffer);
		if (result >= 0)
			printf("%c", result);
		Sleep(500);
	}
}

void write_signal() {
	printf("\nwrite\n");
}

int main() {
	int i;
	rsb_init_array(&buffer, data_buffer);
	buffer.buffer_filled = write_signal;
	CreateThread(NULL, 0, ReaderThread, NULL, 0, NULL);

	for (i = 0; i < 26; i++) {
		rsb_write(&buffer, 'a' + i);
		if (i == 4)
			rsb_set_marker(&buffer);
	}

	rsb_restore_marker(&buffer);
	rsb_write(&buffer, '1');
	rsb_write(&buffer, '2');
	while(!rsb_is_empty(&buffer)) {
		Sleep(100);
	}

	rsb_write(&buffer, 'h');
	rsb_write(&buffer, 'e');
	rsb_write(&buffer, 'e');
	rsb_write(&buffer, 'h');

	while (rsb_length(&buffer) > 2)
	{
		Sleep(100);
	}

	rsb_restore_marker(&buffer);
	while(!rsb_is_empty(&buffer)) {
		Sleep(100);
	}
	rsb_clear_marker(&buffer);
	buffer.always_invoke = true;

	rsb_write(&buffer, 'h');
	rsb_write(&buffer, 'e');
	rsb_write(&buffer, 'e');
	rsb_write(&buffer, 'h');

	rsb_restore_marker(&buffer);
	while(!rsb_is_empty(&buffer)) {
		Sleep(100);
	}
}