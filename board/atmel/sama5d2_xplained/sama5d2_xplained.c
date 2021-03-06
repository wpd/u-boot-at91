/*
 * Copyright (C) 2015 Atmel
 *		      Wenyou.Yang <wenyou.yang@atmel.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/at91_common.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/atmel_usba_udc.h>
#include <asm/arch/gpio.h>
#include <asm/arch/clk.h>
#include <asm/arch/sama5d2.h>
#include <asm/arch/atmel_sdhci.h>
#include <atmel_hlcdc.h>
#include <i2c.h>
#include <lcd.h>
#include <mmc.h>
#include <net.h>
#include <netdev.h>
#include <spi.h>
#include <version.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_ATMEL_SPI
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return bus == 0 && cs == 0;
}

void spi_cs_activate(struct spi_slave *slave)
{
	at91_set_pio_output(AT91_PIO_PORTA, 17, 0);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	at91_set_pio_output(AT91_PIO_PORTA, 17, 1);
}

static void board_spi0_hw_init(void)
{
	at91_set_a_periph(AT91_PIO_PORTA, 14, 0);
	at91_set_a_periph(AT91_PIO_PORTA, 15, 0);
	at91_set_a_periph(AT91_PIO_PORTA, 16, 0);

	at91_set_pio_output(AT91_PIO_PORTA, 17, 1);

	at91_periph_clk_enable(ATMEL_ID_SPI0);
}
#endif /* CONFIG_ATMEL_SPI */

#ifdef CONFIG_ATMEL_QSPI
static void board_qspi_hw_init(void)
{
	at91_set_f_periph(AT91_PIO_PORTA, 22, 0);
	at91_set_f_periph(AT91_PIO_PORTA, 23, 0);
	at91_set_f_periph(AT91_PIO_PORTA, 24, 0);
	at91_set_f_periph(AT91_PIO_PORTA, 25, 0);
	at91_set_f_periph(AT91_PIO_PORTA, 26, 0);
	at91_set_f_periph(AT91_PIO_PORTA, 27, 0);

	at91_periph_clk_enable(ATMEL_ID_QSPI0);
}
#endif

#ifdef CONFIG_CMD_USB
static void board_usb_hw_init(void)
{
	at91_set_pio_output(AT91_PIO_PORTB, 10, 1);
}
#endif

#ifdef CONFIG_LCD
vidinfo_t panel_info = {
	.vl_col = 480,
	.vl_row = 272,
	.vl_clk = 9000000,
	.vl_bpix = LCD_BPP,
	.vl_bpox = LCD_OUTPUT_BPP,
	.vl_tft = 1,
	.vl_hsync_len = 41,
	.vl_left_margin = 2,
	.vl_right_margin = 2,
	.vl_vsync_len = 11,
	.vl_upper_margin = 2,
	.vl_lower_margin = 2,
	.mmio = ATMEL_BASE_LCDC,
};

/* No power up/down pin for the LCD pannel */
void lcd_enable(void)	{ /* Empty! */ }
void lcd_disable(void)	{ /* Empty! */ }

unsigned int has_lcdc(void)
{
	return 1;
}

static void board_lcd_hw_init(void)
{
	at91_set_a_periph(AT91_PIO_PORTC, 28, 0);	/* LCDPWM */
	at91_set_a_periph(AT91_PIO_PORTC, 29, 0);	/* LCDDISP */
	at91_set_a_periph(AT91_PIO_PORTC, 30, 0);	/* LCDVSYNC */
	at91_set_a_periph(AT91_PIO_PORTC, 31, 0);	/* LCDHSYNC */
	at91_set_a_periph(AT91_PIO_PORTD,  0, 0);	/* LCDPCK */
	at91_set_a_periph(AT91_PIO_PORTD,  1, 0);	/* LCDDEN */

	/* LCDDAT0 */
	/* LCDDAT1 */
	at91_set_a_periph(AT91_PIO_PORTC, 10, 0);	/* LCDDAT2 */
	at91_set_a_periph(AT91_PIO_PORTC, 11, 0);	/* LCDDAT3 */
	at91_set_a_periph(AT91_PIO_PORTC, 12, 0);	/* LCDDAT4 */
	at91_set_a_periph(AT91_PIO_PORTC, 13, 0);	/* LCDDAT5 */
	at91_set_a_periph(AT91_PIO_PORTC, 14, 0);	/* LCDDAT6 */
	at91_set_a_periph(AT91_PIO_PORTC, 15, 0);	/* LCDDAT7 */

	/* LCDDAT8 */
	/* LCDDAT9 */
	at91_set_a_periph(AT91_PIO_PORTC, 16, 0);	/* LCDDAT10 */
	at91_set_a_periph(AT91_PIO_PORTC, 17, 0);	/* LCDDAT11 */
	at91_set_a_periph(AT91_PIO_PORTC, 18, 0);	/* LCDDAT12 */
	at91_set_a_periph(AT91_PIO_PORTC, 19, 0);	/* LCDDAT13 */
	at91_set_a_periph(AT91_PIO_PORTC, 20, 0);	/* LCDDAT14 */
	at91_set_a_periph(AT91_PIO_PORTC, 21, 0);	/* LCDDAT15 */

	/* LCDD16 */
	/* LCDD17 */
	at91_set_a_periph(AT91_PIO_PORTC, 22, 0);	/* LCDDAT18 */
	at91_set_a_periph(AT91_PIO_PORTC, 23, 0);	/* LCDDAT19 */
	at91_set_a_periph(AT91_PIO_PORTC, 24, 0);	/* LCDDAT20 */
	at91_set_a_periph(AT91_PIO_PORTC, 25, 0);	/* LCDDAT21 */
	at91_set_a_periph(AT91_PIO_PORTC, 26, 0);	/* LCDDAT22 */
	at91_set_a_periph(AT91_PIO_PORTC, 27, 0);	/* LCDDAT23 */

	at91_periph_clk_enable(ATMEL_ID_LCDC);
}

