#include <stdio.h>
#include <stdint.h>
#include <string>
#include <endian.h>
#include <string.h>
#include "chicago_registers.h"

#pragma pack(push, 1)

struct DCS_CMD
{
	uint8_t data_type;
	uint8_t param1;
	uint8_t param2;
	uint8_t zero;
};
		
struct ANX_MIPI_PORT_CMD
{
	uint8_t offset;
	
	union
	{
		uint32_t raw;
		DCS_CMD dcs;
	} value;
};

struct ANX_SLAVE_CMD
{
	uint8_t slave_id;
	uint8_t offset;
	uint8_t value;
};

struct ANX_SPI_CMD
{
	uint8_t offset;
	uint8_t value;
};

#pragma pack(pop)

#define DELAY							0x00AA
#define END								0x00FF

#define DATASHORT_DCS_WRITE_0			0x05
#define DATASHORT_DCS_WRITE_1			0x15
#define DATALONG_DCS_WRITE				0x39

#define	DCS_nop							0x00
#define	DCS_soft_reset					0x01
#define	DCS_get_compression_mode		0x03
#define	DCS_get_error_count_on_DSI		0x05
#define	DCS_get_red_channel				0x06
#define	DCS_get_green_channel			0x07
#define	DCS_get_blue_channel			0x08
#define	DCS_get_power_mode				0x0A
#define	DCS_get_address_mode			0x0B
#define	DCS_get_pixel_format			0x0C
#define	DCS_get_display_mode			0x0D
#define	DCS_get_signal_mode				0x0E
#define	DCS_get_diagnostic_result		0x0F
#define	DCS_enter_sleep_mode			0x10
#define	DCS_exit_sleep_mode				0x11
#define	DCS_enter_partial_mode			0x12
#define	DCS_enter_normal_mode			0x13
#define	DCS_get_image_checksum_rgb		0x14
#define	DCS_get_image_checksum_ct		0x15
#define	DCS_exit_invert_mode			0x20
#define	DCS_enter_invert_mode			0x21
#define	DCS_set_gamma_curve				0x26
#define	DCS_set_display_off				0x28
#define	DCS_set_display_on				0x29
#define	DCS_set_column_address			0x2A
#define	DCS_set_page_address			0x2B
#define	DCS_write_memory_start			0x2C
#define	DCS_write_LUT					0x2D
#define	DCS_read_memory_start			0x2E
#define	DCS_set_partial_rows			0x30
#define	DCS_set_partial_columns			0x31
#define	DCS_set_scroll_area				0x33
#define	DCS_set_tear_off				0x34
#define	DCS_set_tear_on					0x35
#define	DCS_set_address_mode			0x36
#define	DCS_set_scroll_start			0x37
#define	DCS_exit_idle_mode				0x38
#define	DCS_enter_idle_mode				0x39
#define	DCS_set_pixel_format			0x3A
#define	DCS_write_memory_continue		0x3C
#define	DCS_set_3D_control				0x3D
#define	DCS_read_memory_continue		0x3E
#define	DCS_get_3D_control				0x3F
#define	DCS_set_vsync_timing			0x40
#define	DCS_set_tear_scanline			0x44
#define	DCS_get_scanline				0x45
#define	DCS_set_display_brightness		0x51
#define	DCS_get_display_brightness		0x52
#define	DCS_write_control_display		0x53
#define	DCS_get_control_display			0x54
#define	DCS_write_power_save			0x55
#define	DCS_get_power_save				0x56
#define	DCS_set_CABC_min_brightness		0x5E
#define	DCS_get_CABC_min_brightness		0x5F
#define	DCS_read_DDB_start				0xA1
#define	DCS_read_PPS_start				0xA2
#define	DCS_read_DDB_continue			0xA8
#define	DCS_read_PPS_continue			0xA9

#define CONTROL_DISPLAY_BCTRL		(1 << 5) // Backlight Control Block On, Brightness registers are active
#define CONTROL_DISPLAY_DD		(1 << 3) // Display Dimming
#define CONTROL_DISPLAY_BL		(1 << 2) // Backlight Control

struct ANX_MIPI_PORT_CMD inits[/*207*/] = {
	{ DELAY, { .raw = 1000 } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_1, DCS_set_display_brightness, 0xff } } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_1, DCS_write_control_display, 0x2c } } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_1, DCS_write_power_save, 0x00 } } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_0, DCS_exit_sleep_mode, 0x00 } } },
	{ DELAY, { .raw = 80 } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_0, DCS_set_display_on, 0x00 } } },
	{ DELAY, { .raw = 20 } },
	{ GEN_HDR, { .dcs = { DATASHORT_DCS_WRITE_1, DCS_set_tear_on, 0x00 } } },
	{ END }
};

