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

DEF_GAL_SCENE(title, "title", 64, 64, "→", 0xFF9800);
DEF_TEXT_PART(__title_, 1, "__this", "每行以●符号开头，则可选择并跳转");
DEF_TEXT_PART(__title_, 2, "__this", "左侧上下键移动光标，右侧上键选择");
DEF_TEXT_PART(__title_, 3, "__this", "  ");
DEF_TEXT_PART(__title_, 4, "__this", "————————————————");
DEF_TEXT_PART(__title_, 5, "__this", "  ");
DEF_TEXT_PART(__title_, 6, "start", "● Start | 开始");
DEF_TEXT_PART(__title_, 7, "about", "● About | 关于");
DEF_TEXT_PART(__title_, 8, "__exit", "● Exit  | 退出");

DEF_GAL_SCENE(start, "start", 128, 128, "→", 0xFF9800);
DEF_TEXT_PART(__start_, 1, "__this", "Start Page · 开始页");
DEF_TEXT_PART(__start_, 2, "__this", "  ");
DEF_TEXT_PART(__start_, 3, "__this", "————————————————");
DEF_TEXT_PART(__start_, 4, "__this", "  ");
DEF_TEXT_PART(__start_, 5, "quest1", "● Test  | 性格测试");
DEF_TEXT_PART(__start_, 6, "__this", "  ");
DEF_TEXT_PART(__start_, 7, "__this", "  ");
DEF_TEXT_PART(__start_, 8, "title", "● Title | 标题");

