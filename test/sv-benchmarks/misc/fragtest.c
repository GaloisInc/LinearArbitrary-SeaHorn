#include "seahorn/seahorn.h"

typedef unsigned int size_t;
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef long long __quad_t;
typedef long __off_t;
typedef __quad_t __off64_t;
typedef long __time_t;
typedef long __suseconds_t;
typedef int __ssize_t;
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __ssize_t ssize_t;
struct timeval {
   __time_t tv_sec ;
   __suseconds_t tv_usec ;
};
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef void (*__sighandler_t)(int  );
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef void _IO_lock_t;
struct _IO_marker {
   struct _IO_marker *_next ;
   struct _IO_FILE *_sbuf ;
   int _pos ;
};
struct _IO_FILE {
   int _flags ;
   char *_IO_read_ptr ;
   char *_IO_read_end ;
   char *_IO_read_base ;
   char *_IO_write_base ;
   char *_IO_write_ptr ;
   char *_IO_write_end ;
   char *_IO_buf_base ;
   char *_IO_buf_end ;
   char *_IO_save_base ;
   char *_IO_backup_base ;
   char *_IO_save_end ;
   struct _IO_marker *_markers ;
   struct _IO_FILE *_chain ;
   int _fileno ;
   int _flags2 ;
   __off_t _old_offset ;
   unsigned short _cur_column ;
   signed char _vtable_offset ;
   char _shortbuf[1] ;
   _IO_lock_t *_lock ;
   __off64_t _offset ;
   void *__pad1 ;
   void *__pad2 ;
   int _mode ;
   char _unused2[15U * sizeof(int ) - 2U * sizeof(void *)] ;
};
struct eth_addr {
   uint8_t data[6] ;
};
typedef struct eth_addr eth_addr_t;
struct eth_hdr {
   eth_addr_t eth_dst ;
   eth_addr_t eth_src ;
   uint16_t eth_type ;
};
struct eth_handle;
typedef struct eth_handle eth_t;
typedef uint32_t ip_addr_t;
struct ip_hdr {
   uint8_t ip_hl : 4 ;
   uint8_t ip_v : 4 ;
   uint8_t ip_tos ;
   uint16_t ip_len ;
   uint16_t ip_id ;
   uint16_t ip_off ;
   uint8_t ip_ttl ;
   uint8_t ip_p ;
   uint16_t ip_sum ;
   ip_addr_t ip_src ;
   ip_addr_t ip_dst ;
};
struct ip6_addr {
   uint8_t data[16] ;
};
typedef struct ip6_addr ip6_addr_t;
union __anonunion___addr_u_50 {
   eth_addr_t __eth ;
   ip_addr_t __ip ;
   ip6_addr_t __ip6 ;
   uint8_t __data8[16] ;
   uint16_t __data16[8] ;
   uint32_t __data32[4] ;
};
struct addr {
   uint16_t addr_type ;
   uint16_t addr_bits ;
   union __anonunion___addr_u_50 __addr_u ;
};
struct arp_entry {
   struct addr arp_pa ;
   struct addr arp_ha ;
};
struct arp_handle;
typedef struct arp_handle arp_t;
struct icmp_hdr {
   uint8_t icmp_type ;
   uint8_t icmp_code ;
   uint16_t icmp_cksum ;
};
struct icmp_msg_echo {
   uint16_t icmp_id ;
   uint16_t icmp_seq ;
   uint8_t icmp_data[] ;
};
struct icmp_msg_needfrag {
   uint16_t icmp_void ;
   uint16_t icmp_mtu ;
   uint8_t icmp_ip[] ;
};
struct icmp_msg_quote {
   uint32_t icmp_void ;
   uint8_t icmp_ip[] ;
};
struct icmp_msg_rtr_data {
   uint32_t icmp_void ;
   uint32_t icmp_pref ;
};
struct icmp_msg_rtradvert {
   uint8_t icmp_num_addrs ;
   uint8_t icmp_wpa ;
   uint16_t icmp_lifetime ;
   struct icmp_msg_rtr_data icmp_rtr[] ;
};
struct icmp_msg_tstamp {
   uint32_t icmp_id ;
   uint32_t icmp_seq ;
   uint32_t icmp_ts_orig ;
   uint32_t icmp_ts_rx ;
   uint32_t icmp_ts_tx ;
};
struct icmp_msg_mask {
   uint32_t icmp_id ;
   uint32_t icmp_seq ;
   uint32_t icmp_mask ;
};
struct icmp_msg_traceroute {
   uint16_t icmp_id ;
   uint16_t icmp_void ;
   uint16_t icmp_ohc ;
   uint16_t icmp_rhc ;
   uint32_t icmp_speed ;
   uint32_t icmp_mtu ;
};
struct icmp_msg_dnsreply {
   uint16_t icmp_id ;
   uint16_t icmp_seq ;
   uint32_t icmp_ttl ;
   uint8_t icmp_names[] ;
};
struct icmp_msg_idseq {
   uint16_t icmp_id ;
   uint16_t icmp_seq ;
};
union icmp_msg {
   struct icmp_msg_echo echo ;
   struct icmp_msg_quote unreach ;
   struct icmp_msg_needfrag needfrag ;
   struct icmp_msg_quote srcquench ;
   struct icmp_msg_quote redirect ;
   uint32_t rtrsolicit ;
   struct icmp_msg_rtradvert rtradvert ;
   struct icmp_msg_quote timexceed ;
   struct icmp_msg_quote paramprob ;
   struct icmp_msg_tstamp tstamp ;
   struct icmp_msg_idseq info ;
   struct icmp_msg_mask mask ;
   struct icmp_msg_traceroute traceroute ;
   struct icmp_msg_idseq dns ;
   struct icmp_msg_dnsreply dnsreply ;
};
struct tcp_hdr {
   uint16_t th_sport ;
   uint16_t th_dport ;
   uint32_t th_seq ;
   uint32_t th_ack ;
   uint8_t th_x2 : 4 ;
   uint8_t th_off : 4 ;
   uint8_t th_flags ;
   uint16_t th_win ;
   uint16_t th_sum ;
   uint16_t th_urp ;
};
struct udp_hdr {
   uint16_t uh_sport ;
   uint16_t uh_dport ;
   uint16_t uh_ulen ;
   uint16_t uh_sum ;
};
struct intf_entry {
   u_int intf_len ;
   char intf_name[16] ;
   u_short intf_type ;
   u_short intf_flags ;
   u_int intf_mtu ;
   struct addr intf_addr ;
   struct addr intf_dst_addr ;
   struct addr intf_link_addr ;
   u_int intf_alias_num ;
   struct addr intf_alias_addrs[] ;
};
struct intf_handle;
typedef struct intf_handle intf_t;
struct route_entry {
   struct addr route_dst ;
   struct addr route_gw ;
};
struct route_handle;
typedef struct route_handle route_t;
struct tun;
struct event_base;
struct __anonstruct_ev_next_51 {
   struct event *tqe_next ;
   struct event **tqe_prev ;
};
struct __anonstruct_ev_active_next_52 {
   struct event *tqe_next ;
   struct event **tqe_prev ;
};
struct __anonstruct_ev_signal_next_53 {
   struct event *tqe_next ;
   struct event **tqe_prev ;
};
struct event {
   struct __anonstruct_ev_next_51 ev_next ;
   struct __anonstruct_ev_active_next_52 ev_active_next ;
   struct __anonstruct_ev_signal_next_53 ev_signal_next ;
   unsigned int min_heap_idx ;
   struct event_base *ev_base ;
   int ev_fd ;
   short ev_events ;
   short ev_ncalls ;
   short *ev_pncalls ;
   struct timeval ev_timeout ;
   int ev_pri ;
   void (*ev_callback)(int  , short  , void *arg ) ;
   void *ev_arg ;
   int ev_res ;
   int ev_flags ;
};
union __anonunion_pkt_n_hdr_u_55 {
   u_char *eth_data ;
   struct ip_hdr *ip ;
};
union __anonunion_pkt_t_hdr_u_56 {
   u_char *ip_data ;
   struct icmp_hdr *icmp ;
   struct tcp_hdr *tcp ;
   struct udp_hdr *udp ;
};
union __anonunion_pkt_t_data_u_57 {
   u_char *t_data ;
   union icmp_msg *icmp ;
};
struct __anonstruct_pkt_next_58 {
   struct pkt *tqe_next ;
   struct pkt **tqe_prev ;
};
struct pkt {
   struct timeval pkt_ts ;
   struct event pkt_ev ;
   struct eth_hdr *pkt_eth ;
   union __anonunion_pkt_n_hdr_u_55 pkt_n_hdr_u ;
   union __anonunion_pkt_t_hdr_u_56 pkt_t_hdr_u ;
   union __anonunion_pkt_t_data_u_57 pkt_t_data_u ;
   u_char pkt_buf[1516] ;
   u_char *pkt_data ;
   u_char *pkt_end ;
   struct __anonstruct_pkt_next_58 pkt_next ;
};
struct pktq {
   struct pkt *tqh_first ;
   struct pkt **tqh_last ;
};
typedef struct tun tun_t___0;
struct fr_ctx {
   struct addr src ;
   struct addr dst ;
   struct addr smac ;
   struct addr dmac ;
   int mtu ;
   arp_t *arp ;
   eth_t *eth ;
   intf_t *intf ;
   route_t *route ;
   tun_t___0 *tun ;
};
typedef long bufsize;
struct bfhead;
struct qlinks {
   struct bfhead *flink ;
   struct bfhead *blink ;
};
struct bhead {
   bufsize prevfree ;
   bufsize bsize ;
};
struct bdhead {
   bufsize tsize ;
   struct bhead bh ;
};
struct bfhead {
   struct bhead bh ;
   struct qlinks ql ;
};
struct mod {
   char *name ;
   char *usage ;
   void *(*open)(int argc , char **argv ) ;
   int (*apply)(void *data , struct pktq *pktq ) ;
   void *(*close)(void *data ) ;
};
struct __anonstruct_next_37 {
   struct rule *tqe_next ;
   struct rule **tqe_prev ;
};
struct rule {
   struct mod *mod ;
   void *data ;
   struct __anonstruct_next_37 next ;
};
struct head {
   struct rule *tqh_first ;
   struct rule **tqh_last ;
};
typedef int bpf_int32;
typedef u_int bpf_u_int32;
struct bpf_insn;
struct bpf_program {
   u_int bf_len ;
   struct bpf_insn *bf_insns ;
};
struct bpf_insn {
   u_short code ;
   u_char jt ;
   u_char jf ;
   bpf_int32 k ;
};
typedef __builtin_va_list __gnuc_va_list;
struct pcap;
typedef struct pcap pcap_t;
typedef __gnuc_va_list va_list;
typedef __off_t off_t;
struct rand_handle;
typedef struct rand_handle rand_t;
typedef unsigned long long uint64_t;
struct delay_data {
   rand_t *rnd ;
   int which ;
   struct timeval tv ;
};
struct drop_data {
   rand_t *rnd ;
   int which ;
   int percent ;
};
struct dup_data {
   rand_t *rnd ;
   int which ;
   int percent ;
};
struct ip_opt_data_sec {
   uint16_t s ;
   uint16_t c ;
   uint16_t h ;
   uint8_t tcc[3] ;
} __attribute__((__packed__)) ;
struct ip_opt_data_rr {
   uint8_t ptr ;
   uint32_t iplist[] ;
} __attribute__((__packed__)) ;
struct ip_opt_data_ts {
   uint8_t ptr ;
   uint8_t flg : 4 ;
   uint8_t oflw : 4 ;
   uint32_t ipts[] ;
} __attribute__((__packed__)) ;
struct ip_opt_data_tr {
   uint16_t id ;
   uint16_t ohc ;
   uint16_t rhc ;
   uint32_t origip ;
} __attribute__((__packed__)) ;
union ip_opt_data {
   struct ip_opt_data_sec sec ;
   struct ip_opt_data_rr rr ;
   struct ip_opt_data_ts ts ;
   uint16_t satid ;
   uint16_t mtu ;
   struct ip_opt_data_tr tr ;
   uint32_t addext[2] ;
   uint16_t rtralt ;
   uint32_t sdb[9] ;
   uint8_t data8[38] ;
};
struct ip_opt {
   uint8_t opt_type ;
   uint8_t opt_len ;
   union ip_opt_data opt_data ;
} __attribute__((__packed__)) ;
struct ip_chaff_data {
   rand_t *rnd ;
   int type ;
   int ttl ;
   struct pktq *pktq ;
};
struct ip_frag_data {
   rand_t *rnd ;
   int size ;
   int overlap ;
};
struct ip_ttl_data {
   int ttl ;
};
struct ip_tos_data {
   int tos ;
};
struct order_data {
   rand_t *rnd ;
   int type ;
};
typedef unsigned long __u_long;
typedef __u_long u_long;
union tcp_opt_data {
   uint16_t mss ;
   uint8_t wscale ;
   uint16_t sack[19] ;
   uint32_t echo ;
   uint32_t timestamp[2] ;
   uint32_t cc ;
   uint8_t cksum ;
   uint8_t md5[16] ;
   uint8_t data8[38] ;
};
struct tcp_opt {
   uint8_t opt_type ;
   uint8_t opt_len ;
   union tcp_opt_data opt_data ;
} __attribute__((__packed__)) ;
struct tcp_chaff_data {
   rand_t *rnd ;
   int type ;
   int ttl ;
};
struct tcp_seg_data {
   rand_t *rnd ;
   int size ;
   int overlap ;
};
struct pcap_pkthdr {
   struct timeval ts ;
   bpf_u_int32 caplen ;
   bpf_u_int32 len ;
};
struct tun {
   intf_t *intf ;
   pcap_t *pcap ;
   route_t *route ;
   struct route_entry rtent ;
   struct intf_entry *ifent ;
   u_char buf[1024] ;
   int dloff ;
   int fd ;
   struct event ev ;
   void (*callback)(void *buf , size_t len , void *arg ) ;
   void *arg ;
};

