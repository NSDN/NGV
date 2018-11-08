#include "nsgal.h"

#define DEF_TEXT_PART(name, index, jump, show) \
	TextPart name ##index = { \
		.textBack = 0xFFFFFF, \
		.textFore = 0x000000, \
		.textShow = (uint8_t*) show, \
		.textJump = (uint8_t*) jump, \
		.next = NULL \
	};

#define DEF_GAL_SCENE(val, tag, x, y, ptrStr, ptrF) \
	GalScene val = { \
		.name = (uint8_t*) tag, \
		.backAddr = -0xFFFFFF, \
		.backWidth = 854, \
		.backHeight = 480, \
		.icons = NULL, \
		.text.posX = x, \
		.text.posY = y, \
		.text.ptrS = (uint8_t*) ptrStr, \
		.text.ptrSback = 0xFFFFFF, \
		.text.ptrSfore = ptrF, \
		.text.parts = NULL \
	};

DEF_GAL_SCENE(title, "title", 64, 64, "��", 0xFF9800);
DEF_TEXT_PART(__title_, 1, "__this", "ÿ���ԡ���ſ�ͷ�����ѡ����ת");
DEF_TEXT_PART(__title_, 2, "__this", "������¼��ƶ���꣬�Ҳ��ϼ�ѡ��");
DEF_TEXT_PART(__title_, 3, "__this", "  ");
DEF_TEXT_PART(__title_, 4, "__this", "��������������������������������");
DEF_TEXT_PART(__title_, 5, "__this", "  ");
DEF_TEXT_PART(__title_, 6, "start", "�� Start | ��ʼ");
DEF_TEXT_PART(__title_, 7, "about", "�� About | ����");
DEF_TEXT_PART(__title_, 8, "__exit", "�� Exit  | �˳�");

DEF_GAL_SCENE(start, "start", 128, 128, "��", 0xFF9800);
DEF_TEXT_PART(__start_, 1, "__this", "Start Page �� ��ʼҳ");
DEF_TEXT_PART(__start_, 2, "__this", "  ");
DEF_TEXT_PART(__start_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__start_, 4, "__this", "  ");
DEF_TEXT_PART(__start_, 5, "quest1", "�� Test  | �Ը����");
DEF_TEXT_PART(__start_, 6, "__this", "  ");
DEF_TEXT_PART(__start_, 7, "__this", "  ");
DEF_TEXT_PART(__start_, 8, "title", "�� Title | ����");

