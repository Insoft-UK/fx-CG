#ifndef __FXCG_DISPLAY_H
#define __FXCG_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216

typedef unsigned short color_t;

typedef enum Color : color_t {
    kColor_Aliceblue = 0xF7DF,
    kColor_Antiquewhite = 0xFF5A,
    kColor_Aqua = 0x07Ff,
    kColor_Aquamarine = 0x7Ffa,
    kColor_Azure = 0xf7Ff,
    kColor_Beige = 0xf7Bb,
    kColor_Bisque = 0xff38,
    kColor_Black = 0x0000,
    kColor_Blanchedalmond = 0xff59,
    kColor_Blue = 0x001F,
    kColor_Blueviolet = 0x895C,
    kColor_Brown = 0xa145,
    kColor_Burlywood = 0xddd0,
    kColor_Cadetblue = 0x5Cf4,
    kColor_Chartreuse = 0x7Fe0,
    kColor_Chocolate = 0xd343,
    kColor_Coral = 0xfbea,
    kColor_Cornflowerblue = 0x64Bd,
    kColor_Cornsilk = 0xffdb,
    kColor_Crimson = 0xd8A7,
    kColor_Cyan = 0x07Ff,
    kColor_Darkblue = 0x0011,
    kColor_Darkcyan = 0x0451,
    kColor_Darkgoldenrod = 0xbc21,
    kColor_Darkgray = 0xad55,
    kColor_Darkgreen = 0x0320,
    kColor_Darkkhaki = 0xbdad,
    kColor_Darkmagenta = 0x8811,
    kColor_Darkolivegreen = 0x5345,
    kColor_Darkorange = 0xfc60,
    kColor_Darkorchid = 0x9999,
    kColor_Darkred = 0x8800,
    kColor_Darksalmon = 0xecaf,
    kColor_Darkseagreen = 0x8Df1,
    kColor_Darkslateblue = 0x49F1,
    kColor_Darkslategray = 0x2A69,
    kColor_Darkturquoise = 0x067A,
    kColor_Darkviolet = 0x901A,
    kColor_Deeppink = 0xf8B2,
    kColor_Deepskyblue = 0x05Ff,
    kColor_Dimgray = 0x6B4D,
    kColor_Dodgerblue = 0x1C9F,
    kColor_Firebrick = 0xb104,
    kColor_Floralwhite = 0xffde,
    kColor_Forestgreen = 0x2444,
    kColor_Fuchsia = 0xf81F,
    kColor_Gainsboro = 0xdefb,
    kColor_Ghostwhite = 0xffdf,
    kColor_Gold = 0xfea0,
    kColor_Goldenrod = 0xdd24,
    kColor_Gray = 0x8410,
    kColor_Green = 0x0400,
    kColor_Greenyellow = 0xafe5,
    kColor_Honeydew = 0xf7Fe,
    kColor_Hotpink = 0xfb56,
    kColor_Indianred = 0xcaeb,
    kColor_Indigo = 0x4810,
    kColor_Ivory = 0xfffe,
    kColor_Khaki = 0xf731,
    kColor_Lavender = 0xe73F,
    kColor_Lavenderblush = 0xff9E,
    kColor_Lawngreen = 0x7Fe0,
    kColor_Lemonchiffon = 0xffd9,
    kColor_Lightblue = 0xaedc,
    kColor_Lightcoral = 0xf410,
    kColor_Lightcyan = 0xe7Ff,
    kColor_Lightgoldenrodyellow = 0xffda,
    kColor_Lightgray = 0xd69A,
    kColor_Lightgreen = 0x9772,
    kColor_Lightpink = 0xfdb8,
    kColor_Lightsalmon = 0xfd0F,
    kColor_Lightseagreen = 0x2595,
    kColor_Lightskyblue = 0x867F,
    kColor_Lightslategray = 0x7453,
    kColor_Lightsteelblue = 0xb63B,
    kColor_Lightyellow = 0xfffc,
    kColor_Lime = 0x07E0,
    kColor_Limegreen = 0x3666,
    kColor_Linen = 0xff9C,
    kColor_Magenta = 0xf81F,
    kColor_Maroon = 0x8000,
    kColor_Mediumaquamarine = 0x6675,
    kColor_Mediumblue = 0x0019,
    kColor_Mediumorchid = 0xbaba,
    kColor_Mediumpurple = 0x939B,
    kColor_Mediumseagreen = 0x3D8E,
    kColor_Mediumslateblue = 0x7B5D,
    kColor_Mediumspringgreen = 0x07D3,
    kColor_Mediumturquoise = 0x4E99,
    kColor_Mediumvioletred = 0xc0B0,
    kColor_Midnightblue = 0x18Ce,
    kColor_Mintcream = 0xf7Ff,
    kColor_Mistyrose = 0xff3C,
    kColor_Moccasin = 0xff36,
    kColor_Navajowhite = 0xfef5,
    kColor_Navy = 0x0010,
    kColor_Oldlace = 0xffbc,
    kColor_Olive = 0x8400,
    kColor_Olivedrab = 0x6C64,
    kColor_Orange = 0xfd20,
    kColor_Orangered = 0xfa20,
    kColor_Orchid = 0xdb9A,
    kColor_Palegoldenrod = 0xef55,
    kColor_Palegreen = 0x9Fd3,
    kColor_Paleturquoise = 0xaf7D,
    kColor_Palevioletred = 0xdb92,
    kColor_Papayawhip = 0xff7A,
    kColor_Peachpuff = 0xfed7,
    kColor_Peru = 0xcc27,
    kColor_Pink = 0xfe19,
    kColor_Plum = 0xdd1B,
    kColor_Powderblue = 0xb71C,
    kColor_Purple = 0x8010,
    kColor_Red = 0xf800,
    kColor_Rosybrown = 0xbc71,
    kColor_Royalblue = 0x435C,
    kColor_Saddlebrown = 0x8A22,
    kColor_Salmon = 0xfc0E,
    kColor_Sandybrown = 0xf52C,
    kColor_Seagreen = 0x2C4A,
    kColor_Seashell = 0xffbd,
    kColor_Sienna = 0xa285,
    kColor_Silver = 0xc618,
    kColor_Skyblue = 0x867D,
    kColor_Slateblue = 0x6Ad9,
    kColor_Slategray = 0x7412,
    kColor_Snow = 0xffdf,
    kColor_Springgreen = 0x07Ef,
    kColor_Steelblue = 0x4416,
    kColor_Tan = 0xd5B1,
    kColor_Teal = 0x0410,
    kColor_Thistle = 0xddfb,
    kColor_Tomato = 0xfb08,
    kColor_Turquoise = 0x471A,
    kColor_Violet = 0xec1D,
    kColor_Wheat = 0xf6F6,
    kColor_White = 0xffff,
    kColor_Whitesmoke = 0xf7Be,
    kColor_Yellow = 0xffe0,
    kColor_Yellowgreen = 0x9E66
} Color;

