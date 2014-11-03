#
# Regular cron jobs for the u-boot-cuboxi package
#
0 4	* * *	root	[ -x /usr/bin/u-boot-cuboxi_maintenance ] && /usr/bin/u-boot-cuboxi_maintenance
