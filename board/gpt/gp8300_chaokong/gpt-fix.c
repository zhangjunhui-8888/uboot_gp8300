#include <common.h>
#include <asm/sections.h>
#include <asm/io.h>
#include <i2c.h>

//#define __DEBUG 1
void i2c_init_board(void)
{
	int max = 0;
	int count;

	max = ll_entry_count(struct i2c_adapter, i2c);
	for (count = 0; count < max; count++) {
		i2c_set_bus_num(count);
	}
}

uint8_t i2c_reg_bus_read(uint8_t bus, uint8_t addr, uint8_t reg)
{
	uint8_t buf;

	if (bus != i2c_get_bus_num())
		i2c_set_bus_num(bus);

	i2c_read(addr, reg, 1, &buf, 1);

#ifdef __DEBUG
	printf("%s: bus=%d addr=0x%02x, reg=0x%02x, val=0x%02x\n",
	       __func__, i2c_get_bus_num(), addr, reg, buf);
#endif

	return buf;
}


void i2c_reg_bus_write(uint8_t bus, uint8_t addr, uint8_t reg, uint8_t val)
{
	if (bus != i2c_get_bus_num())
		i2c_set_bus_num(bus);

#ifdef __DEBUG
	printf("%s: bus=%d addr=0x%02x, reg=0x%02x, val=0x%02x\n",
	       __func__, i2c_get_bus_num(), addr, reg, val);
#endif

	i2c_write(addr, reg, 1, &val, 1);
}
