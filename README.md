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
:00:00.000,854] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.008,392] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
led(1)
[00:00:00.062,255] <inf> net_config: Initializing network
[00:00:00.153,076] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:27.069,580] <err> net_config: Timeout while waiting network setup
[00:00:27.077,728] <err> net_config: Network initialization failed (-116)
led(0)
Reached main()
Link local IPv6: fe80::212:4b00:29b9:9aceSleep 1 second
led(1)
Setting up ADC
[00:00:28.096,313] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_channel_setup: Setup 9 acq time 9
Setting up ADC sequence
Reading ADC
[00:00:28.110,260] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ISR flags 0x00000480 fifo 0x00000000
[00:00:28.121,032] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ADC buf 20007B8E val 3107
[00:00:28.130,798] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ISR flags 0x00000400 fifo 0x00000001
an_mb1_val: 3261
led(0)
Setting up ADC
[00:00:29.146,026] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_channel_setup: Setup 9 acq time 9
Setting up ADC sequence
Reading ADC
```