ANX_SLAVE_CMD pdcp_init[] = {
	{ SLAVEID_DPCD, MAX_LINK_RATE, 0x0f },
	{ SLAVEID_DPCD, MAX_LANE_COUNT, 0x82 },
	{ 0x11, 0x00, 0x06 },
	{ 0x11, 0x01, 0x82 },
	{ 0x11, 0x03, 0x01 },
	{ 0x11, 0x04, 0x01 },
	{ 0x11, 0x05, 0x01 },
	{ 0x11, 0x06, 0x01 },
	{ END, END, END }
};

#define SW_H_ACTIVE 1200
#define SW_HFP 40
#define SW_HSYNC 20
#define SW_HBP 40
#define SW_V_ACTIVE 1920
#define SW_VFP 18
#define SW_VSYNC 2
#define SW_VBP 20

ANX_SPI_CMD spi_init[] = {
	{ SW_H_ACTIVE_L, SW_H_ACTIVE & 0xff },
	{ SW_H_ACTIVE_H, SW_H_ACTIVE >> 8 },
	{ SW_HFP_L, SW_HFP & 0xff },
	{ SW_HFP_H, SW_HFP >> 8 },
	{ SW_HSYNC_L, SW_HSYNC & 0xff },
	{ SW_HSYNC_H, SW_HSYNC >> 8 },
	{ SW_HBP_L, SW_HBP & 0xff },
	{ SW_HBP_H, SW_HBP >> 8 },
	{ SW_V_ACTIVE_L, SW_V_ACTIVE & 0xff },
	{ SW_V_ACTIVE_H, SW_V_ACTIVE >> 8 },
	{ SW_VFP_L, SW_VFP & 0xff },
	{ SW_VFP_H, SW_VFP >> 8 },
	{ SW_VSYNC_L, SW_VSYNC & 0xff },
	{ SW_VSYNC_H, SW_VSYNC >> 8 },
	{ SW_VBP_L, SW_VBP & 0xff },
	{ SW_VBP_H, SW_VBP >> 8 },
	{ SW_PANEL_FRAME_RATE, 60 },
	{ SW_PANEL_INFO_0, 3 << REG_MIPI_LANE_COUNT_SHIFT },
	{ SW_PANEL_INFO_1, SET_DPHY_TIMING | (2 << REG_PANEL_TRANS_MODE_SHIFT) }, // DSC_NO_DSC | BURST
	{ MISC_NOTIFY_OCM1, 0x7b }, // ??
	{ MISC_NOTIFY_OCM0, MCU_LOAD_DONE | PANEL_INFO_SET_DONE },
	{ END, END }
};

std::string RemoveSpaces(std::string str)
{
	std::string removed;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != ' ' && str[i] != '\t')
			removed += str[i];
	}
	return removed;
}

size_t FindPattern(std::string buffer, std::string pattern, size_t offset)
{
	pattern = RemoveSpaces(pattern);

	size_t idx = 0;
	while (idx < buffer.size() - pattern.size()/2)
	{
		bool matches = true;
		
		for (unsigned int i = 0; i < pattern.length() / 2; i++)
		{
			std::string chunk = pattern.substr(i * 2, 2);
		
			if (chunk == "??")
				continue;

			if ((uint8_t) buffer[idx + i] != strtol(chunk.c_str(), NULL, 16))
			{
				matches = false;
				break;
			}
		}
		
		if (matches)
			break;
	
		idx++;
	}
	
	if (idx >= buffer.size() - pattern.size()/2)
		return std::string::npos;
		
	return idx + offset;
}

