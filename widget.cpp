#include "widget.h"
#include "ui_widget.h"
#include "wrp.h"
#include <cstdio>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::ReadFile(char *wrpname)
{
	qDebug() << "Reading WRP file: " << wrpname;

	FILE *map;
	char sig[33];
	int x = 0, z = 0, px = 0, pz = 0;
	TexIndex = 0;

	map = fopen (wrpname, "rb");
	if (!map)
	{
		qDebug() << "error in wrp";
		QMessageBox::information(this,
					 tr("Unable to open file"),
					 "sorry could not find / read 4WVR WRP file. exiting...");
		exit (1);
	}
	qDebug() << "opened wrp";

	fread (sig, 4, 1, map);
	sig[4] = 0;
	fread(&MapSize, 4, 1, map);
	fread(&MapSize, 4, 1, map);

	// read elevations
	pz = MapSize;
	for (int zx = 0; zx < MapSize * MapSize; zx++)
	{
		fread(&wrp[x][z].Elevation,sizeof(wrp[x][z].Elevation),1,map);

		// wrp coordinates
		x++;
		if (x == MapSize)
		{
			z++; x = 0;
		}
		if (z == MapSize)
		{
			z = 0;
		}
	}

	// read textures IDs
	x = 0, z = 0, TexIndex = 0;
	px = 0, pz = MapSize;

	for (int tx = 0; tx < MapSize * MapSize; tx++)
	{
		wrp[x][z].TexIndex = 0;
		fread(&wrp[x][z].TexIndex,sizeof(wrp[x][z].TexIndex),1,map);

		// wrp coordinates
		x++;
		if (x == MapSize)
		{
			z++; x = 0;
		}
		if (z == MapSize)
		{
			z = 0;
		}
	}

	//textures 32 char length and total of 512
	for (int ix = 0; ix < 512; ix++)
	{
		sig[0] = 0;
		fread(sig, 32, 1, map);
		strcpy (TexStrings[ix], sig);
	}

	fclose(map);

	// launch the next function
	CreateWRP_to_BMP(wrpname);
}

