#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <test/minitest.h>
#include <gpt_vdo.h>
#include <machine/gpt_io_addrs.h>

struct gpt_vdo_config resolution[] = {

/*index0, 1080p 74.25M, sync packet prog*/
{16, 1, 0, EM_SYNC_RESERVE},
/*1080p sync plannar prog*/
{16, 1, 1, EM_SYNC_RESERVE},
/*1080p embeded packet prog*/
{16, 0, 2, EM_SYNC_1080P24},
/*1080p embeded plannar prog*/
{16, 0, 3, EM_SYNC_1080P24},

/*index4, 720p, 74.25M, sync mode, packet*/
{16, 1, 4, EM_SYNC_RESERVE},
/*720p, sync mode, semi planar*/
{16, 1, 5, EM_SYNC_RESERVE},
/*720p, embeded mode, packet*/
{16, 0, 6, EM_SYNC_720P60},
/*720p, embeded mode, packet, semi plannar*/
{16, 0, 7, EM_SYNC_720P60},

/*index8, 640x480p, 25.2M, sync mode*/
{16, 1, 8, EM_SYNC_RESERVE},
/*640x480p, sync mode, semi planar*/
{16, 1, 9, EM_SYNC_RESERVE},
/*640x480p, embeded mode*/
{16, 0, 10, EM_SYNC_480P},
/*640x480p, embeded mode, semi plannar*/
{16, 0, 11, EM_SYNC_480P},

/*index12, 480p, 25.2M, sync mode*/
{16, 1, 12, EM_SYNC_480P},
/*480p, sync mode, semi plannar*/
{16, 1, 13, EM_SYNC_480P},
/*480p embeded mode*/
{16, 0, 14, EM_SYNC_480P},
/*480p embeded mode, semi planar*/
{16, 0, 15, EM_SYNC_480P},

/*index16, 480i, sync mode interlace*/
{8, 1, 16, EM_SYNC_RESERVE},
/*480i, embeded mode interlace*/
{8, 1, 17, EM_SYNC_RESERVE},
/*480i, sync mode interlace*/
{8, 0, 18, EM_SYNC_480I},
/*480i,  sync mode interlace*/
{8, 0, 19, EM_SYNC_480I},

/*index20,8bit, 576i@50, 27M, sync, packet*/
{8, 1, 20, EM_SYNC_RESERVE},
/*8bit, 576i@50, 27M, sync, semi-plannar*/
{8, 1, 21, EM_SYNC_RESERVE},
/*8bit, 576i@50, 27M, embeded, packet*/
{8, 0, 22, EM_SYNC_576I},
/*8bit, 576i@50, 27M, embeded, semi-plannar*/
{8, 0, 23, EM_SYNC_576I},

/*index 24,720p@60 8bit syncmode, interlace*/
{8, 1, 24, EM_SYNC_720P60},
/*720p@60 8bit syncmode, interlace*/
{8, 1, 25, EM_SYNC_720P60},
/*720p@60 8bit syncmode, prog*/
{8, 0, 26, EM_SYNC_720P60},
/*720p@60 8bit embeded mode, interlace*/
{8, 0, 27, EM_SYNC_720P60},

/*index 28,1920x1080p@30 16bit syncmode, prog*/
{16, 1, 28, EM_SYNC_1080P24},
/*1920x1080p@60 16bit syncmode, prog*/
{16, 1, 29, EM_SYNC_1080P60},
/*1920x1080p@60 16bit syncmode, prog*/
{16, 1, 30, EM_SYNC_1080P60},
/*reserve*/
{16, 1, 31, 0},

/*index32 1920x1080i@60 16bit sync, packet*/
{16, 1, 32, EM_SYNC_1080P60},
/*1920x1080i@60 16bit sync, semi-plannar*/
{16, 1, 33, EM_SYNC_1080P60},
/*1920x1080i@60 16bit embeded , packet*/
{16, 0, 34, EM_SYNC_1080P60},
/*1920x1080i@60 16bit embeded, semi-plannar*/
{16, 0, 35, EM_SYNC_1080P60},

/*index36 480p@60 54M, 8bit sync, packet*/
{8, 1, 36, EM_SYNC_480P},
/*480p@30 54M, 8bit sync, semi-plannar*/
{8, 1, 37, EM_SYNC_480P},
/*480p@60 54M, 8bit sync, packet*/
{8, 0, 38, EM_SYNC_480P},
/*480p@60 54M, 8bit sync, semi-plannar*/
{8, 0, 39, EM_SYNC_480P},

/*index40 640x480p@60 8bit, 50.4M, sync, packet*/
{8, 1, 40, EM_SYNC_480P},
/*640x480p@60 8bit, 50.4M, sync, packet*/
{8, 1, 41, EM_SYNC_480P},
/*640x480p@60 8bit, 50.4M, sync, packet*/
{8, 0, 42, EM_SYNC_480P},
/*640x480p@60 8bit, 50.4M, sync, packet*/
{8, 0, 43, EM_SYNC_480P},

/*index44 480p@60 10bit, 54M, sync, packet*/
{10, 1, 44, EM_SYNC_480P},
/*480p@60 10bit, 54M, sync, packet*/
{10, 1, 45, EM_SYNC_480P},
/*480p@60 10bit, 54M, sync, packet*/
{10, 0, 46, EM_SYNC_480P},
/*480p@60 10bit, 54M, sync, packet*/
{10, 0, 47, EM_SYNC_480P},

/*index48 1080p@30 8bit, 148.5M, sync, packet*/
{8, 1, 48, EM_SYNC_1080P24},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 1, 49, EM_SYNC_1080P24},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 0, 50, EM_SYNC_1080P24},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 0, 51, EM_SYNC_1080P24},

