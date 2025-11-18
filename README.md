# CC1352P7 Zephyr Clock/PM Issues

## Build

```sh
west build -b beagleconnect_freedom -p always .
```

## Flash


```sh
west flash
```

## Issue

```sh
:00:00.000,915] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.008,239] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.019,134] <dbg> net_ieee802154: ieee802154_init: (0x200023b8): Initializing IEEE 802.15.4 stack on iface 0x20001780
[00:00:00.031,127] <dbg> net_ieee802154: ieee802154_enable: (0x200023b8): iface 0x20001780 up
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
[00:00:00.045,471] <inf> aepro: BeagleConnect Freedom ADC repro main()
[00:00:00.052,886] <inf> adc_repro: Sleeping 5s before measurement
[00:00:05.059,783] <inf> adc_repro: AN_MB1 read
[00:00:05.065,093] <dbg> adc_repro: main: Before adc_channel_setup_dt(an_mb1)
[00:00:05.073,120] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_channel_setup: Setup 9 acq time 9
[00:00:05.082,794] <dbg> adc_repro: main: After adc_channel_setup_dt(an_mb1) rc=0
[00:00:05.091,156] <dbg> adc_repro: main: Before adc_sequence_init_dt(an_mb1)
[00:00:05.099,182] <dbg> adc_repro: main: After adc_sequence_init_dt(an_mb1) rc=0
[00:00:05.107,574] <dbg> adc_repro: main: Before adc_read_dt(an_mb1)
[00:00:05.114,776] <inf> adc_repro: [an_mb1] PRIMASK=0 BASEPRI=0x00
[00:00:05.121,917] <inf> adc_repro: [an_mb1] NVIC->ISER[1]=0x00000007 ISPR[1]=0x00000000
[00:00:05.130,981] <inf> adc_repro: [an_mb1] ADC IRQ 32 enabled=1 pending=0
```

Note that it is hung here at the end
