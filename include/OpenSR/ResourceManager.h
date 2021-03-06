/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/config.h"
#include "OpenSR/ResourceAdapter.h"
#include "OpenSR/ResourceObjectManager.h"

#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

struct SDL_RWops;
namespace Rangers
{
class Texture;
class AnimatedTexture;
class Font;
class Sprite;
class AnimatedSprite;
class Object;
struct GAIAnimation;
class LuaWidget;
class Widget;
struct DATRecord;

class RANGERS_ENGINE_API ResourceManager
{
public:
    void addRPKG(const std::string& path);
    void addDir(const std::string& path);

    boost::shared_array<char> loadData(const std::string& name, size_t &size);
    boost::shared_ptr<std::istream> getFileStream(const std::string& name);
    SDL_RWops* getSDLRW(const std::string& name);

    boost::shared_ptr<Texture> loadTexture(const std::string& name);
    boost::shared_ptr<AnimatedTexture> loadAnimation(const std::string& name, bool backgroundLoading = false);

    boost::shared_ptr<Font> loadFont(const FontDescriptor& desc);
    boost::shared_ptr<Font> loadFont(const std::string& name, int size, bool antialiased = true);

    void addDATFile(const std::string& name, bool isCache = false);
    boost::shared_ptr<DATRecord> datRoot();
    std::string datValue(const std::string& path) const;

    bool resourceExists(const std::string& path);

    ResourceObjectManager& objectManager();

    static ResourceManager& instance();

    void processMain();

private:
    ResourceManager();
    ResourceManager(const ResourceManager& other);

    class GAIWorker;
    void processGAIQueue();
    void cleanupUnused();

    std::map<std::string, boost::shared_ptr<ResourceAdapter> > m_files;
    std::list<boost::shared_ptr<ResourceAdapter> > m_adapters;

    std::map<std::string, boost::shared_ptr<Texture> > m_textures;
    std::map<std::string, boost::shared_ptr<AnimatedTexture> > m_animations;
    std::map<std::string, boost::shared_ptr<Font> > m_fonts;
    std::map<boost::shared_ptr<AnimatedTexture>, GAIWorker *> m_gaiQueue;
    boost::shared_ptr<DATRecord> m_datRoot;

    ResourceObjectManager m_objectManager;
};
}

#endif // RESOURCEMANAGER_H