/*index52 640x480p@60 8bit, 50.4M, sync, packet*/
{10, 1, 52, EM_SYNC_480P},
/*640x480p@60 10bit, 50.4M, sync, packet*/
{10, 1, 53, EM_SYNC_480P},
/*640x480p@60 10bit, 50.4M, sync, packet*/
{10, 0, 54, EM_SYNC_480P},
/*640x480p@60 10bit, 50.4M, sync, packet*/
{10, 0, 55, EM_SYNC_480P},

/*index56 720p@60 10bit, 148.5M, sync, packet*/
{10, 1, 56, EM_SYNC_720P60},
/*720p@60 10bit, 148.5M, sync, semi-plannar*/
{10, 1, 57, EM_SYNC_720P60},
/*720p@60 10bit, 148.5M, embeded, packet*/
{10, 0, 58, EM_SYNC_720P60},
/*720p@60 10bit, 148.5M, embeded, semi-plannar*/
{10, 0, 59, EM_SYNC_720P60},

/*index60 720p@60 10bit, 148.5M, sync, packet*/
{10, 1, 60, EM_SYNC_1080P30},
/*720p@60 10bit, 148.5M, sync, semi-plannar*/
{10, 1, 61, EM_SYNC_1080P30},
/*720p@60 10bit, 148.5M, embeded, packet*/
{10, 0, 62, EM_SYNC_1080P30},
/*720p@60 10bit, 148.5M, embeded, semi-plannar*/
{10, 0, 63, EM_SYNC_1080P30},

/*index64, 480i, sync mode interlace*/
{10, 1, 64, EM_SYNC_480I},
/*480i, embeded mode interlace*/
{10, 1, 65, EM_SYNC_480I},
/*480i, sync mode interlace*/
{10, 1, 66, EM_SYNC_480P},
/*480i, sync mode interlace*/
{10, 1, 67, EM_SYNC_480P},

/*index68,8bit, 576i@50, 27M, sync, packet*/
{10, 1, 68, EM_SYNC_576I},
/*8bit, 576i@50, 27M, sync, semi-plannar*/
{10, 1, 69, EM_SYNC_576I},
/*8bit, 576i@50, 27M, embeded, packet*/
{10, 1, 70, EM_SYNC_576P},
/*8bit, 576i@50, 27M, embeded, semi-plannar*/
{10, 1, 71, EM_SYNC_576P},

