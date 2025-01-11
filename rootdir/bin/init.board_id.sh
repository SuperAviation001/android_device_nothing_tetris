#!/vendor/bin/sh

if [ -f /proc/hwid ]; then
    originhwid=`cat /proc/hwid`
    hwid=`echo $originhwid | awk '{print $3}'`
    case "$hwid" in
        "PreT0")
                     setprop persist.vendor.fastrpc.hwid_version PreT0
                     ;;
        "T0")
                     setprop persist.vendor.fastrpc.hwid_version T0
                     ;;
        "T0_India")
                     setprop persist.vendor.fastrpc.hwid_version T0_India
                     ;;
        "EVT")
                     setprop persist.vendor.fastrpc.hwid_version EVT
                     ;;
        "EVT_India")
                     setprop persist.vendor.fastrpc.hwid_version EVT_India
                     ;;
        "DVT")
                     setprop persist.vendor.fastrpc.hwid_version DVT
                     ;;
        "DVT_India")
                     setprop persist.vendor.fastrpc.hwid_version DVT_India
                     ;;
        "PVT")
                     setprop persist.vendor.fastrpc.hwid_version PVT
                     ;;
        "PVT_India")
                     setprop persist.vendor.fastrpc.hwid_version PVT_India
                     ;;
        "MP")
                     setprop persist.vendor.fastrpc.hwid_version MP
                     ;;
        "Unknow")
                     setprop persist.vendor.fastrpc.hwid_version Unknow
                     ;;
    esac
fi