//MARK: - General Display Manipulating SYSCALLS
typedef struct TBdispFill {
    int x1;
    int y1;
    int x2;
    int y2;
    unsigned char mode;
} TBdispFill;

typedef TBdispFill* TBdisplayFillRef;

void Bdisp_AreaClr( TBdispFill *area, unsigned char P2, color_t color );

typedef enum ColorMode : int {
    kColorMode_DisableFullColor     = 0,
    kColorMode_EnableFullColor      = 1
} ColorMode;

/**
 Switches the screen between full color mode (16 bits per pixel, RGB565) and indexed color mode (3 bits per pixel, 8 colors - the same that can be used with PrintXY).
 
 - Parameter full: 0 to disable full color, 1 to enable full color.
 
 - Returns The result of Bdisp_IsZeroDDRegisterB (see disassembly for details).
 */
int Bdisp_EnableColor( ColorMode full );

//MARK: - Frame Control
/**
 Directly changes the color of the screen border, does not involve VRAM (so Bdisp_PutDisp_DD need not follow). See Display for more information.
 
 - Parameter color: The color to draw the border with.
 */
void DrawFrame( color_t color );


typedef enum FrameMode : int {
    kFrameMode_SetToWhite     = 0,
    kFrameMode_SetToColor     = 1
} FrameMode;

