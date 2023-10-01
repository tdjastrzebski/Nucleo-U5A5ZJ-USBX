/*---------------------------------------------------------------------------------------------------
 *  Copyright (c) 2023 Tomasz Jastrzębski. All rights reserved.
 *-------------------------------------------------------------------------------------------------*/

#include "trace.h"

#include "stm32u5xx.h"

static void _trace(uint8_t level, const char* format_msg, ...);

static void _trace(uint8_t level, const char* format_msg, ...) {
#if defined(TRACE_LEVEL)
	if (level > TRACE_LEVEL) return;
	va_list args;
	va_start(args, format_msg);
	vprintf(format_msg, args);
	va_end(args);
#else
	UNUSED(level);
	UNUSED(format_msg);
#endif
}

void trace_HCD(uint8_t level) {
	// 11.8.30 RCC AHB2 peripheral clock enable register 1 (RCC_AHB2ENR1)
	trace(level, "[RCC.AHB2ENR1] OTGEN OTGHSPHYEN\n");
	trace(level, "               %s    %s\n",
	      GET_REG_FLAGX(RCC->AHB2ENR1, RCC_AHB2ENR1_OTGEN), GET_REG_FLAGX(RCC->AHB2ENR1, RCC_AHB2ENR1_USBPHYCEN));

	// 11.8.7 RCC clock configuration register 2 (RCC_CFGR2)
	trace(level, "[RCC.CFGR2] AHB2DIS1\n");
	trace(level, "            %s\n",
	      GET_REG_FLAGX(RCC->CFGR2, RCC_CFGR2_AHB2DIS1));

	// 15.3.12 SYSCFG OTG_HS PHY register (SYSCFG_OTGHSPHYCR)
	trace(level, "[OTGHSPHYCR] CLKSEL PDCTRL EN\n");
	trace(level, "             %i%i%i%i   %s     %s\n",
	      GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_CLKSEL, 3), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_CLKSEL, 2), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_CLKSEL, 1), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_CLKSEL, 0),
	      GET_REG_FLAGX(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_PDCTRL), GET_REG_FLAGX(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_EN));

	// 15.3.13 SYSCFG OTG_HS PHY tune register 2 (SYSCFG_OTGHSPHYTUNER2)
	trace(level, "[OTGHSPHYTUNER2] TXPREEMPAMPTUNE SQRXTUNE COMPDISTUNE\n");
	trace(level, "                 %i%i              %i%i%i      %i%i%i\n",
	      GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_TXPREEMPAMPTUNE, 1), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_TXPREEMPAMPTUNE, 0),
	      GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_SQRXTUNE, 2), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_SQRXTUNE, 1), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_SQRXTUNE, 0),
	      GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_COMPDISTUNE, 2), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_COMPDISTUNE, 1), GET_REG_FIELD_BIT(SYSCFG->OTGHSPHYTUNER2, SYSCFG_OTGHSPHYTUNER2_COMPDISTUNE, 0));

	// 73.13.1 CSR memory map

