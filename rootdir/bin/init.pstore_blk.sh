#!/vendor/bin/sh

/vendor/bin/insmod /vendor/lib/modules/pstore_zone.ko
/vendor/bin/insmod /vendor/lib/modules/pstore_blk.ko best_effort=1
