#include "ImageLoader.h"
#include <iostream>
#include "Plist.hpp"

#include "Common.h"
#include "boost/any.hpp"
#include "PlatformUtils.h"

std::map<std::string, TextureDefinition> ImageLoader::images;

void ImageLoader::ParsePlist(std::string path)
{
	std::map<std::string, boost::any> dict;
	Plist::readPlist(PlatformUtils::getMainResource(path), dict);

	if (dict.find("frames") != dict.end())
	{
		const boost::any& frames = dict["frames"];

		if (frames.type() == typeid(std::map<std::string, boost::any>))
		{
			const std::map<std::string, boost::any>& framesDict =
				boost::any_cast<std::map<std::string, boost::any>>(frames);

			for (const auto& frameEntry : framesDict)
			{
				std::string frameKey = frameEntry.first;

				if (frameEntry.second.type() == typeid(std::map<std::string, boost::any>))
				{
					const std::map<std::string, boost::any>& frameDict =
						boost::any_cast<std::map<std::string, boost::any>>(frameEntry.second);

					sf::Rect<int> texRect;

					std::string textureRect = boost::any_cast<std::string>(frameDict.at("textureRect"));
					textureRect = textureRect.erase(textureRect.length() - 2, textureRect.length() - 1);
					textureRect.erase(0, 2);

					auto split = Common::splitByDelim(textureRect, ',');

					bool rotated = boost::any_cast<bool>(frameDict.at("textureRotated"));

					texRect.left = std::stoi(split[0]);
					texRect.top = std::stoi(split[1].erase(split[1].length() - 1));

					texRect.width = std::stoi(split[2].erase(0, 1));
					texRect.height = std::stoi(split[3].erase(split[3].length()));

					auto split2 = Common::splitByDelim(boost::any_cast<std::string>(frameDict.at("spriteOffset")), ',');

					sf::Vector2i spriteOffset = {0, 0};

					spriteOffset.x = std::stoi(split2[0].erase(0, 1));
					spriteOffset.y = std::stoi(split2[1].erase(split2[1].length() - 1));

					sf::Vector2f anchor = {0, 0};

					anchor.x = -(float)spriteOffset.x / 4.f;
					anchor.y = -(float)spriteOffset.y / 4.f;

					if (rotated)
					{
						anchor = {anchor.y, -anchor.x};
						std::swap(texRect.width, texRect.height);
					}

					TextureDefinition def(texRect, rotated, Common::splitByDelim(path, '.')[0] + ".png", spriteOffset,
										  anchor);

					images.insert({frameKey, def});
				}
			}
		}
	}
}
