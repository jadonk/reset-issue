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
[20:36:30.432] tio v2.7
[20:36:30.432] Press ctrl-t q to quit
[20:36:30.432] Connected
m
[00:00:00.002,593] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,655] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,777] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.003,112] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000002
 - software
Reset cause (SysCtrl): 0x00000006
AON_PMCTL.RESETCTL : 0x00003bec  (CLK_LOSS_EN=1)
DDI[00:00:27.043,395] <inf> resetissue: Reached main()
uart:~$ 
[00:00:00.002,593] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,655] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,777] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.003,112] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000000
Reset cause (SysCtrl): 0x00000007
AON_PMCTL.RESETCTL : 0x00003bee  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x3000030d  (CLK_LOSS_EN=1)
DDI_0_OSC.STAT0    : 0x719cacea  (SCLK_LF_SRC=XOSC_LF 32.768k)
DDI_0_OSC.STAT1    : 0x500061e1
[00:00:00.009,246] <dbg> resetissue: setled: led(1)
[00:00:27.050,354] <inf> resetissue: Reached main()
uart:~$ 
[00:00:00.002,593] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,655] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.002,777] <dbg> pm: pm_system_suspend: No PM operations done.
[00:00:00.003,112] <dbg> pm: pm_system_suspend: No PM operations done.
*** Booting Zephyr OS build v4.3.0-rc3-5-gfbd15ecf9ebe ***
Reset cause: 0x00000000
Reset cause (SysCtrl): 0x00000007
AON_PMCTL.RESETCTL : 0x00003bee  (CLK_LOSS_EN=1)
DDI_0_OSC.CTL0     : 0x3000030d  (CLK_LOSS_EN=1)
DDI_0_OSC.STAT0    : 0x719cace8  (SCLK_LF_SRC=XOSC_LF 32.768k)
DDI_0_OSC.STAT1    : 0x500061e1
[00:00:00.009,246] <dbg> resetissue: setled: led(1)
uart:~$ 
```
