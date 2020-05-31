#include "hal_as5047d.h"

static void as5047d_set_chip(uint8_t cs_num);
static void as5047d_init(void);
static void as5047d_update(void);

extern as50474_dri_t as50474_dri;

as50474_t as5047d = {                   \
    .angle = 0,                         \
    .instance = &as50474_dri,           \
    .set_chip = as5047d_set_chip,       \
    .init = as5047d_init,               \
    .update = as5047d_update,           \
};

static void as5047d_set_chip(uint8_t cs_num) {
    as5047d.instance->CHIP_SELECT = cs_num;
}

static void as5047d_init(void) {
    as50474_dri.write(SETTINGS1, 0x0004);
    as50474_dri.write(SETTINGS2, 0x0000);
    as50474_dri.write(ZPOSM, 0x0000);
    as50474_dri.write(ZPOSL, 0x0000);
}

static void as5047d_update(void) {
    as5047d.angle = as5047d.instance->read(ANGLE);
}
