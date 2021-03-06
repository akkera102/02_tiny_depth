#include <stdarg.h>
#include <Arduino.h>
#include <SPI.h>
#include "oled.h"
#include "../res/font.h"


//---------------------------------------------------------------------------
PROGMEM const u8 OledBootProgram[] = {
	0xAE,	// Display Off
	0XD5,	// Set Display Clock Divisor
	0x80,	// 0x80 is default
	0xA8,	// Set Multiplex Ratio
	0x3F,
	0xD3,	// Set Display Offset
	0x00,	// no offset
	0x40,	// Set Start Line (0)
	0x8D,	// Charge Pump Setting
	0x14,	// Enable
	0x20,	// Set Memory Mode
	0x00,	// Horizontal Addressing
	0xA1,	// Set Segment Re-map (A0) | (b0001)
	0xC8,	// Set COM Output Scan Direction
	0xDA,	// Set COM Pins
	0x12,
	0x81,	// Set Contrast
	0xCF,
	0xD9,	// Set Precharge
	0xF1,
	0xDB,	// Set VCom Detect
	0x40,
	0xA4,	// Entire Display ON
	0xA6,	// Set normal/inverse display
	0xAF,	// Display On
	0x20,	// set display mode
	0x00,	// horizontal addressing mode
	0x21,	// set col address
	0x00,
	0x7F,	// COLUMN_ADDRESS_END
	0x22,	// set page address
	0x00,
	0x07,	// PAGE_ADDRESS_END
};

//---------------------------------------------------------------------------
ST_OLED Oled;