// missing HAL register fileds
#define USB_OTG_GOTGCTL_CURMOD_Pos (21U)
#define USB_OTG_GOTGCTL_CURMOD (0x1U << USB_OTG_GOTGCTL_CURMOD_Pos)
#define USB_OTG_GOTGCTL_OTGVER_Pos (20U)
#define USB_OTG_GOTGCTL_OTGVER (0x1U << USB_OTG_GOTGCTL_OTGVER_Pos)
#define USB_OTG_GOTGCTL_ASVLD_Pos (18U)
#define USB_OTG_GOTGCTL_ASVLD (0x1U << USB_OTG_GOTGCTL_ASVLD_Pos)
#define USB_OTG_GOTGCTL_DBCT_Pos (17U)
#define USB_OTG_GOTGCTL_DBCT (0x1U << USB_OTG_GOTGCTL_DBCT_Pos)
#define USB_OTG_GOTGCTL_CIDSTS_Pos (16U)
#define USB_OTG_GOTGCTL_CIDSTS (0x1U << USB_OTG_GOTGCTL_CIDSTS_Pos)
#define USB_OTG_GOTGCTL_EHEN_Pos (12U)
#define USB_OTG_GOTGCTL_EHEN (0x1U << USB_OTG_GOTGCTL_EHEN_Pos)

	// 73.14.1 OTG control and status register (OTG_GOTGCTL)
	trace(level, "[GOTGCTL] CURMOD OTGVER BSVLD ASVLD DBCT CIDSTS EHEN BVALOVAL BVALOEN AVALOVAL AVALOEN VBVALOVAL VBVALOEN\n");
	trace(level, "          %s     %s     %s    %s    %s   %s     %s   %s       %s      %s       %s      %s        %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_CURMOD), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_OTGVER), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_BSESVLD), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_ASVLD),
	      GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_DBCT), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_CIDSTS), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_EHEN),
	      GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_BVALOVAL), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_BVALOEN), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_AVALOVAL), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_AVALOEN),
	      GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_VBVALOVAL), GET_REG_FLAGX(USB_OTG_HS->GOTGCTL, USB_OTG_GOTGCTL_VBVALOEN));

	// 73.14.2 OTG interrupt register (OTG_GOTGINT)
	trace(level, "[GOTGINT] ADTOCHG SEDET\n");
	trace(level, "          %s      %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GOTGINT, USB_OTG_GOTGINT_ADTOCHG), GET_REG_FLAGX(USB_OTG_HS->GOTGINT, USB_OTG_GOTGINT_SEDET));

	// 73.14.3 OTG AHB configuration register (OTG_GAHBCFG)
	trace(level, "[GAHBCFG] PTXFELVL TXFELVL DMAEN HBSTLEN GINTMSK\n");
	trace(level, "          %s       %s      %s    %i%i%i%i      %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_PTXFELVL), GET_REG_FLAGX(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_TXFELVL), GET_REG_FLAGX(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_DMAEN),
	      GET_REG_FIELD_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_HBSTLEN, 3), GET_REG_FIELD_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_HBSTLEN, 2), GET_REG_FIELD_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_HBSTLEN, 1), GET_REG_FIELD_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_HBSTLEN, 0),
	      GET_REG_FLAGX(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_GINT));

	// 73.14.4 OTG USB configuration register (OTG_GUSBCFG)
	trace(level, "[GUSBCFG] FDMOD FHMOD TSDPS PHYLPCS TRDT TOCAL\n");
	trace(level, "          %s    %s    %s    %s      %s   %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_FDMOD), GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_FHMOD),
	      GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_TSDPS), GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_PHYLPCS), GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_TRDT), GET_REG_FLAGX(USB_OTG_HS->GUSBCFG, USB_OTG_GUSBCFG_TOCAL));

