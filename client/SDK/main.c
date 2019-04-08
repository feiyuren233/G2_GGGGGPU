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

/** CLIENT **/

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
#include "board_control.h"
#include "vga_graphics.h"

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
#define SRC_MAC_ADDR                       \
    {                                      \
        0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 \
    }
#define SRC_IP4_ADDR "192.168.1.10"
#define IP4_NETMASK "255.255.255.0"
#define IP4_GATEWAY "192.168.1.1"
#define SRC_PORT 7

#define DEST_IP4_ADDR "192.168.1.11"
#define DEST_IP6_ADDR "fe80::6600:6aff:fe71:fde6"
#define DEST_PORT 9090

#define TCP_SEND_BUFSIZE 20

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
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_client_err(void *arg, err_t err);
static void tcp_client_close(struct tcp_pcb *pcb);

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
static volatile unsigned char bc_get_request_on = 0;
static volatile unsigned char bc_get_request_handled = 0;

#define IMAGE_SIZE (640 * 480 * 4)
#define RCV_BUF_TOTAL_SIZE (IMAGE_SIZE + 4)

static volatile unsigned received_buf_bytes = 0;
static volatile unsigned rx_total_bytes = 0;
static volatile unsigned receiving_buf = 0;
static u8 *rcv_buf = NULL;

