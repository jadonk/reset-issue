# BCF Reset Issue

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
[21:22:59.974] tio v2.7
[21:22:59.974] Press ctrl-t q to quit
[21:22:59.975] Connected
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
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000000
Reset cause (SysCtrl): 0x00000007
AON_PMCTL.RESETCTL : 0x00003bee  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x3000030d  (CLK_LOSS_EN=1)
DDI_0_OSC.STAT0    : 0x719cacea  (SCLK_LF_SRC=XOSC_LF 32.768k)
DDI_0_OSC.STAT1    : 0x500061e1
led(1)
```