/*index72 1080p@30 8bit, 148.5M, sync, packet*/
{8, 1, 72, EM_SYNC_1080P30},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 1, 73, EM_SYNC_1080P30},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 0, 74, EM_SYNC_1080P30},
/*1080p@30 8bit, 148.5M, sync, packet*/
{8, 0, 75, EM_SYNC_1080P30},

/*index72, 480p, 25.2M, sync mode*/
//{16, 1, 72, EM_SYNC_480P},
/*480p, sync mode, semi plannar*/
//{16, 1, 73, EM_SYNC_480P},
/*480p embeded mode*/
//{16, 0, 74, EM_SYNC_480P},
/*480p embeded mode, semi planar*/
//{16, 0, 75, EM_SYNC_480P},

};

static void do_vdo_menu(void)
{
	debug("resolution menu: \n");
	debug("idex00 16 1080P@30 \t  74.25M \t sync\t packet\n");
	debug("idex01 16 1080P@30 \t  74.25M \t sync\t semi\n");
	debug("idex02 16 1080P@30 \t  74.25M \t nosync\t packet\n");
	debug("idex03 16 1080P@30 \t  74.25M \t nosync\t semi\n");

	debug("idex04 16 720P@60 \t  74.25M \t sync\t packet\n");
	debug("idex05 16 720P@60 \t  74.25M \t sync\t semi\n");
	debug("idex06 16 720P@60 \t  74.25M \t nosync\t packet\n");
	debug("idex07 16 720P@60 \t  74.25M \t nosync\t semi\n");

	debug("idex08 16 640x480 \t  25.2M \t sync\t packet\n");
	debug("idex09 16 640x480 \t  25.2M \t sync\t semi\n");
	debug("idex10 16 640x480 \t  25.2M \t nosync\t packet\n");
	debug("idex11 16 640x480 \t  25.2M \t nosync\t semi\n");

	debug("idex12 16 480p@60 \t  25.2M \t sync\t packet\n");
	debug("idex13 16 480p@60 \t  25.2M \t sync\t semi\n");
	debug("idex14 16 480p@60 \t  25.2M \t nosync\t packet\n");
	debug("idex15 16 480p@p60 \t  25.2M \t nosync\t semi\n");

	debug("idex16 08 480i@60 \t  27.2M \t sync\t packet\n");
	debug("idex17 08 480i@60 \t  27.2M \t sync\t semi\n");
	debug("idex18 08 480i@60 \t  27.2M \t nosync\t packet\n");
	debug("idex19 08 480i@60 \t  27.2M \t nosync\t semi\n");

	debug("idex20 08 576i@60 \t  27.2M \t sync\t packet\n");
	debug("idex21 08 576i@60 \t  27.2M \t sync\t semi\n");
	debug("idex22 08 576i@60 \t  27.2M \t nosync\t packet\n");
	debug("idex23 08 576i@60 \t  27.2M \t nosync\t semi\n");

	debug("idex24 08 720p@60 \t 148.5M \t sync\t packet\n");
	debug("idex25 08 720p@60 \t 148.5M \t sync\t semi\n");
	debug("idex26 08 720p@60 \t 148.5M \t nosync\t packet\n");
	debug("idex27 08 720p@60 \t 148.5M \t nosync\t semi\n");

	debug("idex28 16 1080p@30 \t  74.5M \t sync\t packet\n");
	debug("idex29 16 1080p@60 \t 148.5M \t sync\t semi\n");
	debug("idex30 16 1080p@60 \t 148.5M \t sync\t packet\n");
	debug("idex31 08 reserve  \t 148.5M \t sync\t semi\n");

	debug("idex32 16 1080i@60 \t  74.5M \t sync\t packet\n");
	debug("idex33 16 1080i@60 \t  74.5M \t sync\t semi\n");
	debug("idex34 16 1080i@60 \t  74.5M \t nosync\t packet\n");
	debug("idex35 16 1080i@60 \t  74.5M \t nosync\t semi\n");

	debug("idex36 08 480p@60 \t    54M \t sync\t packet\n");
	debug("idex37 08 480p@60 \t    54M \t sync\t semi\n");
	debug("idex38 08 480p@60 \t    54M \t nosync\t packet\n");
	debug("idex39 08 480p@60 \t    54M \t nosync\t semi\n");

	debug("idex40 08 640x480 \t  50.4M \t sync\t packet\n");
	debug("idex41 08 640x480 \t  50.4M \t sync\t semi\n");
	debug("idex42 08 640x480 \t  50.4M \t nosync\t packet\n");
	debug("idex43 08 640x480 \t  50.4M \t nosync\t semi\n");

	debug("idex44 10 480p@60 \t    54M \t sync\t packet\n");
	debug("idex45 10 480p@60 \t    54M \t sync\t semi\n");
	debug("idex46 10 480p@60 \t    54M \t nosync\t packet\n");
	debug("idex47 10 480p@60 \t    54M \t nosync\t semi\n");

	debug("idex48 08 1080p@24 \t 148.5M \t sync\t packet\n");
	debug("idex49 08 1080p@24 \t 148.5M \t sync\t semi\n");
	debug("idex50 08 1080p@24 \t 148.5M \t nosync\t packet\n");
	debug("idex51 08 1080p@24 \t 148.5M \t nosync\t semi\n");

	debug("idex52 10 640x480 \t  50.4M \t sync\t packet\n");
	debug("idex53 10 640x480 \t  50.4M \t sync\t semi\n");
	debug("idex54 10 640x480 \t  50.4M \t nosync\t packet\n");
	debug("idex55 10 640x480 \t  50.4M \t nosync\t semi\n");

	debug("idex56 10 720p@60 \t 148.5M \t sync\t packet\n");
	debug("idex57 10 720p@60 \t 148.5M \t sync\t semi\n");
	debug("idex58 10 720p@60 \t 148.5M \t nosync\t packet\n");
	debug("idex59 10 720p@60 \t 148.5M \t nosync\t semi\n");

	debug("idex60 10 1080p@30 \t 148.5M \t sync\t packet\n");
	debug("idex61 10 1080p@30 \t 148.5M \t sync\t semi\n");
	debug("idex62 10 1080p@30 \t 148.5M \t nosync\t packet\n");
	debug("idex63 10 1080p@30 \t 148.5M \t nosync\t semi\n");

	debug("idex64 10 480i@60 \t    27M \t sync\t packet\n");
	debug("idex65 10 480i@60 \t    27M \t sync\t semi\n");
	debug("idex66 10 480i@60 \t    27M \t nosync\t packet\n");
	debug("idex67 10 480i@60 \t    27M \t nosync\t semi\n");

	debug("idex68 10 576i@60 \t    27M \t sync\t packet\n");
	debug("idex69 10 576i@60 \t    27M \t sync\t semi\n");
	debug("idex70 10 576i@60 \t    27M \t nosync\t packet\n");
	debug("idex71 10 576i@60 \t    27M \t nosync\t semi\n");
	
	debug("idex72 08 1080p@30 \t 148.5M \t sync\t packet\n");
	debug("idex73 08 1080p@30 \t 148.5M \t sync\t semi\n");
	debug("idex74 08 1080p@30 \t 148.5M \t nosync\t packet\n");
	debug("idex75 08 1080p@30 \t 148.5M \t nosync\t semi\n");
}