int main()
{
	initGraphics();
//	Color lettercolor = { .R = 0x00, .G = 0xa, .B = 0xf };
//
//	// "E"
//	drawFilledRect(50, 95, 160, 60, lettercolor);
//	drawFilledRect(50, 215, 160, 60, lettercolor);
//	drawFilledRect(50, 335, 160, 60, lettercolor);
//	drawFilledRect(50, 95, 45, 300, lettercolor);
//
//	// "C"
//	drawFilledRect(240, 95, 60, 300, lettercolor);
//	drawFilledRect(240, 95, 160, 80, lettercolor);
//	drawFilledRect(240, 315, 160, 80, lettercolor);
//
//	// "E"
//	drawFilledRect(430, 95, 45, 300, lettercolor);
//	drawFilledRect(430, 95, 160, 60, lettercolor);
//	drawFilledRect(430, 215, 160, 60, lettercolor);
//	drawFilledRect(430, 335, 160, 60, lettercolor);
//
//	Color circlecolor = { .R = 0xd, .G = 0x4, .B = 0x9 };
//	Color black = { .R = 0x0, .G = 0x0, .B = 0x0 };
//	u32 x_pos, y_pos, old_x_pos = 0, old_y_pos = 0;
//	u32 radius = 20;
//	u32 speed = 48;
//	// a very simple animation that uses loop as delay
//	while (1) {
//		for (y_pos = radius; y_pos < 95 - radius; y_pos += speed * 2) {
//			for (x_pos = radius; x_pos < 480 - radius; x_pos += speed) {
//
//				// erase the old moving circle
//				// drawFilledCircle(old_x_pos, old_y_pos, radius, black);
//				drawFilledRect(old_x_pos-radius, old_y_pos-radius, 2*radius, 2*radius, black);
//				old_x_pos = x_pos;
//				old_y_pos = y_pos;
//
//				// a moving circle
//				// drawFilledCircle(x_pos, y_pos, radius, circlecolor);
//				drawFilledRect(x_pos-radius, y_pos-radius, 2*radius, 2*radius, circlecolor);
//			}
//
//			// randomly changes the color for the moving circle
//			circlecolor.R = (circlecolor.B + 1) & 0x0f;
//			circlecolor.G = (circlecolor.R + 2) & 0x0f;
//			circlecolor.B =(circlecolor.B + 3) & 0x0f;
//
//			drawFilledRect(50, 95, 160, 60, circlecolor);
//			drawFilledRect(50, 215, 160, 60, circlecolor);
//			drawFilledRect(50, 335, 160, 60, circlecolor);
//			drawFilledRect(50, 95, 45, 300, circlecolor);
//
//			// "C"
//			drawFilledRect(240, 95, 60, 300, circlecolor);
//			drawFilledRect(240, 95, 160, 80, circlecolor);
//			drawFilledRect(240, 315, 160, 80, circlecolor);
//
//			// "E"
//			drawFilledRect(430, 95, 45, 300, circlecolor);
//			drawFilledRect(430, 95, 160, 60, circlecolor);
//			drawFilledRect(430, 215, 160, 60, circlecolor);
//			drawFilledRect(430, 335, 160, 60, circlecolor);
//		}
//	}

	xil_printf("========== CLIENT ==========\n");

	rcv_buf = (u8*)malloc(RCV_BUF_TOTAL_SIZE);
	xil_printf("TCP client: rcv_buf=%p\n", rcv_buf);

    /* Varibales for IP parameters */
#if LWIP_IPV6 == 0
    ip_addr_t ipaddr, netmask, gw;
#endif

    /* The mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address[] = SRC_MAC_ADDR;

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
    (void)inet_aton(SRC_IP4_ADDR, &ipaddr);
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
            xil_printf("Configuring default IP of %s\n", SRC_IP4_ADDR);
            (void)inet_aton(SRC_IP4_ADDR, &(app_netif->ip_addr));
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
    setup_client_conn();

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

        unsigned bc_valid = BOARD_CONTROL_mReadReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG3_OFFSET);
        if (bc_valid)
        {
            unsigned bc_transmit = BOARD_CONTROL_mReadReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG2_OFFSET);

            if (bc_transmit)
            {
                u32 bc_transmit_data = BOARD_CONTROL_mReadReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG0_OFFSET);
                xil_printf("Board Control POST request: %x\n", bc_transmit_data);

                /* Just send a single packet */
                u8_t apiflags = TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE;
                char send_buf[8];

                send_buf[0] = 'P';
                send_buf[1] = 'O';
                send_buf[2] = 'S';
                send_buf[3] = 'T';
                send_buf[4] = bc_transmit_data >> 24 & 0xff;
                send_buf[5] = bc_transmit_data >> 16 & 0xff;
                send_buf[6] = bc_transmit_data >> 8 & 0xff;
                send_buf[7] = bc_transmit_data & 0xff;

                /* Loop until enough room in buffer (should be right away) */
                while (tcp_sndbuf(c_pcb) < 8)
                    ;

                /* Enqueue some data to send */
                unsigned err = tcp_write(c_pcb, send_buf, 8, apiflags);
                if (err != ERR_OK)
                {
                    xil_printf("TCP client: Error on tcp_write: %d\n", err);
                }

                /* send the data packet */
                err = tcp_output(c_pcb);
                if (err != ERR_OK)
                {
                    xil_printf("TCP client: Error on tcp_output: %d\n", err);
                }

                xil_printf("Packet data sent\n");

                /* ack board control */
                BOARD_CONTROL_mWriteReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG4_OFFSET, 0x01);
                unsigned delay = 0;
                for (delay = 0; delay < 5; delay++)
                {
                }
                /* remove the ack sig */
                BOARD_CONTROL_mWriteReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG4_OFFSET, 0x00);
            }
            else
            {
                if (!bc_get_request_on)
                {
                    xil_printf("Board Control GET request.\n");

                    bc_get_request_on = 1;
                    bc_get_request_handled = 0;

                    /* Just send a single packet */
                    u8_t apiflags = TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE;
                    char send_buf[3];

                    send_buf[0] = 'G';
                    send_buf[1] = 'E';
                    send_buf[2] = 'T';

                    /* Loop until enough room in buffer (should be right away) */
                    while (tcp_sndbuf(c_pcb) < 3)
                        ;

                    /* Enqueue some data to send */
                    unsigned err = tcp_write(c_pcb, send_buf, 3, apiflags);
                    if (err != ERR_OK)
                    {
                        xil_printf("TCP client: Error on tcp_write: %d\n", err);
                    }

                    /* send the data packet */
                    err = tcp_output(c_pcb);
                    if (err != ERR_OK)
                    {
                        xil_printf("TCP client: Error on tcp_output: %d\n", err);
                    }

                    xil_printf("Packet data sent\n");
                }

                /* wait for response, only ack board control if the bc_get_request has been handled */
                if (bc_get_request_on && bc_get_request_handled)
                {
                    /* ack board control */
                    BOARD_CONTROL_mWriteReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG4_OFFSET, 0x01);
                    unsigned delay = 0;
                    for (delay = 0; delay < 5; delay++)
                    {
                    }
                    /* remove the ack sig */
                    BOARD_CONTROL_mWriteReg(BOARD_CONTROL_BASE_ADDR, BOARD_CONTROL_S00_AXI_SLV_REG4_OFFSET, 0x00);

                    bc_get_request_on = 0;
                    bc_get_request_handled = 0;
                }
            }
        }
    }

    free(rcv_buf);
    rcv_buf = NULL;

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

