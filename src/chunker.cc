/*
 *  Copyright (C) 2005 IXA Research Group / IXA Ikerkuntza Taldea.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
 *  02110-1301  USA
 */

#include <lttoolbox/fst_processor.h>
#include <lttoolbox/ltstr.h>

#include <string>
#include <iostream>
#include <sstream>
#include <locale>

#include "config.h"
#include "matxin_string_utils.h"
#include "io_redirect_handler.h"

#include <data_manager.h>
#include <xml_reader.h>

using namespace std;

bool doTrace = false;

wstring procNODE(xmlTextReaderPtr reader, wstring parent_attribs)
{
  wstring nodes;
  wstring chunk_open=L"";
  wstring chunk_close=L"";
  wstring attribs;
  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);

  if (tagName == L"NODE" and tagType != XML_READER_TYPE_END_ELEMENT)
  {
    attribs = write_xml(allAttrib(reader));

    nodes = L"<NODE " +  write_xml(allAttrib_except(reader, L"si"));

    wstring chunktype = get_FromCondDict(attribs, parent_attribs);
    if (chunktype != L"-")
    {
      wstring ord = attrib(reader, "ord");
      wstring si = attrib(reader, "si");
      wstring alloc = attrib(reader, "alloc");

      chunk_open = L"<CHUNK ord='" + ord + L"' alloc='" + alloc + L"' type='" + chunktype + L"' si='" + si + L"'>\n";
      chunk_close = L"</CHUNK>\n";
    }

    if (xmlTextReaderIsEmptyElement(reader) == 1)
    {
      // NODE hutsa bada (<NODE .../>), NODE hutsa sortzen da eta
      // momentuko NODEarekin bukatzen dugu.
      nodes = chunk_open + nodes + L"/>\n" + chunk_close;
      return nodes;
    }
    else
    {
      // bestela NODE hasiera etiketaren bukaera idatzi eta
      // etiketa barrukoa tratatzera pasatzen gara.
      nodes += L">\n";
    }
  }
  else
  {
    wcerr << L"ERROR: invalid tag: <" << tagName
          << L"> when <NODE> was expected..." << endl;
    exit(-1);
  }

  int ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);

  // NODEaren azpian dauden NODE guztietarako
  while (ret == 1 and tagName == L"NODE" and
         tagType == XML_READER_TYPE_ELEMENT)
  {
    wstring child = procNODE(reader, attribs);
    nodes += child;

    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }

  // NODE bukaera etiketa tratatzen da.
  if (tagName == L"NODE" and tagType == XML_READER_TYPE_END_ELEMENT)
  {
    nodes += L"</NODE>\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </NODE> was expected..." << endl;
    exit(-1);
  }

  nodes = chunk_open + nodes + chunk_close;
  return nodes;
}

wstring procSENTENCE (xmlTextReaderPtr reader)
{
  wstring tree;
  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);

  if (tagName == L"SENTENCE" and tagType != XML_READER_TYPE_END_ELEMENT)
  {
    tree = L"<SENTENCE " + write_xml(allAttrib(reader)) + L">\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when <SENTENCE> was expected..." << endl;
    exit(-1);
  }

  int ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);

  // SENTENCE barruan dauden CHUNK guztietarako
  while (ret == 1 and tagName == L"NODE")
  {
    // NODEa irakurri eta prozesatzen du.
    tree += procNODE(reader, L"");

    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }

  if (ret == 1 and tagName == L"SENTENCE" and
      tagType == XML_READER_TYPE_END_ELEMENT)
  {
    tree += L"</SENTENCE>\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </SENTENCE> was expected..." << endl;
    exit(-1);
  }

  return tree;
}


int main(int argc, char *argv[])
{
  // This sets the C++ locale and affects to C and C++ locales.
  // wcout.imbue doesn't have any effect but the in/out streams use the proper encoding.
  locale::global(locale(""));

//        <file name="eng.chunking-rules.dat"/>


  // Add error checking 
  string Chunking_rules = string(argv[1]);
  init_condDict(Chunking_rules);

  while (true)
  {
    // redirect io
//    Fd0WcoutRedirectHandler ioredirect(cfg);

    // libXml liburutegiko reader hasieratzen da, sarrera estandarreko fitxategia irakurtzeko.
    xmlTextReaderPtr reader;
    reader = xmlReaderForFd(0, "", NULL, 0);
  
    int ret = nextTag(reader);
    wstring tagName = getTagName(reader);
    int tagType = xmlTextReaderNodeType(reader);
  
    if(tagName == L"corpus" and tagType != XML_READER_TYPE_END_ELEMENT)
    {
      wcout << L"<?xml version='1.0' encoding='UTF-8'?>" << endl;
      wcout << L"<corpus " << write_xml(allAttrib(reader)) << ">" << endl;
    }
    else
    {
      wcerr << L"ERROR: invalid document: found <" << tagName
            << L"> when <corpus> was expected..." << endl;
      exit(-1);
    }
  
    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  
    int i = 0;
    // corpus barruan dauden SENTENCE guztietarako
    while (ret == 1 and tagName == L"SENTENCE")
    {
      //SENTENCE irakurri eta prozesatzen du.
      wstring tree = procSENTENCE(reader);
      wcout << tree << endl;
      wcout.flush();
  
      if (doTrace)
      {
        ostringstream log_fileName_osoa;
        wofstream log_file;
  
	log_fileName_osoa.imbue(std::locale("C"));
        log_fileName_osoa << "/dev/stderr" << i++ << ".xml";
  
        log_file.open(log_fileName_osoa.str().c_str(),
                      wofstream::out | wofstream::app);
	log_file << L"<!-- Chunking -->" << endl;
        log_file << tree;
        log_file.close();
      }
  
      ret = nextTag(reader);
      tagName = getTagName(reader);
      tagType = xmlTextReaderNodeType(reader);
    }
    xmlFreeTextReader(reader);
    xmlCleanupParser();
  
    if(ret == 1 and tagName == L"corpus" and
       tagType == XML_READER_TYPE_END_ELEMENT)
    {
      wcout << L"</corpus>\n";
      break ;
    }
    else
    {
      wcerr << L"ERROR: invalid document: found <" << tagName
            << L"> when </corpus> was expected..." << endl;
      exit(-1);
    }
//    if (!ioredirect.serverOK()) {
//      break;
//    }
  }
}

