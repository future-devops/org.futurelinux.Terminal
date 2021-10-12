/* Author: Fuat Bölük <mektup@fuatboluk.com.tr> */


/* UTF-8 Decode */
void utf8_decode(char *dst, const char *src) 
{
    char a, b;
    if (dst == NULL) return;
    while(*src) 
    {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) 
        {
            if (a >= 'a') { a -= 'a'-'A';  }
            if (a >= 'A') { a -= ('A'-10); } else { a -= '0'; }
            if (b >= 'a') { b -= 'a'-'A';  }
            if (b >= 'A') { b -= ('A'-10); } else { b -= '0'; }
            *dst++ = 16 * a + b;
            src+=3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}


/* End */