#define USB_OTG_GINTSTS_RSTDET_Pos (23U)
#define USB_OTG_GINTSTS_RSTDET (0x1U << USB_OTG_GINTSTS_RSTDET_Pos)
	// 73.14.6 OTG core interrupt register [alternate] (OTG_GINTSTS) - host mode
	trace(level, "[GINTSTS] WKUPINT SRQINT DISCINT CIDSCHG LPMINT PTXFE HCINT HPRTINT RSTDET DATAFSUSP IPXFR IISOIXFR OEPINT IEPINT EOPF ISOODRP ENUMDNE USBRST USBSUSP ESUSP GONAKEFF GINAKEFF NPTXFE SOF OTGINT MMIS CMOD\n");
	trace(level, "          %s      %s     %s      %s      %s     %s    %s    %s      %s     %s        %s    %s       %s     %s     %s   %s      %s      %s     %s      %s    %s       %s       %s     %s  %s     %s   %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_WKUINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_SRQINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_DISCINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_CIDSCHG),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_LPMINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_PTXFE), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_HCINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_HPRTINT),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_RSTDET), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_DATAFSUSP), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_IISOIXFR), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_OEPINT),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_IEPINT), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_EOPF), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_ISOODRP), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_ENUMDNE),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_USBRST), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_USBSUSP), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_ESUSP), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_BOUTNAKEFF),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_GINAKEFF), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_NPTXFE), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_SOF), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_OTGINT),
	      GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_MMIS), GET_REG_FLAGX(USB_OTG_HS->GINTSTS, USB_OTG_GINTSTS_CMOD));
	// 73.14.7 OTG core interrupt register [alternate] (OTG_GINTSTS) - device mode [TODO]

	// 73.14.8 OTG interrupt mask register [alternate] (OTG_GINTMSK) - host mode
	trace(level, "[GINTMSK] WUIM SRQIM DISCINT CIDSCHGM LPMINTM PTXFEM HCIM PRTIM IPXFRM NPTXFEM RXFLVLM SOFM OTGINT MMISM\n");
	trace(level, "          %s   %s    %s      %s       %s      %s     %s   %s    %s     %s      %s      %s   %s     %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_WUIM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_SRQIM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_DISCINT), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_CIDSCHGM),
	      GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_LPMINTM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_PTXFEM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_HCIM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_PRTIM),
	      GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_PXFRM_IISOOXFRM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_NPTXFEM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_RXFLVLM), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_SOFM),
	      GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_OTGINT), GET_REG_FLAGX(USB_OTG_HS->GINTMSK, USB_OTG_GINTMSK_MMISM));
	// 73.14.9 OTG interrupt mask register [alternate] (OTG_GINTMSK) - device mode [TODO]

#define USB_OTG_GCCFG_FSVMINUS__Pos (2U)
#define USB_OTG_GCCFG_FSVMINUS_ (0x1U << USB_OTG_GCCFG_FSVMINUS__Pos)
	// 73.14.18 OTG general core configuration register (OTG_GCCFG)
	trace(level, "[GCCFG] FORCEHOSTPD VBVALOVEN VBVALOVAL SDEN VBDEN PDEN DCDEN HVDMSRCEN HCDPDETEN HCDPEN SESSVLD FSVMINUS FSVPLUS CHGDET\n");
	trace(level, "        %s          %s        %s        %s   %s    %s   %s    %s        %s        %s     %s      %s       %s      %s\n",
	      GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_PULLDOWNEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBVALEXTOEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBVALOVAL), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_SDEN),
	      GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBDEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_PDEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_DCDEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_H_VDMSRCEN),
	      GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_H_CDPDETEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_H_CDPEN), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_SESSVLD), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_FSVMINUS_),
	      GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_FSVPLUS), GET_REG_FLAGX(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_CHGDET));

	// 73.14.24 OTG host configuration register (OTG_HCFG)
	const USB_OTG_HostTypeDef* usb_host = (USB_OTG_HostTypeDef*)((uint32_t)USB_OTG_HS + USB_OTG_HOST_BASE);
	trace(level, "[HCFG] FSLSS FSLSPCS\n");
	trace(level, "       %s    %i%i\n", GET_REG_FLAGX(usb_host->HCFG, USB_OTG_HCFG_FSLSS), GET_REG_FIELD_BIT(usb_host->HCFG, USB_OTG_HCFG_FSLSPCS, 1), GET_REG_FIELD_BIT(usb_host->HCFG, USB_OTG_HCFG_FSLSPCS, 0));

	// 73.14.30 OTG host port control and status register (OTG_HPRT)
	// look for HAL USBX_HPRT0 register definition, not part of any typedef
	__IO uint32_t host_port = *(__IO uint32_t*)((uint32_t)USB_OTG_HS + USB_OTG_HOST_PORT_BASE);
	trace(level, "[HPRT] PSPD PTCTL PPWR PLSTS PRST PSUSP PRES POCCHNG POCA PENCHNG PENA PCDET PCSTS\n");
	trace(level, "       %i%i   %i%i%i%i  %s   %i%i    %s   %s    %s   %s      %s   %s      %s   %s    %s\n",
	      GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PSPD, 1), GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PSPD, 0),
	      GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PTCTL, 3), GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PTCTL, 2), GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PTCTL, 1), GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PTCTL, 0),
	      GET_REG_FLAGX(host_port, USB_OTG_HPRT_PPWR),
	      GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PLSTS, 1), GET_REG_FIELD_BIT(host_port, USB_OTG_HPRT_PLSTS, 0),
	      GET_REG_FLAGX(host_port, USB_OTG_HPRT_PRST), GET_REG_FLAGX(host_port, USB_OTG_HPRT_PSUSP), GET_REG_FLAGX(host_port, USB_OTG_HPRT_PRES), GET_REG_FLAGX(host_port, USB_OTG_HPRT_POCCHNG),
	      GET_REG_FLAGX(host_port, USB_OTG_HPRT_POCA), GET_REG_FLAGX(host_port, USB_OTG_HPRT_PENCHNG), GET_REG_FLAGX(host_port, USB_OTG_HPRT_PENA), GET_REG_FLAGX(host_port, USB_OTG_HPRT_PCDET),
	      GET_REG_FLAGX(host_port, USB_OTG_HPRT_PCSTS));
}

