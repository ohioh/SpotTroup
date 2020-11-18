
#ifndef Utilities_h
#define Utilities_h

#include "stdint.h"
#include <QChar>
#include "huctx.h"
#include "sbdef.h"
#include <bb/platform/PlatformInfo>

class Utilities {

public:
	static uint8_t hexToInt(QChar c1, QChar c2);
	static void hexToIntArray(QString hex_string,uint8_t* output);
	static QString intToHex(int decimal);
	static void alert(QString msg);
	static uint getSysTimeSeconds();
	static QString getOSVersion();

private:
	static const QString HEX_CHARS;
};

#endif // ifndef Utilities_h