static struct pkt **pvbase  ;
static int pvlen  ;
void main (int argh, char** argv) 
//(rand_t *r , struct pktq *pktq ) 
{ 
  rand_t * r = (rand_t*)argv[0];
  struct pktq* pktq = (struct pktq*)argv[1];
  struct pkt *pkt ;
  int i ;
  void *tmp ;
  void *tmp___0 ;
  int tmp___1 ;
  int k = 0;
  int n;
  {
  i = 0;
  pkt = pktq->tqh_first;
  while (1) {
    if ((unsigned int )pkt != (unsigned int )((void *)0)) {

    } else {
      break;
    }
    i = i + 1;
    pkt = pkt->pkt_next.tqe_next;
  }
  if (i > pvlen) {
    pvlen = i;
    if ((unsigned int )pvbase == (unsigned int )((void *)0)) {
      tmp = malloc(sizeof(pkt) * (unsigned int )pvlen);
      pvbase = (struct pkt **)tmp;
    } else {
      tmp___0 = realloc((void *)pvbase, sizeof(pkt) * (unsigned int )pvlen);
      pvbase = (struct pkt **)tmp___0;
    }
  } else {

  }
  i = 0;
  pkt = pktq->tqh_first;
  while (1) {
    if ((unsigned int )pkt != (unsigned int )((void *)0)) {

    } else {
      break;
    }
    tmp___1 = i;
    i = i + 1;
    k = k +1;
    *(pvbase + tmp___1) = pkt;
    pkt = pkt->pkt_next.tqe_next;
  }
  while (1) {
    pktq->tqh_first = (struct pkt *)((void *)0);
    pktq->tqh_last = & pktq->tqh_first;
    break;
  }
  int j = 0;
  n = i;
  rand_shuffle(r, (void *)pvbase, (unsigned int )i, sizeof(pkt));
  while (1) {

    sassert(k >= 0);
    k = k -1;
    i = i - 1;
    j = j + 1;
    if (j < n) {

    } else {
      break;
    }

    while (1) {
      (*(pvbase + i))->pkt_next.tqe_next = (struct pkt *)((void *)0);
      (*(pvbase + i))->pkt_next.tqe_prev = pktq->tqh_last;
      *(pktq->tqh_last) = *(pvbase + i);
      pktq->tqh_last = & (*(pvbase + i))->pkt_next.tqe_next;
      break;
    }
  }
  return;
}
}
