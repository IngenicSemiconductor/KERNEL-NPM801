
obj-y += printer-misc.o printer-pm.o printer-lcd.o  

obj-$(CONFIG_I2C)		+= printer-i2c.o
obj-$(CONFIG_MFD_ACT8600)	+= printer-regulator.o
obj-$(CONFIG_MMC_JZ4780)	+= printer-mmc.o
obj-$(CONFIG_JZ4780_INTERNAL_CODEC) += printer-sound.o
obj-$(CONFIG_NAND_JZ4780)	+= printer-nand.o

ifeq ($(CONFIG_BOARD_TROOPER), y)
EXTRA_CFLAGS+=-DCONFIG_BOARD_WARRIOR_V_1_1
endif