int patchEC(std::string &ec)
{
	std::string ori_ec = ec;

	size_t displayid_offset = FindPattern(ec, "e0 70 09 90 16 01 e0 44 20 f0 80 06 90 16 15 74 80 f0 90 16 0a e0 90 03 56 20 e6 04 74 01 f0 22 74 02 f0 22", 0x21);
	if (displayid_offset == std::string::npos)
	{
		printf("Display id offset not found!\n");
		return -1;
	}
	ec.data()[displayid_offset] = 1; // display_id = 2 -> 1
	
	size_t edid_offset = FindPattern(ec, "00 ff ff ff ff ff ff 00 59 96 01 30 01 00 00 00 1e 1c 01 04 a5 0a 0f 78 16 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 01 00 01 00 01 00 01 00 01 00 01 00 01 00 0f 1b 20 48 30 00 2c 50 20 14 02 04 3c 3c 00 00 00 1e 00 00 00 fc 00 41 4e 58 37 35 33 30 20 55 0a 20 20 20 00 00 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 93", 0);
	if (edid_offset == std::string::npos)
	{
		printf("EDID not found!\n");
		return -1;
	}
	
	uint8_t *edid = (uint8_t *) &ec.data()[edid_offset];
	{
		FILE *f = fopen("edid.bin", "rb");
		fread(edid, 1, 0x80, f);
		fclose(f);
	}
		
	size_t mipi_port_cmds_offset = FindPattern(ec, "aa 00 00 00 64 6c 00 00 e0 15 6c 00 93 e1 15 6c 00 65 e2 15 6c 00 f8 e3 15 6c 00 03 80 15 6c 00 01 e0 15 6c 00 00 00 15 6c 00 8c 01 15 6c 00 00 03 15 6c 00 a8 04 15 6c 00 00 17 15 6c 00 b1 18 15 6c 00 01 19 15 6c 00 00 1a 15 6c 00 b1 1b 15 6c 00 01 1c 15 6c 00 3e 1f 15 6c 00 2d 20 15 6c 00 2d 21 15 6c 00 7e 22 15 6c 00 df 26 15 6c 00 19 37 15 6c 00 05 38 15 6c 00 00 39 15 6c 00 01 3a 15 6c 00 78 3c 15 6c 00 ff 3d 15 6c 00 ff 3e 15 6c 00 ff 3f 15 6c 00 06 40 15 6c 00 a0 41 15 6c 00 08 43 15 6c 00 09 44 15 6c 00 28 45 15 6c 00 0f 55 15 6c 00 89 57 15 6c 00 0a 58 15 6c 00 0a 59 15 6c 00 28 5a 15 6c 00 19 5b 15 6c 00 7c 5d 15 6c 00 67 5e 15 6c 00 58 5f 15 6c 00 4c 60 15 6c 00 47 61 15 6c 00 38 62 15 6c 00 3a 63 15 6c 00 20 64 15 6c 00 36 65 15 6c 00 34 66 15 6c 00 32 67 15 6c 00 4e 68 15 6c 00 3b 69 15 6c 00 40 6a 15 6c 00 31 6b 15 6c 00 2b 6c 15 6c 00 1d 6d 15 6c 00 0b 6e 15 6c 00 00 6f 15 6c 00 7c 70 15 6c 00 67 71 15 6c 00 58 72 15 6c 00 4c 73 15 6c 00 47 74 15 6c 00 38 75 15 6c 00 3a 76 15 6c 00 20 77 15 6c 00 36 78 15 6c 00 34 79 15 6c 00 32 7a 15 6c 00 4e 7b 15 6c 00 3b 7c 15 6c 00 40 7d 15 6c 00 31 7e 15 6c 00 2b 7f 15 6c 00 1d 80 15 6c 00 0b 81 15 6c 00 00 82 15 6c 00 02 e0 15 6c 00 45 00 15 6c 00 45 01 15 6c 00 44 02 15 6c 00 44 03 15 6c 00 47 04 15 6c 00 47 05 15 6c 00 46 06 15 6c 00 46 07 15 6c 00 40 08 15 6c 00 1f 09 15 6c 00 1f 0a 15 6c 00 1f 0b 15 6c 00 1f 0c 15 6c 00 1f 0d 15 6c 00 1f 0e 15 6c 00 41 0f 15 6c 00 1f 10 15 6c 00 1f 11 15 6c 00 1f 12 15 6c 00 1f 13 15 6c 00 1f 14 15 6c 00 1f 15 15 6c 00 45 16 15 6c 00 45 17 15 6c 00 44 18 15 6c 00 44 19 15 6c 00 47 1a 15 6c 00 47 1b 15 6c 00 46 1c 15 6c 00 46 1d 15 6c 00 40 1e 15 6c 00 1f 1f 15 6c 00 1f 20 15 6c 00 1f 21 15 6c 00 1f 22 15 6c 00 1f 23 15 6c 00 1f 24 15 6c 00 41 25 15 6c 00 1f 26 15 6c 00 1f 27 15 6c 00 1f 28 15 6c 00 1f 29 15 6c 00 1f 2a 15 6c 00 1f 2b 15 6c 00 40 58 15 6c 00 10 5b 15 6c 00 06 5c 15 6c 00 20 5d 15 6c 00 01 5e 15 6c 00 02 5f 15 6c 00 66 63 15 6c 00 66 64 15 6c 00 32 67 15 6c 00 08 68 15 6c 00 66 69 15 6c 00 66 6a 15 6c 00 08 6b 15 6c 00 00 6c 15 6c 00 04 6d 15 6c 00 04 6e 15 6c 00 88 6f 15 6c 00 03 e0 15 6c 00 3e 98 15 6c 00 04 9b 15 6c 00 00 af 15 6c 00 04 e0 15 6c 00 10 09 15 6c 00 48 0e 15 6c 00 2b 2c 15 6c 00 44 2f 15 6c 00 2a 36 15 6c 00 ff 49 15 6c 00 ff 51 15 6c 00 21 9a 15 6c 00 25 9b 15 6c 00 26 9c 15 6c 00 00 e0 15 6c 00 02 e6 15 6c 00 06 e7 15 6c 00 ff 51 15 6c 00 2c 53 15 6c 00 00 55 15 aa 00 00 00 32 6c 00 00 11 05 aa 00 00 00 78 6c 00 00 29 05 ab 00 00 00 06 6c 00 03 e0 15 6c 00 0e ab 15 6c 00 00 e0 15 6c 00 00 35 15 ff 00 00 00 00", 0);
	if (mipi_port_cmds_offset == std::string::npos)
	{
		printf("MIPI port cmds not found!\n");
		return -1;
	}
	
	
	ANX_MIPI_PORT_CMD *cmds = (ANX_MIPI_PORT_CMD *) &ec.data()[mipi_port_cmds_offset];
	for (int i = 0; i < sizeof(inits) / sizeof(inits[0]); i++)
	{
		inits[i].value.raw = be32toh(inits[i].value.raw);
	}
	memcpy(cmds, inits, sizeof(inits));
	
	size_t pdcp_cmds_offset = FindPattern(ec, "10 01 06 10 02 82 11 00 06 11 01 82 11 03 01 11 04 01 11 05 01 11 06 01 ff ff ff", 0);
	if (pdcp_cmds_offset == std::string::npos)
	{
		printf("PDCP cmds not found!\n");
		return -1;
	}
	
	ANX_SLAVE_CMD *pdcp_init_cmds = (ANX_SLAVE_CMD *) &ec.data()[pdcp_cmds_offset];
	memcpy(pdcp_init_cmds, pdcp_init, sizeof(pdcp_init));
	
	size_t spi_cmds_offset = FindPattern(ec, "a0 20 a1 03 a2 20 a3 00 a4 14 a5 00 a6 14 a7 00 a8 00 a9 05 aa 10 ab 00 ac 02 ad 00 ae 1a af 00 9d 3c b0 0c b1 48 9f 7b 9e c0 ff ff", 0);
	if (spi_cmds_offset == std::string::npos)
	{
		printf("PDCP cmds not found!\n");
		return -1;
	}
	
	ANX_SPI_CMD *spi_init_cmds = (ANX_SPI_CMD *) &ec.data()[spi_cmds_offset];
	memcpy(spi_init_cmds, spi_init, sizeof(spi_init));
	
	uint16_t checksum = 0;
	for (int i = 0x2000; i < 0x1f7fe; i++)
		checksum += (uint8_t) ec[i];
	*(uint16_t *) &ec.data()[0x1f7fe] = __builtin_bswap16(checksum);
	
	return 0;
}

