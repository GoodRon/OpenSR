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

#include "gai.h"
#include <fstream>
#include <IL/il.h>
#include <cstring>
#include <cstdlib>
#include <libRanger.h>
#include "utils.h"

namespace Rangers
{
int extractGAI2PNG(const std::string& gaiFile, const std::string& outName)
{
    std::ifstream gaiStream(gaiFile, std::ios_base::in | std::ios_base::binary);
    GAIHeader header = loadGAIHeader(gaiStream);
    GIFrame *bg = 0;
    if (header.haveBackground)
    {
        std::ifstream bgStream(directory(gaiFile) + "/" + basename(gaiFile) + ".gi", std::ios_base::in | std::ios_base::binary);
        bg = new GIFrame();
        *bg = loadGIFrame(bgStream, true, 0, 0, header.startX, header.startY, header.finishX, header.finishY);
        bgStream.close();
    }
    gaiStream.seekg(0, std::ios_base::beg);
    GAIAnimation gai = loadGAIAnimation(gaiStream,  bg);
    gaiStream.close();

    ILuint *frames = new ILuint[gai.frameCount];
    char counts[5];
    ilInit();
    ilGenImages(gai.frameCount, frames);
    for (int i = 0; i < gai.frameCount; i++)
    {
        sprintf(counts, "%04d", i);
        GIFrame g = gai.frames[i];
        ilBindImage(frames[i]);
        ilTexImage(g.width, g.height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, g.data);
        ilSaveImage((directory(outName) + "/" + basename(outName) + counts + ".png").c_str());
        ilDeleteImage(frames[i]);
        delete g.data;
    }
    delete gai.frames;
    return 0;
}
}
