#
# Regular cron jobs for the u-boot-cubox-i package
#
0 4	* * *	root	[ -x /usr/bin/u-boot-cubox-i_maintenance ] && /usr/bin/u-boot-cubox-i_maintenance