/**
 Sets the word at 0xFD801460, which controls the system-wide frame color.

 - Parameter mode: If 0, the word at 0xFD801460 is set to 0xFFFF. If 1, the word at 0xFD801460 is set to color.
 - Parameter color: The color to which the frame is set when mode is 1.
 
 - Returns The value of word at 0xFD801460.
*/
color_t FrameColor( FrameMode mode, color_t color );

void DrawFrameWorkbench( int, int, int, int, int );

//MARK: - VRAM General Display Manipulating SYSCALLS

/**
 - Returns Return a pointer to the system's video memory.
 */
void * GetVRAMAddress( void );

/**
 - Returns Return a pointer to the memory used by SaveVRAM_1 and LoadVRAM_1.
 */
void * GetSecondaryVRAMAddress( void );

void Bdisp_AllClr_VRAM( void );
void Bdisp_SetPoint_VRAM( int x, int y, int color );
void Bdisp_SetPointWB_VRAM( int x, int y, int color );
unsigned short Bdisp_GetPoint_VRAM( int x, int y );
void SaveVRAM_1( void );
void LoadVRAM_1( void );
void Bdisp_Fill_VRAM( int color, int mode );

//MARK: - DD Display Manipulating SYSCALLS
void Bdisp_AreaClr_DD_x3( void*p1 );
void Bdisp_DDRegisterSelect( int registerno );
void Bdisp_PutDisp_DD( void );
void Bdisp_PutDisp_DD_stripe( int y1, int y2 );
void Bdisp_SetPoint_DD( int x, int y, int color );
unsigned short Bdisp_GetPoint_DD_Workbench( int x, int y );
unsigned short Bdisp_GetPoint_DD( int x, int y );
void DirectDrawRectangle( int x1, int y1, int x2, int y2, color_t color );
void HourGlass( void );
void Bdisp_DefineDMARange(int x1, int x2, int y1, int y2);
unsigned short Bdisp_WriteDDRegister3_bit7(int value);

//MARK: - Graphic Writing
struct TBdispGraph {
    int x;
    int y;
    int xOffset;
    int yOffset;
    int width;
    int height;
    char colorMode;
    char zero4;
    char P20_1;
    char P20_2;
    int bitmap;
    char colorIndex1;
    char colorIndex2;
    char colorIndex3;
    char P20_3;
    char writeModify;
    char writeKind;
    char zero6;
    char one1;
    int transparency;
};

void Bdisp_WriteGraphVRAM(struct TBdispGraph * gd);
void Bdisp_WriteGraphDD_WB(struct TBdispGraph * gd);

//MARK: - Shape Drawing

typedef struct TBdispShape {
    int dx;
    int dy;
    int wx;
    int wy;
    int color;
    struct TBdispFill saved;
} TBdispShape;

typedef TBdispShape* TBdispShapeRef;

void Bdisp_ShapeBase3XVRAM( void*shape );
void Bdisp_ShapeBase( unsigned char * work, TBdispShapeRef shape, int color, int line_width, int zero1, int zero2 );
void Bdisp_ShapeToVRAM16C( void*, int color );
void Bdisp_ShapeToDD( void*shape, int color );

// The following rectangle-related syscalls draw a rectangle to VRAM, x between 0 and 383 (inclusive), y between 0 and 191 (inclusive).
// These add 24 pixels automatically, avoiding the status area:

/// Draws a rectangle to VRAM using a TEXT_COLOR.
void Bdisp_Rectangle( int x1, int y1, int x2, int y2, char color );

/// Draws a filled rectangle to VRAM using a TEXT_COLOR
void Bdisp_FilledRectangle( int x1, int y1, int x2, int y2, char color );

/// Draws a filled rectangle to VRAM using a color_t
void Bdisp_FilledRectangleFullColor( int x1, int y1, int x2, int y2, unsigned short color );


//MARK: - Background-Related SYSCALLS
void SetBackGround( int );
void WriteBackground( void*target, int width, int height, void*source, int, int, int );