int main()
{
	std::string bios;

{
	FILE *f = fopen("BIOSIMG.bin", "rb");
	fseek(f, 0, SEEK_END);
	bios.resize(ftell(f));
	fseek(f, 0, SEEK_SET);
	fread(bios.data(), 1, bios.size(), f);
	fclose(f);
}

	std::string ec1 = bios.substr(0, 0x20000);
	std::string ec2 = bios.substr(0x40000, 0x20000);
		
	if(patchEC(ec1) != 0)
		return -1;
	if(patchEC(ec2) != 0)
		return -1;

	memcpy(bios.data(), ec1.data(), ec1.size());
	memcpy(&bios.data()[0x40000], ec2.data(), ec2.size());
	
	size_t offset = FindPattern(bios, "24 42 56 44 54 24", 22);
	if (offset == std::string::npos)
	{
		printf("Version 1 not found!\n");
		return -1;
	}
	while (bios[offset] != 0)
		offset++;
	size_t offset2 = FindPattern(bios.substr(offset), "24 42 56 44 54 24", 22);
	if (offset2 == std::string::npos)
	{
		printf("Version 2 not found!\n");
		return -1;
	}
	offset2 += offset;
	while (bios[offset2] != 0)
		offset2++;
	memcpy(&bios[offset-1], " DeckHD", 7);
	memcpy(&bios[offset2-1], " DeckHD", 7);

{
	FILE *f = fopen("BIOSIMG_DeckHD_EC.bin", "wb");
	fwrite(bios.data(), 1, bios.size(), f);
	fclose(f);
}

	return 0;
}