#ifdef CONFIG_LCD_INFO
void lcd_show_board_info(void)
{
	ulong dram_size;
	int i;
	char temp[32];

	lcd_printf("%s\n", U_BOOT_VERSION);
	lcd_printf("2015 ATMEL Corp\n");
	lcd_printf("%s CPU at %s MHz\n", get_cpu_name(),
		   strmhz(temp, get_cpu_clk_rate()));

	dram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		dram_size += gd->bd->bi_dram[i].size;

	lcd_printf("%ld MB SDRAM\n", dram_size >> 20);

}
#endif /* CONFIG_LCD_INFO */
#endif /* CONFIG_LCD */

#ifdef CONFIG_MACB
static void board_gmac_hw_init(void)
{
	at91_set_f_periph(AT91_PIO_PORTB, 14, 0);	/* GTXCK */
	at91_set_f_periph(AT91_PIO_PORTB, 15, 0);	/* GTXEN */
	at91_set_f_periph(AT91_PIO_PORTB, 16, 0);	/* GRXDV */
	at91_set_f_periph(AT91_PIO_PORTB, 17, 0);	/* GRXER */
	at91_set_f_periph(AT91_PIO_PORTB, 18, 0);	/* GRX0 */
	at91_set_f_periph(AT91_PIO_PORTB, 19, 0);	/* GRX1 */
	at91_set_f_periph(AT91_PIO_PORTB, 20, 0);	/* GTX0 */
	at91_set_f_periph(AT91_PIO_PORTB, 21, 0);	/* GTX1 */
	at91_set_f_periph(AT91_PIO_PORTB, 22, 0);	/* GMDC */
	at91_set_f_periph(AT91_PIO_PORTB, 23, 0);	/* GMDIO */

	at91_periph_clk_enable(ATMEL_ID_GMAC);
}
#endif

#ifdef CONFIG_ATMEL_SDHCI
#ifdef CONFIG_ATMEL_SDHCI0
static void board_sdhci0_hw_init(void)
{
	at91_set_a_periph(AT91_PIO_PORTA, 0, 0);	/* SDMMC0_CK */
	at91_set_a_periph(AT91_PIO_PORTA, 1, 0);	/* SDMMC0_CMD */
	at91_set_a_periph(AT91_PIO_PORTA, 2, 0);	/* SDMMC0_DAT0 */
	at91_set_a_periph(AT91_PIO_PORTA, 3, 0);	/* SDMMC0_DAT1 */
	at91_set_a_periph(AT91_PIO_PORTA, 4, 0);	/* SDMMC0_DAT2 */
	at91_set_a_periph(AT91_PIO_PORTA, 5, 0);	/* SDMMC0_DAT3 */
	at91_set_a_periph(AT91_PIO_PORTA, 6, 0);	/* SDMMC0_DAT4 */
	at91_set_a_periph(AT91_PIO_PORTA, 7, 0);	/* SDMMC0_DAT5 */
	at91_set_a_periph(AT91_PIO_PORTA, 8, 0);	/* SDMMC0_DAT6 */
	at91_set_a_periph(AT91_PIO_PORTA, 9, 0);	/* SDMMC0_DAT7 */
	at91_set_a_periph(AT91_PIO_PORTA, 10, 0);	/* SDMMC0_RSTN */
	at91_set_a_periph(AT91_PIO_PORTA, 11, 0);	/* SDMMC0_VDDSEL */

	at91_periph_clk_enable(ATMEL_ID_SDHCI0);
	atmel_enable_periph_generated_clk(ATMEL_ID_SDHCI0);
}
#endif

