#include "stg.h"

#include "lcd.h"
#include "key.h"

#define STG_COLORB 0x000000

#define RANGE_X1 4
#define RANGE_Y1 4
#define RANGE_X2 92
#define RANGE_Y2 123

extern LCD* lcd;
#define dev lcd

void onInitPlayer(void);
void onUpdatePlayer(void);
struct Player {
	uint32_t color;
	uint8_t size;
	double x;
	double y;
	double prevX;
	double prevY;
	void (*onInit)();
	void (*onUpdate)();
} player = { 0xFF9800, 1,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	&onInitPlayer,
	&onUpdatePlayer
};
void onInitPlayer() {
	player.x = (RANGE_X2 + RANGE_X1) / 2;
	player.y = 108;
}
void onUpdatePlayer() {
	player.prevX = player.x; player.prevY = player.y;
	if (checkKey(LPAD_LEFT)) { player.x -= 0.4; }
	if (checkKey(LPAD_RIGHT)) { player.x += 0.4; }
	if (checkKey(LPAD_UP)) { player.y -= 0.4; }
	if (checkKey(LPAD_DOWN)) { player.y += 0.4; }

	if (player.x > RANGE_X2 - player.size) player.x = RANGE_X2 - player.size;
	if (player.x < RANGE_X1 + player.size) player.x = RANGE_X1 + player.size;
	if (player.y > RANGE_Y2 - player.size) player.y = RANGE_Y2 - player.size;
	if (player.y < RANGE_Y1 + player.size) player.y = RANGE_Y1 + player.size;

	if ((char) player.x != (char) player.prevX || (char) player.y != (char) player.prevY) {
		dev->colorf(dev->p, STG_COLORB);
		dev->rect(dev->p,
			player.prevX - player.size, player.prevY - player.size,
			player.prevX + player.size, player.prevY + player.size,
			1
		);
	}
	dev->colorf(dev->p, player.color);
	dev->rect(dev->p,
		player.x - player.size, player.y - player.size,
		player.x + player.size, player.y + player.size,
		1
	);
}

void stg_main(uint8_t type) {
	if (type == STG_BACK) {
		dev->colorf(dev->p, 0xFFFFFF);
		dev->rect(dev->p, RANGE_X1 - 4, RANGE_Y1 - 4, RANGE_X2 + 4, RANGE_Y2 + 4, 1);
		dev->colorf(dev->p, STG_COLORB);
		dev->rect(dev->p, RANGE_X1 - 2, RANGE_Y1 - 2, RANGE_X2 + 2, RANGE_Y2 + 2, 1);
		dev->colorf(dev->p, 0xFFFFFF);
		dev->rect(dev->p, RANGE_X1 - 1, RANGE_Y1 - 1, RANGE_X2 + 1, RANGE_Y2 + 1, 0);
		dev->colorf(dev->p, 0x7986CB);
		dev->font(dev->p, Big);
		dev->draw(dev->p, RANGE_X2 + 12, RANGE_Y1 + 4, 'T');
		dev->draw(dev->p, RANGE_X2 + 12 + 8, RANGE_Y1 + 4, 'H');
		dev->font(dev->p, Small);
		dev->colorf(dev->p, 0xFFFFFF);

		player.onInit();
	} else if (type == STG_MAIN) {
		player.onUpdate();
	}
}
