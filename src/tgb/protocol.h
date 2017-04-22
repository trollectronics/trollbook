#ifndef PROTOCOL_H_
#define PROTOCOL_H_

typedef enum ProtocolCommand ProtocolCommand;
enum ProtocolCommand {
	PROTOCOL_COMMAND_STATUS,
	PROTOCOL_COMMAND_CONTROL,
	PROTOCOL_COMMAND_KEYBOARD_EVENT,
	PROTOCOL_COMMAND_MOUSE_EVENT,
	PROTOCOL_COMMAND_POWER_OFF,
	PROTOCOL_COMMAND_POWER_REBOOT,
	PROTOCOL_COMMAND_VOLUME,
	PROTOCOL_COMMAND_ADC,
	PROTOCOL_COMMAND_I2C,
};

typedef struct StatusRegister StatusRegister;
struct StatusRegister {
	uint8_t keyboard_if : 1;
	uint8_t mouse_if : 1;
	uint8_t powerbutton_if : 1;
	uint8_t sd_present : 1;
	uint8_t headphones_present : 1;
	uint8_t i2c_if : 1;
};

typedef struct ControlRegister ControlRegister;
struct ControlRegister {
	uint8_t keyboard_ie : 1;
	uint8_t mouse_ie : 1;
	uint8_t powerbutton_ie : 1;
	uint8_t sd_ie : 1;
	uint8_t headphones_ie : 1;
	uint8_t i2c_ie : 1;
};

extern StatusRegister reg_status;
extern ControlRegister reg_control;

#endif
