#ifndef CRC32_H
#define CRC32_H

class Crc32{
public:
	//! Рассчитывает и возвращает CRC32 строки
	/*!
		@param data - указатель на строку
		@param length - длина строки
		@return CRC32

		Пример использования:

			char* str = {"123456789"};
			unsigned int result = calcCRC32(str, 9);
	*/
	static unsigned int calcCrc(const unsigned char *data, int length);
};

#endif