DEF_GAL_SCENE(about, "about", 64, 64, "→", 0xFF9800);
DEF_TEXT_PART(__about_, 1, "__this", "About This · 关于此测试");
DEF_TEXT_PART(__about_, 2, "__this", "  ");
DEF_TEXT_PART(__about_, 3, "__this", "————————————————");
DEF_TEXT_PART(__about_, 4, "__this", "  ");
DEF_TEXT_PART(__about_, 5, "__this", "NSASM-C++ ver0.51 | NSHEL ver0.03 | NSGDX & NSGAL ver0.01");
DEF_TEXT_PART(__about_, 6, "__this", "NyaGame Vita v1.0 with STM32F429ZGT6 and AL3S10LG144");
DEF_TEXT_PART(__about_, 7, "__this", "Copyright [C] NSDN 2014 - 2019, all rights reserved.");
DEF_TEXT_PART(__about_, 8, "__this", "  ");
DEF_TEXT_PART(__about_, 9, "__this", "  ");
DEF_TEXT_PART(__about_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest1, "quest1", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest1_, 1, "__this", "你是不是觉得自己的性格很好呢？");
DEF_TEXT_PART(__quest1_, 2, "__this", "  ");
DEF_TEXT_PART(__quest1_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest1_, 4, "__this", "  ");
DEF_TEXT_PART(__quest1_, 5, "quest2", "● 是的");
DEF_TEXT_PART(__quest1_, 6, "quest4", "● 不是的");
DEF_TEXT_PART(__quest1_, 7, "quest3", "● 看情况");
DEF_TEXT_PART(__quest1_, 8, "__this", "  ");
DEF_TEXT_PART(__quest1_, 9, "__this", "  ");
DEF_TEXT_PART(__quest1_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest2, "quest2", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest2_, 1, "__this", "你是不是觉得自己的性格给自己带来利益呢？");
DEF_TEXT_PART(__quest2_, 2, "__this", "  ");
DEF_TEXT_PART(__quest2_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest2_, 4, "__this", "  ");
DEF_TEXT_PART(__quest2_, 5, "quest4", "● 是的");
DEF_TEXT_PART(__quest2_, 6, "quest3", "● 不是的");
DEF_TEXT_PART(__quest2_, 7, "quest6", "● 看情况");
DEF_TEXT_PART(__quest2_, 8, "__this", "  ");
DEF_TEXT_PART(__quest2_, 9, "__this", "  ");
DEF_TEXT_PART(__quest2_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest3, "quest3", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest3_, 1, "__this", "你是不是喜欢自己现在的性格呢？");
DEF_TEXT_PART(__quest3_, 2, "__this", "  ");
DEF_TEXT_PART(__quest3_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest3_, 4, "__this", "  ");
DEF_TEXT_PART(__quest3_, 5, "quest4", "● 是的");
DEF_TEXT_PART(__quest3_, 6, "quest7", "● 不是的");
DEF_TEXT_PART(__quest3_, 7, "quest5", "● 看情况");
DEF_TEXT_PART(__quest3_, 8, "__this", "  ");
DEF_TEXT_PART(__quest3_, 9, "__this", "  ");
DEF_TEXT_PART(__quest3_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest4, "quest4", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest4_, 1, "__this", "你是不是会为了别人改变自己的性格呢？");
DEF_TEXT_PART(__quest4_, 2, "__this", "  ");
DEF_TEXT_PART(__quest4_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest4_, 4, "__this", "  ");
DEF_TEXT_PART(__quest4_, 5, "quest6", "● 是的");
DEF_TEXT_PART(__quest4_, 6, "quest7", "● 不是的");
DEF_TEXT_PART(__quest4_, 7, "quest8", "● 看情况");
DEF_TEXT_PART(__quest4_, 8, "__this", "  ");
DEF_TEXT_PART(__quest4_, 9, "__this", "  ");
DEF_TEXT_PART(__quest4_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest5, "quest5", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest5_, 1, "__this", "你是不是觉得改变性格很难呢？");
DEF_TEXT_PART(__quest5_, 2, "__this", "  ");
DEF_TEXT_PART(__quest5_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest5_, 4, "__this", "  ");
DEF_TEXT_PART(__quest5_, 5, "quest7", "● 是的");
DEF_TEXT_PART(__quest5_, 6, "quest8", "● 不是的");
DEF_TEXT_PART(__quest5_, 7, "quest6", "● 看情况");
DEF_TEXT_PART(__quest5_, 8, "__this", "  ");
DEF_TEXT_PART(__quest5_, 9, "__this", "  ");
DEF_TEXT_PART(__quest5_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest6, "quest6", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest6_, 1, "__this", "你是不是会隐藏自己的性格呢？");
DEF_TEXT_PART(__quest6_, 2, "__this", "  ");
DEF_TEXT_PART(__quest6_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest6_, 4, "__this", "  ");
DEF_TEXT_PART(__quest6_, 5, "quest9", "● 是的");
DEF_TEXT_PART(__quest6_, 6, "quest7", "● 不是的");
DEF_TEXT_PART(__quest6_, 7, "quest10", "● 看情况");
DEF_TEXT_PART(__quest6_, 8, "__this", "  ");
DEF_TEXT_PART(__quest6_, 9, "__this", "  ");
DEF_TEXT_PART(__quest6_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest7, "quest7", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest7_, 1, "__this", "身边人的性格会影响你吗？");
DEF_TEXT_PART(__quest7_, 2, "__this", "  ");
DEF_TEXT_PART(__quest7_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest7_, 4, "__this", "  ");
DEF_TEXT_PART(__quest7_, 5, "quest8", "● 会的");
DEF_TEXT_PART(__quest7_, 6, "quest10", "● 不会的");
DEF_TEXT_PART(__quest7_, 7, "quest9", "● 看情况");
DEF_TEXT_PART(__quest7_, 8, "__this", "  ");
DEF_TEXT_PART(__quest7_, 9, "__this", "  ");
DEF_TEXT_PART(__quest7_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest8, "quest8", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest8_, 1, "__this", "你是不是会羡慕别人的性格呢？");
DEF_TEXT_PART(__quest8_, 2, "__this", "  ");
DEF_TEXT_PART(__quest8_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest8_, 4, "__this", "  ");
DEF_TEXT_PART(__quest8_, 5, "quest9", "● 是的");
DEF_TEXT_PART(__quest8_, 6, "questB", "● 不是的");
DEF_TEXT_PART(__quest8_, 7, "quest10", "● 看情况");
DEF_TEXT_PART(__quest8_, 8, "__this", "  ");
DEF_TEXT_PART(__quest8_, 9, "__this", "  ");
DEF_TEXT_PART(__quest8_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest9, "quest9", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest9_, 1, "__this", "你觉得性格是不是会遗传呢？");
DEF_TEXT_PART(__quest9_, 2, "__this", "  ");
DEF_TEXT_PART(__quest9_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest9_, 4, "__this", "  ");
DEF_TEXT_PART(__quest9_, 5, "questB", "● 是的");
DEF_TEXT_PART(__quest9_, 6, "questC", "● 不是的");
DEF_TEXT_PART(__quest9_, 7, "quest10", "● 看情况");
DEF_TEXT_PART(__quest9_, 8, "__this", "  ");
DEF_TEXT_PART(__quest9_, 9, "__this", "  ");
DEF_TEXT_PART(__quest9_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(quest10, "quest10", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__quest10_, 1, "__this", "你是不是很努力在生活呢？");
DEF_TEXT_PART(__quest10_, 2, "__this", "  ");
DEF_TEXT_PART(__quest10_, 3, "__this", "————————————————");
DEF_TEXT_PART(__quest10_, 4, "__this", "  ");
DEF_TEXT_PART(__quest10_, 5, "questA", "● 是的");
DEF_TEXT_PART(__quest10_, 6, "questC", "● 不是的");
DEF_TEXT_PART(__quest10_, 7, "questD", "● 看情况");
DEF_TEXT_PART(__quest10_, 8, "__this", "  ");
DEF_TEXT_PART(__quest10_, 9, "__this", "  ");
DEF_TEXT_PART(__quest10_, 10, "title", "● Title | 标题");

DEF_GAL_SCENE(questA, "questA", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__questA_, 1, "__this", "你的本色性格是粗心");
DEF_TEXT_PART(__questA_, 2, "__this", "  ");
DEF_TEXT_PART(__questA_, 3, "__this", "————————————————");
DEF_TEXT_PART(__questA_, 4, "__this", "  ");
DEF_TEXT_PART(__questA_, 5, "__this", "你是一个粗枝大叶的人，和你在一起的时候往往都需要别人给你照顾");
DEF_TEXT_PART(__questA_, 6, "__this", "你很幸运有很多人的疼爱，时间久了你也就习惯了自己这样的性格");
DEF_TEXT_PART(__questA_, 7, "__this", "你很想改变自己这样的性格，你觉得粗心并是什么好事情");
DEF_TEXT_PART(__questA_, 8, "__this", "你希望在出现错误之前可以改变自己，毕竟谁也不能够一直帮自己");
DEF_TEXT_PART(__questA_, 9, "__this", "  ");
DEF_TEXT_PART(__questA_, 10, "__this", "  ");
DEF_TEXT_PART(__questA_, 11, "title", "● Title | 标题");

DEF_GAL_SCENE(questB, "questB", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__questB_, 1, "__this", "你的本色性格是单纯");
DEF_TEXT_PART(__questB_, 2, "__this", "  ");
DEF_TEXT_PART(__questB_, 3, "__this", "————————————————");
DEF_TEXT_PART(__questB_, 4, "__this", "  ");
DEF_TEXT_PART(__questB_, 5, "__this", "你是一个觉得世界上没有坏人的人，你的心依旧像个孩子一样单纯");
DEF_TEXT_PART(__questB_, 6, "__this", "生活中的你是一个乐于助人的人");
DEF_TEXT_PART(__questB_, 7, "__this", "你的性格给你招来了好多的朋友，但是有好的朋友，也有想要利用你的朋友");
DEF_TEXT_PART(__questB_, 8, "__this", "你在做任何决定之前都要比别人有更多的考虑，你的单纯往往是别人的利用点");
DEF_TEXT_PART(__questB_, 9, "__this", "  ");
DEF_TEXT_PART(__questB_, 10, "__this", "  ");
DEF_TEXT_PART(__questB_, 11, "title", "● Title | 标题");

DEF_GAL_SCENE(questC, "questC", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__questC_, 1, "__this", "你的本色性格是体贴");
DEF_TEXT_PART(__questC_, 2, "__this", "  ");
DEF_TEXT_PART(__questC_, 3, "__this", "————————————————");
DEF_TEXT_PART(__questC_, 4, "__this", "  ");
DEF_TEXT_PART(__questC_, 5, "__this", "你是一个懂得照顾身边人的人，你的体贴让大家觉得很温馨，这是你身上最大的优点");
DEF_TEXT_PART(__questC_, 6, "__this", "但是时间长了之后你发现大家习惯了你的体贴，而你自己变得越来越累了");
DEF_TEXT_PART(__questC_, 7, "__this", "你也渴望自己被人照顾一次，但是一直只是渴望而已");
DEF_TEXT_PART(__questC_, 8, "__this", "有时候你会讨厌自己太过于体贴懂事了");
DEF_TEXT_PART(__questC_, 9, "__this", "  ");
DEF_TEXT_PART(__questC_, 10, "__this", "  ");
DEF_TEXT_PART(__questC_, 11, "title", "● Title | 标题");

DEF_GAL_SCENE(questD, "questD", 128, 128, "¤", 0xFF0098);
DEF_TEXT_PART(__questD_, 1, "__this", "你的本色性格是直爽");
DEF_TEXT_PART(__questD_, 2, "__this", "  ");
DEF_TEXT_PART(__questD_, 3, "__this", "————————————————");
DEF_TEXT_PART(__questD_, 4, "__this", "  ");
DEF_TEXT_PART(__questD_, 5, "__this", "直爽是你认为自己身上最大的优点了，但是随着年纪的增长，你把自己的有点隐藏了起来");
DEF_TEXT_PART(__questD_, 6, "__this", "你觉得现在的社会中并不需要直爽的人");
DEF_TEXT_PART(__questD_, 7, "__this", "你曾经因为自己的直爽吃了很多的亏");
DEF_TEXT_PART(__questD_, 8, "__this", "在受过了教训之后你觉得自己的直爽并不能够给自己带来好处，你再不觉得那是你的优点了");
DEF_TEXT_PART(__questD_, 9, "__this", "  ");
DEF_TEXT_PART(__questD_, 10, "__this", "  ");
DEF_TEXT_PART(__questD_, 11, "title", "● Title | 标题");

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
