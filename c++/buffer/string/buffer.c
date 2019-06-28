#include "buffer.h"
#include "dbg.h"


#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int vasprintf(char **strp, const char *fmt, va_list ap)
{
  int r = -1, size = _vscprintf(fmt, ap);

  if ((size >= 0) && (size < INT_MAX))
  {
    *strp = (char *)malloc(size+1); //+1 for null
    if (*strp)
    {
      r = vsnprintf(*strp, size+1, fmt, ap);  //+1 for null
      if ((r < 0) || (r > size))
      {
        { free(*strp); (*strp) = 0; };
        r = -1;
      }
    }
  }
  else { *strp = 0; }

  return(r);
}

#endif

Buffer *
buffer_alloc(int initial_size)
{
    Buffer *buf = (Buffer*)malloc(sizeof(Buffer));
    char   *tmp = (char*)calloc(1, initial_size * sizeof(char));

    jump_to_error_if(buf == NULL || tmp == NULL);

    buf->contents   = tmp;
    buf->bytes_used = 0;
    buf->total_size = initial_size;

    return buf;
error:
    if (buf) { buffer_free(buf); }
    if (tmp) { free(tmp); }

    return NULL;
}

int
buffer_strlen(Buffer *buf)
{
    return buf->bytes_used;
}

void
buffer_free(Buffer *buf)
{
    free(buf->contents);
    free(buf);
}

int
buffer_has_space(Buffer *buf, int desired_length)
{
    int bytes_remaining = buf->total_size - buf->bytes_used;

    debug("Requesting %d bytes, %d available", desired_length, bytes_remaining);

    return desired_length <= bytes_remaining;
}

int
buffer_grow(Buffer *buf, int minimum_size)
{
	int new_size = 0;
    int factor = buf->total_size;
	char *tmp = 0;
    if (factor < minimum_size) {
        factor = minimum_size;
    }

    new_size = factor * 2;

    debug("Growing buffer from %d to %d bytes", buf->total_size, new_size);

    tmp = (char*)realloc(buf->contents, new_size * sizeof(char));
    jump_to_error_if(tmp == NULL);

    buf->contents   = tmp;
    buf->total_size = new_size;

    return 0;
error:
    return -1;
}

void
buffer_cat(Buffer *buf, char *append, int length)
{
    int i               = 0;
    int bytes_copied    = 0;
    int buffer_position = 0;

    for (i = 0; i < length; i++) {
        if (append[i] == '\0') { break; }

        buffer_position = buf->bytes_used + i;
        *(buf->contents + buffer_position) = append[i];

        bytes_copied++;
    }

    buf->bytes_used += bytes_copied;
    *(buf->contents + buf->bytes_used) = '\0';
}

int
buffer_append(Buffer *buf, char *append, int length)
{
    int status         = 0;
    int desired_length = length + 1; // Space for NUL byte

    if (!buffer_has_space(buf, desired_length)) {
        status = buffer_grow(buf, desired_length);
        jump_to_error_unless(status == 0)
    }

    buffer_cat(buf, append, length);

    return 0;
error:
    return -1;
}

int
buffer_appendf(Buffer *buf, const char *format, ...)
{
    char *tmp = NULL;
    int bytes_written, status;

    va_list argp;
    va_start(argp, format);
	
    bytes_written = vasprintf(&tmp, format, argp);
    jump_to_error_if(bytes_written < 0);

    va_end(argp);

    status = buffer_append(buf, tmp, bytes_written);
    jump_to_error_unless(status == 0);

    free(tmp);

    return 0;
error:
    if (tmp != NULL) { free(tmp); }
    return -1;
}

int
buffer_nappendf(Buffer *buf, size_t length, const char *format, ...)
{
	va_list argp;
    int status        = 0;
	int printf_length = length + 1;
	
	char *tmp  = (char*)calloc(1, printf_length * sizeof(char));
    
	
    

    jump_to_error_if(tmp == NULL);

    
    va_start(argp, format);
#ifdef _WIN32
    status = _vsnprintf(tmp, printf_length, format, argp);
#else
	status = vsnprintf(tmp, printf_length, format, argp);
	jump_to_error_if(status < 0);
#endif
    

    va_end(argp);

    status = buffer_append(buf, tmp, length);
    jump_to_error_unless(status == 0);

    free(tmp);

    return 0;
error:
    if (tmp != NULL) { free(tmp); }
    return -1;

}

char *
buffer_to_s(Buffer *buf)
{
    char *result = calloc(1, buf->bytes_used + 1);
    strncpy(result, buf->contents, buffer_strlen(buf));

    return result;
}