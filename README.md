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
tio /dev/ttyACM0
[20:51:09.109] tio v2.7
[20:51:09.109] Press ctrl-t q to quit
[20:51:09.111] Connected
0:00:00.002,563] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.010,131] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000002
 - software
Reset cause (SysCtrl): 0x00000006
AON_PMCTL.RESETCTL : 0x00003bec  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x0000010d  (CLK_LOSS_EN=0)
DDI_0_OSC.STAT0    : 0x17dcace8  (SCLK_LF_SRC=HF RCOSC ÷1536)
DDI_0_OSC.STAT1    : 0x500061e1
[00:00:00.053,802] <dbg> resetissue: setled: led(1)

�[00:00:00.002,563] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.010,162] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9[00:00:27.100,036] <inf> resetissue: Reached main()
[00:00:27.100,036] <inf> resetissue: Reached main()
uart:~$ [00:00:32.114,410] <dbg> resetissue: main: Woke from first sleep
[00:00:32.114,410] <dbg> resetissue: main: Woke from first sleep
uart:~$ [00:00:32.131,286] <dbg> resetissue: setled: led(1)
[00:00:32.131,286] <dbg> resetissue: setled: led(1)
uart:~$ [00:00:32.145,599] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:32.145,599] <dbg> pm: pm_system_suspend: No PM operations done.
uart:~$ [00:00:32.163,513] <dbg> resetissue: setled: led(0)
[00:00:32.163,513] <dbg> resetissue: setled: led(0)
uart:~$ [00:00:32.177,764] <dbg> resetissue: main: Disabling network
[00:00:32.177,764] <dbg> resetissue: main: Disabling network
uart:~$ [00:00:00.002,563] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.010,192] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000000
Reset cause (SysCtrl): 0x00000007
AON_PMCTL.RESETCTL : 0x00003bee  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x3000030d  (CLK_LOSS_EN=1)
DDI_0_OSC.STAT0    : 0x719cace8  (SCLK_LF_SRC=XOSC_LF 32.768k)
DDI_0_OSC.STAT1    : 0x500061e1
[00:00:00.054,626] <dbg> resetissue: setled: led(1)

uart:~$ 
```
