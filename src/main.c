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
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/udp.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <errno.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ieee802154_radio.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>
#include <zephyr/net/net_config.h>

#include <zephyr/drivers/hwinfo.h>
#include <zephyr/sys/printk.h>
#include <ti/devices/DeviceFamily.h>
#include <ti/drivers/Power.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_aon_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_ddi.h)
#include DeviceFamily_constructPath(inc/hw_ddi_0_osc.h)
#include DeviceFamily_constructPath(driverlib/sys_ctrl.h)
#include DeviceFamily_constructPath(driverlib/osc.h)

#include <math.h>
#include <string.h>

static inline uint32_t reg_resetctl(void) { return HWREG(AON_PMCTL_BASE + AON_PMCTL_O_RESETCTL); }
static inline uint32_t reg_ctl0(void)     { return HWREG(AUX_DDI0_OSC_BASE + DDI_0_OSC_O_CTL0); }
static inline uint32_t reg_stat0(void)    { return HWREG(AUX_DDI0_OSC_BASE + DDI_0_OSC_O_STAT0); }
static inline uint32_t reg_stat1(void)    { return HWREG(AUX_DDI0_OSC_BASE + DDI_0_OSC_O_STAT1); }

#define SLEEP_S 5

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct device * const uart = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
static const struct ieee802154_radio_api * netdeviceapi;
static const struct device * const netdevice = DEVICE_DT_GET(DT_NODELABEL(ieee802154g));
static const struct device * const adc = DEVICE_DT_GET(DT_NODELABEL(adc0));
static const struct adc_dt_spec an_mb1_dt = ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user),an_mb1);

#define SEND_IP "ff02::1"
static const char * const outstr = "TEST";
static struct sockaddr_in6 send_addr;
static int send_fd = -1;

static void send_test_value()
{
	if ((send_fd >= 0) && (strlen(outstr) > 0)) {
		sendto(send_fd, outstr, strlen(outstr), 0,
			(const struct sockaddr *) &send_addr,
			sizeof(send_addr));
	}
}

void setled(int on)
{
	printk("led(%d)\n", on);
	gpio_pin_set_dt(&led0, on);
}

int main(void)
{
	int rc, rc_s, rc_r;
	netdeviceapi = netdevice->api;
	uint16_t an_mb1_buf;
	struct adc_sequence an_mb1_seq = {
		.buffer = &an_mb1_buf,
		.buffer_size = sizeof(an_mb1_buf),
	};
	int32_t an_mb1_val;

	printk("Reached main()\n");

	k_sleep(K_SECONDS(SLEEP_S));

	printk("Woke from first sleep\n");

	send_fd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	__ASSERT(send_fd >= 0, "Failed to open socket for sending data: 0x%08x", send_fd);
	memset(&send_addr, 0, sizeof(struct sockaddr_in6));
	send_addr.sin6_family = AF_INET6;
	send_addr.sin6_port = htons(9999);
	inet_pton(AF_INET6, SEND_IP, &send_addr.sin6_addr);

	for (;;) {
		k_usleep(10);
		printk("Disabling network\n");
		netdeviceapi->stop(netdevice);
		setled(0);
		rc_s = pm_device_action_run(uart, PM_DEVICE_ACTION_SUSPEND);
		k_sleep(K_SECONDS(SLEEP_S));
		rc_r = pm_device_action_run(uart, PM_DEVICE_ACTION_RESUME);
		printk("UART suspend rc=%d\n", rc_s);
		printk("UART resume rc=%d\n", rc_r);
		setled(1);
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
		printk("Enabling network\n");
		netdeviceapi->start(netdevice);
		k_usleep(100); // not sure how long to wait for interface to be "up"
		printk("Sending test value over network\n");
		send_test_value();
	}

	return(0);
}