void trace_UCPD(uint8_t level) {
	// 74.8.2 UCPD1 configuration register 2 (UCPD_CFGR2)
	trace(level, "[CFGR1] UCPDEN RXDMAEN TXDMAEN RXORDSETEN PSC_USBPDCLK TRANSWIN IFRGAP HBITCLKDIV\n");
	trace(level, "[CFGR1] %s     %s      %s      0x%02X       0x%01X          0x%02X     0x%02X   0x%02X\n", GET_REG_FLAGX(UCPD1->CFG1, UCPD_CFG1_UCPDEN), GET_REG_FLAGX(UCPD1->CFG1, UCPD_CFG1_RXDMAEN), GET_REG_FLAGX(UCPD1->CFG1, UCPD_CFG1_TXDMAEN),
	      GET_REG_FIELD(UCPD1->CFG1, UCPD_CFG1_RXORDSETEN), GET_REG_FIELD(UCPD1->CFG1, UCPD_CFG1_PSC_UCPDCLK), GET_REG_FIELD(UCPD1->CFG1, UCPD_CFG1_TRANSWIN), GET_REG_FIELD(UCPD1->CFG1, UCPD_CFG1_IFRGAP), GET_REG_FIELD(UCPD1->CFG1, UCPD_CFG1_HBITCLKDIV));

	trace(level, "[CFGR2] RXAFILTEN WUPEN FORCECLK RXFILT2N3 RXFILTDIS\n");
	trace(level, "[CFGR2] %s        %s    %s       %s        %s\n", GET_REG_FLAGX(UCPD1->CFG2, UCPD_CFG2_RXAFILTEN), GET_REG_FLAGX(UCPD1->CFG2, UCPD_CFG2_WUPEN), GET_REG_FLAGX(UCPD1->CFG2, UCPD_CFG2_FORCECLK),
	      GET_REG_FLAGX(UCPD1->CFG2, UCPD_CFG2_RXFILT2N3), GET_REG_FLAGX(UCPD1->CFG2, UCPD_CFG2_RXFILTDIS));

	trace(level, "[CFGR3] TRIM_CC1_RP TRIM_CC1_RD TRIM_CC2_RP TRIM_CC2_RD\n");
	trace(level, "[CFGR3] 0x%01X         0x%01X         0x%01X         0x%01X\n", GET_REG_FIELD(UCPD1->CFG3, UCPD_CFG3_TRIM_CC1_RP), GET_REG_FIELD(UCPD1->CFG3, UCPD_CFG3_TRIM_CC1_RD), GET_REG_FIELD(UCPD1->CFG3, UCPD_CFG3_TRIM_CC2_RP), GET_REG_FIELD(UCPD1->CFG3, UCPD_CFG3_TRIM_CC2_RD));

	trace(level, "[CR] CC2TCDIS CC1TCDIS RDCH FRSTX FRSRXEN CCENABLE ANAMODE ANASUBMODE PHYCCSEL PHYRXEN RXMODE TXHRST TXSEND TXMODE\n");
	trace(level, "[CR] %s       %s       %s   %s    %s      0x%01X      %s      0x%01X        %s       %s      %s     %s     %s     0x%01X\n", GET_REG_FLAGX(UCPD1->CR, UCPD_CR_CC2TCDIS), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_CC1TCDIS), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_RDCH),
	      GET_REG_FLAGX(UCPD1->CR, UCPD_CR_FRSTX), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_FRSRXEN), GET_REG_FIELD(UCPD1->CR, UCPD_CR_CCENABLE), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_ANAMODE), GET_REG_FIELD(UCPD1->CR, UCPD_CR_ANASUBMODE), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_PHYCCSEL),
	      GET_REG_FLAGX(UCPD1->CR, UCPD_CR_PHYRXEN), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_RXMODE), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_TXHRST), GET_REG_FLAGX(UCPD1->CR, UCPD_CR_TXSEND), GET_REG_FIELD(UCPD1->CR, UCPD_CR_TXMODE));

	trace(level, "[IMR] FRSEVTIE TYPECEVT1IE TYPECEVT2IE RXMSGENDIE RXOVRIE RXHRSTDETIE RXORDDETIE RXNEIE TXUNDIE HRSTSENTIE HRSTDISCIE TXMSGABTIE TXMSGSENTIE TXMSGDISCIE TXISIE\n");
	trace(level, "[IMR] %s       %s          %s          %s         %s      %s          %s         %s     %s      %s         %s         %s         %s          %s          %s\n",
	      GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_FRSEVTIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TYPECEVT1IE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TYPECEVT2IE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_RXMSGENDIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_RXOVRIE),
	      GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_RXHRSTDETIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_RXORDDETIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_RXNEIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TXUNDIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_HRSTSENTIE),
	      GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_HRSTDISCIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TXMSGABTIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TXMSGSENTIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TXMSGDISCIE), GET_REG_FLAGX(UCPD1->IMR, UCPD_IMR_TXISIE));

	trace_UCPD_ISR(level);
}

