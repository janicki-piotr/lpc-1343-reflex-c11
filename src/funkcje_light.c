/*
 * funkcje_light.c
 *
 *  Created on: 20 lis 2017
 *      Author: A
 */
#include "mcu_regs.h"
#include "type.h"
#include "funkcje_i2c.h"
#include "funkcje_light.h"

#define LIGHT_I2C_ADDR    (0x44 << 1)

#define ADDR_CMD        0x00
#define ADDR_CTRL       0x01
#define ADDR_IRQTH_HI   0x02
#define ADDR_IRQTH_LO   0x03
#define ADDR_LSB_SENSOR 0x04
#define ADDR_MSB_SENSOR 0x05
#define ADDR_LSB_TIMER  0x06
#define ADDR_MSB_TIMER  0x07

#define ADDR_CLAR_INT   0x40

#define CMD_ENABLE    (1 << 7)
#define CMD_APDCP     (1 << 6)
#define CMD_TIM_EXT   (1 << 5)
#define CMD_MODE(m)  ((m) << 2)
#define CMD_WIDTH(w) ((w) << 0)

#define CTRL_GAIN(g)        ((g) << 2)
#define CTRL_IRQ_PERSIST(p) ((p) << 0)
#define CTRL_IRQ_FLAG       (1 << 5)

/*
 * The Range (k) values are based on Rext = 100k
 */
#define RANGE_K1   973
#define RANGE_K2  3892
#define RANGE_K3 15568
#define RANGE_K4 62272

#define WIDTH_16_VAL (1 << 16)
#define WIDTH_12_VAL (1 << 12)
#define WIDTH_08_VAL (1 << 8)
#define WIDTH_04_VAL (1 << 4)

static uint32_t range = RANGE_K1;
static uint32_t width = WIDTH_16_VAL;

void light_init(void)
{
	/* nothing to initialize. light_enable enables the sensor */
}

void light_enable(void)
{
	uint8_t buf[2];
	buf[0] = ADDR_CMD;
	buf[1] = CMD_ENABLE;
	I2CWrite(LIGHT_I2C_ADDR, buf, 2);

	range = RANGE_K1;
	width = WIDTH_16_VAL;
}

static uint8_t readControlReg(void)
{
    uint8_t buf[1];
    buf[0] = ADDR_CTRL;
    I2CWrite(LIGHT_I2C_ADDR, buf, 1);

    I2CRead(LIGHT_I2C_ADDR, buf, 1);

    return buf[0];
}

void light_setRange(light_range_t newRange)
{
	uint8_t buf[2];
	uint8_t ctrl = readControlReg();

	/* clear range */
	ctrl &= ~(3 << 2);

	ctrl |= CTRL_GAIN(newRange);

	buf[0] = ADDR_CTRL;
	buf[1] = ctrl;
	I2CWrite(LIGHT_I2C_ADDR, buf, 2);

	switch (newRange) {
	case LIGHT_RANGE_1000:
		range = RANGE_K1;
		break;
	case LIGHT_RANGE_4000:
		range = RANGE_K2;
		break;
	case LIGHT_RANGE_16000:
		range = RANGE_K3;
		break;
	case LIGHT_RANGE_64000:
		range = RANGE_K4;
		break;
	}
}