static void do_vdo_help(void)
{
	debug("GPT Vdo Controller Test\n");
	debug("\tinit      \t adv7511 init function\n");
	debug("\tmode      \t mode 1-64\n");
	debug("\tbasetest  \t adv7511 init function\n");
	debug("\tread off  \t reg offset of 7511 (hex)\n");
	debug("\twrite off data\treg offset of 7511(hex), data(hex)\n");
	debug("\treadreg off  \t reg offset of host(hex)\n");
	debug("\twritereg off data\treg offset of host(hex), data(hex)\n");
	debug("\tposition  \t get debug register in adv7511\n");
	debug("\tdata type\t generate data for video out\n");
	debug("\tbackground\t set color of background\n");
	debug("\tconfig dma_h dma_v dma_bust simip_en \t gpt video out dma configs\n");
	debug("\tresolution\t gpt video out set up resolution\n");
	debug("\trun num   \t video out transfer data, num: times \n");
	debug("\trgb2yuv mode\t change rgb to yuv, mode 1: 601, mode 2: 709\n");
    debug("\tclk_sel     \t clk select, option:1, 2, 3 \n");
    debug("\tclk_delay   \t clk delay, option:0~0x1F. \n");
    debug("\tdpregs      \t dump all reg and fill test. every reg write 0xffffffff.\n");
}

