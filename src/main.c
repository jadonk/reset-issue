/*
 * Copyright (c) 2025 Jason Kridner, BeagleBoard.org Foundation
 *
 * Sourced from ChatGPT
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cmsis_core.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/irq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_aux_evctl.h)
#include DeviceFamily_constructPath(inc/hw_aux_anaif.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)

LOG_MODULE_REGISTER(adc_repro, LOG_LEVEL_DBG);

#define SAMPLES 256

/* Match BeagleConnect Freedom overlay names */
static const struct device *const adc = DEVICE_DT_GET(DT_NODELABEL(adc0));
static const struct adc_dt_spec an_mb1_dt =
    ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), an_mb1);
static const struct adc_dt_spec an_mb2_dt =
    ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), an_mb2);

#define ADC_NODE DT_NODELABEL(adc0)
#define ADC_IRQ DT_IRQN(ADC_NODE)

static void dump_irq_state(const char *tag) {
  uint32_t primask = __get_PRIMASK();
  uint32_t basepri = __get_BASEPRI();
  uint32_t idx = ADC_IRQ / 32;
  uint32_t bit = BIT(ADC_IRQ % 32);
  uint32_t iser = NVIC->ISER[idx];
  uint32_t ispr = NVIC->ISPR[idx];

  LOG_INF("[%s] PRIMASK=%u BASEPRI=0x%02x", tag, primask, basepri);
  LOG_INF("[%s] NVIC->ISER[%u]=0x%08x ISPR[%u]=0x%08x", tag, idx, iser, idx,
          ispr);
  LOG_INF("[%s] ADC IRQ %d enabled=%d pending=%d", tag, ADC_IRQ,
          irq_is_enabled(ADC_IRQ), (ispr & bit) ? 1 : 0);
}

static void dump_aux_adc_flags(const char *tag) {
  uint32_t evflags = HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGS);
  uint32_t adcctl = HWREG(AUX_ANAIF_BASE + AUX_ANAIF_O_ADCCTL);

  LOG_INF("[%s] AUX_EVCTL.EVTOMCUFLAGS = 0x%08x", tag, evflags);
  LOG_INF("[%s] AUX_ANAIF.ADCCTL       = 0x%08x", tag, adcctl);
}

int main(void) {
  int r;
  uint16_t mb1_buf;
  uint16_t mb2_buf;

  struct adc_sequence mb1_seq = {
      .buffer = &mb1_buf,
      .buffer_size = sizeof(mb1_buf),
  };
  struct adc_sequence mb2_seq = {
      .buffer = &mb2_buf,
      .buffer_size = sizeof(mb2_buf),
  };

  LOG_INF("BeagleConnect Freedom ADC repro main()");

  if (!device_is_ready(adc)) {
    LOG_ERR("ADC device not ready");
    return -1;
  }

  while (1) {
    LOG_INF("Sleeping 5s before measurement");
    k_sleep(K_SECONDS(5));

    /* --- AN_MB1 read --- */
    LOG_INF("AN_MB1 read");

    LOG_DBG("Before adc_channel_setup_dt(an_mb1)");
    r = adc_channel_setup_dt(&an_mb1_dt);
    LOG_DBG("After adc_channel_setup_dt(an_mb1) rc=%d", r);

    LOG_DBG("Before adc_sequence_init_dt(an_mb1)");
    r = adc_sequence_init_dt(&an_mb1_dt, &mb1_seq);
    LOG_DBG("After adc_sequence_init_dt(an_mb1) rc=%d", r);

    LOG_DBG("Before adc_read_dt(an_mb1)");
    dump_irq_state("an_mb1");
    dump_aux_adc_flags("an_mb1");
    r = adc_read_dt(&an_mb1_dt, &mb1_seq);
    LOG_DBG("After adc_read_dt(an_mb1) rc=%d", r);

    if (r == 0) {
      LOG_INF("AN_MB1 raw: %u", mb1_buf);
    }

    return 0;

    /* --- AN_MB2 read: 256 samples --- */
    LOG_INF("AN_MB2 read, %d samples", SAMPLES);

    for (size_t j = 0; j < SAMPLES; j++) {
      LOG_DBG("AN_MB2 sample %u", (unsigned)j);

      r = adc_channel_setup_dt(&an_mb2_dt);
      if (r < 0) {
        LOG_ERR("adc_channel_setup_dt(an_mb2) failed: %d", r);
        break;
      }

      r = adc_sequence_init_dt(&an_mb2_dt, &mb2_seq);
      if (r != 0) {
        LOG_ERR("adc_sequence_init_dt(an_mb2) failed: %d", r);
        break;
      }

      dump_irq_state("an_mb2");
      r = adc_read_dt(&an_mb2_dt, &mb2_seq);
      if (r != 0) {
        LOG_ERR("adc_read_dt(an_mb2) failed: %d", r);
        break;
      }

      LOG_DBG("AN_MB2 raw = %u", mb2_buf);
      /* Uncomment if you want ~1 kHz sampling spacing */
      // k_sleep(K_MSEC(1));
    }
  }

  return 0;
}