int setup_client_conn()
{
    struct tcp_pcb *pcb;
    err_t err;
    ip_addr_t remote_addr;

    xil_printf("Setting up client connection\n");

#if LWIP_IPV6 == 1
    remote_addr.type = IPADDR_TYPE_V6;
    err = inet6_aton(DEST_IP6_ADDR, &remote_addr);
#else
    err = inet_aton(DEST_IP4_ADDR, &remote_addr);
#endif

    if (!err)
    {
        xil_printf("Invalid Server IP address: %d\n", err);
        return -1;
    }

    /* Create new TCP PCB structure */
    pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb)
    {
        xil_printf("Error creating PCB. Out of Memory\n");
        return -1;
    }

    /* Bind to specified @port */
    err = tcp_bind(pcb, IP_ANY_TYPE, SRC_PORT);
    if (err != ERR_OK)
    {
        xil_printf("Unable to bind to port %d: err = %d\n", SRC_PORT, err);
        return -2;
    }

    /* Connect to remote server (with callback on connection established) */
    err = tcp_connect(pcb, &remote_addr, DEST_PORT, tcp_client_connected);
    if (err)
    {
        xil_printf("Error on tcp_connect: %d\n", err);
        tcp_client_close(pcb);
        return -1;
    }

    is_connected = 0;

    xil_printf("Waiting for server to accept connection\n");

    return 0;
}

