/******************************************************************************
*
* Copyright (C) 2009 - 2017 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/** SERVER **/

/* Standard library includes */
#include <stdio.h>
#include <string.h>

/* BSP includes for peripherals */
#include "xparameters.h"
#include "netif/xadapter.h"

#include "platform.h"
#include "platform_config.h"
#if defined(__arm__) || defined(__aarch64__)
#include "xil_printf.h"
#endif
#include "xil_cache.h"

#include "xil_io.h"

/* LWIP include files */
#include "lwip/ip_addr.h"
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#if LWIP_IPV6 == 1
#include "lwip/ip.h"
#else
#if LWIP_DHCP == 1
#include "lwip/dhcp.h"
#endif
#endif

/* missing declaration in lwIP */
void lwip_init();

/* TCP Network Params */
#define SERVER_MAC_ADDR                    \
    {                                      \
        0x00, 0x0a, 0x35, 0x00, 0x03, 0x07 \
    }
#define SERVER_IP4_ADDR "192.168.1.11"
#define IP4_NETMASK "255.255.255.0"
#define IP4_GATEWAY "192.168.1.1"
#define SERVER_PORT 9090

/* Function prototypes */
#if LWIP_IPV6 == 1
void print_ip6(char *msg, ip_addr_t *ip);
#else
void print_ip(char *msg, ip_addr_t *ip);
void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
#endif
int setup_client_conn();
void tcp_fasttmr(void);
void tcp_slowtmr(void);

/* Function prototypes for callbacks */
static err_t server_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t server_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t server_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);

static err_t start_server();

/* DHCP global variables */
#if LWIP_IPV6 == 0
#if LWIP_DHCP == 1
extern volatile int dhcp_timoutcntr;
err_t dhcp_start(struct netif *netif);
#endif
#endif

/* Networking global variables */
extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;
static struct netif server_netif;
struct netif *app_netif;
static struct tcp_pcb *c_pcb;
char is_connected;

#define BOARD_CONTROL_BASE_ADDR 0x44a00000
static volatile u32 server_stored_value = 0xdeadbeef;

#define PACKET_SIZE 1024
#define IMAGE_SIZE (640 * 480 * 4)
#define IMAGE_SEND_BUF_TOTAL_SIZE IMAGE_SIZE

static volatile unsigned image_send_buf_bytes_sent = 0;
static volatile unsigned sending_image = 0;
static u8 *image_send_buf = NULL;

static u8 *image_0 = NULL;
static u8 *image_1 = NULL;

