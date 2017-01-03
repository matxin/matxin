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

#include <clocale>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

#include <libxslt/transform.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>

#include "data_manager.h"
#include "matxin_string_utils.h"
#include "transfer.h"

using namespace std;

bool debug = false ;

void usage(char *name)
{
  wcout << L"" << name << L" <transfer file>" << endl ;
}

int main(int argc, char *argv[])
{
  // This sets the C++ locale and affects to C and C++ locales.
  // wcout.imbue doesn't have any effect but the in/out streams use the proper encoding.
  locale::global(locale(""));

  if(argc != 2)
  {
    usage(argv[0]);
    exit(-1);
  }

  struct rule_record {
    int id; 
    int linia;   // 
    double pisu; // el peso
    int rlen;
  };

  FILE *rin = fopen(argv[1], "rb");

  vector<xsltStylesheetPtr> cascade; 

  while(!feof(rin))
  {
    rule_record line = {0, 0, 0.0, 0} ; 
    fread(&line, sizeof(rule_record), 1, rin);
 
    if(line.rlen == 0)  
    {
      break;
    }

    if(debug) 
    {
      fwprintf(stderr, L"%d\t%d\tweight(%.4f)\tlen(%d)\n", line.id, line.linia, line.pisu, line.rlen);
    }
    
    void *regla = calloc(line.rlen+1, sizeof(wchar_t));
    int res = fread(regla, line.rlen, sizeof(wchar_t), rin);
    wstring wregla = header + wstring((wchar_t *)regla) + footer; 

    if(debug) 
    {
      wcerr << wregla << endl;
    }

    xmlDocPtr doc = NULL;
    string rule = wstos(wregla);
    doc = xmlReadMemory(rule.c_str(), rule.size(), "noname.xml", NULL, 0);
    xsltStylesheetPtr xsls = xsltParseStylesheetDoc(doc);
    if(xsls != NULL) 
    {
      cascade.push_back(xsls);
    }
    else
    {
      wcerr << L"Error loading rule " << line.id << endl; 
      wcerr << wregla << endl;
      exit(-1);
    }

    free(regla);
  }

  xmlDocPtr doc = NULL;
  doc = xmlReadFd(0, "/", NULL, 0);
  std::vector<xmlDocPtr> docs;

  for (vector<xsltStylesheetPtr>::iterator it = cascade.begin() ; it != cascade.end(); ++it)
  { 
    xmlDocPtr res = xsltApplyStylesheet(*it, doc, NULL);
    if(res == NULL)
    {
      wcerr << L"Error." << endl;
      break;
    }    

    docs.push_back(doc);
    doc = res;
  }

  xmlSubstituteEntitiesDefault(1);
  xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

//    buf = xmlBufferCreate();

  for (vector<xsltStylesheetPtr>::iterator it = cascade.begin() ; it != cascade.end(); ++it)
  {
    xsltFreeStylesheet(*it);
  } 

/*
  res = xsltApplyStylesheet(cur, doc, NULL);
  xsltSaveResultToFile(stdout, res, cur);
*/

  xmlFreeDoc(doc);

  for (std::vector<xmlDocPtr>::const_reverse_iterator it = docs.crbegin();
       it != docs.crend(); ++it) {
    xmlFreeDoc(*it);
  }

  xsltCleanupGlobals();
  xmlCleanupParser();
}

