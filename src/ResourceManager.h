/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RANGERS_RESOURCEMANAGER_H
#define RANGERS_RESOURCEMANAGER_H
#include "ResourceAdapter.h"
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>

namespace Rangers
{
class Texture;
class AnimatedTexture;
class Font;
class Sprite;
class AnimatedSprite;
class Object;
struct GAIAnimation;

class ResourceManager
{
public:
    void addRPKG(const std::wstring& path);
    void addDir(const std::wstring& path);

    char* loadData(const std::wstring& name, size_t &size);
    boost::shared_ptr<Texture> loadTexture(const std::wstring& name);
    boost::shared_ptr<AnimatedTexture> loadAnimation(const std::wstring& name, bool backgroundLoading = false, bool needBackground = false);
    boost::shared_ptr<Font> loadFont(const std::wstring& name, int size);
    
    Sprite getSprite(const std::wstring& name, Object *parent = 0);
    AnimatedSprite getAnimatedSprite(const std::wstring& name, bool backgroundLoading = false, bool needBackground = false,  Object *parent = 0);

    static ResourceManager *instance();
    
    void processMain();


private:
    void processGAIQueue();
    void cleanupUnused();
    std::map<std::wstring, boost::shared_ptr<ResourceAdapter> > files;
    std::list<boost::shared_ptr<ResourceAdapter> > adaptors;
    
    std::map<std::wstring, boost::shared_ptr<Texture> > textures;
    std::map<std::wstring, boost::shared_ptr<AnimatedTexture> > animations;
    std::map<std::wstring, boost::shared_ptr<Font> > fonts;
    std::map<boost::shared_ptr<AnimatedTexture>, GAIAnimation> onDemandGAIQueue;
};
};

#endif // RESOURCEMANAGER_H