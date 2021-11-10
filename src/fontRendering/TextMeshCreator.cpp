#include "TextMeshCreator.h"

TextMeshCreator::TextMeshCreator(std::string metaFile)
{
	metaData = new MetaFile(metaFile);
}

TextMeshData* TextMeshCreator::createTextMesh(GUIText* text)
{
	std::vector<Line*>* lines = createStructure(text);
	TextMeshData *data = createQuadVertices(text, lines);
	return data;
}

std::vector<Line*>* TextMeshCreator::createStructure(GUIText *text)
{
	std::string chars = text->getTextString();
	auto lines = new std::vector<Line*>;
	Line* currentLine = new Line(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
	Word* currentWord = new Word(text->getFontSize());
	
	for (char c : chars) {
		unsigned ascii = (unsigned) c;
		if (ascii == SPACE_ASCII) {
			bool added = currentLine->attemptToAddWord(*currentWord);
			if (!added) {
				lines->push_back(currentLine);
				currentLine = new Line(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
				currentLine->attemptToAddWord(*currentWord);
			}
			currentWord = new Word(text->getFontSize());
			continue;
		}
		
		Character* character = metaData->getCharacter(ascii);
		if (character != nullptr)
			currentWord->addCharacter(*character);
	}
	completeStructure(lines, currentLine, currentWord, text);
	return lines;
}

void TextMeshCreator::completeStructure(std::vector<Line*>* lines, Line* currentLine, Word* currentWord, GUIText* text)
{
	bool added = currentLine->attemptToAddWord(*currentWord);
	if (!added) {
		lines->push_back(currentLine);
		currentLine = new Line(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
		currentLine->attemptToAddWord(*currentWord);
	}
	lines->push_back(currentLine);
}

TextMeshData* TextMeshCreator::createQuadVertices(GUIText* text, std::vector<Line*>* lines)
{
	text->setNumberOfLines(lines->size());
	double cursorX = 0.0;
	double cursorY = 0.0;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> textureCoords;
	
	for (Line* line : *lines) {
		if (text->isCentered()) {
			cursorX = (line->getMaxLength() - line->getLineLength()) / 2;
		}
		
		for (Word word : line->getWords()) {
			for (Character letter: word.getCharacters()) {
				addVerticesForCharacter(cursorX, cursorY, letter, text->getFontSize(), vertices);
				addTexCoords(textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
							letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
				cursorX += letter.getxAdvance() * text->getFontSize();				
			}
			cursorX += metaData->getSpaceWidth() * text->getFontSize();
		}
		
		cursorX = 0;
		cursorY += LINE_HEIGHT * text->getFontSize();
	}

	return new TextMeshData(vertices, textureCoords);
}

void TextMeshCreator::addVerticesForCharacter(
	double cursorX, double cursorY, 
	Character character, double fontSize,
	std::vector<GLfloat>& vertices)
{
	double x = cursorX + (character.getxOffset() * fontSize);
	double y = cursorY + (character.getyOffset() * fontSize);
	double maxX = x + (character.getSizeX() * fontSize);
	double maxY = y + (character.getSizeY() * fontSize);
	double properX = (2 * x) - 1;
	double properY = (-2 * y) + 1;
	double properMaxX = (2 * maxX) - 1;
	double properMaxY = (-2 * maxY) + 1;
	addVertices(vertices, properX, properY, properMaxX, properMaxY);	
}

void TextMeshCreator::addVertices(std::vector<GLfloat>& vertices, double x, double y, double maxX, double maxY)
{
	vertices.push_back((GLfloat) x);
	vertices.push_back((GLfloat) y);
	vertices.push_back((GLfloat) x);
	vertices.push_back((GLfloat) maxY);
	vertices.push_back((GLfloat) maxX);
	vertices.push_back((GLfloat) maxY);
	vertices.push_back((GLfloat) maxX);
	vertices.push_back((GLfloat) maxY);
	vertices.push_back((GLfloat) maxX);
	vertices.push_back((GLfloat) y);
	vertices.push_back((GLfloat) x);
	vertices.push_back((GLfloat) y);	
}

void TextMeshCreator::addTexCoords(std::vector<GLfloat>& texCoords, double x, double y, double maxX, double maxY)
{
	texCoords.push_back((GLfloat) x);
	texCoords.push_back((GLfloat) y);
	texCoords.push_back((GLfloat) x);
	texCoords.push_back((GLfloat) maxY);
	texCoords.push_back((GLfloat) maxX);
	texCoords.push_back((GLfloat) maxY);
	texCoords.push_back((GLfloat) maxX);
	texCoords.push_back((GLfloat) maxY);
	texCoords.push_back((GLfloat) maxX);
	texCoords.push_back((GLfloat) y);
	texCoords.push_back((GLfloat) x);
	texCoords.push_back((GLfloat) y);	
}
