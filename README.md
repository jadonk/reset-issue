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
.
.
.
[00:00:27.459,259] <err> net_config: Timeout while waiting network setup
[00:00:27.467,102] <err> net_config: Network initialization failed (-116)
Reached main()
Setting up ADC
[00:00:27.478,027] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_channel_setup: Setup 9 acq time 9
Setting up ADC sequence
Reading ADC
[00:00:27.491,485] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ISR flags 0x00000480 fifo 0x00000000
[00:00:27.501,861] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ADC buf 2000793E val 3092
[00:00:27.511,291] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_isr: ISR flags 0x00000400 fifo 0x00000001
an_mb1_val: 3245
Stop network device
[00:00:27.525,451] <dbg> ieee802154_cc13xx_cc26xx_subg: cmd_prop_rx_adv_callback: ch: 21 cmd: 3804 st: 3405 e: 0x2000000000000000
Setting up ADC
[00:00:27.539,825] <dbg> adc_cc13xx_cc26xx: adc_cc13xx_cc26xx_channel_setup: Setup 9 acq time 9
Setting up ADC sequence
Reading ADC
```
