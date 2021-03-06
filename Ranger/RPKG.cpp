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

#include "OpenSR/libRanger.h"

#include <cstdlib>
#include <cstring>

using namespace Rangers;
//TODO: Normal error handling

/*!
 * \param stream input stream
 * \return list of file entries
 */
std::list<RPKGEntry> Rangers::loadRPKG(std::istream& stream)
{
    uint32_t sig;
    stream.read((char *)&sig, 4);
    if (sig != 0x474b5052)
        return std::list<RPKGEntry>();
    uint32_t count;
    stream.read((char *)&count, 4);
    std::list<RPKGEntry> r;
    for (unsigned int i = 0; i < count; i++)
    {
        uint32_t fileNameLength = 0;
        stream.read((char *)&fileNameLength, 4);
        char *fileName = new char[fileNameLength];
        stream.read(fileName, fileNameLength);
        RPKGEntry e;
        e.name = std::string(fileName, fileNameLength);
        delete[] fileName;
        stream.read((char *)&e.offset, 4);
        stream.read((char *)&e.size, 4);
        r.push_back(e);
    }
    return r;
}

/*!
 * \param e file entry
 * \param stream archive stream
 * \return extracted data
 */
char *Rangers::extractFile(const RPKGEntry& e, std::istream& stream, size_t &size)
{
    size = 0;
    stream.seekg(e.offset, std::ios_base::beg);
    RPKGItem item;
    stream.read((char *)&item.packType, 4);
    stream.read((char *)&item.size, 4);
    stream.read((char *)&item.packSize, 4);
    stream.read((char *)&item.chunkSize, 4);
    item.data = new unsigned char[item.packSize];
    stream.read((char *)item.data, item.packSize);

    char *d = 0;

    if (item.packType == *((uint32_t*)"RSZL"))
        d = unpackRSZL(item);
    else if (item.packType == *((uint32_t*)"RSXZ"))
        d = unpackRSXZ(item);
    else if (item.packType == *((uint32_t*)"NONE"))
    {
        d = new char[item.size];
        memcpy(d, item.data, item.size);
    }

    if (d != 0)
        size = item.size;
    else
        size = 0;
    delete item.data;

    return d;
}

/*!
 * \param entryList input file entries
 * \return size of header
 */
uint32_t Rangers::calculateRPKGHeaderSize(const std::vector<RPKGEntry>& entryList)
{
    uint32_t result = 0;
    for (std::vector<RPKGEntry>::const_iterator i = entryList.begin(); i != entryList.end(); ++i)
        result += (*i).name.length() + 13;
    return result;
}

/*!
 * \param out archive file
 * \param e file entry reference
 * \param data input data
 * \param size size of data
 */
void Rangers::appendRPKGFile(std::ofstream& out, RPKGEntry& e, const char *data, uint32_t size, RPKGCompression compression)
{
    //out.seekp(0, std::ios::end);
    e.offset = out.tellp();
    RPKGItem i;
    if (compression == RPKG_SEEKABLE_ZLIB)
        packRSZL(data, size, i);
    else if (compression == RPKG_SEEKABLE_LZMA)
        packRSXZ(data, size, i);
    else if (compression == RPKG_NONE)
    {
        i.chunkSize = 0;
        i.packSize = size;
        i.packType = *((uint32_t*)"NONE");
        i.size = size;
        i.data = (unsigned char*)data;
    }
    out.write((const char *)&i.packType, 4);
    out.write((const char *)&i.size, 4);
    out.write((const char *)&i.packSize, 4);
    out.write((const char *)&i.chunkSize, 4);
    out.write((const char *)i.data, i.packSize);
    if (compression != RPKG_NONE)
        free(i.data);
    e.size = i.packSize;
}

/*!
 * \param out archive file
 * \param entryList list of file entries
 */
void Rangers::writeRPKGHeader(std::ofstream& out, const std::vector<RPKGEntry>& entryList)
{
    //out.seekp(0, std::ios::beg);
    out.write("RPKG", 4);
    int s = entryList.size();
    out.write((const char *)&s, 4);
    for (std::vector<RPKGEntry>::const_iterator i = entryList.begin(); i != entryList.end(); ++i)
    {
        uint32_t l = (*i).name.length();
        out.write((const char *)&l, 4);
        out.write((*i).name.c_str(), l + 1);
        uint32_t offset = (*i).offset;
        out.write((const char *)&offset, 4);
        uint32_t size = (*i).size;
        out.write((const char *)&size, 4);
    }
}
