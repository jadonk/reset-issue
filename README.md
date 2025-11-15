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
 Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000002
 - software
Reset cause (SysCtrl): 0x00000006
AON_PMCTL.RESETCTL : 0x00003bec  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x0000010d  (CLK_LOSS_EN=0)
DDI_0_OSC.STAT0    : 0x17dcacea  (SCLK_LF_SRC=HF RCOSC ÷1536)
DDI_0_OSC.STAT1    : 0x500061e1
led(1)
Reached main()
Woke from first sleep
Disabling network
led(0)
UART suspend rc=0
UART resume rc=0
led(1)
Setting up ADC
Setting up ADC sequence
Reading ADC
```
