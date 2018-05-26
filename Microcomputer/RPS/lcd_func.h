#ifndef _LCD_FUNC_H

#define _LCD_FUNC_H


#ifndef BOARD_VERSION
#define BOARD_VERSION 200
#endif


#if BOARD_VERSION == 100

union un_lcd_cb {                    // Bit�̍\���̐錾
	struct { 
       	unsigned char OT:5;     //    Bit7
       	unsigned char RS:1;     //    Bit2
       	unsigned char RW:1;     //    Bit1
       	unsigned char E :1;     //    Bit0
	} BIT;
	unsigned char BYTE;
};

union un_lcd_data {              // Bit�̍\���̐錾
	struct {
       	unsigned char DATA:4;   //    Bit7-4
       	unsigned char OTHR:4;   //    Bit3-0
	} BIT;
	unsigned char BYTE;
}; 

//LCD�\���֘A�̒�`                                 // ���W�X�^�̒�`

#define LCD_CB_PCR (*(volatile union un_lcd_cb *)0xFFE4)     //    H8 PORT1 �R���g���[�����W�X�^�̒�`
#define LCD_CB (*(volatile union un_lcd_cb *)0xFFD4)         //    H8 PORT1 �f�[�^���W�X�^�̒�`

#define LCD_DB_PCR (*(volatile union un_lcd_data *)0xFFE9)   //    H8 PORT6 �R���g���[�����W�X�^�̒�`
#define LCD_DB (*(volatile union un_lcd_data *)0xFFD9)       //    H8 PORT6 LCD �f�[�^�o�X�ɒ�`


#else

union un_lcd_cb {                    // Bit�̍\���̐錾
	struct { 
       	unsigned char OT:5;     //    Bit7
       	unsigned char RS:1;     //    Bit2
       	unsigned char RW:1;     //    Bit1
       	unsigned char E :1;     //    Bit0
	} BIT;
	unsigned char BYTE;
};

union un_lcd_data {              // Bit�̍\���̐錾
	struct {
       	unsigned char DATA:4;   //    Bit7-4
       	unsigned char OTHR:4;   //    Bit3-0
	} BIT;
	unsigned char BYTE;
};

//LCD�\���֘A�̒�`                                 // ���W�X�^�̒�`

#define LCD_CB_PCR (*(volatile union un_lcd_cb *)0xFFE4)     //    H8 PORT1 �R���g���[�����W�X�^�̒�`
#define LCD_CB (*(volatile union un_lcd_cb *)0xFFD4)         //    H8 PORT1 �f�[�^���W�X�^�̒�`

#define LCD_DB_PCR (*(volatile union un_lcd_data *)0xFFE8)   //    H8 PORT6 �R���g���[�����W�X�^�̒�`
#define LCD_DB (*(volatile union un_lcd_data *)0xFFD8)       //    H8 PORT6 LCD �f�[�^�o�X�ɒ�`
#endif
                                    // H8�|�[�g���o�̓��[�h�ݒ�̒�`
#define INPUT_BYTE      0x00        //    H8�|�[�g ���̓��[�h�ݒ�̒�`(�o�C�g�P�ʁj
#define INPUT_BIT       0           //    H8�|�[�g ���̓��[�h�ݒ�̒�`(�r�b�g�P�ʁj
#define OUTPUT_BYTE     0xff        //    H8�|�[�g �o�̓��[�h�ݒ�̒�`(�o�C�g�P�ʁj
#define OUTPUT_BIT      1           //    H8�|�[�g �o�̓��[�h�ݒ�̒�`(�r�b�g�P�ʁj

                                    // LCD���[�h�ݒ�̒�`
#define LCD_CMD         0           //    LCD �R�}���h���[�h�ݒ�̒�`(RS=0)
#define LCD_DAT         1           //    LCD �f�[�^���[�h�ݒ�̒�`(RS=1)
#define LCD_WRITE       0           //    LCD �������݃��[�h�ݒ�̒�`(R/W=0)
#define LCD_READ        1           //    LCD �ǂݏo�����[�h�ݒ�̒�`(R/W=1)

                                    // �C���X�g���N�V�������[�h�̒�`
#define LCD_CLAR        0x01        //    LCD �\���N���A�ݒ�̒�`
#define LCD_HOME        0x02        //    LCD �J�[�\���z�[���ݒ�̒�`
#define LCD_ENTSET      0x06        //    LCD �G���g�����[�h�ݒ�̒�`
                                    //          I/D=1(�C���N�������g), S=0(�\���V�t�g���Ȃ��j
#define LCD_DISP_OFF    0x08        //    LCD �\���I���^�I�t�R���g���[���ݒ�̒�`
                                    //          D=0(�\���I�t), C=0(�J�[�\���Ȃ�), B=0(�u�����N�Ȃ�)
#define LCD_DISP_NCUR   0x0c        //    LCD �\���I���^�I�t�R���g���[���ݒ�̒�`
                                    //          D=1(�\���I��), C=0(�J�[�\���Ȃ�), B=0(�u�����N�Ȃ�)
#define LCD_DISP_CUR    0x0e        //    LCD �\���I���^�I�t�R���g���[���ݒ�̒�`
                                    //          D=1(�\���I��), C=1(�J�[�\������), B=0(�u�����N�Ȃ�)
#define LCD_DISP_BNK    0x0d        //    LCD �\���I���^�I�t�R���g���[���ݒ�̒�`
                                    //          D=1(�\���I��), C=0(�J�[�\���Ȃ�), B=1(�u�����N����)
#define LCD_DISP_ALL    0x0f        //    LCD �\���I���^�I�t�R���g���[���ݒ�̒�`
                                    //          D=1(�\���I��), C=0(�J�[�\������), B=1(�u�����N����)
#define LCD_INIT8B      0x3         //    LCD �t�@���N�V�����Z�b�g�̒�`(�f�[�^��8�r�b�g�̂Ƃ��CLCD �����ݒ莞�Ɏg�p)
                                    //          DL=1(�f�[�^��8�r�b�g)
#define LCD_FCSET8B     0x38        //    LCD �t�@���N�V�����Z�b�g�̒�`
                                    //          DL=1(�f�[�^��8�r�b�g), N=1(1/16�f���[�e�B), F=0(5*8�h�b�g)
#define LCD_INIT4B      0x2         //    LCD �t�@���N�V�����Z�b�g�̒�`(�f�[�^��4�r�b�g�̂Ƃ��CLCD �����ݒ莞�Ɏg�p)
                                    //          DL=1(�f�[�^��4�r�b�g)
#define LCD_FCSET4B     0x28        //    LCD �t�@���N�V�����Z�b�g�̒�`
                                    //          DL=1(�f�[�^��4�r�b�g), N=1(1/16�f���[�e�B), F=0(5*8�h�b�g)

#define loop_const      3500        // �E�G�C�g�̒��������߂鐔�l,
                                    //    3500�Ŗ�1ms���x(��܂��Ȑ��l�ł悢)


extern void _lcd_wait(unsigned int wait_time);
extern void lcd_write_data(unsigned char data, unsigned char rs);
extern void lcd_pcr_init(void);
extern void lcd_init(void);
extern void lcd_putc(unsigned char c);
extern void lcd_puts( char *str);
extern void lcd_xy(unsigned char x, unsigned char y);
extern void lcd_dataout( unsigned long data );
extern void lcd_puti(int data);
extern void lcd_clear();


extern void init_lcd();
extern void putc_lcd(unsigned char c);
extern void puti_lcd(int data);
extern void clear_lcd();
extern void write_data_lcd(unsigned char data, unsigned char rs);

#endif
