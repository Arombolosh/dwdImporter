#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include "QtExt_IOCompress.h"

int main()
{
	const QByteArray sourceText = "This example demonstrates how to compress and write the contents of a QByteArray to a file using IOCompress";

	// Write to file.
	{
		QFile file("test");
		QtExt::IOCompress compressor(&file);
		compressor.open(QIODevice::WriteOnly);
		compressor.write(sourceText);

		QFile textfile("test.txt");
		textfile.open(QIODevice::WriteOnly);
		textfile.write(sourceText);
	}

	// Read all from file and print.
	{
		QFile file("test");
		QtExt::IOCompress compressor(&file);
		compressor.open(QIODevice::ReadOnly);
		const QByteArray readText = compressor.readAll();
		std::cout << readText.constData();
		std::cin.get();
	}
}
