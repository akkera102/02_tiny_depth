#include <avr/eeprom.h>
#include "eep.h"

//---------------------------------------------------------------------------
ST_EEP Eep;


//---------------------------------------------------------------------------
void EepInit(void)
{
	_Memset(&Eep, 0x00, sizeof(ST_EEP));
}
//---------------------------------------------------------------------------
void EepSeek(u16 adr)
{
	Eep.adr = (u8*)adr;
}
//---------------------------------------------------------------------------
u8 EepRead8(void)
{
	eeprom_busy_wait();
	return eeprom_read_byte(Eep.adr++);
}
//---------------------------------------------------------------------------
u16 EepRead16(void)
{
	u16 ret = 0;

	eeprom_busy_wait();
	ret |= eeprom_read_byte(Eep.adr++);
	eeprom_busy_wait();
	ret |= eeprom_read_byte(Eep.adr++) << 8;

	return ret;
}
//---------------------------------------------------------------------------
void EepWrite8(u8 data)
{
	eeprom_busy_wait();
	eeprom_write_byte(Eep.adr++, data);
}
//---------------------------------------------------------------------------
void EepWrite16(u16 data)
{
	eeprom_busy_wait();
	eeprom_write_byte(Eep.adr++, data & 0xff);
	eeprom_busy_wait();
	eeprom_write_byte(Eep.adr++, data >> 8);
}
