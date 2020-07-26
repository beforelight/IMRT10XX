
#include "svbmp.h"
#include <utilities/fsl_debug_console.h>
#define BMP_GRAY_COLORS(x) (uint32_t)((x)|(x<<8)|(x<<16))
#pragma  pack(1)
typedef struct tagBITMAPFILEHEADER {
    /* bmfh 14bytes */
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

#pragma  pack(1)
typedef struct tagBITMAPINFOHEADER {
    /* bmih 54bytes */
    BITMAPFILEHEADER bmfh;
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

#pragma  pack(1)
typedef struct tagBITMAPGRAYHEADER {
    /* bmgh 1080bytes */
    //BITMAPINFOHEADER bmif;
    uint32_t bmColors[256];
} BITMAPGRAYHEADER;

#pragma  pack(1)
typedef struct tagBITMAPRGBHEADER {
    /* bmrh 72bytes */
    //BITMAPINFOHEADER bmif;
    uint32_t bmColors[4];
} BITMAPRGBHEADER;

const BITMAPGRAYHEADER bmp_gray_color_palette = {
.bmColors[0] = BMP_GRAY_COLORS(0),
.bmColors[1] = BMP_GRAY_COLORS(1),
.bmColors[2] = BMP_GRAY_COLORS(2),
.bmColors[3] = BMP_GRAY_COLORS(3),
.bmColors[4] = BMP_GRAY_COLORS(4),
.bmColors[5] = BMP_GRAY_COLORS(5),
.bmColors[6] = BMP_GRAY_COLORS(6),
.bmColors[7] = BMP_GRAY_COLORS(7),
.bmColors[8] = BMP_GRAY_COLORS(8),
.bmColors[9] = BMP_GRAY_COLORS(9),
.bmColors[10] = BMP_GRAY_COLORS(10),
.bmColors[11] = BMP_GRAY_COLORS(11),
.bmColors[12] = BMP_GRAY_COLORS(12),
.bmColors[13] = BMP_GRAY_COLORS(13),
.bmColors[14] = BMP_GRAY_COLORS(14),
.bmColors[15] = BMP_GRAY_COLORS(15),
.bmColors[16] = BMP_GRAY_COLORS(16),
.bmColors[17] = BMP_GRAY_COLORS(17),
.bmColors[18] = BMP_GRAY_COLORS(18),
.bmColors[19] = BMP_GRAY_COLORS(19),
.bmColors[20] = BMP_GRAY_COLORS(20),
.bmColors[21] = BMP_GRAY_COLORS(21),
.bmColors[22] = BMP_GRAY_COLORS(22),
.bmColors[23] = BMP_GRAY_COLORS(23),
.bmColors[24] = BMP_GRAY_COLORS(24),
.bmColors[25] = BMP_GRAY_COLORS(25),
.bmColors[26] = BMP_GRAY_COLORS(26),
.bmColors[27] = BMP_GRAY_COLORS(27),
.bmColors[28] = BMP_GRAY_COLORS(28),
.bmColors[29] = BMP_GRAY_COLORS(29),
.bmColors[30] = BMP_GRAY_COLORS(30),
.bmColors[31] = BMP_GRAY_COLORS(31),
.bmColors[32] = BMP_GRAY_COLORS(32),
.bmColors[33] = BMP_GRAY_COLORS(33),
.bmColors[34] = BMP_GRAY_COLORS(34),
.bmColors[35] = BMP_GRAY_COLORS(35),
.bmColors[36] = BMP_GRAY_COLORS(36),
.bmColors[37] = BMP_GRAY_COLORS(37),
.bmColors[38] = BMP_GRAY_COLORS(38),
.bmColors[39] = BMP_GRAY_COLORS(39),
.bmColors[40] = BMP_GRAY_COLORS(40),
.bmColors[41] = BMP_GRAY_COLORS(41),
.bmColors[42] = BMP_GRAY_COLORS(42),
.bmColors[43] = BMP_GRAY_COLORS(43),
.bmColors[44] = BMP_GRAY_COLORS(44),
.bmColors[45] = BMP_GRAY_COLORS(45),
.bmColors[46] = BMP_GRAY_COLORS(46),
.bmColors[47] = BMP_GRAY_COLORS(47),
.bmColors[48] = BMP_GRAY_COLORS(48),
.bmColors[49] = BMP_GRAY_COLORS(49),
.bmColors[50] = BMP_GRAY_COLORS(50),
.bmColors[51] = BMP_GRAY_COLORS(51),
.bmColors[52] = BMP_GRAY_COLORS(52),
.bmColors[53] = BMP_GRAY_COLORS(53),
.bmColors[54] = BMP_GRAY_COLORS(54),
.bmColors[55] = BMP_GRAY_COLORS(55),
.bmColors[56] = BMP_GRAY_COLORS(56),
.bmColors[57] = BMP_GRAY_COLORS(57),
.bmColors[58] = BMP_GRAY_COLORS(58),
.bmColors[59] = BMP_GRAY_COLORS(59),
.bmColors[60] = BMP_GRAY_COLORS(60),
.bmColors[61] = BMP_GRAY_COLORS(61),
.bmColors[62] = BMP_GRAY_COLORS(62),
.bmColors[63] = BMP_GRAY_COLORS(63),
.bmColors[64] = BMP_GRAY_COLORS(64),
.bmColors[65] = BMP_GRAY_COLORS(65),
.bmColors[66] = BMP_GRAY_COLORS(66),
.bmColors[67] = BMP_GRAY_COLORS(67),
.bmColors[68] = BMP_GRAY_COLORS(68),
.bmColors[69] = BMP_GRAY_COLORS(69),
.bmColors[70] = BMP_GRAY_COLORS(70),
.bmColors[71] = BMP_GRAY_COLORS(71),
.bmColors[72] = BMP_GRAY_COLORS(72),
.bmColors[73] = BMP_GRAY_COLORS(73),
.bmColors[74] = BMP_GRAY_COLORS(74),
.bmColors[75] = BMP_GRAY_COLORS(75),
.bmColors[76] = BMP_GRAY_COLORS(76),
.bmColors[77] = BMP_GRAY_COLORS(77),
.bmColors[78] = BMP_GRAY_COLORS(78),
.bmColors[79] = BMP_GRAY_COLORS(79),
.bmColors[80] = BMP_GRAY_COLORS(80),
.bmColors[81] = BMP_GRAY_COLORS(81),
.bmColors[82] = BMP_GRAY_COLORS(82),
.bmColors[83] = BMP_GRAY_COLORS(83),
.bmColors[84] = BMP_GRAY_COLORS(84),
.bmColors[85] = BMP_GRAY_COLORS(85),
.bmColors[86] = BMP_GRAY_COLORS(86),
.bmColors[87] = BMP_GRAY_COLORS(87),
.bmColors[88] = BMP_GRAY_COLORS(88),
.bmColors[89] = BMP_GRAY_COLORS(89),
.bmColors[90] = BMP_GRAY_COLORS(90),
.bmColors[91] = BMP_GRAY_COLORS(91),
.bmColors[92] = BMP_GRAY_COLORS(92),
.bmColors[93] = BMP_GRAY_COLORS(93),
.bmColors[94] = BMP_GRAY_COLORS(94),
.bmColors[95] = BMP_GRAY_COLORS(95),
.bmColors[96] = BMP_GRAY_COLORS(96),
.bmColors[97] = BMP_GRAY_COLORS(97),
.bmColors[98] = BMP_GRAY_COLORS(98),
.bmColors[99] = BMP_GRAY_COLORS(99),
.bmColors[100] = BMP_GRAY_COLORS(100),
.bmColors[101] = BMP_GRAY_COLORS(101),
.bmColors[102] = BMP_GRAY_COLORS(102),
.bmColors[103] = BMP_GRAY_COLORS(103),
.bmColors[104] = BMP_GRAY_COLORS(104),
.bmColors[105] = BMP_GRAY_COLORS(105),
.bmColors[106] = BMP_GRAY_COLORS(106),
.bmColors[107] = BMP_GRAY_COLORS(107),
.bmColors[108] = BMP_GRAY_COLORS(108),
.bmColors[109] = BMP_GRAY_COLORS(109),
.bmColors[110] = BMP_GRAY_COLORS(110),
.bmColors[111] = BMP_GRAY_COLORS(111),
.bmColors[112] = BMP_GRAY_COLORS(112),
.bmColors[113] = BMP_GRAY_COLORS(113),
.bmColors[114] = BMP_GRAY_COLORS(114),
.bmColors[115] = BMP_GRAY_COLORS(115),
.bmColors[116] = BMP_GRAY_COLORS(116),
.bmColors[117] = BMP_GRAY_COLORS(117),
.bmColors[118] = BMP_GRAY_COLORS(118),
.bmColors[119] = BMP_GRAY_COLORS(119),
.bmColors[120] = BMP_GRAY_COLORS(120),
.bmColors[121] = BMP_GRAY_COLORS(121),
.bmColors[122] = BMP_GRAY_COLORS(122),
.bmColors[123] = BMP_GRAY_COLORS(123),
.bmColors[124] = BMP_GRAY_COLORS(124),
.bmColors[125] = BMP_GRAY_COLORS(125),
.bmColors[126] = BMP_GRAY_COLORS(126),
.bmColors[127] = BMP_GRAY_COLORS(127),
.bmColors[128] = BMP_GRAY_COLORS(128),
.bmColors[129] = BMP_GRAY_COLORS(129),
.bmColors[130] = BMP_GRAY_COLORS(130),
.bmColors[131] = BMP_GRAY_COLORS(131),
.bmColors[132] = BMP_GRAY_COLORS(132),
.bmColors[133] = BMP_GRAY_COLORS(133),
.bmColors[134] = BMP_GRAY_COLORS(134),
.bmColors[135] = BMP_GRAY_COLORS(135),
.bmColors[136] = BMP_GRAY_COLORS(136),
.bmColors[137] = BMP_GRAY_COLORS(137),
.bmColors[138] = BMP_GRAY_COLORS(138),
.bmColors[139] = BMP_GRAY_COLORS(139),
.bmColors[140] = BMP_GRAY_COLORS(140),
.bmColors[141] = BMP_GRAY_COLORS(141),
.bmColors[142] = BMP_GRAY_COLORS(142),
.bmColors[143] = BMP_GRAY_COLORS(143),
.bmColors[144] = BMP_GRAY_COLORS(144),
.bmColors[145] = BMP_GRAY_COLORS(145),
.bmColors[146] = BMP_GRAY_COLORS(146),
.bmColors[147] = BMP_GRAY_COLORS(147),
.bmColors[148] = BMP_GRAY_COLORS(148),
.bmColors[149] = BMP_GRAY_COLORS(149),
.bmColors[150] = BMP_GRAY_COLORS(150),
.bmColors[151] = BMP_GRAY_COLORS(151),
.bmColors[152] = BMP_GRAY_COLORS(152),
.bmColors[153] = BMP_GRAY_COLORS(153),
.bmColors[154] = BMP_GRAY_COLORS(154),
.bmColors[155] = BMP_GRAY_COLORS(155),
.bmColors[156] = BMP_GRAY_COLORS(156),
.bmColors[157] = BMP_GRAY_COLORS(157),
.bmColors[158] = BMP_GRAY_COLORS(158),
.bmColors[159] = BMP_GRAY_COLORS(159),
.bmColors[160] = BMP_GRAY_COLORS(160),
.bmColors[161] = BMP_GRAY_COLORS(161),
.bmColors[162] = BMP_GRAY_COLORS(162),
.bmColors[163] = BMP_GRAY_COLORS(163),
.bmColors[164] = BMP_GRAY_COLORS(164),
.bmColors[165] = BMP_GRAY_COLORS(165),
.bmColors[166] = BMP_GRAY_COLORS(166),
.bmColors[167] = BMP_GRAY_COLORS(167),
.bmColors[168] = BMP_GRAY_COLORS(168),
.bmColors[169] = BMP_GRAY_COLORS(169),
.bmColors[170] = BMP_GRAY_COLORS(170),
.bmColors[171] = BMP_GRAY_COLORS(171),
.bmColors[172] = BMP_GRAY_COLORS(172),
.bmColors[173] = BMP_GRAY_COLORS(173),
.bmColors[174] = BMP_GRAY_COLORS(174),
.bmColors[175] = BMP_GRAY_COLORS(175),
.bmColors[176] = BMP_GRAY_COLORS(176),
.bmColors[177] = BMP_GRAY_COLORS(177),
.bmColors[178] = BMP_GRAY_COLORS(178),
.bmColors[179] = BMP_GRAY_COLORS(179),
.bmColors[180] = BMP_GRAY_COLORS(180),
.bmColors[181] = BMP_GRAY_COLORS(181),
.bmColors[182] = BMP_GRAY_COLORS(182),
.bmColors[183] = BMP_GRAY_COLORS(183),
.bmColors[184] = BMP_GRAY_COLORS(184),
.bmColors[185] = BMP_GRAY_COLORS(185),
.bmColors[186] = BMP_GRAY_COLORS(186),
.bmColors[187] = BMP_GRAY_COLORS(187),
.bmColors[188] = BMP_GRAY_COLORS(188),
.bmColors[189] = BMP_GRAY_COLORS(189),
.bmColors[190] = BMP_GRAY_COLORS(190),
.bmColors[191] = BMP_GRAY_COLORS(191),
.bmColors[192] = BMP_GRAY_COLORS(192),
.bmColors[193] = BMP_GRAY_COLORS(193),
.bmColors[194] = BMP_GRAY_COLORS(194),
.bmColors[195] = BMP_GRAY_COLORS(195),
.bmColors[196] = BMP_GRAY_COLORS(196),
.bmColors[197] = BMP_GRAY_COLORS(197),
.bmColors[198] = BMP_GRAY_COLORS(198),
.bmColors[199] = BMP_GRAY_COLORS(199),
.bmColors[200] = BMP_GRAY_COLORS(200),
.bmColors[201] = BMP_GRAY_COLORS(201),
.bmColors[202] = BMP_GRAY_COLORS(202),
.bmColors[203] = BMP_GRAY_COLORS(203),
.bmColors[204] = BMP_GRAY_COLORS(204),
.bmColors[205] = BMP_GRAY_COLORS(205),
.bmColors[206] = BMP_GRAY_COLORS(206),
.bmColors[207] = BMP_GRAY_COLORS(207),
.bmColors[208] = BMP_GRAY_COLORS(208),
.bmColors[209] = BMP_GRAY_COLORS(209),
.bmColors[210] = BMP_GRAY_COLORS(210),
.bmColors[211] = BMP_GRAY_COLORS(211),
.bmColors[212] = BMP_GRAY_COLORS(212),
.bmColors[213] = BMP_GRAY_COLORS(213),
.bmColors[214] = BMP_GRAY_COLORS(214),
.bmColors[215] = BMP_GRAY_COLORS(215),
.bmColors[216] = BMP_GRAY_COLORS(216),
.bmColors[217] = BMP_GRAY_COLORS(217),
.bmColors[218] = BMP_GRAY_COLORS(218),
.bmColors[219] = BMP_GRAY_COLORS(219),
.bmColors[220] = BMP_GRAY_COLORS(220),
.bmColors[221] = BMP_GRAY_COLORS(221),
.bmColors[222] = BMP_GRAY_COLORS(222),
.bmColors[223] = BMP_GRAY_COLORS(223),
.bmColors[224] = BMP_GRAY_COLORS(224),
.bmColors[225] = BMP_GRAY_COLORS(225),
.bmColors[226] = BMP_GRAY_COLORS(226),
.bmColors[227] = BMP_GRAY_COLORS(227),
.bmColors[228] = BMP_GRAY_COLORS(228),
.bmColors[229] = BMP_GRAY_COLORS(229),
.bmColors[230] = BMP_GRAY_COLORS(230),
.bmColors[231] = BMP_GRAY_COLORS(231),
.bmColors[232] = BMP_GRAY_COLORS(232),
.bmColors[233] = BMP_GRAY_COLORS(233),
.bmColors[234] = BMP_GRAY_COLORS(234),
.bmColors[235] = BMP_GRAY_COLORS(235),
.bmColors[236] = BMP_GRAY_COLORS(236),
.bmColors[237] = BMP_GRAY_COLORS(237),
.bmColors[238] = BMP_GRAY_COLORS(238),
.bmColors[239] = BMP_GRAY_COLORS(239),
.bmColors[240] = BMP_GRAY_COLORS(240),
.bmColors[241] = BMP_GRAY_COLORS(241),
.bmColors[242] = BMP_GRAY_COLORS(242),
.bmColors[243] = BMP_GRAY_COLORS(243),
.bmColors[244] = BMP_GRAY_COLORS(244),
.bmColors[245] = BMP_GRAY_COLORS(245),
.bmColors[246] = BMP_GRAY_COLORS(246),
.bmColors[247] = BMP_GRAY_COLORS(247),
.bmColors[248] = BMP_GRAY_COLORS(248),
.bmColors[249] = BMP_GRAY_COLORS(249),
.bmColors[250] = BMP_GRAY_COLORS(250),
.bmColors[251] = BMP_GRAY_COLORS(251),
.bmColors[252] = BMP_GRAY_COLORS(252),
.bmColors[253] = BMP_GRAY_COLORS(253),
.bmColors[254] = BMP_GRAY_COLORS(254),
.bmColors[255] = BMP_GRAY_COLORS(255),
};

const BITMAPRGBHEADER bmp_rgb_color_palette = {
	.bmColors[0] = 0xF800,
	.bmColors[1] = 0x07E0,
	.bmColors[2] = 0x001F,
	.bmColors[3] = 0,
};

BITMAPINFOHEADER bmih;

int BMP_FileWrite(FIL* fp, const void* buff, uint32_t btw)
{
#ifdef ON_WINDOWS
	fwrite(buff, 1, btw, fp);
#else
	UINT bw =0;
    f_write(fp,buff,btw,&bw);
    if(bw!=btw){PRINTF("the volume got full\r\n");}
#endif // ON_WINDOWS
	return 0;
}

int BMP_Save(FIL* fp, img_t* img)
{
	if (img->format == PixelFormatGray) {
		bmih.bmfh.bfType = 0x4d42;//"BM"
		bmih.bmfh.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPGRAYHEADER) + img->height * img->width;
		bmih.bmfh.bfReserved1 = 0;
		bmih.bmfh.bfReserved2 = 0;
		bmih.bmfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPGRAYHEADER);

		bmih.biSize = 40;
		bmih.biWidth = img->width;
		bmih.biHeight = -img->height;
		bmih.biPlanes = 1;
		bmih.biBitCount = 8;
		bmih.biCompression = 0;
		bmih.biSizeImage = img->height * img->width;
		bmih.biXPelsPerMeter = 12000;
		bmih.biYPelsPerMeter = 12000;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;
	}
	else if (img->format == PixelFormatRGB565)
	{
		bmih.bmfh.bfType = 0x4d42;//"BM"
		bmih.bmfh.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPRGBHEADER) + img->height * img->width * 2;
		bmih.bmfh.bfReserved1 = 0;
		bmih.bmfh.bfReserved2 = 0;
		bmih.bmfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPRGBHEADER);

		bmih.biSize = 40;
		bmih.biWidth = img->width;
		bmih.biHeight = -img->height;
		bmih.biPlanes = 1;
		bmih.biBitCount = 16;
		bmih.biCompression = 3;//比特掩码压缩格式为RGB565
		bmih.biSizeImage = img->height * img->width * 2;
		bmih.biXPelsPerMeter = 12000;
		bmih.biYPelsPerMeter = 12000;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;
	}

	BMP_FileWrite(fp, &bmih, sizeof(BITMAPINFOHEADER));

	if (img->format == PixelFormatGray) {
		BMP_FileWrite(fp, &bmp_gray_color_palette, sizeof(BITMAPGRAYHEADER));
		BMP_FileWrite(fp, img->pImg, img->height * img->width);
	}
	else if (img->format == PixelFormatRGB565)
	{
		BMP_FileWrite(fp, &bmp_rgb_color_palette, sizeof(BITMAPRGBHEADER));
		BMP_FileWrite(fp, img->pImg, img->height * img->width * 2);
	}
	return 0;
}
