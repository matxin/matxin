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

#include <string>
#include <iostream>
#include <sstream>
#include <locale>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "matxin_string_utils.h"

#include <data_manager.h>


using namespace std;

bool doTrace = false ;
int depth = 0;

void procAttr(xmlNodePtr p, wstring &res)
{
    xmlAttr* attr = p->properties;
    if(attr) 
    {
      res = res + L" ";
    }
    while(attr)
    {
      xmlChar* value = xmlNodeListGetString(p->doc, attr->children, 1);
      res = res + towstring(attr->name) + L"=\"" + towstring(value) + L"\"";
      xmlFree(value); 
      if(attr->next)
      {
        res = res + L" ";
      }
      attr = attr->next;
    }
    xmlFree(attr);
}

wstring full(wstring &s)
{
    if(s == L"attr")
    { 
      return L"xsl:attribute";
    }
    else if(s == L"elem")
    { 
      return L"xsl:element";
    }
    else if(s == L"NODE" or s == L"CHUNK")
    {
      return s;
    }
    else
    {
      return L"xsl:" + s;
    } 
}

void procNode(xmlNodePtr p, wstring &res) 
{
    depth = depth + 1;

    while(p != NULL) 
    {
      if(p->type == 1)
      {
        wstring name = towstring(p->name);
        res = res + L"<" + full(name); 
        procAttr(p, res);
      }
      else if(p->type == 3)
      {
        res = res + towstring(p->content);
      }

      if(p->xmlChildrenNode)
      {
        res = res + L">";
        procNode(p->xmlChildrenNode, res);
        wstring name = towstring(p->name);
        res = res + L"</" + full(name) + L">";
      }
      else if(p->type == 1)
      {
        res = res + L"/>";
      }

      p = p->next;
    }
 
    depth = depth - 1;

    return;
}


void usage(char *name)
{
  wcout << L"" << name << L" <transfer file> <output file>" << endl ;
}

int main(int argc, char *argv[])
{
  // This sets the C++ locale and affects to C and C++ locales.
  // wcout.imbue doesn't have any effect but the in/out streams use the proper encoding.
  locale::global(locale(""));

  if(argc != 3)
  {
    usage(argv[0]);
    exit(-1);
  }

  string transferFile = string(argv[1]);

  FILE *output = fopen(argv[2], "wb");

  xmlDocPtr doc = NULL;

  xmlChar *findRules = (xmlChar*)"//def-rule" ;

  doc = xmlReadFile(transferFile.c_str(), "", 0);
  xmlSubstituteEntitiesDefault(1);

  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr res = xmlXPathEvalExpression(findRules, context);
  xmlNodeSetPtr nodes = res->nodesetval;

  int size = (res->nodesetval) ? nodes->nodeNr : 0;

  struct rule_record {
    int id; 
    int linia;   // 
    double pisu; // el peso
    int rlen;
  };
 
  xmlNodePtr cur;
  for(int i = 0; i < size; ++i) 
  {
    cur = nodes->nodeTab[i];
    
    wstring res = L"";
    int rline = xmlGetLineNo(cur);
    wcout << L"<" << towstring(cur->name);
    //procAttr(cur, res);
    if(cur->xmlChildrenNode) 
    {
      wcout << L">";
      procNode(cur->xmlChildrenNode, res);
      wcout << L"</" << towstring(cur->name) << L">";
    }
    else
    {
      wcout << L"/>"; 
    }

    wcerr << res << endl;
    int rid = i + 1;
    rule_record line = {
         rid, 
         rline, 
         1.0,                 // default weight
         wcslen(res.c_str())
    };
 
    void *regla = calloc(line.rlen + 1, sizeof(wchar_t));
    regla = (void *)res.c_str();

    fwrite((void *)&line, 1, sizeof(rule_record), output);
    fwrite(regla, sizeof(wchar_t), line.rlen, output);

    cur = cur->next;
  }
    
  wcerr << L"Result (" << size << L")" << endl ;

  xmlXPathFreeContext(context); 
  xmlFreeDoc(doc);

  xsltCleanupGlobals();
  xmlCleanupParser();
  fclose(output);
}