#define is_digit(c)	((c) >= '0' && (c) <= '9')
static int atoi(const char *str)
{
	int c, total;

	c = *str++;
	total = 0;
	while (is_digit(c)) {
		total = total * 10 + (c - '0');
		c = *str++;
	}

	return total;
}

static unsigned int atohex(const char *str)
{
        unsigned int num = 0;
        int len = strlen(str);
        int i = 0;

        if ((str[0] == '0' && str[1] == 'x') || (str[0] == '0' && str[1] == 'X'))
                i += 2;
        for (; i < len; i++) {
                if (str[i] <= '9')
                        num += ((str[i] - '0') << ((len - i - 1) * 4));
		else if (str[i] <= 'Z')
                        num += ((str[i] - 'A' + 10) << ((len - i - 1) * 4));
                else
                        num += ((str[i] - 'a' + 10) << ((len - i - 1) * 4));
        }

        return num;
}

static int get_user_argument(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i,j;
	char *delim = "=,:";
	char *token;
	int data = 0;
	int offset = 0;
	char buf[1024];
	int dma_h = 0;
	int dma_v = 0;
	int dma_burst_len = 0;
	int dma_simip_en = 0;
	int y, cb, cr;
	//uint8_t clk_num= 0;
	static int clk_sel = 3;
	static int mode = 0;
	static int genData=0;
	int ret_status = 0;

	if((!strcmp((argv[1]), "help")) || (argc < 2)) {
		do_vdo_help();
		return -1;
	}


	if((!strcmp((argv[1]), "menu")) || (argc < 2)) {
		do_vdo_menu();
		return -1;
	}

	for (i = 1; i < argc; i++) {
		strcpy(buf, argv[i]);
		token = strtok(buf, delim);
		int val = 0;
		if(!strcmp(token, "clk_sel")){
			token = strtok(NULL, delim);
			val = atoi(token);
			clk_sel = val;
			set_video_out_clk(clk_sel);
			//debug("Set clk_sel:%d\n", clk_sel);
			continue;
		}
		if(!strcmp(token, "clk_delay")){
			token = strtok(NULL, delim);
			val = atoi(token);
			gpt_vdo_enable_clock_delay(val);
			//debug("Set clk_delay:%d\n", val);
			continue;
		}
		
		if (!strcmp(token, "init")) {
			set_video_out_clk(clk_sel);//cap2
			ret_status = gpt_vdo_init();
			ret_status += adv7511_init();
			continue;
		}

        if (!strcmp(token, "dumpreg")) {
			set_video_out_clk(clk_sel);//cap2
			ret_status = gpt_vdo_init();
			ret_status += adv7511_init();
			continue;
		}

		if (!strcmp(token, "stop")) {
			gpt_vdo_disable();
			continue;
		}

		if (!strcmp(token, "memu")) {
			do_vdo_menu();
			continue;
		}

		if (!strcmp(token, "config")) {
			if (argc < 5) {
				do_vdo_help();
				continue;
			}
			dma_h  = atohex(argv[++i]);
			dma_v = atohex(argv[++i]);
			dma_burst_len = atohex(argv[++i]);
			dma_simip_en = atohex(argv[++i]);
			debug("dma config dma_h:0x%x, dma_v:0x%x, dma_burst:0x%x, simip_en:0x%x\n",
						dma_h, dma_v, dma_burst_len, dma_simip_en);
			gpt_vdo_set_configs(dma_h, dma_v, dma_burst_len, dma_simip_en);
			continue;
		}

		if (!strcmp(token, "background")) {
			if (argc < 4) {
				do_vdo_help();
				continue;
			}

			y  = atohex(argv[++i]);
			cb = atohex(argv[++i]);
			cr = atohex(argv[++i]);
			debug("background clor Y:0x%x, cb:0x%x, cr:0x%x\n", y, cb, cr);
			gpt_vdo_background(y, cb, cr);
			continue;
		}

		if (!strcmp(token, "yuv")) {
			if (argc < 4) {
				do_vdo_help();
				continue;
			}

			y  = atoi(argv[++i]);
			cb = atoi(argv[++i]);
			cr = atoi(argv[++i]);
			debug("background clor Y:0x%x, cb:0x%x, cr:0x%x\n", y, cb, cr);
			gpt_vdo_yuvdata(y, cb, cr);
			continue;
		}

		if (!strcmp(token, "run")) {
			if (argc < 2) {
				do_vdo_help();
				continue;
			}
			y  = atohex(argv[++i]);
			debug("count of transfering data :0x%x\n", y);
			gpt_vdo_run(y);
			continue;
		}


		if (!strcmp(token, "show")) {
			if (argc < 2) {
				do_vdo_help();
				continue;
			}
			y  = atohex(argv[++i]);
			debug("count of transfering data :0x%x\n", y);
			for (j = 0; j < y; j++) {
				gpt_vdo_data_generate((j%5) + 1, mode);
				udelay(10* 1000*1000);
			}
			continue;
		}

		if (!strcmp(token, "readreg")) {
			if (argv[++i] != NULL) {
				offset = atoi(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			offset = atohex(argv[i]);
			data =gpt_vdo_reg_read(offset);
			debug("read 0x%x from 0x%x \n", data, offset);
			continue;
		}

		if (!strcmp(token, "writereg")) {
			if (argv[++i] != NULL)
				offset = atohex(argv[i]);
			else {
				do_vdo_help();
				continue;
			}
			if (argv[++i] != NULL) {
				data = atohex(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			debug("write 0x%x to 0x%x \n", data, offset);
			gpt_vdo_reg_write(offset, data);
			continue;
		}

		if (!strcmp(token, "read")) {
			if (argv[++i] != NULL) {
				offset = atoi(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			offset = atohex(argv[i]);
			data = adv7511_read(offset);
			debug("read 0x%x from 0x%x \n", data, offset);
			continue;
		}

		if (!strcmp(token, "write")) {
			if (argv[++i] != NULL)
				offset = atohex(argv[i]);
			else {
				do_vdo_help();
				continue;
			}
			if (argv[++i] != NULL) {
				data = atohex(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			debug("write 0x%x to 0x%x \n", data, offset);
			adv7511_write(offset, data);
			continue;
		}

		if (!strcmp(token, "position")) {
			adv7511_show_positon();
			continue;
		}


		if (!strcmp(token, "data")) {
			if (argv[++i] != NULL) {
				genData  = atohex(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			debug("data type: %x \n", genData);
			gpt_vdo_data_generate(genData, mode);
			continue;
		}

		if (!strcmp(token, "dpregs")) {
            gpt_vdo_dumpregs();
            gpt_vdo_compare_regs();
            gpt_vdo_fill_regs();
            gpt_vdo_dumpregs();
            gpt_vdo_compare_regs();
			continue;
		}

		if (!strcmp(token, "mode")) {
			if (argv[++i] != NULL) {
				mode  = atoi(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			debug("id:%d,bittype: %d, mode: %d, resl:%d,  hostid:%d \n", mode, resolution[mode].bittype, resolution[mode].syncmode,  resolution[mode].resl ,resolution[mode].hostid);
			ret_status += gpt_vdo_mode(resolution[mode].hostid);
			ret_status += adv7511_set_resolution(resolution[mode].bittype, resolution[mode].syncmode, resolution[mode].resl);
			continue;
		}

		if (!strcmp(token, "basetest")) {
			memset((void*)GPT_VDO_APB_BASE,0xff,0x100);
			gpt_vdo_dumpregs();
			memset((void*)GPT_VDO_APB_BASE,0x00,0x100);
			gpt_vdo_dumpregs();
			continue;
		}

		if (!strcmp(token, "pxiaddr")) {
			gpt_vdo_dma_init(GPT_VDO_DMA_START, GPT_DATA_PXI_ADDR, GPT_DATA_S_PXI_ADDR, 0x10);
			continue;
		}

		if (!strcmp(token, "rgb2yuv")) {
			if (argv[++i] != NULL) {
				offset = atoi(argv[i]);
			} else {
				do_vdo_help();
				continue;
			}
			offset = atohex(argv[i]);
			debug("mode %d\n", offset);
			gpt_vdo_color_change_loop(offset);
			continue;
		}
		debug("Bad argument: <%s>\n", token);
		return -1;
	}

	if (ret_status != 0){
		debug("video out test fail !\n");
		return ret_status;
	}
	else {
		debug("video out test pass !\n");
		return 0;
	}
	
}
/*video out clk soure select
 *0: from vcap0
 *1: from vcap1
 *2: from vcap2
 *3: from other 
 *default: from vcap0 
 * */
void set_video_out_clk(uint8_t vcapn){
	u32 val = 0;
	val = readl_debug((unsigned int *)0xF0000010);
	debug("%s,%d  val 0x%x\n",__func__,__LINE__,val);

	switch(vcapn){
		case 0:
			writel_debug(val & 0x9FFFFFFF, (unsigned int *)0xF0000010);
			debug("%s,%d video out clk select 0x%x\n",__func__,__LINE__,readl_debug((unsigned int *)0xF0000010));
			break;
		case 1:
			writel_debug(val | 0x20000000, (unsigned int *)0xF0000010);
			debug("%s,%d video out clk select 0x%x\n",__func__,__LINE__,readl_debug((unsigned int *)0xF0000010));
			break;
		case 2:
			writel_debug(val | 0x40000000, (unsigned int *)0xF0000010);
			break;
		case 3:
			writel_debug(val | 0x60000000, (unsigned int *)0xF0000010);
			debug("%s,%d video out clk select external  0x%x\n",__func__,__LINE__,readl_debug((unsigned int *)0xF0000010));
			break;
		default:
			writel_debug(val&0x9FFFFFFF, (unsigned int *)0xF0000010);
			break;
	}
}

static void pinmux_vout_reg(void){
	unsigned int val = 0;
	val = readl_debug((unsigned int *)(GPTXSOC_MMIO_BASE + 0x200));
#if defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	writel_debug(val | 0x8000040, (unsigned int *)(GPTXSOC_MMIO_BASE + 0x200));
#else
	writel_debug(val | 0x40, (unsigned int *)(GPTXSOC_MMIO_BASE + 0x200));
#endif
	//debug("pinmux select 0x%x \n", readl_debug((unsigned int *)(GPTXSOC_MMIO_BASE + 0x200)));
}

int do_video_out_test(cmd_tbl_t *cmdtp,
		int flag, int argc, char * const argv[])
{
#ifdef CONFIG_TEST_ON_SOC
	pinmux_vout_reg();
#if (defined(CONFIG_GPT_VIDMEM_TEST) || defined(CONFIG_GPT_VDO_VDMEM))
	open_vidmem();
	pxi_ctl();
#endif
#endif
	get_user_argument(cmdtp, flag, argc, argv);
	return 0;
}

U_BOOT_MINITEST_CMD(vdo, CONFIG_SYS_MAXARGS, 0,
			do_video_out_test, "video out test.", "video out control test");

static char vdo_help_text [] =
	"vdo init mode 4\n"
	"vdo clk_sel=num[0,1,2,3] clk_delay=num[0~0x1f] init mode num[0~73]\n";

U_BOOT_CMD(
	vdo, 6, 1, do_video_out_test,
	"GPT VIDEO TEST sub-system",
	vdo_help_text
);