void trace_UCPD_ISR(uint8_t level) {
	trace(level, "[SR] FRSEVT TYPEC_VSTATE_CC1 TYPEC_VSTATE_CC2 TYPECEVT1 TYPECEVT2 RXERR RXMSGEND RXOVR RXHRSTDET RXORDDET RXNE TXUND HRSTSENT HRSTDISC TXMSGABT TXMSGSENT TXMSGDISC TXIS\n");
	trace(level, "[SR] %s     0x%01X              0x%01X              %s        %s        %s    %s       %s    %s        %s       %s   %s    %s       %s       %s       %s         %s       %s\n",
	      GET_REG_FLAGX(UCPD1->SR, UCPD_SR_FRSEVT), GET_REG_FIELD(UCPD1->SR, UCPD_SR_TYPEC_VSTATE_CC1), GET_REG_FIELD(UCPD1->SR, UCPD_SR_TYPEC_VSTATE_CC2), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TYPECEVT1), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TYPECEVT2), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXERR),
	      GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXMSGEND), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXOVR), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXHRSTDET), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXORDDET), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_RXNE), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TXUND),
	      GET_REG_FLAGX(UCPD1->SR, UCPD_SR_HRSTSENT), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_HRSTDISC), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TXMSGABT), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TXMSGSENT), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TXMSGDISC), GET_REG_FLAGX(UCPD1->SR, UCPD_SR_TXIS));
}