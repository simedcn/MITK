//
// Compress.cpp
//
// $Id$
//
// Library: Zip
// Package: Zip
// Module:  Compress
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Zip/Compress.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/ZipArchiveInfo.h"
#include "Poco/Zip/ZipDataInfo.h"
#include "Poco/Zip/ZipException.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include <fstream>


namespace Poco {
namespace Zip {


Compress::Compress(std::ostream& out, bool seekableOut):
	_out(out),
	_seekableOut(seekableOut),
	_files(),
	_infos(),
	_dirs(),
	_offset(0)
{
}


Compress::~Compress()
{
}


void Compress::addEntry(std::istream& in, const Poco::DateTime& lastModifiedAt, const Poco::Path& fileName, ZipCommon::CompressionMethod cm, ZipCommon::CompressionLevel cl)
{
	std::string fn = ZipUtil::validZipEntryFileName(fileName);

	if (_files.size() >= 65535)
		throw ZipException("Maximum number of entries for a ZIP file reached: 65535");
	if (!in.good())
		throw ZipException("Invalid input stream");

	std::streamoff localHeaderOffset = _offset;
	ZipLocalFileHeader hdr(fileName, lastModifiedAt, cm, cl);
	hdr.setStartPos(localHeaderOffset);

	ZipOutputStream zipOut(_out, hdr, _seekableOut);
	Poco::StreamCopier::copyStream(in, zipOut);
	zipOut.close();
	hdr.setStartPos(localHeaderOffset); // reset again now that compressed Size is known
	_offset = hdr.getEndPos();
	if (hdr.searchCRCAndSizesAfterData())
		_offset += ZipDataInfo::getFullHeaderSize();
	_files.insert(std::make_pair(fileName.toString(Poco::Path::PATH_UNIX), hdr));
	poco_assert (_out);
	ZipFileInfo nfo(hdr);
	nfo.setOffset(localHeaderOffset);
	_infos.insert(std::make_pair(fileName.toString(Poco::Path::PATH_UNIX), nfo));
	EDone.notify(this, hdr);
}


void Compress::addFileRaw(std::istream& in, const ZipLocalFileHeader& h, const Poco::Path& fileName)
{
	std::string fn = ZipUtil::validZipEntryFileName(fileName);
	//bypass the header of the input stream and point to the first byte of the data payload
	in.seekg(h.getDataStartPos(), std::ios_base::beg);

	if (_files.size() >= 65535)
		throw ZipException("Maximum number of entries for a ZIP file reached: 65535");
	if (!in.good())
		throw ZipException("Invalid input stream");

	std::streamoff localHeaderOffset = _offset;
	ZipLocalFileHeader hdr(h);
	hdr.setFileName(fn, h.isDirectory());
	hdr.setStartPos(localHeaderOffset);
	//bypass zipoutputstream
	//write the header directly
	std::string header = hdr.createHeader();
	_out.write(header.c_str(), static_cast<std::streamsize>(header.size()));
	// now fwd the payload to _out in chunks of size CHUNKSIZE
	Poco::UInt32 totalSize = hdr.getCompressedSize();
	if (totalSize > 0)
	{
		Poco::Buffer<char> buffer(COMPRESS_CHUNK_SIZE);
		Poco::UInt32 remaining = totalSize;
		while(remaining > 0)
		{
			if (remaining > COMPRESS_CHUNK_SIZE)
			{
				in.read(buffer.begin(), COMPRESS_CHUNK_SIZE);
				std::streamsize n = in.gcount();
				poco_assert_dbg (n == COMPRESS_CHUNK_SIZE);
				_out.write(buffer.begin(), n);
				remaining -= COMPRESS_CHUNK_SIZE;
			}
			else
			{
				in.read(buffer.begin(), remaining);
				std::streamsize n = in.gcount();
				poco_assert_dbg (n == remaining);
				_out.write(buffer.begin(), n);
				remaining = 0;
			}
		}
	}
	//write optional block afterwards
	if (hdr.searchCRCAndSizesAfterData())
	{
		ZipDataInfo info(in, false);
		_out.write(info.getRawHeader(), static_cast<std::streamsize>(info.getFullHeaderSize()));
	}
	hdr.setStartPos(localHeaderOffset); // reset again now that compressed Size is known
	_offset = hdr.getEndPos();
	if (hdr.searchCRCAndSizesAfterData())
		_offset += ZipDataInfo::getFullHeaderSize();
	_files.insert(std::make_pair(fileName.toString(Poco::Path::PATH_UNIX), hdr));
	poco_assert (_out);
	ZipFileInfo nfo(hdr);
	nfo.setOffset(localHeaderOffset);
	_infos.insert(std::make_pair(fileName.toString(Poco::Path::PATH_UNIX), nfo));
	EDone.notify(this, hdr);
}


void Compress::addFile(std::istream& in, const Poco::DateTime& lastModifiedAt, const Poco::Path& fileName, ZipCommon::CompressionMethod cm, ZipCommon::CompressionLevel cl)
{
	if (!fileName.isFile())
		throw ZipException("Not a file: "+ fileName.toString());

	if (fileName.depth() > 1)
	{
		addDirectory(fileName.parent(), lastModifiedAt);
	}
	addEntry(in, lastModifiedAt, fileName, cm, cl);
}


void Compress::addFile(const Poco::Path& file, const Poco::Path& fileName, ZipCommon::CompressionMethod cm, ZipCommon::CompressionLevel cl)
{
	Poco::File aFile(file);
	std::ifstream in(file.toString().c_str(), std::ios::binary);
	if (!in.good())
		throw ZipException("Invalid input stream for " + aFile.path());
	if (fileName.depth() > 1)
	{
		Poco::File aParent(file.parent());
		addDirectory(fileName.parent(), aParent.getLastModified());
	}
	addFile(in, aFile.getLastModified(), fileName, cm, cl);
}


void Compress::addDirectory(const Poco::Path& entryName, const Poco::DateTime& lastModifiedAt)
{
	if (!entryName.isDirectory())
		throw ZipException("Not a directory: "+ entryName.toString());

	std::string fileStr = entryName.toString(Poco::Path::PATH_UNIX);
	if (_files.find(fileStr) != _files.end())
		return; // ignore duplicate add
	if (_files.size() >= 65535)
		throw ZipException("Maximum number of entries for a ZIP file reached: 65535");
	if (fileStr == "/")
		throw ZipException("Illegal entry name /");
	if (fileStr.empty())
		throw ZipException("Illegal empty entry name");
	if (fileStr.find(ZipCommon::ILLEGAL_PATH) != std::string::npos)
		throw ZipException("Illegal entry name " + fileStr + " containing " + ZipCommon::ILLEGAL_PATH);

	if (entryName.depth() > 1)
	{
		addDirectory(entryName.parent(), lastModifiedAt);
	}

	std::streamoff localHeaderOffset = _offset;
	ZipCommon::CompressionMethod cm = ZipCommon::CM_STORE;
	ZipCommon::CompressionLevel cl = ZipCommon::CL_NORMAL;
	ZipLocalFileHeader hdr(entryName, lastModifiedAt, cm, cl);
	hdr.setStartPos(localHeaderOffset);
	ZipOutputStream zipOut(_out, hdr, _seekableOut);
	zipOut.close();
	hdr.setStartPos(localHeaderOffset); // reset again now that compressed Size is known
	_offset = hdr.getEndPos();
	if (hdr.searchCRCAndSizesAfterData())
		_offset += ZipDataInfo::getFullHeaderSize();
	_files.insert(std::make_pair(entryName.toString(Poco::Path::PATH_UNIX), hdr));
	poco_assert (_out);
	ZipFileInfo nfo(hdr);
	nfo.setOffset(localHeaderOffset);
	_infos.insert(std::make_pair(entryName.toString(Poco::Path::PATH_UNIX), nfo));
	EDone.notify(this, hdr);
}


void Compress::addRecursive(const Poco::Path& entry, ZipCommon::CompressionLevel cl, bool excludeRoot, const Poco::Path& name)
{
	Poco::File aFile(entry);
	if (!aFile.isDirectory())
		throw ZipException("Not a directory: "+ entry.toString());
	Poco::Path aName(name);
	aName.makeDirectory();
	if (!excludeRoot)
	{
		if (aName.depth() == 0)
		{
			Poco::Path tmp(entry);
			tmp.makeAbsolute(); // eliminate ../
			aName = Poco::Path(tmp[tmp.depth()-1]);
			aName.makeDirectory();
		}

		addDirectory(aName, aFile.getLastModified());
	}

	// iterate over children
	std::vector<std::string> children;
	aFile.list(children);
	std::vector<std::string>::const_iterator it = children.begin();
	std::vector<std::string>::const_iterator itEnd = children.end();
	for (; it != itEnd; ++it)
	{
		Poco::Path realFile(entry, *it);
		Poco::Path renamedFile(aName, *it);
		Poco::File aFile(realFile);
		if (aFile.isDirectory())
		{
			realFile.makeDirectory();
			renamedFile.makeDirectory();
			addRecursive(realFile, cl, false, renamedFile);
		}
		else
		{
			realFile.makeFile();
			renamedFile.makeFile();
			addFile(realFile, renamedFile, ZipCommon::CM_DEFLATE, cl);
		}
	}
}


ZipArchive Compress::close()
{
	if (!_dirs.empty())
		return ZipArchive(_files, _infos, _dirs);

	poco_assert (_infos.size() == _files.size());
	poco_assert (_files.size() < 65536);
	Poco::UInt32 centralDirStart = _offset;
	Poco::UInt32 centralDirSize = 0;
	// write all infos
	ZipArchive::FileInfos::const_iterator it = _infos.begin();
	ZipArchive::FileInfos::const_iterator itEnd = _infos.end();
	for (; it != itEnd; ++it)
	{
		const ZipFileInfo& nfo = it->second;
		std::string info(nfo.createHeader());
		_out.write(info.c_str(), static_cast<std::streamsize>(info.size()));
		Poco::UInt32 entrySize = static_cast<Poco::UInt32>(info.size());
		centralDirSize += entrySize;
		_offset += entrySize;
	}
	poco_assert (_out);

	
	Poco::UInt16 numEntries = static_cast<Poco::UInt16>(_infos.size());
	ZipArchiveInfo central;
	central.setCentralDirectorySize(centralDirSize);
	central.setNumberOfEntries(numEntries);
	central.setTotalNumberOfEntries(numEntries);
	central.setHeaderOffset(centralDirStart);
	std::string centr(central.createHeader());
	_out.write(centr.c_str(), static_cast<std::streamsize>(centr.size()));
	_out.flush();
	_dirs.insert(std::make_pair(0, central));
	return ZipArchive(_files, _infos, _dirs);
}


} } // namespace Poco::Zip