//MARK: - Message Boxes, Error Messages, Dialogs...
void Box( int, int, int, int, int );
void BoxInnerClear( int );
void Box2( int, int );
void BoxYLimits( int lines, int*top, int*bottom );
void AUX_DisplayErrorMessage( int msgno );
void MsgBoxPush( int lines );
void MsgBoxPop( void );
void DisplayMessageBox( unsigned char*message );
short CharacterSelectDialog( void );
unsigned char ColorIndexDialog1( unsigned char initial_index, unsigned short disable_mask );
void MsgBoxMoveWB( void*buffer, int x0, int y0, int x1, int y1, int direction ); //it's more general purpose, works not only for MsgBoxes but for any VRAM contents.

//MARK: - Cursor Manipulation SYSCALLS

/**
 Sets the cursor position for Print_OS

 - Parameter x: Must be in range of [1,21]
 - Parameter y: Must be in range of [1,8]
*/
void locate_OS( int x, int y );

void Cursor_SetFlashOn( unsigned char cursor_type );
void Cursor_SetFlashOff( void );
int SetCursorFlashToggle( int );
void Keyboard_CursorFlash( void );

//MARK: - Character Printing SYSCALLS

/*!
    @enum            TextColor
    @discussion      These colors are index colors, NOT! 565 RGB color values.
 
    @constant        kTextColor_Black
                    The index color for black.
 
    @constant        kTextColor_Blue
                    The index color for Blue.
 
*/
typedef enum TextColor : char {
    kTextColor_Black   = 0,
    kTextColor_Blue    = 1,
    kTextColor_Green   = 2,
    kTextColor_Cyan    = 3,
    kTextColor_Red     = 4,
    kTextColor_Purple  = 5,
    kTextColor_Yellow  = 6,
    kTextColor_White   = 7
} TextColor;

typedef enum TextModeFlags : int {
    kTextModeFlags_Normal   = 0x00,
    kTextModeFlags_Invert   = 0x01,
    kTextModeFlags_TransparentBackground   = 0x20,
    kTextModeFlags_And   = 0x21,
} TextModeFlags;


void PrintLine(const char *msg, int imax);
void PrintLine2(int, int, const char *, int, int, int, int, int);
void PrintXY_2( int mode, int x, int y, int msgno, int color );
void PrintXY( int x, int y, const char *string, int mode, int color );
void PrintCXY( int, int, const char *, int, int, int, int, int, int );
void PrintGlyph( int, int, unsigned char * glyph, int, color_t color, color_t back_color, int );
void*GetMiniGlyphPtr( unsigned short mb_glyph_no, unsigned short*glyph_info );
void PrintMiniGlyph(int x, int y, void*glyph, TextModeFlags mode_flags, int glyph_width, int, int, int, int, int color, int back_color, int );
void PrintMini( int *x, int *y, const char *MB_string, TextModeFlags mode_flags, unsigned int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11 );
void PrintMiniMini( int *x, int *y, const char *MB_string, int mode1, int color, int mode2 );
void Print_OS( const char*msg, int mode, int zero2 );
void Bdisp_WriteSystemMessage( int x, int y, int msgno, int mode, char color3 );
void Bdisp_MMPrintRef(int*x, int*y, unsigned char*, int mode, int xmax, int, int, int color, int, int, int);
void Bdisp_MMPrint(int x, int y, unsigned char*, int mode, int xmax, int, int, int color, int backcolor, int, int);

//MARK: - Progressbars and Scrollbars
typedef struct TScrollbar
{
  unsigned int _unknown_1; // unknown changes indicator height, set to 0
  unsigned int indicatorMaximum; // max logical indicator range
  unsigned int indicatorHeight; // height of the indicator in units
  unsigned int indicatorPosition; // indicator position in units of max
  unsigned int _unknown_5; // unknown, set to 0
  unsigned short barLeft; // x position of bar
  unsigned short barTop; // y position of bar
  unsigned short barHeight; // height of bar
  unsigned short barWidth; // width of bar
} TScrollbar ;

void Scrollbar(TScrollbar * scrollbar);
void StandardScrollbar( void* );
void ProgressBar(int, int );
void ProgressBar0(int P1, int P2, int P3, int current, int max);
void ProgressBar2(unsigned char *heading, int current, int max);

//MARK: - Status Area or Header Related SYSCALLS

typedef enum DefineStatusArea : int {
    kDefineStatusArea_Clear       = 0,
    kDefineStatusArea_SetDefault  = 1
} DefineStatusArea;