static const char *name_from_flag(uint32_t f) {
	switch (f) {
		case RESET_PIN: return "external pin";
		case RESET_SOFTWARE: return "software";
		case RESET_WATCHDOG: return "watchdog";
		case RESET_POR: return "power-on";
		case RESET_BROWNOUT: return "brownout";
		case RESET_LOW_POWER_WAKE: return "low-power wake";
		case RESET_CPU_LOCKUP: return "CPU lockup";
		case RESET_DEBUG: return "debug";
		case RESET_SECURITY: return "security";
		case RESET_TEMPERATURE: return "temperature";
		case RESET_PLL: return "PLL";
		case RESET_CLOCK: return "clock";
		case RESET_HARDWARE: return "hardware";
		case RESET_USER: return "user";
		case RESET_BOOTLOADER: return "bootloader";
		case RESET_FLASH: return "flash ECC";
		default: return "unknown";
	}
}

static const char *lf_src_name(uint32_t stat0)
{
	/* STAT0.SCLK_LF_SRC is 2 bits: 0=RCOSC_HF/1536, 1=XOSC_HF/1536, 2=RCOSC_LF, 3=XOSC_LF */
	uint32_t src = (stat0 & DDI_0_OSC_STAT0_SCLK_LF_SRC_M) >> DDI_0_OSC_STAT0_SCLK_LF_SRC_S;
	switch (src) {
		case 0: return "HF RCOSC ÷1536";
		case 1: return "HF XOSC/HPOSC ÷1536";
		case 2: return "RCOSC_LF 32k";
		case 3: return "XOSC_LF 32.768k";
		default: return "?";
	}
}

static inline void disable_clock_loss_reset(void)
{
    /* AON: disable reset on clock loss */
    SysCtrlClockLossResetDisable();
    /* DDI: disable clock-loss event */
    OSCClockLossEventDisable();
}

static inline void force_lf_rcosc(void)
{
    OSCClockSourceSet(OSC_SRC_CLK_LF, OSC_RCOSC_LF);
    while (OSCClockSourceGet(OSC_SRC_CLK_LF) != OSC_RCOSC_LF) { /* spin */ }
}

/* This should run before network bring-up */
static int cc_init()
{
	uint32_t cause = 0, supported = 0;

	if (hwinfo_get_supported_reset_cause(&supported) == 0 &&
		hwinfo_get_reset_cause(&cause) == 0) {

		printk("Reset cause: 0x%08x\n", cause);
		for (uint32_t bit = 1U; bit; bit <<= 1) {
			if ((cause & bit) && (supported & bit)) {
				printk(" - %s\n", name_from_flag(bit));
			}
		}

		/* Lower level cause */
		uint32_t ll_cause = SysCtrlResetSourceGet(); /* Latches the root cause */
		uint32_t resetctl = reg_resetctl();
		uint32_t ctl0 = reg_ctl0();
		uint32_t stat0 = reg_stat0();
		uint32_t stat1 = reg_stat1();

		printk("Reset cause (SysCtrl): 0x%08x\n", ll_cause);
		printk("AON_PMCTL.RESETCTL : 0x%08x  (CLK_LOSS_EN=%d)\n",
			resetctl, (resetctl & AON_PMCTL_RESETCTL_CLK_LOSS_EN) ? 1 : 0);
		printk("DDI_0_OSC.CTL0     : 0x%08x  (CLK_LOSS_EN=%d)\n",
			ctl0, (ctl0 & DDI_0_OSC_CTL0_CLK_LOSS_EN) ? 1 : 0);
		printk("DDI_0_OSC.STAT0    : 0x%08x  (SCLK_LF_SRC=%s)\n", stat0, lf_src_name(stat0));
		printk("DDI_0_OSC.STAT1    : 0x%08x\n", stat1);

		(void)hwinfo_clear_reset_cause();
	} else {
		printk("Reset cause not supported on this SoC\n");
	}

	//disable_clock_loss_reset();
	//force_lf_rcosc();

	gpio_is_ready_dt(&led0);
	gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	setled(1);

	pm_device_wakeup_enable(uart, true);

	return 0;
}

SYS_INIT(cc_init, APPLICATION, 0);
