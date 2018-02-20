
#include "../Touch_Pannel/TouchPanel.h"
void Detect_for_games  (uint8_t algorithm);
void space_invaders(void);
void PONG(void);
extern uint16_t bgColor;


extern uint8_t time_pong;
// extern const uint8_t nos [153600];

extern uint16_t player1_y, player2_y;

extern uint16_t lowest_invader;     /* X position of the lowest invader */
extern uint8_t num_destroyed;		/* Num of destroyed invaders */
extern uint8_t num_invaders; /* Number of initial invaders */
extern uint32_t destroyed;		/* Binary array that tells us which invaders are destroyed */
extern int vel_invaders;
extern uint8_t game;
extern uint16_t time_game;
extern uint16_t misil_active;
extern uint8_t newGame;

void init_PONG (void);
void init_space(void);
void DrawInvaders(void);
void masscentre_detect_players (void);
uint8_t hitTest (Rectangle r1, Rectangle r2);
void CANDY_CRUSH(void);
void init_CANDY (void);
uint32_t rand_number (void);
extern uint8_t move_inv_flag;

extern uint8_t caramels[7][33*33*2];
/*
extern const uint8_t muelle[2768];
extern const uint8_t click_audio[1462];
*/

extern const uint8_t muelle[2];
extern const uint8_t click_audio[2];
void draw_RGB565_image (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *image);

void candy_combo(void);
void candy_combo2(void);
void draw_all_candy (void);

int min_int(int a, int b);


#define CANDY_WAITING 0
#define	CANDY_SELECTED 1
#define CANDY_SWAPPING 2
#define CANDY_DELETING 3

#define CANDY_GAPS	5

#define CANDY_COL 7
#define CANDY_ROW 7

#define CANDY_SIZE 33		// Size of the candy images in px
#define CANDY_SEP 33		// Separation between candy
#define CANDY_X	 (80 +4)		// OFFSET IN THE X AXIS
#define CANDY_Y	 4		// OFFSET IN THE y AXIS
#define CANDY_VEL 3
#define CANDY_DELETED 6		  // Position of the deleted candy