//---------------------------------------------------------------------------
void OledInit(void)
{
	_Memset(&Oled, 0x00, sizeof(ST_OLED));


	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);

	pinMode(OLED_PIN_DC,  OUTPUT);
	pinMode(OLED_PIN_CS,  OUTPUT);
	pinMode(OLED_PIN_RST, OUTPUT);

	// VDD (3.3V) goes high at start, lets just chill for a ms
	digitalWrite(OLED_PIN_RST, HIGH);
	delay(1);

	// bring reset low. wait 10ms
	digitalWrite(OLED_PIN_RST, LOW);
	delay(10);

	// bring out of reset
	digitalWrite(OLED_PIN_RST, HIGH);

	// setup the ports we need to talk to the OLED
	vu8* pCs    = portOutputRegister(digitalPinToPort(OLED_PIN_CS));
	vu8* pDc    = portOutputRegister(digitalPinToPort(OLED_PIN_DC));
	u8   csMask = digitalPinToBitMask(OLED_PIN_CS);
	u8   dcMask = digitalPinToBitMask(OLED_PIN_DC);

	// Command Mode
	*pCs |=  csMask;
	*pDc &= ~dcMask;
	*pCs &= ~csMask;

	// Send boot Program
	u8 i;

	for(i=0; i<sizeof(OledBootProgram); i++)
	{
		SPI.transfer(__LPM(OledBootProgram + i));
	}

	// Data Mode
	*pDc |=  dcMask;
	*pCs &= ~csMask;
}
//---------------------------------------------------------------------------
void OledDisplay(void)
{
	u16 i;

	for(i=0; i<sizeof(Oled.buf); i++)
	{
		SPI.transfer(Oled.buf[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawStr(u8 fx, u8 fy, const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);


	u8 i;

	for(i=0; s[i] != '\0'; i++)
	{
		OledDrawChr(fx++, fy, s[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawChr(u8 fx, u8 fy, char chr)
{
	if(fx >= OLED_SCREEN_FONT_CX || fy >= OLED_SCREEN_FONT_CY)
	{
		return;
	}

	u8 x;

	for(x=0; x<OLED_FONT_SIZE; x++)
	{
		Oled.buf[(fx * OLED_FONT_CX + x) + (fy * OLED_SCREEN_CX)] = __LPM(font + (chr * OLED_FONT_SIZE) + x);
	}
}
//---------------------------------------------------------------------------
void OledDrawDot(u8 x, u8 y)
{
	Oled.buf[x + (y / 8) * OLED_SCREEN_CX] |= _BV(y % 8);
}
//---------------------------------------------------------------------------
void OledDrawLineH(u8 y)
{
	u8 x;

	for(x=0; x<OLED_SCREEN_CX; x++)
	{
		Oled.buf[x + (y / 8) * OLED_SCREEN_CX] |= _BV(y % 8);
	}
}
//---------------------------------------------------------------------------
void OledDrawCls(void)
{
//	u16 i;
//	for(i=0; i<sizeof(Oled.buf); i++) Oled.buf[i] = 0x00;

	asm volatile(
		"movw  r30, %0                \n\t"
		"eor __tmp_reg__, __tmp_reg__ \n\t"
		"loop:                        \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"inc __tmp_reg__              \n\t"
		"brne loop                    \n\t"

		: : "r" (Oled.buf) : "r30","r31"
	);
}
//---------------------------------------------------------------------------
void OledDrawBmp(s8 sx, s8 sy, const u8* p)
{
	u8* d  = (u8*)p;
	u8  cx = __LPM(d++);
	u8  cy = __LPM(d++);

	u8 chr, mask;
	u8 x, y, b;

	for(y=0; y<cy; y++)
	{
		for(x=0; x<cx; x+=8)
		{
			if(sy + y < 0 || sy + y >= OLED_SCREEN_CY)
			{
				break;
			}

			chr  = __LPM(d++);
			mask = 0x80;

			for(b=0; b<8; b++)
			{
				if(sx + x + b >= 0 && sx + x + b < OLED_SCREEN_CX)
				{
					if(chr & mask)
					{
						Oled.buf[(sx + x + b) + ((sy + y) / 8) * OLED_SCREEN_CX] |= _BV((sy + y) & 0x7);
					}
				}

			 	mask >>=1;
			}
		}
	}
}
//---------------------------------------------------------------------------
void OledDrawImg8(u8 x, u8 y, const u8* p)
{
	u8* d  = (u8*)p;
	u8  cx = __LPM(d++);
	u8  cy = __LPM(d++);

	if(x + cx > OLED_SCREEN_CX || y + cy > OLED_SCREEN_CY)
	{
		return;
	}


	if(y & 0x7)
	{
		u16 sht1 = y & 0x7;
		u16 sht2 = 8 - sht1;

		u16 adr1 = x + ((y & 0xf8) / 8) * (OLED_SCREEN_CX);
		u16 adr2 = adr1 + OLED_SCREEN_CX;

		u16 chr1, chr2;
		u8  i;

		for(i=0; i<cx; i++)
		{
			chr1 = __LPM(d + i);
			chr2 = __LPM(d + i + cx);

			Oled.buf[adr1 + i] |= (chr1 << sht1);
			Oled.buf[adr2 + i] |= (chr1 >> sht2) | (chr2 << sht1);
		}
	}
	else
	{
		u16 adr = x + (y / 8) * OLED_SCREEN_CX;
		u8  i;

		for(i=0; i<cx; i++)
		{
			Oled.buf[adr + i] |= __LPM(d + i);
		}
	}
}
//---------------------------------------------------------------------------
void OledDrawImg16(u8 x, u8 y, const u8* p)
{
	u8* d  = (u8*)p;
	u8  cx = __LPM(d++);
	u8  cy = __LPM(d++);

	if(x + cx > OLED_SCREEN_CX || y + cy > OLED_SCREEN_CY)
	{
		return;
	}

	if(y & 0x7)
	{
		u16 sht1 = y & 0x7;
		u16 sht2 = 8 - sht1;

		u16 adr1 = x + ((y & 0xf8) / 8) * (OLED_SCREEN_CX);
		u16 adr2 = adr1 + OLED_SCREEN_CX;
		u16 adr3 = adr2 + OLED_SCREEN_CX;

		u16 chr1, chr2;
		u8  i;

		for(i=0; i<cx; i++)
		{
			chr1 = __LPM(d + i);
			chr2 = __LPM(d + i + cx);

			Oled.buf[adr1 + i] |= (chr1 << sht1);
			Oled.buf[adr2 + i] |= (chr1 >> sht2) | (chr2 << sht1);
			Oled.buf[adr3 + i] |= (chr2 >> sht2);
		}
	}
	else
	{
		u16 adr1 = x + (y / 8) * OLED_SCREEN_CX;
		u16 adr2 = adr1 + OLED_SCREEN_CX;

		u8 i;

		for(i=0; i<cx; i++)
		{
			Oled.buf[adr1 + i] |= __LPM(d + i);
			Oled.buf[adr2 + i] |= __LPM(d + i + cx);
		}
	}
}
//---------------------------------------------------------------------------
void OledDrawImgScn(const u8* p)
{
	u8* d = (u8*)p + 2;
	u16 i;

	for(i=0; i<sizeof(Oled.buf); i++)
	{
		Oled.buf[i] = __LPM(d++);;
	}
}
