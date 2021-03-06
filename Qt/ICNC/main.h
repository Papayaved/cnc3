#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#define CNC_APP_MODEL ("0.3.0.3b")
/*
    0.2.2.1 изменен информационный пакет ЧПУ
    0.2.2.2 попытка перевернуть оси в QWT
    0.2.2.3 исправление ошибок Idle Run, Circle generator, Options->Write
    0.2.2.4 debug feedback
    0.2.2.8 Command keys in Code Editor, clear Run panel
    0.2.2.0 Time AM PM for non-russian languages
    0.3.0.0 Slow speed, write default settings on the disk if settings are empty
    0.3.0.1 Fixed an error step and scale
    0.3.0.2 Fixed an arc-to-arc intersection, enabled Default button on the pass window
    0.3.0.3 Edited English help, controls texts, set help focus when it opens, changed translates, add memtest
*/

//#define STONE
#define TEST_PANEL
#ifndef STONE
    #define SHOW_ENCODERS
#endif
//#define TEST_ADC

#ifdef LINUX // - defined in pro file
#define DEFAULT_COM ("/dev/ttyACM0")
#else
#define DEFAULT_COM ("COM6")
#endif

#define ST_VENDOR_ID (0x483)
#define ST_PRODUCT_ID (0x5740)

#define FULL_SCREEN

#if defined(STONE)
    #define CNC_APP_TYPE (" (" + tr("stone") + ")")
#else
    #define CNC_APP_TYPE ("")
#endif

#endif // CONSTRAINTS_H
