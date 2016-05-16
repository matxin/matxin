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
#include <clocale>

#include <lttoolbox/fst_processor.h>
#include <lttoolbox/ltstr.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include "matxin_string_utils.h"
#include "transfer.h"

#include <data_manager.h>

using namespace std;

bool debug = false;
bool rawGen = false;

int nodeCount = 0;
int depth = 0;

void usage(char *name)
{
  wcout << L"" << name << L" [-g] <generation rules> <morphological generator>" << endl ;
}

void procAttr(xmlNodePtr p)
{
    xmlAttr* attr = p->properties;
    if(attr) 
    {
      wcout << L" ";
    }
    while(attr)
    {
      xmlChar* value = xmlNodeListGetString(p->doc, attr->children, 1);
      wstring val = towstring(value);
      wcout << towstring(attr->name) << L"=\"" << write_xml(val) << L"\"";
      xmlFree(value); 
      if(attr->next)
      {
        wcout << L" ";
      }
      attr = attr->next;
    }
    xmlFree(attr);
}

void procNode(FSTProcessor &fstp, xmlNodePtr p)
{
    while(p != NULL) 
    {
      if(p->type == 1)
      {
        bool sub = false;
        if(p->xmlChildrenNode)
        {
          sub = true;
        }
        wcout << L"<" << towstring(p->name); 
        if(!xmlStrcmp(p->name, (xmlChar *)"NODE")) 
        {
          nodeCount++;
          wstring mstr, form = L"";

          xmlChar *lem = xmlGetProp(p, (xmlChar *)"lem");
          xmlChar *smi = xmlGetProp(p, (xmlChar *)"smi");
          xmlChar *mi = xmlGetProp(p, (xmlChar *)"mi");
          if(mi != NULL && lem != NULL) 
          {
            mstr = L"^" + towstring(lem) + L"<mi>" + towstring(mi) + L"$";
            form = fstp.biltrans(mstr);
            form = form.substr(1, form.size() - 2);
            if(debug) 
            {
              wcerr << sub << L" " << depth << L"[" << nodeCount << L"]" << L"FORM: " << form << L"\t||\t"  << L"\t||\t" << towstring(lem) << L" " << towstring(mi) << endl;
            }
            if(form[0] == L'@') 
            {
              form = L"#" + towstring(lem) + L"|" + towstring(mi); 
            }
            xmlSetProp(p, (xmlChar *)"form", (xmlChar *)wstos(form).c_str());
          }
          else if(lem != NULL && mi == NULL && smi != NULL)  // No morphology in target
          {
            form = L"%" + towstring(lem) + L"|" + towstring(smi) ; 
            if(debug) 
            {
              wcerr << sub << L" " << depth << L"[" << nodeCount << L"]" << L"NOMI: " << form << L"\t||\t" << endl;
            } 
            xmlSetProp(p, (xmlChar *)"form", (xmlChar *)wstos(form).c_str());
          }
          else if(lem != NULL && mi == NULL && smi == NULL)  // No morphology in source or target
          {
            form = L"=" + towstring(lem); 
            if(debug) 
            {
              wcerr << sub << L" " << depth << L"[" << nodeCount << L"]" << L"NOMI2: " << form << L"\t||\t" << endl;
            } 
            xmlSetProp(p, (xmlChar *)"form", (xmlChar *)wstos(form).c_str());
          }
          else
          { 
            wstring err = L"";
            if(lem) err = err + towstring(lem);
            if(mi) err = err + towstring(mi);
            if(debug)
            {
              wcerr << sub << L" " << depth << L"[" << nodeCount << L"]" << L"FAIL: not received!\t||\t" << err << endl;
            }
          }
        }
        procAttr(p);
      }
      else if(p->type == 3)
      {
        wcout << towstring(p->content);
      }

      if(p->xmlChildrenNode)
      {
        wcout << L">";
        depth = depth + 1;
        procNode(fstp, p->xmlChildrenNode);
        depth = depth - 1;
        wstring name = towstring(p->name);
        wcout << L"</" << name << L">";
      }
      else if(p->type == 1)
      {
        wcout << L"/>";
      }

      p = p->next;
    }
 

    return;
}


int main(int argc, char *argv[])
{
  FSTProcessor fstp;

  // This sets the C++ locale and affects to C and C++ locales.
  // wcout.imbue doesn't have any effect but the in/out streams use the proper encoding.
  locale::global(locale(""));

  string genRulesFile; 
  string morphGenFile;

  if(argc == 3)
  {
    genRulesFile = string(argv[1]);
    morphGenFile = string(argv[2]);
  }
  else if(argc == 4) 
  {
    string opt = string(argv[1]); 
    if(opt != "-g") 
    {
      usage(argv[0]);
      exit(-1);
    }
    rawGen = true;
    genRulesFile = string(argv[2]);
    morphGenFile = string(argv[3]);
  }
  else
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


  FILE *transducer = fopen(morphGenFile.c_str(), "r");
  fstp.load(transducer);
  fclose(transducer);
  fstp.initBiltrans();

  FILE *rin = fopen(genRulesFile.c_str(), "rb");

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
    doc = xmlReadMemory(rule.c_str(), line.rlen*sizeof(wchar_t), "noname.xml", NULL, 0);
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

    //free(regla);
  }

  xmlDocPtr doc, res = NULL;
  doc = xmlReadFd(0, "/", NULL, 0);

  if(doc == NULL) 
  {
    wcerr << L"Error reading document." << endl ;
    exit(-1);
  }

  const char *params[16 + 1];
  int nbparams = 0;
  params[nbparams] = NULL;

  res = doc; 
  xsltStylesheetPtr last = NULL;
  int rcount = 0;
  for (vector<xsltStylesheetPtr>::iterator it = cascade.begin() ; it != cascade.end(); ++it)
  { 
    res = xsltApplyStylesheet(*it, res, params);

    if(res == NULL)
    {
      wcerr << L"Error." << endl;
      exit(-1);
    }    

    if(debug) 
    {
      wcerr << L"[" << rcount << L"]==========================================================================" << endl;
      xmlSaveFormatFileEnc("/dev/stderr", res, "UTF-8", 1);
      FILE *err = stderr;
      fflush(err);
      wcerr << L"[" << rcount << L"]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }

    xsltFreeStylesheet(*it);
    rcount++;
  }

  // now generate 

  xmlChar *sentences = (xmlChar*)"//SENTENCE" ;

  xmlXPathContextPtr context = xmlXPathNewContext(res);
  xmlXPathObjectPtr rres = xmlXPathEvalExpression(sentences, context);
  xmlNodeSetPtr nodes = rres->nodesetval;

  int size = (rres->nodesetval) ? nodes->nodeNr : 0;

  xmlSubstituteEntitiesDefault(1);

  wcout << L"<corpus>" << endl;

  xmlNodePtr cur;
  for(int i = 0; i < size; i++) 
  {
    cur = nodes->nodeTab[i];
    nodeCount = 0;

    wcout << L"<" << towstring(cur->name);
    procAttr(cur);
    if(cur->xmlChildrenNode) 
    {
      wcout << L">";
      procNode(fstp, cur->xmlChildrenNode);
      wcout << L"</" << towstring(cur->name) << L">";
    }
    else
    {
      wcout << L"/>" << endl; 
    }

    cur = cur->next;
  }

  wcout << L"</corpus>" << endl;
//  wcerr << nodeCount << endl;

//  xmlFreeDoc(doc);
  xmlFreeDoc(res);

  xsltCleanupGlobals();
  xmlCleanupParser();
}

