/*
 * (C) Copyright 2006
 * DENX Software Engineering <mk@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

#if defined(CONFIG_USB_OHCI_NEW) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT)

#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/clk.h>

int usb_cpu_init(void)
{
	at91_pmc_t *pmc	= (at91_pmc_t *)ATMEL_BASE_PMC;

#if defined(CONFIG_AT91CAP9) || defined(CONFIG_AT91SAM9260) || \
    defined(CONFIG_AT91SAM9263) || defined(CONFIG_AT91SAM9G20) || \
    defined(CONFIG_AT91SAM9261)
	/* Enable PLLB */
	writel(get_pllb_init(), &pmc->pllbr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKB) != AT91_PMC_LOCKB)
		;
#elif defined(CONFIG_AT91SAM9G45) || defined(CONFIG_AT91SAM9M10G45) || \
	defined(CONFIG_AT91SAM9X5) || defined(CONFIG_SAMA5D3)
	/* Enable UPLL */
	writel(readl(&pmc->uckr) | AT91_PMC_UPLLEN | AT91_PMC_BIASEN,
		&pmc->uckr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKU) != AT91_PMC_LOCKU)
		;

	/* Select PLLA as input clock of OHCI */
	writel(AT91_PMC_USBS_USB_UPLL | AT91_PMC_USBDIV_10, &pmc->usb);
#endif

	/* Enable USB host clock. */
#ifdef CONFIG_SAMA5D3
	writel(1 << (ATMEL_ID_UHP - 32), &pmc->pcer1);
#else
	writel(1 << ATMEL_ID_UHP, &pmc->pcer);
#endif

#if defined(CONFIG_AT91SAM9261) || defined(CONFIG_AT91SAM9G10)
	writel(ATMEL_PMC_UHP | AT91_PMC_HCK0, &pmc->scer);
#else
	writel(ATMEL_PMC_UHP, &pmc->scer);
#endif

	return 0;
}

int usb_cpu_stop(void)
{
	at91_pmc_t *pmc	= (at91_pmc_t *)ATMEL_BASE_PMC;

	/* Disable USB host clock. */
#ifdef CONFIG_SAMA5D3
	writel(1 << (ATMEL_ID_UHP - 32), &pmc->pcdr1);
#else
	writel(1 << ATMEL_ID_UHP, &pmc->pcdr);
#endif

#if defined(CONFIG_AT91SAM9261) || defined(CONFIG_AT91SAM9G10)
	writel(ATMEL_PMC_UHP | AT91_PMC_HCK0, &pmc->scdr);
#else
	writel(ATMEL_PMC_UHP, &pmc->scdr);
#endif

#if defined(CONFIG_AT91CAP9) || defined(CONFIG_AT91SAM9260) || \
    defined(CONFIG_AT91SAM9263) || defined(CONFIG_AT91SAM9G20)
	/* Disable PLLB */
	writel(0, &pmc->pllbr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKB) != 0)
		;
#elif defined(CONFIG_AT91SAM9G45) || defined(CONFIG_AT91SAM9M10G45) || \
	defined(CONFIG_AT91SAM9X5) || defined(CONFIG_SAMA5D3)
	/* Disable UPLL */
	writel(readl(&pmc->uckr) & (~AT91_PMC_UPLLEN), &pmc->uckr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKU) == AT91_PMC_LOCKU)
		;
#endif

	return 0;
}

int usb_cpu_init_fail(void)
{
	return usb_cpu_stop();
}

#endif /* defined(CONFIG_USB_OHCI) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT) */
