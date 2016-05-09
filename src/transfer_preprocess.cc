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

void procAttr(xmlNodePtr p, string &res)
{
    xmlAttr* attr = p->properties;
    if(attr) 
    {
      wcout << L" ";
    }
    while(attr)
    {
      xmlChar* value = xmlNodeListGetString(p->doc, attr->children, 1);
      wcout << towstring(attr->name) << L"=\"" << towstring(value) << L"\"";
      xmlFree(value); 
      if(attr->next)
      {
        wcout << L" ";
      }
      attr = attr->next;
    }
    xmlFree(attr);
}

void procNode(xmlNodePtr p, string &res) 
{
    depth = depth + 1;

    while(p != NULL) 
    {

      if(p->type == 1)
      {
        wcout << L"<" << towstring(p->name);
        procAttr(p, res);
      }
      else if(p->type == 3)
      {
        wcout << towstring(p->content);
      }

      if(p->xmlChildrenNode)
      {
        wcout << L">";
        procNode(p->xmlChildrenNode, res);
        wcout << L"</" << towstring(p->name) << L">";
      }
      else if(p->type == 1)
      {
        wcout << L"/>"; 
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

  vector<string> rules;

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
    string regla;  // la regla

 
  xmlNodePtr cur;
  for(int i = 0; i < size; ++i) 
  {
    cur = nodes->nodeTab[i];
    
    string res;
    rule_record line = {
         i, 
         xmlGetLineNo(cur), 
         0.0, 
         0
    };
    wcout << L"<" << towstring(cur->name);
    procAttr(cur, res);
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

    fwrite((void *)&line, 1, sizeof(rule_record), output);

    cur = cur->next;
  }
    
  wcerr << L"Result (" << size << L")" << endl ;

  xmlXPathFreeContext(context); 
  xmlFreeDoc(doc);

  xsltCleanupGlobals();
  xmlCleanupParser();
  fclose(output);
}

