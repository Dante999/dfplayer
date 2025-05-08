#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <stdint.h>

enum dfplayer_playback_source {
	// DFPLAYER_PLAYBACK_SOURCE_U         = 0x00,
	DFPLAYER_PLAYBACK_SOURCE_USB_FLASH = 0x01,
	DFPLAYER_PLAYBACK_SOURCE_SD_CARD   = 0x02,
	// DFPLAYER_PLAYBACK_SOURCE_SLEEP     = 0x03,
	// DFPLAYER_PLAYBACK_SOURCE_FLASH     = 0x04
};

enum dfplayer_eq {
	DFPLAYER_EQ_NORMAL  = 0x00,
	DFPLAYER_EQ_POP     = 0x01,
	DFPLAYER_EQ_ROCK    = 0x02,
	DFPLAYER_EQ_JAZZ    = 0x03,
	DFPLAYER_EQ_CLASSIC = 0x04,
	DFPLAYER_EQ_BASE    = 0x05
};

struct dfplayer_dev {
	void (*serial_send)(const uint8_t *data, uint8_t len);
	uint8_t (*serial_receive)(uint8_t *data, uint8_t len);
	enum dfplayer_playback_source playback_source;
};

void dfplayer_init(struct dfplayer_dev *dev);
void dfplayer_send_raw(struct dfplayer_dev *dev, uint8_t cmd, uint8_t data_high, uint8_t data_low);

void dfplayer_volume_up(struct dfplayer_dev *dev);
void dfplayer_volume_down(struct dfplayer_dev *dev);
void dfplayer_set_volume(struct dfplayer_dev *dev, uint8_t volume);

void dfplayer_previous(struct dfplayer_dev *dev);
void dfplayer_next(struct dfplayer_dev *dev);
void dfplayer_play(struct dfplayer_dev *dev);
void dfplayer_stop(struct dfplayer_dev *dev);
void dfplayer_pause(struct dfplayer_dev *dev);

void dfplayer_stop_advertisement(struct dfplayer_dev *dev);
void dfplayer_play_track(struct dfplayer_dev *dev, uint16_t file_num);
void dfplayer_play_track_in_folder(struct dfplayer_dev *dev, uint8_t folder_num, uint8_t file_num);
void dfplayer_play_and_repeat_track(struct dfplayer_dev *dev, uint16_t file_num);
void dfplayer_play_and_repeat_folder(struct dfplayer_dev *dev, uint8_t folder_num);

void dfplayer_set_equalizer(struct dfplayer_dev *dev, enum dfplayer_eq eq);

void dfplayer_enable_repeat_all(struct dfplayer_dev *dev);
void dfplayer_disable_repeat_all(struct dfplayer_dev *dev);
void dfplayer_enable_repeat_track(struct dfplayer_dev *dev);
void dfplayer_disable_repeat_track(struct dfplayer_dev *dev);

#endif // DFPLAYER_H
