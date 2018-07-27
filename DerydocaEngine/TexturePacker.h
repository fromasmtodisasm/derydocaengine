#pragma once
#include <vector>
#include <map>
#include "Rectangle.h"
#include "TexturePackerImage.h"
#include "TexturePackerTextureData.h"

using namespace std;

class Texture;

class TexturePacker
{
public:
	TexturePacker();
	~TexturePacker();

	void addImage(int id, int width, int height, int bearingX, int bearyingY, int advanceX, int advanceY, unsigned char* imageBuffer);
	void packImages();
	bool getIsDirty() const { return m_isDirty; }
	Texture* allocTexture() { return dest.allocTexture(); };
	void freeSubImageData();
	vector<TexturePackerImage> getSubImageData() { return m_images; }
private:
	const int IMAGE_SIZE = 512;
	vector<TexturePackerImage> m_images;
	map<int, unsigned char*> m_imageBuffers;
	bool m_isDirty;
	TexturePackerTextureData dest;
	vector<IntRectangle> m_imageBounds;

	bool isPointFree(int x, int y);
	bool isRectFree(int x, int y, int w, int h);
};
