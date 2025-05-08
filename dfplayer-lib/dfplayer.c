#include "dfplayer.h"

#define DFPLAYER_START_BYTE 0x7E
#define DFPLAYER_END_BYTE   0xEF

#define DFPLAYER_VOLUME_MAX         30
#define DFPLAYER_VERSION_INFO       0xFF
#define DFPLAYER_LENGTH_BYTE        0x06
#define DFPLAYER_SEND_BUFFER_LEN    10
#define DFPLAYER_RECEIVE_BUFFER_LEN 10

enum dfplayer_feedback { // force-newline
	DFPLAYER_FEEDBACK_OFF = 0x00,
	DFPLAYER_FEEDBACK_ON  = 0x01
};

enum dfplayer_cmd {
	DFPLAYER_CMD_NEXT                      = 0x01,
	DFPLAYER_CMD_PREVIOS                   = 0x02,
	DFPLAYER_CMD_PLAY_TRACK                = 0x03,
	DFPLAYER_CMD_VOLUME_UP                 = 0x04,
	DFPLAYER_CMD_VOLUME_DOWN               = 0x05,
	DFPLAYER_CMD_SET_VOLUME                = 0x06,
	DFPLAYER_CMD_SET_EQUALIZER             = 0x07,
	DFPLAYER_CMD_PLAY_AND_REPEAT_TRACK     = 0x08,
	DFPLAYER_CMD_SET_PLAYBACK_SOURCE       = 0x09,
	DFPLAYER_CMD_ENTER_STANDBY             = 0x0A,
	// DFPLAYER_CMD_ENTER_NORMAL_MODE    = 0x0B,
	DFPLAYER_CMD_RESET_MODULE              = 0x0C,
	DFPLAYER_CMD_PLAY                      = 0x0D,
	DFPLAYER_CMD_PAUSE                     = 0x0E,
	DFPLAYER_CMD_PLAY_TRACK_IN_FOLDER      = 0x0F,
	DFPLAYER_CMD_HEADPHONE_AMP             = 0x10,
	DFPLAYER_CMD_REPEAT_ALL                = 0x11,
	DFPLAYER_CMD_PLAY_MP3_FOLDER           = 0x12,
	DFPLAYER_CMD_PLAY_ADVERTISEMENT        = 0x13,
	DFPLAYER_CMD_PLAY_TRACK_IN_HUGE_FOLDER = 0x14,
	DFPLAYER_CMD_STOP_ADVERTISEMENT        = 0x15,
	DFPLAYER_CMD_STOP                      = 0x16,
	DFPLAYER_CMD_PLAY_AND_REPEAT_FOLDER    = 0x17,
	DFPLAYER_CMD_PLAY_RANDOM               = 0x18,
	DFPLAYER_CMD_REPEAT_CURRENT_TRACK      = 0x19,
	DFPLAYER_CMD_SET_DAC                   = 0x1A
};

#pragma pack(1)
struct dfplayer_package {
	uint8_t  start_byte;
	uint8_t  version;
	uint8_t  length;
	uint8_t  command;
	uint8_t  feedback;
	uint8_t  data_high;
	uint8_t  data_low;
	uint8_t  checksum_high;
	uint8_t  checksum_low;
	uint8_t  end_byte;
};
#pragma pack(reset)

static void dfplayer_set_checksum(struct dfplayer_package *package)
{
	uint16_t sum = 0;

	sum += (uint16_t)package->version;
	sum += (uint16_t)package->length;
	sum += (uint16_t)package->command;
	sum += (uint16_t)package->feedback;
	sum += (uint16_t)package->data_high;
	sum += (uint16_t)package->data_low;

	sum = -sum;

	package->checksum_low  = (uint8_t)(sum);
	package->checksum_high = (uint8_t)(sum >> 8);
}

static void dfplayer_receive(struct dfplayer_dev *dev, struct dfplayer_package *package)
{
	dev->serial_receive((uint8_t *)package, sizeof(struct dfplayer_package));
}

