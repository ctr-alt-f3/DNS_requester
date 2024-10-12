#include <arpa/inet.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define HOSTNAME "google.com"
#define DNS_SERVER "8.8.8.8"
#define BUFFSIZE 400
int main() {
  void *buff = malloc(BUFFSIZE);
  char *dns_server = malloc(20);
  char *hostname = malloc(20);
  strcpy(hostname, HOSTNAME);
  strcpy(dns_server, DNS_SERVER);
  res_init();
  _res.nscount = 1;
  _res.nsaddr_list[0].sin_family = AF_INET;
  inet_pton(AF_INET, dns_server, &_res.nsaddr_list[0].sin_addr);
  unsigned char answer[9000];
  int len;
  len = res_query(hostname, ns_c_in, ns_t_a, answer, sizeof(answer));
  if (len == -1) {
    perror("error:");
  }
  ns_msg handle;
  ns_initparse(answer, len, &handle);

  int rrcount = ns_msg_count(handle, ns_s_an);
  char ipstr[INET6_ADDRSTRLEN];
  ns_rr rr;
  for (int i = 0; i < rrcount; i++) {
    ns_parserr(&handle, ns_s_an, i, &rr);

    inet_ntop(AF_INET, ns_rr_rdata(rr), ipstr, sizeof(ipstr));
    printf("IP Address: %s\n", ipstr);
  }
  free(hostname);
  free(buff);
  free(dns_server);
  return 0;
};