#ifdef CONFIG_ATMEL_SDHCI1
static void board_sdhci1_hw_init(void)
{
	at91_set_e_periph(AT91_PIO_PORTA, 18, 0);	/* SDMMC1_DAT0 */
	at91_set_e_periph(AT91_PIO_PORTA, 19, 0);	/* SDMMC1_DAT1 */
	at91_set_e_periph(AT91_PIO_PORTA, 20, 0);	/* SDMMC1_DAT2 */
	at91_set_e_periph(AT91_PIO_PORTA, 21, 0);	/* SDMMC1_DAT3 */
	at91_set_e_periph(AT91_PIO_PORTA, 22, 0);	/* SDMMC1_CK */
	at91_set_e_periph(AT91_PIO_PORTA, 27, 0);	/* SDMMC1_RSTN */
	at91_set_e_periph(AT91_PIO_PORTA, 28, 0);	/* SDMMC1_CMD */
	at91_set_e_periph(AT91_PIO_PORTA, 30, 0);	/* SDMMC1_CD */

	at91_periph_clk_enable(ATMEL_ID_SDHCI1);
	atmel_enable_periph_generated_clk(ATMEL_ID_SDHCI1);
}
#endif

int board_mmc_init(bd_t *bis)
{
#ifdef CONFIG_ATMEL_SDHCI0
	atmel_sdhci_init((void *)ATMEL_BASE_SDHCI0, ATMEL_ID_SDHCI0);
#endif
#ifdef CONFIG_ATMEL_SDHCI1
	atmel_sdhci_init((void *)ATMEL_BASE_SDHCI1, ATMEL_ID_SDHCI1);
#endif

	return 0;
}
#endif

static void board_uart1_hw_init(void)
{
	at91_set_a_periph(AT91_PIO_PORTD, 2, 1);	/* URXD1 */
	at91_set_a_periph(AT91_PIO_PORTD, 3, 0);	/* UTXD1 */

	at91_periph_clk_enable(ATMEL_ID_UART1);
}

int board_early_init_f(void)
{
	at91_periph_clk_enable(ATMEL_ID_PIOA);
	at91_periph_clk_enable(ATMEL_ID_PIOB);
	at91_periph_clk_enable(ATMEL_ID_PIOC);
	at91_periph_clk_enable(ATMEL_ID_PIOD);

	board_uart1_hw_init();

	return 0;
}

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

#ifdef CONFIG_ATMEL_SPI
	board_spi0_hw_init();
#endif
#ifdef CONFIG_ATMEL_QSPI
	board_qspi_hw_init();
#endif
#ifdef CONFIG_ATMEL_SDHCI
#ifdef CONFIG_ATMEL_SDHCI0
	board_sdhci0_hw_init();
#endif
#ifdef CONFIG_ATMEL_SDHCI1
	board_sdhci1_hw_init();
#endif
#endif
#ifdef CONFIG_MACB
	board_gmac_hw_init();
#endif
#ifdef CONFIG_LCD
	board_lcd_hw_init();
#endif
#ifdef CONFIG_CMD_USB
	board_usb_hw_init();
#endif
#ifdef CONFIG_USB_GADGET_ATMEL_USBA
	at91_udp_hw_init();
#endif

	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
				    CONFIG_SYS_SDRAM_SIZE);
	return 0;
}

/* Set MAC address which is read from EEPROM */
int set_mac_addr_from_eeprom(int eeprom_addr, int reg)
{
	const int MAC_ADDR_LEN = 6;
	unsigned char mac_addr[MAC_ADDR_LEN];
	const char * ETHADDR_NAME = "ethaddr";

	if (getenv(ETHADDR_NAME))
		return 0;	/* No need to set mac address */

	/* Set I2C bus to 0 */
	if (i2c_set_bus_num(0)) {
		printf("i2c bus 0 is not valid\n");
		return -1;
	}

	if (i2c_read(eeprom_addr, reg, 1, mac_addr, MAC_ADDR_LEN)) {
		printf("MAC address read failed from i2c addr: 0x%02x, reg: 0x%02x\n",
				eeprom_addr, reg);
		return -1;
	}

	if (!is_valid_ether_addr(mac_addr)) {
		printf("MAC address read from EEPROM is not valid!\n");
		return -1;
	}

	return eth_setenv_enetaddr(ETHADDR_NAME, mac_addr);
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;

#ifdef CONFIG_MACB
	rc = macb_eth_initialize(0, (void *)ATMEL_BASE_GMAC, 0x00);
	if (rc)
		printf("GMAC register failed\n");
	else
		set_mac_addr_from_eeprom(CONFIG_AT24MAC_ADDR,
					 CONFIG_AT24MAC_REG);
#endif

#ifdef CONFIG_USB_GADGET_ATMEL_USBA
	usba_udc_probe(&pdata);
#ifdef CONFIG_USB_ETH_RNDIS
	usb_eth_initialize(bis);
#endif
#endif

	return rc;
}