DEF_GAL_SCENE(about, "about", 64, 64, "��", 0xFF9800);
DEF_TEXT_PART(__about_, 1, "__this", "About This �� ���ڴ˲���");
DEF_TEXT_PART(__about_, 2, "__this", "  ");
DEF_TEXT_PART(__about_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__about_, 4, "__this", "  ");
DEF_TEXT_PART(__about_, 5, "__this", "NSASM-C++ ver0.51 | NSHEL ver0.03 | NSGDX & NSGAL ver0.01");
DEF_TEXT_PART(__about_, 6, "__this", "NyaGame Vita v1.0 with STM32F429ZGT6 and AL3S10LG144");
DEF_TEXT_PART(__about_, 7, "__this", "Copyright [C] NSDN 2014 - 2019, all rights reserved.");
DEF_TEXT_PART(__about_, 8, "__this", "  ");
DEF_TEXT_PART(__about_, 9, "__this", "  ");
DEF_TEXT_PART(__about_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest1, "quest1", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest1_, 1, "__this", "���ǲ��Ǿ����Լ����Ը�ܺ��أ�");
DEF_TEXT_PART(__quest1_, 2, "__this", "  ");
DEF_TEXT_PART(__quest1_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest1_, 4, "__this", "  ");
DEF_TEXT_PART(__quest1_, 5, "quest2", "�� �ǵ�");
DEF_TEXT_PART(__quest1_, 6, "quest4", "�� ���ǵ�");
DEF_TEXT_PART(__quest1_, 7, "quest3", "�� �����");
DEF_TEXT_PART(__quest1_, 8, "__this", "  ");
DEF_TEXT_PART(__quest1_, 9, "__this", "  ");
DEF_TEXT_PART(__quest1_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest2, "quest2", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest2_, 1, "__this", "���ǲ��Ǿ����Լ����Ը���Լ����������أ�");
DEF_TEXT_PART(__quest2_, 2, "__this", "  ");
DEF_TEXT_PART(__quest2_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest2_, 4, "__this", "  ");
DEF_TEXT_PART(__quest2_, 5, "quest4", "�� �ǵ�");
DEF_TEXT_PART(__quest2_, 6, "quest3", "�� ���ǵ�");
DEF_TEXT_PART(__quest2_, 7, "quest6", "�� �����");
DEF_TEXT_PART(__quest2_, 8, "__this", "  ");
DEF_TEXT_PART(__quest2_, 9, "__this", "  ");
DEF_TEXT_PART(__quest2_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest3, "quest3", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest3_, 1, "__this", "���ǲ���ϲ���Լ����ڵ��Ը��أ�");
DEF_TEXT_PART(__quest3_, 2, "__this", "  ");
DEF_TEXT_PART(__quest3_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest3_, 4, "__this", "  ");
DEF_TEXT_PART(__quest3_, 5, "quest4", "�� �ǵ�");
DEF_TEXT_PART(__quest3_, 6, "quest7", "�� ���ǵ�");
DEF_TEXT_PART(__quest3_, 7, "quest5", "�� �����");
DEF_TEXT_PART(__quest3_, 8, "__this", "  ");
DEF_TEXT_PART(__quest3_, 9, "__this", "  ");
DEF_TEXT_PART(__quest3_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest4, "quest4", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest4_, 1, "__this", "���ǲ��ǻ�Ϊ�˱��˸ı��Լ����Ը��أ�");
DEF_TEXT_PART(__quest4_, 2, "__this", "  ");
DEF_TEXT_PART(__quest4_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest4_, 4, "__this", "  ");
DEF_TEXT_PART(__quest4_, 5, "quest6", "�� �ǵ�");
DEF_TEXT_PART(__quest4_, 6, "quest7", "�� ���ǵ�");
DEF_TEXT_PART(__quest4_, 7, "quest8", "�� �����");
DEF_TEXT_PART(__quest4_, 8, "__this", "  ");
DEF_TEXT_PART(__quest4_, 9, "__this", "  ");
DEF_TEXT_PART(__quest4_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest5, "quest5", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest5_, 1, "__this", "���ǲ��Ǿ��øı��Ը�����أ�");
DEF_TEXT_PART(__quest5_, 2, "__this", "  ");
DEF_TEXT_PART(__quest5_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest5_, 4, "__this", "  ");
DEF_TEXT_PART(__quest5_, 5, "quest7", "�� �ǵ�");
DEF_TEXT_PART(__quest5_, 6, "quest8", "�� ���ǵ�");
DEF_TEXT_PART(__quest5_, 7, "quest6", "�� �����");
DEF_TEXT_PART(__quest5_, 8, "__this", "  ");
DEF_TEXT_PART(__quest5_, 9, "__this", "  ");
DEF_TEXT_PART(__quest5_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest6, "quest6", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest6_, 1, "__this", "���ǲ��ǻ������Լ����Ը��أ�");
DEF_TEXT_PART(__quest6_, 2, "__this", "  ");
DEF_TEXT_PART(__quest6_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest6_, 4, "__this", "  ");
DEF_TEXT_PART(__quest6_, 5, "quest9", "�� �ǵ�");
DEF_TEXT_PART(__quest6_, 6, "quest7", "�� ���ǵ�");
DEF_TEXT_PART(__quest6_, 7, "quest10", "�� �����");
DEF_TEXT_PART(__quest6_, 8, "__this", "  ");
DEF_TEXT_PART(__quest6_, 9, "__this", "  ");
DEF_TEXT_PART(__quest6_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest7, "quest7", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest7_, 1, "__this", "����˵��Ը��Ӱ������");
DEF_TEXT_PART(__quest7_, 2, "__this", "  ");
DEF_TEXT_PART(__quest7_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest7_, 4, "__this", "  ");
DEF_TEXT_PART(__quest7_, 5, "quest8", "�� ���");
DEF_TEXT_PART(__quest7_, 6, "quest10", "�� �����");
DEF_TEXT_PART(__quest7_, 7, "quest9", "�� �����");
DEF_TEXT_PART(__quest7_, 8, "__this", "  ");
DEF_TEXT_PART(__quest7_, 9, "__this", "  ");
DEF_TEXT_PART(__quest7_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest8, "quest8", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest8_, 1, "__this", "���ǲ��ǻ���Ľ���˵��Ը��أ�");
DEF_TEXT_PART(__quest8_, 2, "__this", "  ");
DEF_TEXT_PART(__quest8_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest8_, 4, "__this", "  ");
DEF_TEXT_PART(__quest8_, 5, "quest9", "�� �ǵ�");
DEF_TEXT_PART(__quest8_, 6, "questB", "�� ���ǵ�");
DEF_TEXT_PART(__quest8_, 7, "quest10", "�� �����");
DEF_TEXT_PART(__quest8_, 8, "__this", "  ");
DEF_TEXT_PART(__quest8_, 9, "__this", "  ");
DEF_TEXT_PART(__quest8_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest9, "quest9", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest9_, 1, "__this", "������Ը��ǲ��ǻ��Ŵ��أ�");
DEF_TEXT_PART(__quest9_, 2, "__this", "  ");
DEF_TEXT_PART(__quest9_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest9_, 4, "__this", "  ");
DEF_TEXT_PART(__quest9_, 5, "questB", "�� �ǵ�");
DEF_TEXT_PART(__quest9_, 6, "questC", "�� ���ǵ�");
DEF_TEXT_PART(__quest9_, 7, "quest10", "�� �����");
DEF_TEXT_PART(__quest9_, 8, "__this", "  ");
DEF_TEXT_PART(__quest9_, 9, "__this", "  ");
DEF_TEXT_PART(__quest9_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(quest10, "quest10", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__quest10_, 1, "__this", "���ǲ��Ǻ�Ŭ���������أ�");
DEF_TEXT_PART(__quest10_, 2, "__this", "  ");
DEF_TEXT_PART(__quest10_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__quest10_, 4, "__this", "  ");
DEF_TEXT_PART(__quest10_, 5, "questA", "�� �ǵ�");
DEF_TEXT_PART(__quest10_, 6, "questC", "�� ���ǵ�");
DEF_TEXT_PART(__quest10_, 7, "questD", "�� �����");
DEF_TEXT_PART(__quest10_, 8, "__this", "  ");
DEF_TEXT_PART(__quest10_, 9, "__this", "  ");
DEF_TEXT_PART(__quest10_, 10, "title", "�� Title | ����");

DEF_GAL_SCENE(questA, "questA", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__questA_, 1, "__this", "��ı�ɫ�Ը��Ǵ���");
DEF_TEXT_PART(__questA_, 2, "__this", "  ");
DEF_TEXT_PART(__questA_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__questA_, 4, "__this", "  ");
DEF_TEXT_PART(__questA_, 5, "__this", "����һ����֦��Ҷ���ˣ�������һ���ʱ����������Ҫ���˸����չ�");
DEF_TEXT_PART(__questA_, 6, "__this", "��������кܶ��˵��۰���ʱ�������Ҳ��ϰ�����Լ��������Ը�");
DEF_TEXT_PART(__questA_, 7, "__this", "�����ı��Լ��������Ը�����ô��Ĳ���ʲô������");
DEF_TEXT_PART(__questA_, 8, "__this", "��ϣ���ڳ��ִ���֮ǰ���Ըı��Լ����Ͼ�˭Ҳ���ܹ�һֱ���Լ�");
DEF_TEXT_PART(__questA_, 9, "__this", "  ");
DEF_TEXT_PART(__questA_, 10, "__this", "  ");
DEF_TEXT_PART(__questA_, 11, "title", "�� Title | ����");

DEF_GAL_SCENE(questB, "questB", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__questB_, 1, "__this", "��ı�ɫ�Ը��ǵ���");
DEF_TEXT_PART(__questB_, 2, "__this", "  ");
DEF_TEXT_PART(__questB_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__questB_, 4, "__this", "  ");
DEF_TEXT_PART(__questB_, 5, "__this", "����һ������������û�л��˵��ˣ�����������������һ������");
DEF_TEXT_PART(__questB_, 6, "__this", "�����е�����һ���������˵���");
DEF_TEXT_PART(__questB_, 7, "__this", "����Ը���������˺ö�����ѣ������кõ����ѣ�Ҳ����Ҫ�����������");
DEF_TEXT_PART(__questB_, 8, "__this", "�������κξ���֮ǰ��Ҫ�ȱ����и���Ŀ��ǣ���ĵ��������Ǳ��˵����õ�");
DEF_TEXT_PART(__questB_, 9, "__this", "  ");
DEF_TEXT_PART(__questB_, 10, "__this", "  ");
DEF_TEXT_PART(__questB_, 11, "title", "�� Title | ����");

DEF_GAL_SCENE(questC, "questC", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__questC_, 1, "__this", "��ı�ɫ�Ը�������");
DEF_TEXT_PART(__questC_, 2, "__this", "  ");
DEF_TEXT_PART(__questC_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__questC_, 4, "__this", "  ");
DEF_TEXT_PART(__questC_, 5, "__this", "����һ�������չ�����˵��ˣ���������ô�Ҿ��ú���ܰ�����������������ŵ�");
DEF_TEXT_PART(__questC_, 6, "__this", "����ʱ�䳤��֮���㷢�ִ��ϰ������������������Լ����Խ��Խ����");
DEF_TEXT_PART(__questC_, 7, "__this", "��Ҳ�����Լ������չ�һ�Σ�����һֱֻ�ǿ�������");
DEF_TEXT_PART(__questC_, 8, "__this", "��ʱ����������Լ�̫��������������");
DEF_TEXT_PART(__questC_, 9, "__this", "  ");
DEF_TEXT_PART(__questC_, 10, "__this", "  ");
DEF_TEXT_PART(__questC_, 11, "title", "�� Title | ����");

DEF_GAL_SCENE(questD, "questD", 128, 128, "��", 0xFF0098);
DEF_TEXT_PART(__questD_, 1, "__this", "��ı�ɫ�Ը���ֱˬ");
DEF_TEXT_PART(__questD_, 2, "__this", "  ");
DEF_TEXT_PART(__questD_, 3, "__this", "��������������������������������");
DEF_TEXT_PART(__questD_, 4, "__this", "  ");
DEF_TEXT_PART(__questD_, 5, "__this", "ֱˬ������Ϊ�Լ����������ŵ��ˣ�����������͵�����������Լ����е�����������");
DEF_TEXT_PART(__questD_, 6, "__this", "��������ڵ�����в�����Ҫֱˬ����");
DEF_TEXT_PART(__questD_, 7, "__this", "��������Ϊ�Լ���ֱˬ���˺ܶ�Ŀ�");
DEF_TEXT_PART(__questD_, 8, "__this", "���ܹ��˽�ѵ֮��������Լ���ֱˬ�����ܹ����Լ������ô������ٲ�������������ŵ���");
DEF_TEXT_PART(__questD_, 9, "__this", "  ");
DEF_TEXT_PART(__questD_, 10, "__this", "  ");
DEF_TEXT_PART(__questD_, 11, "title", "�� Title | ����");

GalScene* galScenes[] = {
	&title, &start, &about,
	&quest1, &quest2, &quest3, &quest4, &quest5,
	&quest6, &quest7, &quest8, &quest9, &quest10,
	&questA, &questB, &questC, &questD,
	NULL
};

#define LINKT2S(this, scene) do { scene.text.parts = &this; } while (0);
#define LINKT2T(this, new) do { this.next = &new; } while (0);

#define __LINKT2T(header, a, b) LINKT2T(header ##a, header ##b)

void loadGalScenes() {
	__LINKT2T(__title_, 1, 2); __LINKT2T(__title_, 2, 3); __LINKT2T(__title_, 3, 4); __LINKT2T(__title_, 4, 5);
	__LINKT2T(__title_, 5, 6); __LINKT2T(__title_, 6, 7); __LINKT2T(__title_, 7, 8); LINKT2S(__title_1, title);

	__LINKT2T(__start_, 1, 2); __LINKT2T(__start_, 2, 3); __LINKT2T(__start_, 3, 4); __LINKT2T(__start_, 4, 5);
	__LINKT2T(__start_, 5, 6); __LINKT2T(__start_, 6, 7); __LINKT2T(__start_, 7, 8); LINKT2S(__start_1, start);

	__LINKT2T(__about_, 1, 2); __LINKT2T(__about_, 2, 3); __LINKT2T(__about_, 3, 4); __LINKT2T(__about_, 4, 5); __LINKT2T(__about_, 5, 6);
	__LINKT2T(__about_, 6, 7); __LINKT2T(__about_, 7, 8); __LINKT2T(__about_, 8, 9); __LINKT2T(__about_, 9, 10); LINKT2S(__about_1, about);

	__LINKT2T(__quest1_, 1, 2); __LINKT2T(__quest1_, 2, 3); __LINKT2T(__quest1_, 3, 4); __LINKT2T(__quest1_, 4, 5); __LINKT2T(__quest1_, 5, 6);
	__LINKT2T(__quest1_, 6, 7); __LINKT2T(__quest1_, 7, 8); __LINKT2T(__quest1_, 8, 9); __LINKT2T(__quest1_, 9, 10); LINKT2S(__quest1_1, quest1);

	__LINKT2T(__quest2_, 1, 2); __LINKT2T(__quest2_, 2, 3); __LINKT2T(__quest2_, 3, 4); __LINKT2T(__quest2_, 4, 5); __LINKT2T(__quest2_, 5, 6);
	__LINKT2T(__quest2_, 6, 7); __LINKT2T(__quest2_, 7, 8); __LINKT2T(__quest2_, 8, 9); __LINKT2T(__quest2_, 9, 10); LINKT2S(__quest2_1, quest2);

	__LINKT2T(__quest3_, 1, 2); __LINKT2T(__quest3_, 2, 3); __LINKT2T(__quest3_, 3, 4); __LINKT2T(__quest3_, 4, 5); __LINKT2T(__quest3_, 5, 6);
	__LINKT2T(__quest3_, 6, 7); __LINKT2T(__quest3_, 7, 8); __LINKT2T(__quest3_, 8, 9); __LINKT2T(__quest3_, 9, 10); LINKT2S(__quest3_1, quest3);

	__LINKT2T(__quest4_, 1, 2); __LINKT2T(__quest4_, 2, 3); __LINKT2T(__quest4_, 3, 4); __LINKT2T(__quest4_, 4, 5); __LINKT2T(__quest4_, 5, 6);
	__LINKT2T(__quest4_, 6, 7); __LINKT2T(__quest4_, 7, 8); __LINKT2T(__quest4_, 8, 9); __LINKT2T(__quest4_, 9, 10); LINKT2S(__quest4_1, quest4);

	__LINKT2T(__quest5_, 1, 2); __LINKT2T(__quest5_, 2, 3); __LINKT2T(__quest5_, 3, 4); __LINKT2T(__quest5_, 4, 5); __LINKT2T(__quest5_, 5, 6);
	__LINKT2T(__quest5_, 6, 7); __LINKT2T(__quest5_, 7, 8); __LINKT2T(__quest5_, 8, 9); __LINKT2T(__quest5_, 9, 10); LINKT2S(__quest5_1, quest5);

	__LINKT2T(__quest6_, 1, 2); __LINKT2T(__quest6_, 2, 3); __LINKT2T(__quest6_, 3, 4); __LINKT2T(__quest6_, 4, 5); __LINKT2T(__quest6_, 5, 6);
	__LINKT2T(__quest6_, 6, 7); __LINKT2T(__quest6_, 7, 8); __LINKT2T(__quest6_, 8, 9); __LINKT2T(__quest6_, 9, 10); LINKT2S(__quest6_1, quest6);

	__LINKT2T(__quest7_, 1, 2); __LINKT2T(__quest7_, 2, 3); __LINKT2T(__quest7_, 3, 4); __LINKT2T(__quest7_, 4, 5); __LINKT2T(__quest7_, 5, 6);
	__LINKT2T(__quest7_, 6, 7); __LINKT2T(__quest7_, 7, 8); __LINKT2T(__quest7_, 8, 9); __LINKT2T(__quest7_, 9, 10); LINKT2S(__quest7_1, quest7);

	__LINKT2T(__quest8_, 1, 2); __LINKT2T(__quest8_, 2, 3); __LINKT2T(__quest8_, 3, 4); __LINKT2T(__quest8_, 4, 5); __LINKT2T(__quest8_, 5, 6);
	__LINKT2T(__quest8_, 6, 7); __LINKT2T(__quest8_, 7, 8); __LINKT2T(__quest8_, 8, 9); __LINKT2T(__quest8_, 9, 10); LINKT2S(__quest8_1, quest8);

	__LINKT2T(__quest9_, 1, 2); __LINKT2T(__quest9_, 2, 3); __LINKT2T(__quest9_, 3, 4); __LINKT2T(__quest9_, 4, 5); __LINKT2T(__quest9_, 5, 6);
	__LINKT2T(__quest9_, 6, 7); __LINKT2T(__quest9_, 7, 8); __LINKT2T(__quest9_, 8, 9); __LINKT2T(__quest9_, 9, 10); LINKT2S(__quest9_1, quest9);

	__LINKT2T(__quest10_, 1, 2); __LINKT2T(__quest10_, 2, 3); __LINKT2T(__quest10_, 3, 4); __LINKT2T(__quest10_, 4, 5); __LINKT2T(__quest10_, 5, 6);
	__LINKT2T(__quest10_, 6, 7); __LINKT2T(__quest10_, 7, 8); __LINKT2T(__quest10_, 8, 9); __LINKT2T(__quest10_, 9, 10); LINKT2S(__quest10_1, quest10);

	__LINKT2T(__questA_, 1, 2); __LINKT2T(__questA_, 2, 3); __LINKT2T(__questA_, 3, 4); __LINKT2T(__questA_, 4, 5); __LINKT2T(__questA_, 5, 6);
	__LINKT2T(__questA_, 6, 7); __LINKT2T(__questA_, 7, 8); __LINKT2T(__questA_, 8, 9); __LINKT2T(__questA_, 9, 10); __LINKT2T(__questA_, 10, 11); LINKT2S(__questA_1, questA);

	__LINKT2T(__questB_, 1, 2); __LINKT2T(__questB_, 2, 3); __LINKT2T(__questB_, 3, 4); __LINKT2T(__questB_, 4, 5); __LINKT2T(__questB_, 5, 6);
	__LINKT2T(__questB_, 6, 7); __LINKT2T(__questB_, 7, 8); __LINKT2T(__questB_, 8, 9); __LINKT2T(__questB_, 9, 10); __LINKT2T(__questB_, 10, 11); LINKT2S(__questB_1, questB);

	__LINKT2T(__questC_, 1, 2); __LINKT2T(__questC_, 2, 3); __LINKT2T(__questC_, 3, 4); __LINKT2T(__questC_, 4, 5); __LINKT2T(__questC_, 5, 6);
	__LINKT2T(__questC_, 6, 7); __LINKT2T(__questC_, 7, 8); __LINKT2T(__questC_, 8, 9); __LINKT2T(__questC_, 9, 10); __LINKT2T(__questC_, 10, 11); LINKT2S(__questC_1, questC);

	__LINKT2T(__questD_, 1, 2); __LINKT2T(__questD_, 2, 3); __LINKT2T(__questD_, 3, 4); __LINKT2T(__questD_, 4, 5); __LINKT2T(__questD_, 5, 6);
	__LINKT2T(__questD_, 6, 7); __LINKT2T(__questD_, 7, 8); __LINKT2T(__questD_, 8, 9); __LINKT2T(__questD_, 9, 10); __LINKT2T(__questD_, 10, 11); LINKT2S(__questD_1, questD);
}