void Widget::CreateWRP_to_BMP(char *filename)
{
	int x = 0, z = 0, px = 0, pz = 0;
	char tmpOFPTexture[32];

	// open bitmap file
	QImage image(MapSize, MapSize, QImage::Format_Indexed8);
	QRgb value;

	qDebug() << "Depth (BPP): " << QString::number(image.depth());
	qDebug() << "Height: " << QString::number(image.height());
	qDebug() << "Width: " << QString::number(image.width());

/*
old values

	// VTE seabed 155, 155, 255
	value = qRgb(155, 155, 255);
	image.setColor(0, value);

	// VTE grass 128, 255, 0
	value = qRgb(128, 255, 0);
	image.setColor(1, value);

	// VTE airstrip 255, 0, 0
	value = qRgb(255, 0, 0);
	image.setColor(2, value);

	// VTE asphalt 255, 255, 0
	value = qRgb(255, 255, 0);
	image.setColor(3, value);

	// VTE brown grass 255, 255, 255
	value = qRgb(255, 255, 255);
	image.setColor(4, value);

	// VTE concrete 0, 255, 255
	value = qRgb(0, 255, 255);
	image.setColor(5, value);

	// VTE concrete block 0, 0, 255
	value = qRgb(0, 0, 255);
	image.setColor(6, value);

	// VTE dry grass 128, 0, 0
	value = qRgb(128, 0, 0);
	image.setColor(7, value);

	// VTE farm field 128, 128, 0
	value = qRgb(128, 128, 0);
	image.setColor(8, value);

	// VTE forest 128, 128, 128
	value = qRgb(128, 128, 128);
	image.setColor(9, value);

	// VTE gravel grass 128, 255, 255
	value = qRgb(128, 255, 255);
	image.setColor(10, value);

	// VTE mountain rock
	value = qRgb(255, 128, 0);
	image.setColor(11, value);

	// VTE mountain rock solid 255, 128, 128
	value = qRgb(255, 128, 128);
	image.setColor(12, value);

	// VTE rocks 100, 0, 0
	value = qRgb(100, 0, 0);
	image.setColor(13, value);

	// VTE runway 100, 100, 0
	value = qRgb(100, 100, 0);
	image.setColor(14, value);

	// VTE sand 100, 100, 100
	value = qRgb(100, 100, 100);
	image.setColor(15, value);

	// VTE sand pink 255, 100, 0
	value = qRgb(255, 100, 0);
	image.setColor(16, value);

	// VTE sand red 255, 100, 100
	value = qRgb(255, 100, 100);
	image.setColor(17, value);
*/
	// default pink for all purposes
	value = qRgb(255, 0, 255);
	image.setColor(0, value);

	// pmc_grass, o\\b1
	value = qRgb(0, 255, 0);
	image.setColor(1, value);

	// pmc_concrete, o\\beton
	value = qRgb(0, 0, 255);
	image.setColor(2, value);

	// , o\\blita
	value = qRgb(0, 0, 0);
	image.setColor(3, value);

	// pmc_forest, o\\j1
	value = qRgb(0, 100, 255);
	image.setColor(4, value);

	// , o\\l1
	value = qRgb(0, 0, 0);
	image.setColor(5, value);

	// , o\\lom2
	value = qRgb(0, 0, 0);
	image.setColor(6, value);

	// , o\\n1
	value = qRgb(0, 0, 0);
	image.setColor(7, value);

	// pmc_city, o\\naves
	value = qRgb(255, 0, 100);
	image.setColor(8, value);

	// , o\\park
	value = qRgb(0, 0, 0);
	image.setColor(9, value);

	// pmc_farm_land, o\\pole
	value = qRgb(0, 255, 255);
	image.setColor(10, value);

	// , o\\ps
	value = qRgb(0, 0, 0);
	image.setColor(11, value);

	// pmc_sand, o\\pt
	value = qRgb(255, 0, 0);
	image.setColor(12, value);

	// pmc_runway, o\\rollpi
	value = qRgb(255, 100, 0);
	image.setColor(13, value);

	// pmc_runway, o\\runpi_
	value = qRgb(255, 100, 0);
	image.setColor(14, value);

	// pmc_runway_grass, o\\runtr_
	value = qRgb(255, 100, 100);
	image.setColor(15, value);

	// pmc_rock, o\\s2
	value = qRgb(100, 100, 100);
	image.setColor(16, value);

	// , o\\snih
	value = qRgb(0, 0, 0);
	image.setColor(17, value);

	// pmc_grass, o\\t1
	value = qRgb(255, 255, 0 );
	image.setColor(18, value);

	// , o\\trava
	value = qRgb(0, 0, 0);
	image.setColor(19, value);

	// OFP Texture .PAC RGB's.
	TexIndex = 0;
	pz = MapSize;

// this has to reference the STRING name for texture .PAC file.
	for (int tx = 0; tx < MapSize * MapSize; tx++)
	{
		TexIndex = wrp[x][z].TexIndex;
		// copy the OFP texture name into variable.
		strcpy (tmpOFPTexture, TexStrings[TexIndex]);
		// lower case it.
		strlwr (tmpOFPTexture);

		// compare the texture name to OFP texturing and set pixel.

		// green light grass
		if (strstr (tmpOFPTexture, "o\\b1"))
		{
			image.setPixel(px, pz, 1);
		}

		if (strstr (tmpOFPTexture, "o\\beton"))
		{
			image.setPixel(px, pz, 2);
		}

		if (strstr (tmpOFPTexture, "o\\blita"))
		{
			image.setPixel(px, pz, 3);
		}

		// bottom of sea brownish
		if (strstr (tmpOFPTexture, "o\\j1"))
		{
			image.setPixel(px, pz, 4);
		}

		if (strstr (tmpOFPTexture, "o\\l1"))
		{
			image.setPixel(px, pz, 5);
		}

		if (strstr (tmpOFPTexture, "o\\lom2"))
		{
			image.setPixel(px, pz, 6);
		}

		if (strstr (tmpOFPTexture, "o\\n1"))
		{
			image.setPixel(px, pz, 7);
		}

		if (strstr (tmpOFPTexture, "o\\naves"))
		{
			image.setPixel(px, pz, 8);
		}

		if (strstr (tmpOFPTexture, "o\\park"))
		{
			image.setPixel(px, pz, 9);
		}

		if (strstr (tmpOFPTexture, "o\\pole"))
		{
			image.setPixel(px, pz, 10);
		}

		if (strstr (tmpOFPTexture, "o\\ps"))
		{
			image.setPixel(px, pz, 11);
		}

		// sand
		if (strstr (tmpOFPTexture, "o\\pt"))
		{
			image.setPixel(px, pz, 12);
		}

		if (strstr (tmpOFPTexture, "o\\rollpi"))
		{
			image.setPixel(px, pz, 13);
		}

		if (strstr (tmpOFPTexture, "o\\runpi_"))
		{
			image.setPixel(px, pz, 14);
		}

		if (strstr (tmpOFPTexture, "o\\runtr_"))
		{
			image.setPixel(px, pz, 15);
		}

		if (strstr (tmpOFPTexture, "o\\s2"))
		{
			image.setPixel(px, pz, 16);
		}

		if (strstr (tmpOFPTexture, "o\\snih"))
		{
			image.setPixel(px, pz, 17);
		}

		// green dark grass
		if (strstr (tmpOFPTexture, "o\\t1"))
		{
			image.setPixel(px, pz, 18);
		}

		// green dark grass with shrubbery
		if (strstr (tmpOFPTexture, "o\\trava"))
		{
			image.setPixel(px, pz, 19);
		}

		// bitmap coordinates
		px++;
		if (px == MapSize)
		{
			pz--;
			px = 0;
		}
		if (pz == 0)
		{
			pz = MapSize;
		}

		// wrp coordinates
		x++;
		if (x == MapSize)
		{
			z++;
			x = 0;
		}
		if (z == MapSize)
		{
			z = 0;
		}
	}

	// Elevation RGB's.
	x = 0, z = 0, px = 0, pz = MapSize;
	for (int zx = 0; zx < MapSize * MapSize; zx++)
	{
		// all below sea level will be set to specific RGB.
		if (wrp[x][z].Elevation < 0)
		{
			image.setPixel(px, pz, 0);
		}

		// bitmap coordinates
		px++;
		if (px == MapSize)
		{
			pz--;
			px = 0;
		}
		if (pz == 0)
		{
			pz = MapSize;
		}

		// wrp coordinates
		x++;
		if (x == MapSize)
		{
			z++;
			x = 0;
		}
		if (z == MapSize)
		{
			z = 0;
		}
	}

	strcat(filename, "_mask.png");
	image.save(filename, 0, -1);
	qDebug() << "saved: " << filename;
}