typedef enum StatusAreaFlags : int {
    kStatusAreaFlags_Battery            = 1 << 0,
    kStatusAreaFlags_AlphaShift         = 1 << 1,
    kStatusAreaFlags_SetupInputOutput   = 1 << 2,
    kStatusAreaFlags_SetupFracResult    = 1 << 3,
    kStatusAreaFlags_SetupAngle         = 1 << 4,
    kStatusAreaFlags_SetupComplexMode   = 1 << 5,
    kStatusAreaFlags_SetupDisplay       = 1 << 6,
    kStatusAreaFlags_Text               = 1 << 8,
    kStatusAreaFlags_Glyph              = 1 << 9
} StatusAreaFlags;

typedef struct TGlyphDefine {
    short    dx;    // usually 0x24, width of the glyph
    short    dy;    // usually 0x16, height of the glyph
    void*    bitmap; // 2-byte-per-pixel (RGB565) bitmap
    short    xAlignment; // alignment of the glyph in the status bar, see below.
    short    xOffset; // offset of the glyph, meaning varies with xalignment.
} TGlyphDefine;

typedef TGlyphDefine* TGlyphDefineRef;

int DefineStatusAreaFlags( int, StatusAreaFlags, void*, void* );

/**
 Defines a custom glyph to be shown on the status area.
 
 - Parameter index: Index of the glyph in the array.
 - Parameter glypth: Pointer to a struct containing information on the glyph to display
 */
void DefineStatusGlyph( int index, TGlyphDefineRef glypth );

void DefineStatusMessage( char*msg, short P2, char P3, char P4 );
void DisplayStatusArea( void );
void DrawHeaderLine( void );
void EnableStatusArea( int );
void Bdisp_HeaderFill( unsigned char color_idx1, unsigned char color_idx2 );
void Bdisp_HeaderFill2( unsigned int, unsigned int, unsigned char, unsigned char );
void Bdisp_HeaderText( void );
void Bdisp_HeaderText2( void );
void EnableDisplayHeader( int, int );

//MARK: - Status Area Icon SYSCALLS
//NOTE: Status area icon syscalls: (it may be more appropriate to use the status area flags)

void APP_EACT_StatusIcon( int ); //not sure what this is exactly for, if it displays something on screen it's here, otherwise in app.h. will test some day (gbl08ma)
void SetupMode_StatusIcon( void ); //not sure what this does, doesn't seem to be documented anywhere. will test some day (gbl08ma)
void d_c_Icon( unsigned int );
void BatteryIcon( unsigned int );
void KeyboardIcon( unsigned int );
void LineIcon( unsigned int );
void NormIcon( unsigned int );
void RadIcon( unsigned int );
void RealIcon( unsigned int );

//Other:
void FKey_Display( int, void* );
void GetFKeyPtr( int, void* );
void DispInt( int, int ); //not sure what this does, doesn't seem to be documented anywhere. will test some day (gbl08ma)
int LocalizeMessage1( int msgno, char*result );
int SMEM_MapIconToExt( unsigned char*filename, unsigned short*foldername, unsigned int*msgno, unsigned short*iconbuffer ); // despite starting with SMEM, this is mostly a graphical function used to get icons for different file types.

//MARK: - NOT! SYSCALLS (defined within libfxcg)

typedef enum BlobMode : int {
    kBlobMode_Copy         = 0,
    kBlobMode_XOR          = 1,
    kBlobMode_AND          = 2,
    kBlobMode_OR           = 3,
    kBlobMode_Invert       = 4
} BlobMode;

/**
 Renders a BLOB (Image) to VRAM
 
 - Parameter x: The x position for the image to be rendered at.
 - Parameter y: The y position for the image to be rendered at.
 - Parameter width: The image width.
 - Parameter height: The image height.
 - Parameter blob: The binary large object (Image)
 - Parameter mode: How the data will be rendered to VRAM, (Copy, XOR, AND, OR or Inverted)
 */
void fxCG_PutBlob_VRAM( int x, int y, int width, int height, const color_t* blob, BlobMode mode );


#ifdef __cplusplus
}
#endif

#endif /* __FXCG_DISPLAY_H */