int main()
{
	xil_printf("========== SERVER ==========\n");

	image_0 = (u8*)malloc(IMAGE_SEND_BUF_TOTAL_SIZE);
	image_1 = (u8*)malloc(IMAGE_SEND_BUF_TOTAL_SIZE);
	// image_send_buf = (u8*)malloc(IMAGE_SEND_BUF_TOTAL_SIZE);
	xil_printf("TCP server: image_0=%p\n", image_0);
	xil_printf("TCP server: image_1=%p\n", image_1);
	image_send_buf = image_0;


	// Manual create the image
	for (int j = 0; j < 480; j++){
		for (int i = 0; i < 640; i++){
			unsigned off = (j * 640 + i) * 4;
			if((i - 320) * (i - 320) + (j - 240) * (j - 240) < 10000 && (i - 320) * (i - 320) + (j - 240) * (j - 240) > 8100){
				image_0[off + 0] = 0xff;
				image_0[off + 1] = 0xff;
				image_0[off + 2] = 0xff;
				image_0[off + 3] = 0xff;

				image_1[off + 0] = 0x0;
				image_1[off + 1] = 0x0;
				image_1[off + 2] = 0x0;
				image_1[off + 3] = 0x0;
			} else{
				image_0[off + 0] = 0x0;
				image_0[off + 1] = 0x0;
				image_0[off + 2] = 0x0;
				image_0[off + 3] = 0x0;

				image_1[off + 0] = 0xff;
				image_1[off + 1] = 0xff;
				image_1[off + 2] = 0xff;
				image_1[off + 3] = 0xff;
			}
		}
	}

	xil_printf("TCP server: image ready!\n");

    /* Varibales for IP parameters */
#if LWIP_IPV6 == 0
    ip_addr_t ipaddr, netmask, gw;
#endif

    /* The mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address[] = SERVER_MAC_ADDR;

    /* Network interface */
    app_netif = &server_netif;

    /* Initialize platform */
    init_platform();

    /* Defualt IP parameter values */
#if LWIP_IPV6 == 0
#if LWIP_DHCP == 1
    ipaddr.addr = 0;
    gw.addr = 0;
    netmask.addr = 0;
#else
    (void)inet_aton(SERVER_IP4_ADDR, &ipaddr);
    (void)inet_aton(IP4_NETMASK, &netmask);
    (void)inet_aton(IP4_GATEWAY, &gw);
#endif
#endif

    /* LWIP initialization */
    lwip_init();

    /* Setup Network interface and add to netif_list */
#if (LWIP_IPV6 == 0)
    if (!xemac_add(app_netif, &ipaddr, &netmask,
                   &gw, mac_ethernet_address,
                   PLATFORM_EMAC_BASEADDR))
    {
        xil_printf("Error adding N/W interface\n");
        return -1;
    }
#else
    if (!xemac_add(app_netif, NULL, NULL, NULL, mac_ethernet_address,
                   PLATFORM_EMAC_BASEADDR))
    {
        xil_printf("Error adding N/W interface\n");
        return -1;
    }
    app_netif->ip6_autoconfig_enabled = 1;

    netif_create_ip6_linklocal_address(app_netif, 1);
    netif_ip6_addr_set_state(app_netif, 0, IP6_ADDR_VALID);

#endif
    netif_set_default(app_netif);

    /* Now enable interrupts */
    platform_enable_interrupts();

    /* Specify that the network is up */
    netif_set_up(app_netif);

#if (LWIP_IPV6 == 0)
#if (LWIP_DHCP == 1)
    /* Create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */
    dhcp_start(app_netif);
    dhcp_timoutcntr = 24;

    while (((app_netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0))
        xemacif_input(app_netif);

    if (dhcp_timoutcntr <= 0)
    {
        if ((app_netif->ip_addr.addr) == 0)
        {
            xil_printf("DHCP Timeout\n");
            xil_printf("Configuring default IP of %s\n", SERVER_IP4_ADDR);
            (void)inet_aton(SERVER_IP4_ADDR, &(app_netif->ip_addr));
            (void)inet_aton(IP4_NETMASK, &(app_netif->netmask));
            (void)inet_aton(IP4_GATEWAY, &(app_netif->gw));
        }
    }

    ipaddr.addr = app_netif->ip_addr.addr;
    gw.addr = app_netif->gw.addr;
    netmask.addr = app_netif->netmask.addr;
#endif
#endif

    /* Print connection settings */
#if (LWIP_IPV6 == 0)
    print_ip_settings(&ipaddr, &netmask, &gw);
#else
    print_ip6("Board IPv6 address ", &app_netif->ip6_addr[0].u_addr.ip6);
#endif

    /* Setup connection */
    start_server();

    /* Event loop */
    while (1)
    {
        /* Call tcp_tmr functions, must be called regularly */
        if (TcpFastTmrFlag)
        {
            tcp_fasttmr();
            TcpFastTmrFlag = 0;
        }
        if (TcpSlowTmrFlag)
        {
            tcp_slowtmr();
            TcpSlowTmrFlag = 0;
        }

        /* Process data queued after interupt */
        xemacif_input(app_netif);
    }

    free(image_0);
    free(image_1);
    image_0 = NULL;
    image_1 = NULL;
    image_send_buf = NULL;

    /* Never reached */
    cleanup_platform();

    return 0;
}

#if LWIP_IPV6 == 1
void print_ip6(char *msg, ip_addr_t *ip)
{
    print(msg);
    xil_printf(" %x:%x:%x:%x:%x:%x:%x:%x\n",
               IP6_ADDR_BLOCK1(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK2(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK3(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK4(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK5(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK6(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK7(&ip->u_addr.ip6),
               IP6_ADDR_BLOCK8(&ip->u_addr.ip6));
}
#else
void print_ip(char *msg, ip_addr_t *ip)
{
    print(msg);
    xil_printf("%d.%d.%d.%d\n", ip4_addr1(ip), ip4_addr2(ip),
               ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{

    print_ip("Board IP: ", ip);
    print_ip("Netmask : ", mask);
    print_ip("Gateway : ", gw);
}
#endif

static err_t start_server()
{
    struct tcp_pcb *pcb;
    err_t err;

    /* create new TCP PCB structure */
    pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb)
    {
        xil_printf("Error creating PCB. Out of Memory\n\r");
        return -1;
    }

    /* bind to specified @port */
    err = tcp_bind(pcb, IP_ANY_TYPE, SERVER_PORT);
    if (err != ERR_OK)
    {
        xil_printf("Unable to bind to port %d: err = %d\n\r", SERVER_PORT, err);
        return -2;
    }

    /* we do not need any arguments to callback functions */
    tcp_arg(pcb, NULL);

    /* listen for connections */
    pcb = tcp_listen(pcb);
    if (!pcb)
    {
        xil_printf("Out of memory while tcp_listen\n\r");
        return -3;
    }

    /* specify callback to use for incoming connections */
    tcp_accept(pcb, server_accept_callback);

    xil_printf("TCP server started @ port %d\n\r", SERVER_PORT);

    return 0;
}

static err_t server_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    static int connection = 1;

    tcp_sent(newpcb, server_sent_callback);

    /* set the receive callback for this connection */
    tcp_recv(newpcb, server_recv_callback);

    /* just use an integer number indicating the connection id as the
	   callback argument */
    tcp_arg(newpcb, (void *)(UINTPTR)connection);

    tcp_nagle_disable(newpcb);

    /* increment for subsequent accepted connections */
    connection++;

    /* store state */
    c_pcb = newpcb;

    xil_printf("TCP Server: Connection with client established.\n");

    return ERR_OK;
}

static err_t server_recv_callback(void *arg, struct tcp_pcb *tpcb,
                                  struct pbuf *p, err_t err)
{
    //	/* do not read the packet if we are not in ESTABLISHED state */
    //	if (!p) {
    //		tcp_close(tpcb);
    //		tcp_recv(tpcb, NULL);
    //		return ERR_OK;
    //	}

    if (!p)
    {
        xil_printf("No data received\n");
        return 0;
    }

    //	/* indicate that the packet has been received */
    //	tcp_recved(tpcb, p->len);
    //
    //	/* echo back the payload */
    //	/* in this case, we assume that the payload is < TCP_SND_BUF */
    //	if (tcp_sndbuf(tpcb) > p->len) {
    //		err = tcp_write(tpcb, p->payload, p->len, 1);
    //	} else
    //		xil_printf("no space in tcp_sndbuf\n\r");
    //
    //	/* free the received pbuf */
    //	pbuf_free(p);
    //
    //	return ERR_OK;

    /* Print message */
    xil_printf("Packet received, %d bytes\n", p->tot_len);

    /* Print packet contents to terminal */
    unsigned char *packet_data = (unsigned char *)malloc(p->tot_len);
    pbuf_copy_partial(p, packet_data, p->tot_len, 0); //Note - inefficient way to access packet data
    u32_t i;

    for (i = 0; i < p->tot_len; i = i + 1)
        putchar(packet_data[i]);

    if (p->tot_len == 3 && packet_data[0] == 'G' && packet_data[1] == 'E' && packet_data[2] == 'T' && sending_image == 0)
    {
        /* GET request */
        /* Send server_stored_value back */
        xil_printf("Received <GET>, going to send image\n", server_stored_value);

        u8 send_buf[4];
        /* Send the first four bytes of image */
		send_buf[0] = IMAGE_SIZE >> 24 & 0xff;
		send_buf[1] = IMAGE_SIZE >> 16 & 0xff;
		send_buf[2] = IMAGE_SIZE >> 8 & 0xff;
		send_buf[3] = IMAGE_SIZE & 0xff;

		/* Loop until enough room in buffer (should be right away) */
		while (tcp_sndbuf(c_pcb) < 4);

		/* Enqueue some data to send */
		u8_t apiflags = TCP_WRITE_FLAG_COPY;

		unsigned err = tcp_write(c_pcb, send_buf, 4, apiflags);
		if (err != ERR_OK)
		{
			xil_printf("TCP server: Error on tcp_write: %d\n", err);
		}

		/* Send the data packet */
		err = tcp_output(c_pcb);
		if (err != ERR_OK)
		{
			xil_printf("TCP server: Error on tcp_output: %d\n", err);
		}

		xil_printf("TCP server: Sent %d bytes in total \n", 4);

		sending_image = 1;
		image_send_buf_bytes_sent = 0;
    }
    else if (p->tot_len == 8 && packet_data[0] == 'P' && packet_data[1] == 'O' && packet_data[2] == 'S' && packet_data[3] == 'T')
    {
        /* POST request */
        u32 received_word = 0;
        received_word |= 0xff & packet_data[4];
        received_word <<= 8;
        received_word |= 0xff & packet_data[5];
        received_word <<= 8;
        received_word |= 0xff & packet_data[6];
        received_word <<= 8;
        received_word |= 0xff & packet_data[7];

        xil_printf("Received <POST%x>\n", received_word);

        /* Store the received_word */
        server_stored_value = received_word;
    }
    else
    {
        xil_printf("Unsupported packet!\n");
    }

    /* Indicate done processing */
    tcp_recved(tpcb, p->tot_len);

    /* Free the received pbuf */
    pbuf_free(p);

    return 0;
}

static err_t server_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len) {
	if (sending_image) {
		if(image_send_buf_bytes_sent == IMAGE_SEND_BUF_TOTAL_SIZE) {
			xil_printf("TCP server: Image sent: (%d bytes/%d bytes)\n", image_send_buf_bytes_sent, IMAGE_SIZE);
			image_send_buf_bytes_sent = 0;
			sending_image = 0;

			if(image_send_buf == image_0)
				image_send_buf = image_1;
			else
				image_send_buf = image_0;

			return ERR_OK;
		}

		unsigned bytes_to_send = (IMAGE_SEND_BUF_TOTAL_SIZE - image_send_buf_bytes_sent) < PACKET_SIZE ?  (IMAGE_SEND_BUF_TOTAL_SIZE - image_send_buf_bytes_sent) : PACKET_SIZE;

		/* Loop until enough room in buffer (should be right away) */
		while (tcp_sndbuf(c_pcb) < bytes_to_send)
			xil_printf("dead loop tcp_sndbuf(c_pcb)=%d, bytes_to_send=%d\n", tcp_sndbuf(c_pcb), bytes_to_send);

		/* Enqueue some data to send */
		// u8_t apiflags = TCP_WRITE_FLAG_COPY;
		u8_t apiflags = 0;

		unsigned err = tcp_write(c_pcb, image_send_buf + image_send_buf_bytes_sent, bytes_to_send, apiflags);
		if (err != ERR_OK)
		{
			xil_printf("TCP server: Error on tcp_write: %d\n", err);
		}

		/* Send the data packet */
		err = tcp_output(c_pcb);
		if (err != ERR_OK)
		{
			xil_printf("TCP server: Error on tcp_output: %d\n", err);
		}

		image_send_buf_bytes_sent += bytes_to_send;
		// xil_printf("TCP server: Sent %d bytes in total \n", image_send_buf_bytes_sent);
	}

	return ERR_OK;
}