static void dfplayer_send(struct dfplayer_dev *dev, enum dfplayer_cmd cmd, uint8_t high_byte, uint8_t low_byte)
{
	struct dfplayer_package package;

	package.start_byte = DFPLAYER_START_BYTE;
	package.version    = DFPLAYER_VERSION_INFO;
	package.length     = DFPLAYER_LENGTH_BYTE;
	package.command    = (uint8_t)cmd;
	package.feedback   = DFPLAYER_FEEDBACK_ON;
	package.data_high  = high_byte;
	package.data_low   = low_byte;
	package.end_byte   = DFPLAYER_END_BYTE;

	dfplayer_set_checksum(&package);

	dev->serial_send((uint8_t *)&package, sizeof(package));

	dfplayer_receive(dev, &package);
}

void dfplayer_init(struct dfplayer_dev *dev) {}

void dfplayer_send_raw(struct dfplayer_dev *dev, uint8_t cmd, uint8_t data_high, uint8_t data_low)
{
	dfplayer_send(dev, cmd, data_high, data_low);
}

void dfplayer_set_volume(struct dfplayer_dev *dev, uint8_t volume)
{

	if (volume > DFPLAYER_VOLUME_MAX) {
		volume = DFPLAYER_VOLUME_MAX;
	}

	dfplayer_send(dev, DFPLAYER_CMD_SET_VOLUME, 0x00, volume);
}

void dfplayer_next(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_NEXT, 0x00, 0x00);
}

void dfplayer_pause(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_PAUSE, 0x00, 0x00);
}

void dfplayer_previous(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_PREVIOS, 0x00, 0x00);
}

void dfplayer_volume_up(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_VOLUME_UP, 0x00, 0x00);
}

void dfplayer_volume_down(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_VOLUME_DOWN, 0x00, 0x00);
}

void dfplayer_play(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_PLAY, 0x00, 0x00);
}

void dfplayer_play_and_repeat_folder(struct dfplayer_dev *dev, uint8_t folder_num)
{
	dfplayer_send(dev, DFPLAYER_CMD_PLAY_AND_REPEAT_FOLDER, 0, folder_num);
}

void dfplayer_play_track_in_folder(struct dfplayer_dev *dev, uint8_t folder_num, uint8_t file_num)
{
	dfplayer_send(dev, DFPLAYER_CMD_PLAY_TRACK_IN_FOLDER, folder_num, file_num);
}

void dfplayer_stop(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_STOP, 0x00, 0x00);
}

void dfplayer_stop_advertisement(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_STOP_ADVERTISEMENT, 0x00, 0x00);
}

void dfplayer_set_equalizer(struct dfplayer_dev *dev, enum dfplayer_eq eq)
{
	dfplayer_send(dev, DFPLAYER_CMD_SET_EQUALIZER, 0x00, eq);
}

void dfplayer_play_track(struct dfplayer_dev *dev, uint16_t file_num)
{
	dfplayer_send(dev, DFPLAYER_CMD_PLAY_TRACK, file_num >> 8, file_num);
}

void dfplayer_play_and_repeat_track(struct dfplayer_dev *dev, uint16_t file_num)
{
	dfplayer_send(dev, DFPLAYER_CMD_PLAY_AND_REPEAT_TRACK, file_num >> 8, file_num);
}

void dfplayer_enable_repeat_all(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_REPEAT_ALL, 0x00, 0x01);
}

void dfplayer_disable_repeat_all(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_REPEAT_ALL, 0x00, 0x00);
}

void dfplayer_enable_repeat_track(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_REPEAT_CURRENT_TRACK, 0x00, 0x01);
}
void dfplayer_disable_repeat_track(struct dfplayer_dev *dev)
{
	dfplayer_send(dev, DFPLAYER_CMD_REPEAT_CURRENT_TRACK, 0x00, 0x00);
}
