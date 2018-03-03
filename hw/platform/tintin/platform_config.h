#define DISPLAY_ROWS 168
#define DISPLAY_COLS 144

//We are a square device
#define PBL_RECT

//We use pin B5 for backlight
#define BL_PIN GPIO_Pin_5
#define BL_PIN_SOURCE GPIO_PinSource5
#define BL_PORT RCC_AHB1Periph_GPIOB
#define BL_TIM TIM3
#define GBL_TIM GPIO_AF_TIM3
#define RBL_TIM RCC_APB1Periph_TIM3