static void tcp_client_close(struct tcp_pcb *pcb)
{
    err_t err;

    xil_printf("Closing Client Connection\n");

    if (pcb != NULL)
    {
        tcp_sent(pcb, NULL);
        tcp_recv(pcb, NULL);
        tcp_err(pcb, NULL);
        err = tcp_close(pcb);
        if (err != ERR_OK)
        {
            /* Free memory with abort */
            tcp_abort(pcb);
        }
    }
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if (err != ERR_OK)
    {
        tcp_client_close(tpcb);
        xil_printf("Connection error\n");
        return err;
    }

    xil_printf("Connection to server established\n");

    /* Store state (for callbacks) */
    c_pcb = tpcb;
    is_connected = 1;

    /* Set callback values & functions */
    tcp_arg(c_pcb, NULL);
    tcp_recv(c_pcb, tcp_client_recv);
    tcp_sent(c_pcb, tcp_client_sent);
    tcp_err(c_pcb, tcp_client_err);

    //	//Just send a single packet
    //	u8_t apiflags = TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE;
    //	char send_buf[TCP_SEND_BUFSIZE];
    //	u32_t i;
    //
    //	for(i = 0; i < TCP_SEND_BUFSIZE-1; i = i + 1)
    //	{
    //		send_buf[i] = i+'a';
    //	}
    //	send_buf[TCP_SEND_BUFSIZE-1] = '\n';
    //
    //	//Loop until enough room in buffer (should be right away)
    //	while (tcp_sndbuf(c_pcb) < TCP_SEND_BUFSIZE);
    //
    //	//Enqueue some data to send
    //	err = tcp_write(c_pcb, send_buf, TCP_SEND_BUFSIZE, apiflags);
    //	if (err != ERR_OK) {
    //		xil_printf("TCP client: Error on tcp_write: %d\n", err);
    //		return err;
    //	}
    //
    //	//send the data packet
    //	err = tcp_output(c_pcb);
    //	if (err != ERR_OK) {
    //		xil_printf("TCP client: Error on tcp_output: %d\n",err);
    //		return err;
    //	}
    //
    //	xil_printf("Packet data sent\n");

    //END OF ADDED CODE

    return ERR_OK;
}

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    //	//If no data, connection closed
    //	if (!p) {
    //		xil_printf("No data received\n");
    //		tcp_client_close(tpcb);
    //		return ERR_OK;
    //	}

    /* If no data, connection closed */
    if (!p)
    {
        xil_printf("No data received\n");
        return 0;
    }

    /* Print message */
    // xil_printf("Packet received, %d bytes\n", p->tot_len);

	if(receiving_buf == 0) {
		// First packet for receiving a single message.
		if(p->tot_len >= 4) {
			// We must have the header info to proceed.

		    pbuf_copy_partial(p, rcv_buf, p->tot_len, 0); //Note - inefficient way to access packet data

			rx_total_bytes = 0;
			rx_total_bytes |= 0xff & rcv_buf[0];
			rx_total_bytes <<= 8;
			rx_total_bytes |= 0xff & rcv_buf[1];
			rx_total_bytes <<= 8;
			rx_total_bytes |= 0xff & rcv_buf[2];
			rx_total_bytes <<= 8;
			rx_total_bytes |= 0xff & rcv_buf[3];
			rx_total_bytes += 4; // this counter also includes the size header itself.

			xil_printf("Packet received, %d bytes\n", p->tot_len);
			xil_printf("Message total bytes: %d\n", rx_total_bytes);

			receiving_buf = 1;
			received_buf_bytes = p->tot_len;

			// Indicate done processing.
			tcp_recved(tpcb, p->tot_len);
		    // Free the received pbuf.
		    pbuf_free(p);
		}
	} else {
		// Continue to put packet into rcv_buf untill either rcv_buf full
		// or message all received.

		// Only accept up to RCV_BUF_TOTAL_SIZE
		unsigned bytes_to_read = (p->tot_len + received_buf_bytes) > RCV_BUF_TOTAL_SIZE ? RCV_BUF_TOTAL_SIZE - received_buf_bytes : p->tot_len;
		// Only accept up to rx_total_bytes
		bytes_to_read = (bytes_to_read + received_buf_bytes) > rx_total_bytes ? rx_total_bytes - received_buf_bytes : bytes_to_read;

		pbuf_copy_partial(p, rcv_buf + received_buf_bytes, bytes_to_read, 0); //Note - inefficient way to access packet data

		// Indicate done processing.
		tcp_recved(tpcb, bytes_to_read);
	    // Free the received pbuf.
		// This might cause problems!
	    pbuf_free(p);

	    received_buf_bytes += bytes_to_read;

	    if(received_buf_bytes == RCV_BUF_TOTAL_SIZE || received_buf_bytes == rx_total_bytes) {
	    	// We have received everything
			xil_printf("Received Message: (%d bytes/%d bytes/%d bytes)\n", received_buf_bytes, rx_total_bytes, RCV_BUF_TOTAL_SIZE);

			// Call other function!
			writeImage(rcv_buf + 4, 640, 480);

			receiving_buf = 0;
			received_buf_bytes = 0;
			rx_total_bytes = 0;
	    }
	}

    if (bc_get_request_on && !bc_get_request_handled)
    {
        /* notify done */
        bc_get_request_on = 1;
        bc_get_request_handled = 1;
    }


    return 0;
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    /* Print message */
    xil_printf("Packet sent successfully, %d bytes\n", len);

    return 0;
}

static void tcp_client_err(void *arg, err_t err)
{
    LWIP_UNUSED_ARG(err);
    tcp_client_close(c_pcb);
    c_pcb = NULL;
    xil_printf("TCP connection aborted\n");
}
