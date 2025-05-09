// List of Register Declarations
#define RCC_BASE   (0x40023800)
#define GPIOA_BASE (0x40020000)
#define GPIOB_BASE (0x40020400)
#define I2C1_BASE  (0x40005400)
#define TIM2_BASE  (0x40000000)

#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x40))

#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER (*(volatile unsigned int *)(GPIOA_BASE + 0x04))
#define GPIOA_ODR   (*(volatile unsigned int *)(GPIOA_BASE + 0x14))

#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile unsigned int *)(GPIOB_BASE + 0x04))
#define GPIOB_PUPDR (*(volatile unsigned int *)(GPIOB_BASE + 0x0C))
#define GPIOB_AFRH  (*(volatile unsigned int *)(GPIOB_BASE + 0x24))

#define I2C1_CR1 (*(volatile unsigned int *)(I2C1_BASE + 0x00))
#define I2C1_CR2 (*(volatile unsigned int *)(I2C1_BASE + 0x04))
#define I2C1_CCR (*(volatile unsigned int *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE (*(volatile unsigned int *)(I2C1_BASE + 0x20))
#define I2C1_SR1 (*(volatile unsigned int *)(I2C1_BASE + 0x14))
#define I2C1_SR2 (*(volatile unsigned int *)(I2C1_BASE + 0x18))
#define I2C1_DR  (*(volatile unsigned int *)(I2C1_BASE + 0x10))

#define TIM2_PSC (*(volatile unsigned int *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile unsigned int *)(TIM2_BASE + 0x2C))
#define TIM2_CR1 (*(volatile unsigned int *)(TIM2_BASE + 0x00))
#define TIM2_SR  (*(volatile unsigned int *)(TIM2_BASE + 0x10))

// List of Function Declarations
void I2C1_Init(void);
void I2C1_Send(char saddr, int n, char* str);
void Delay(unsigned int ms);

int main(void){
	I2C1_Init();
	while(1){
		I2C1_Send(0x12, 4, "Miju");
	}
}

void I2C1_Init(void){
	RCC_AHB1ENR |= (1<<1);

	GPIOB_MODER |= (1<<19);
	GPIOB_MODER &= ~(1<<18);
	GPIOB_MODER |= (1<<17);
	GPIOB_MODER &= ~(1<<16);

	/*Set PB8 & PB9 output type to open-drain*/
	GPIOB_OTYPER |= (1<<8);
	GPIOB_OTYPER |= (1<<9);

	/*Set PB8 & PB9 to output pull-up*/
	GPIOB_PUPDR &= ~(1<<19);
	GPIOB_PUPDR |= (1<<18);
	GPIOB_PUPDR &= ~(1<<17);
	GPIOB_PUPDR |= (1<<16);

	/*Set PB8 & PB9 alternate function type to I2C (AF4)*/
	GPIOB_AFRH &= ~(0xFF<<0);
	GPIOB_AFRH |= (1<<2);
	GPIOB_AFRH |= (1<<6);

	I2C1_CR1 |= (1<<15);
	I2C1_CR1 &= ~(1<<15);

	I2C1_CR2 |= (1<<4);
	I2C1_CCR = 80;
	I2C1_TRISE = 17;

	I2C1_CR1 |= (1<<0);
}

void I2C1_Send(char sadr, int n, char *str){
	while(I2C1_SR2 & (1<<1)){}

	I2C1_CR1 |=(1<<8);

	while(!(I2C1_SR1 & (1<<0))){}

	I2C1_DR = (sadr<<1);

	while(!(I2C1_SR1 & (1<<1))){}

	(void)I2C1_SR2;

	for(int i=0; i<n; i++){
		while(!(I2C1_SR1 & (1<<7))){}
		I2C1_DR = *str++;
	}
	while(!(I2C1_SR1 & (1<<2))){}

	I2C1_CR1 |= (1<<9);

}






























