struct slice {
	unsigned int slice_num;
	unsigned int pwm_min;
	unsigned int pwm_max;
};

struct chassis {
	struct slice slice_l;
	struct slice slice_r;

	int8_t l;
	int8_t r;
};

void chassis_set(struct chassis *chassis, int8_t linear, int8_t rot);
void chassis_init(struct chassis *chassis, uint8_t pin_la, uint8_t pin_ra);
static inline int8_t clamp8(int16_t value);