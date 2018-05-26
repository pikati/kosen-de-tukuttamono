#ifndef _LCD_FUNC_H

#define _LCD_FUNC_H


#ifndef BOARD_VERSION
#define BOARD_VERSION 200
#endif


#if BOARD_VERSION == 100

union un_lcd_cb {                    // Bitの構造体宣言
	struct { 
       	unsigned char OT:5;     //    Bit7
       	unsigned char RS:1;     //    Bit2
       	unsigned char RW:1;     //    Bit1
       	unsigned char E :1;     //    Bit0
	} BIT;
	unsigned char BYTE;
};

union un_lcd_data {              // Bitの構造体宣言
	struct {
       	unsigned char DATA:4;   //    Bit7-4
       	unsigned char OTHR:4;   //    Bit3-0
	} BIT;
	unsigned char BYTE;
}; 

//LCD表示関連の定義                                 // レジスタの定義

#define LCD_CB_PCR (*(volatile union un_lcd_cb *)0xFFE4)     //    H8 PORT1 コントロールレジスタの定義
#define LCD_CB (*(volatile union un_lcd_cb *)0xFFD4)         //    H8 PORT1 データレジスタの定義

#define LCD_DB_PCR (*(volatile union un_lcd_data *)0xFFE9)   //    H8 PORT6 コントロールレジスタの定義
#define LCD_DB (*(volatile union un_lcd_data *)0xFFD9)       //    H8 PORT6 LCD データバスに定義


#else

union un_lcd_cb {                    // Bitの構造体宣言
	struct { 
       	unsigned char OT:5;     //    Bit7
       	unsigned char RS:1;     //    Bit2
       	unsigned char RW:1;     //    Bit1
       	unsigned char E :1;     //    Bit0
	} BIT;
	unsigned char BYTE;
};

union un_lcd_data {              // Bitの構造体宣言
	struct {
       	unsigned char DATA:4;   //    Bit7-4
       	unsigned char OTHR:4;   //    Bit3-0
	} BIT;
	unsigned char BYTE;
};

//LCD表示関連の定義                                 // レジスタの定義

#define LCD_CB_PCR (*(volatile union un_lcd_cb *)0xFFE4)     //    H8 PORT1 コントロールレジスタの定義
#define LCD_CB (*(volatile union un_lcd_cb *)0xFFD4)         //    H8 PORT1 データレジスタの定義

#define LCD_DB_PCR (*(volatile union un_lcd_data *)0xFFE8)   //    H8 PORT6 コントロールレジスタの定義
#define LCD_DB (*(volatile union un_lcd_data *)0xFFD8)       //    H8 PORT6 LCD データバスに定義
#endif
                                    // H8ポート入出力モード設定の定義
#define INPUT_BYTE      0x00        //    H8ポート 入力モード設定の定義(バイト単位）
#define INPUT_BIT       0           //    H8ポート 入力モード設定の定義(ビット単位）
#define OUTPUT_BYTE     0xff        //    H8ポート 出力モード設定の定義(バイト単位）
#define OUTPUT_BIT      1           //    H8ポート 出力モード設定の定義(ビット単位）

                                    // LCDモード設定の定義
#define LCD_CMD         0           //    LCD コマンドモード設定の定義(RS=0)
#define LCD_DAT         1           //    LCD データモード設定の定義(RS=1)
#define LCD_WRITE       0           //    LCD 書き込みモード設定の定義(R/W=0)
#define LCD_READ        1           //    LCD 読み出しモード設定の定義(R/W=1)

                                    // インストラクションモードの定義
#define LCD_CLAR        0x01        //    LCD 表示クリア設定の定義
#define LCD_HOME        0x02        //    LCD カーソルホーム設定の定義
#define LCD_ENTSET      0x06        //    LCD エントリモード設定の定義
                                    //          I/D=1(インクリメント), S=0(表示シフトしない）
#define LCD_DISP_OFF    0x08        //    LCD 表示オン／オフコントロール設定の定義
                                    //          D=0(表示オフ), C=0(カーソルなし), B=0(ブリンクなし)
#define LCD_DISP_NCUR   0x0c        //    LCD 表示オン／オフコントロール設定の定義
                                    //          D=1(表示オン), C=0(カーソルなし), B=0(ブリンクなし)
#define LCD_DISP_CUR    0x0e        //    LCD 表示オン／オフコントロール設定の定義
                                    //          D=1(表示オン), C=1(カーソルあり), B=0(ブリンクなし)
#define LCD_DISP_BNK    0x0d        //    LCD 表示オン／オフコントロール設定の定義
                                    //          D=1(表示オン), C=0(カーソルなし), B=1(ブリンクあり)
#define LCD_DISP_ALL    0x0f        //    LCD 表示オン／オフコントロール設定の定義
                                    //          D=1(表示オン), C=0(カーソルあり), B=1(ブリンクあり)
#define LCD_INIT8B      0x3         //    LCD ファンクションセットの定義(データ長8ビットのとき，LCD 初期設定時に使用)
                                    //          DL=1(データ長8ビット)
#define LCD_FCSET8B     0x38        //    LCD ファンクションセットの定義
                                    //          DL=1(データ長8ビット), N=1(1/16デューティ), F=0(5*8ドット)
#define LCD_INIT4B      0x2         //    LCD ファンクションセットの定義(データ長4ビットのとき，LCD 初期設定時に使用)
                                    //          DL=1(データ長4ビット)
#define LCD_FCSET4B     0x28        //    LCD ファンクションセットの定義
                                    //          DL=1(データ長4ビット), N=1(1/16デューティ), F=0(5*8ドット)

#define loop_const      3500        // ウエイトの長さを決める数値,
                                    //    3500で約1ms程度(大まかな数値でよい)


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
