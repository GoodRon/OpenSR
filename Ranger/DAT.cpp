/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <locale>

namespace Rangers
{
namespace
{
std::string readWideString(std::istream &stream)
{
    //TODO: Use std::codecvt_utf8
    wchar_t c = 0;
    std::wstring result;

    while (stream.good())
    {
        stream.read((char*)&c, 2);
        if (c == 0)
            return toUTF8(result);
        result.push_back(c);
    }
    return std::string();
}

void writeWideString(std::ostream &stream, const std::string& value)
{
    //TODO: Use std::codecvt_utf8
    int l;
    char *utf16 = convertText("UCS-2", "UTF-8", value.c_str(), value.length(), l);
    stream.write(utf16, l);
    delete[] utf16;
    uint16_t nullterm = 0;
    stream.write((char*)&nullterm, 2);
}

DATRecord::iterator binary_find(std::vector<DATRecord> &r, const std::string& key)
{
    DATRecord dummy;
    dummy.name = key;
    DATRecord::iterator i = std::lower_bound(r.begin(), r.end(), dummy, [&](const DATRecord & a1, const DATRecord & a2)
    {
        return a1.name < a2.name;
    });

    if (i != r.end() && !(dummy.name < i->name))
        return i;
    else
        return r.end(); // not found
}


DATRecord::const_iterator binary_find(const std::vector<DATRecord> &r, const std::string& key)
{
    DATRecord dummy;
    dummy.name = key;
    DATRecord::const_iterator i = std::lower_bound(r.begin(), r.end(), dummy, [&](const DATRecord & a1, const DATRecord & a2)
    {
        return a1.name < a2.name;
    });

    if (i != r.end() && !(dummy.name < i->name))
        return i;
    else
        return r.end(); // not found
}
}

/*!
 * Write recursively key-value tree to *.dat file.
 * \param stream output stream
 * \param node current node
 */
void writeDATTree(std::ostream &stream, const DATRecord& node)
{
    if (node.type == DATRecord::NODE)
    {
        uint32_t count = 0;
        uint8_t isTree = 0;
        for (const DATRecord& c : node)
        {
            if (c.type != DATRecord::VALUE)
            {
                isTree = 1;
                if (c.type == DATRecord::ARRAY)
                    count += c.size() - 1;
            }
        }
        count += node.size();
        stream.write((char*)&isTree, 1);
        stream.write((char*)&count, 4);

        if (isTree)
        {
            for (const DATRecord& c : node)
            {
                uint32_t index = 0;
                uint32_t arrayCount = 1;
                uint8_t isText = 1;
                if (c.type != DATRecord::VALUE)
                    isText = 2;
                if (c.type != DATRecord::ARRAY)
                {
                    stream.write((char*)&index, 4);
                    stream.write((char*)&arrayCount, 4);
                    stream.write((char*)&isText, 1);
                    writeWideString(stream, c.name);
                }
                writeDATTree(stream, c);
            }
        }
        else
        {
            for (const DATRecord& c : node)
            {
                uint8_t isText = 1;
                stream.write((char*)&isText, 1);
                writeWideString(stream, c.name);
                writeDATTree(stream, c);
            }
        }
    }
    else if (node.type == DATRecord::ARRAY)
    {
        uint32_t count = node.size();
        uint32_t i = 0;
        for (const DATRecord& c : node)
        {
            uint32_t index = i;
            uint32_t arrayCount = 0;
            uint8_t isText = 1;
            if (i == 0)
                arrayCount = count;
            if (c.type != DATRecord::VALUE)
                isText = 2;
            stream.write((char*)&index, 4);
            stream.write((char*)&arrayCount, 4);
            stream.write((char*)&isText, 1);
            writeWideString(stream, node.name);
            writeDATTree(stream, c);
            i++;
        }
    }
    else
    {
        writeWideString(stream, node.value);
    }
}

/*!
 * Read recursively key-value tree from *.dat file.
 * \param stream input stream (uncompressed DAT!)
 * \param node current node
 */
void readDATTree(std::istream &stream, DATRecord& node, bool isCache)
{
    uint8_t isTree = 0;
    uint32_t count;

    if (!isCache)
        stream.read((char*)&isTree, 1);
    stream.read((char*)&count, 4);

    node.type = DATRecord::NODE;

    if (isTree)
    {
        DATRecord* currentNode = &node;
        uint32_t currentCount = 0;
        for (int i = 0; i < count; i++)
        {
            uint32_t index, arrayCount;
            uint8_t isText;

            DATRecord r;
            r.type = DATRecord::VALUE;

            stream.read((char*)&index, 4);
            stream.read((char*)&arrayCount, 4);
            stream.read((char*)&isText, 1);
            r.name = readWideString(stream);

            if (isText == 2)
                readDATTree(stream, r, isCache);
            else
                r.value = readWideString(stream);

            if ((arrayCount > 1) && (index == 0))
            {
                DATRecord array;
                array.type = DATRecord::ARRAY;
                array.name = r.name;

                r.name = std::string();

                currentCount = arrayCount;
                DATRecord::iterator i = currentNode->add(array);
                currentNode = i.operator->();

                currentNode->add(r);
            }
            else if ((arrayCount == 0) && (index < currentCount - 1))
            {
                r.name = std::string();
                currentNode->add(r);
            }
            else if ((arrayCount == 0) && (index == currentCount - 1))
            {
                currentCount = 0;
                currentNode->add(r);
                currentNode = &node;
            }
            else
            {
                currentNode->add(r);
            }
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            uint8_t isText = 0;
            stream.read((char*)&isText, 1);

            DATRecord r;
            r.name = readWideString(stream);

            if (isText == 2)
            {
                readDATTree(stream, r, isCache);
            }
            else
            {
                r.type = DATRecord::VALUE;
                r.value = readWideString(stream);
            }

            node.add(r);
        }
    }
}

/*!
 * Load whole *.dat file.
 * \param stream - input stream (uncompressed DAT!)
 * \param isCache - is CacheData.dat format
 * \return root node
 */
DATRecord loadDAT(std::istream &stream, bool isCache)
{
    DATRecord root;
    std::streampos start = stream.tellg();
    uint32_t sig;
    stream.read((char*)&sig, 4);

    if (sig == 0x31304c5a)
    {
        stream.seekg(0, std::ios_base::end);
        uint32_t size = stream.tellg() - start;
        uint8_t *data = new uint8_t[size];
        stream.seekg(start, std::ios_base::beg);
        stream.read((char*)data, size);
        size_t resultSize = 0;
        uint8_t *datData = unpackZL01(data, size, resultSize);
        delete[] data;
        boost::iostreams::stream<boost::iostreams::array_source> datStream(boost::iostreams::array_source((const char*)datData, resultSize));
        readDATTree(datStream, root, isCache);
        delete[] datData;
    }
    else
    {
        stream.seekg(start, std::ios_base::beg);
        readDATTree(stream, root, isCache);
    }
    return root;
}

/*!
 * Save *.dat file.
 * \param stream - output stream (uncompressed)
 * \param root root node
 */
void saveDAT(std::ostream &stream, const DATRecord& root)
{
    writeDATTree(stream, root);
}

DATRecord::DATRecord(Type type , const std::string& name, const std::string& value):
    type(type), name(name), value(value)
{

}

std::vector<DATRecord> DATRecord::children() const
{
    return m_children;
}

std::vector<DATRecord>::size_type DATRecord::size() const
{
    return m_children.size();
}

bool DATRecord::empty() const
{
    return m_children.empty();
}

DATRecord::iterator DATRecord::add(const DATRecord& record)
{
    if (type == DATRecord::ARRAY)
    {
        m_children.push_back(record);
        return m_children.end() - 1;
    }
    else if (type == DATRecord::NODE)
    {
        iterator pos = std::lower_bound(
                           m_children.begin(),
                           m_children.end(),
                           record,
                           [&](const DATRecord & a1, const DATRecord & a2)
        {
            return a1.name < a2.name;
        }
                       );
        if (pos == m_children.end())
        {
            m_children.push_back(record);
            pos = m_children.end() - 1;
        }
        else if (pos->name == record.name)
            *pos = record;
        else
            pos = m_children.insert(pos, record);
        return pos;
    }
    return iterator();
}

void DATRecord::erase(int i)
{
    m_children.erase(m_children.begin() + i);
}

void DATRecord::erase(const std::string& key)
{
    iterator pos = binary_find(m_children, key);
    m_children.erase(pos);
}

void DATRecord::clear()
{
    m_children.clear();
}

const DATRecord& DATRecord::at(int i) const
{
    return m_children.at(i);
}

const DATRecord& DATRecord::at(const std::string& key) const
{
    const_iterator pos = binary_find(m_children, key);
    if (pos == m_children.end())
        throw std::out_of_range("DATRecord::at");
    return *pos;
}

DATRecord& DATRecord::operator[](int i)
{
    return m_children.at(i);
}

DATRecord::const_iterator DATRecord::find(const std::string& key) const
{
    if (type == DATRecord::ARRAY)
    {
        return std::find_if(m_children.begin(), m_children.end(), [key](const DATRecord & a)
        {
            return a.name == key;
        });
    }
    else if (type == DATRecord::NODE)
    {
        return binary_find(m_children, key);
    }
    return m_children.end();
}

DATRecord::iterator DATRecord::find(const std::string& key)
{
    if (type == DATRecord::ARRAY)
    {
        return std::find_if(m_children.begin(), m_children.end(), [key](const DATRecord & a)
        {
            return a.name == key;
        });
    }
    else if (type == DATRecord::NODE)
    {
        return binary_find(m_children, key);
    }
    return m_children.end();
}

DATRecord& DATRecord::operator[](const std::string& key)
{
    DATRecord dummy;
    dummy.name = key;
    iterator pos = binary_find(m_children, key);

    if (pos == m_children.end())
    {
        return *(add(dummy));
    }
    else
        return *pos;
}

DATRecord::iterator DATRecord::begin()
{
    return m_children.begin();
}

DATRecord::const_iterator DATRecord::begin() const
{
    return m_children.begin();
}


DATRecord::iterator DATRecord::end()
{
    return m_children.end();
}

DATRecord::const_iterator DATRecord::end() const
{
    return m_children.end();
}
}
