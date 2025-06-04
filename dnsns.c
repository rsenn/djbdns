#include "buffer.h"
#include "exit.h"
#include "strerr.h"
#include "byte.h"
#include "dns.h"

#define FATAL "dnsns: fatal: "

static char seed[128];

static stralloc fqdn;
static stralloc out;

int main(int argc,char **argv)
{
  int i;
  int j;

  dns_random_init(seed);

  if (*argv) ++argv;

  while (*argv) {
    if (!stralloc_copys(&fqdn,*argv))
      strerr_die2x(111,FATAL,"out of memory");
    if (dns_ns(&out,&fqdn) == -1)
      strerr_die4sys(111,FATAL,"unable to find NS records for ",*argv,": ");

    i = 0;
    while(i + 2 < out.len) {
      j = byte_chr(out.s + i,out.len - i,0);
      buffer_put(buffer_1,out.s + i,j);
      buffer_puts(buffer_1," ");
      i += j + 1;
    }

    buffer_puts(buffer_1,"\n");

    ++argv;
  }

  buffer_flush(buffer_1);
  _exit(0);
}
