/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 * Copyright (c) 2020-2025 Jason Kridner, BeagleBoard.org Foundation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/devicetree.h>
#include <errno.h>
#include <zephyr/linker/sections.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>
#include <zephyr/sys/printk.h>
#include <math.h>
#include <string.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/udp.h>
#include <zephyr/net/ieee802154_radio.h>

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct device * const uart = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
static const struct device * const netdevice = DEVICE_DT_GET(DT_NODELABEL(ieee802154g));
static const struct device * const adc = DEVICE_DT_GET(DT_NODELABEL(adc0));
static const struct adc_dt_spec an_mb1_dt = ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user),an_mb1);

void setled(int on)
{
	printk("led(%d)\n", on);
	gpio_pin_set_dt(&led0, on);
}

static void read_an_mb1()
{
	uint16_t an_mb1_buf;
	struct adc_sequence an_mb1_seq = {
		.buffer = &an_mb1_buf,
		.buffer_size = sizeof(an_mb1_buf),
	};
	int32_t an_mb1_val;
	int rc;
	printk("Setting up ADC\n");
	rc = adc_channel_setup_dt(&an_mb1_dt);
	if(rc < 0) printk("adc_channel_setup_dt(AN_MB1) failed: %d", rc);
	printk("Setting up ADC sequence\n");
	rc = adc_sequence_init_dt(&an_mb1_dt, &an_mb1_seq);
	if(rc != 0) printk("adc_sequence_init_dt(AN_MB1) failed: %d", rc);
	printk("Reading ADC\n");
	rc = adc_read_dt(&an_mb1_dt, &an_mb1_seq);
	if(rc != 0) printk("adc_read_dt(AN_MB1) failed: %d", rc);
	an_mb1_val = (int32_t)an_mb1_buf;
	rc = adc_raw_to_millivolts_dt(&an_mb1_dt, &an_mb1_val);
	if(rc != 0) printk("adc_raw_to_millivolts(AN_MB1) failed: %d", rc);
	printk("an_mb1_val: %d\n", an_mb1_val);
}

#define SEND_IP "ff02::1"
const char send_msg[] = "TEST";

int main(void)
{
	const struct ieee802154_radio_api * netdeviceapi = netdevice->api;
	static int send_fd = -1;
	static struct sockaddr_in6 send_addr;
	const struct in6_addr * my_ll_addr;
	char astr[INET6_ADDRSTRLEN];

	setled(0);
	printk("Reached main()\n");

	memset(&send_addr, 0, sizeof(struct sockaddr_in6));
	send_addr.sin6_family = AF_INET6;
	send_addr.sin6_port = htons(9999);
	inet_pton(AF_INET6, SEND_IP, &send_addr.sin6_addr);
	my_ll_addr = net_if_ipv6_get_ll(net_if_get_default(), NET_ADDR_ANY_STATE);
	if (my_ll_addr && inet_ntop(AF_INET6, my_ll_addr, astr, sizeof(astr))) {
		printk("Link local IPv6: %s", astr);
	}

	printk("Sleep 1 second\n");
	k_sleep(K_SECONDS(1));
	setled(1);
	read_an_mb1();
	setled(0);
	netdeviceapi->stop(netdevice);
	k_sleep(K_SECONDS(1));
	read_an_mb1();
	netdeviceapi->start(netdevice);
	sendto(send_fd, send_msg, strlen(send_msg), 0,
		(const struct sockaddr *) &send_addr,
		sizeof(send_addr));

	printk("Exiting main()\n");
	return(0);
}

static int led_init()
{
	gpio_is_ready_dt(&led0);
	gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	setled(1);
	return 0;
}

SYS_INIT(led_init, APPLICATION, 0);
