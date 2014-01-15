/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2013  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the Sinsy working group nor the names of    */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include <stdexcept>
#include <algorithm>
#include <fstream>
#include "util_log.h"
#include "StringTokenizer.h"
#include "Deleter.h"
#include "ConfManager.h"
#include "ConfGroup.h"
#include "util_converter.h"
#include "JConf.h"

using namespace sinsy;

namespace
{
const std::string CODE_SEPARATOR = "|";
};

std::string const ConfManager::UTF_8_STRS = std::string("utf_8, utf8, utf-8");
std::string const ConfManager::SHIFT_JIS_STRS = std::string("shift_jis, shift-jis, sjis");
std::string const ConfManager::EUC_JP_STRS = std::string("euc-jp, euc_jp, eucjp");

/*!
 constructor
 */
ConfManager::ConfManager() : uJConf(NULL), sJConf(NULL), eJConf(NULL), jConfs(NULL)
{
}

/*!
 destructor
*/
ConfManager::~ConfManager()
{
   clear();
}

/*!
 @internal

 clear all confs
*/
void ConfManager::clear()
{
   std::for_each(deleteList.begin(), deleteList.end(), Deleter<IConf>());
   deleteList.clear();
   uJConf = NULL;
   sJConf = NULL;
   eJConf = NULL;
   jConfs = NULL;
   confList.clear();
}

/*!
 @internal

 add Japanese conf
 */
void ConfManager::addJConf(IConf* conf)
{
   if (!jConfs) {
      jConfs = new ConfGroup();
      deleteList.push_back(jConfs);
      confList.push_back(jConfs);
   }
   jConfs->add(conf);
}

void ConfManager::setJapaneseUTF8Conf(JConf * conf)
{
   uJConf = conf;
   addJConf(uJConf);
   deleteList.push_back(uJConf);
}

void ConfManager::setJapaneseShiftJISConf(JConf * conf)
{
   sJConf = conf;
   addJConf(sJConf);
   deleteList.push_back(sJConf);
}

void ConfManager::setJapaneseEUCJPConf(JConf * conf)
{
   eJConf = conf;
   addJConf(eJConf);
   deleteList.push_back(eJConf);
}

/*!
 set languages
 (Currently, you can set only Japanese (j))
 */
bool ConfManager::setLanguages(const std::string& languages, const std::string& dirPath)
{
   clear();

   const std::string::const_iterator itrEnd(languages.end());
   for (std::string::const_iterator itr(languages.begin()); itrEnd != itr; ++itr) {
      char lang(*itr);
      switch (lang) {
      case 'j' : { // Japanese
         const std::string TABLE_UTF_8(dirPath + "/japanese.utf_8.table");
         const std::string CONF_UTF_8(dirPath + "/japanese.utf_8.conf");
         const std::string TABLE_SHIFT_JIS(dirPath + "/japanese.shift_jis.table");
         const std::string CONF_SHIFT_JIS(dirPath + "/japanese.shift_jis.conf");
         const std::string TABLE_EUC_JP(dirPath + "/japanese.euc_jp.table");
         const std::string CONF_EUC_JP(dirPath + "/japanese.euc_jp.conf");
         const std::string MACRON_TABLE(dirPath + "/japanese.macron");

         // utf-8
         {
            std::unique_ptr<JConf> jconf(new JConf(UTF_8_STRS));
            std::ifstream table(TABLE_UTF_8.c_str());
            std::ifstream conf(CONF_UTF_8.c_str());
            std::ifstream macron(MACRON_TABLE.c_str());
            if (jconf->read(table, conf, macron)) {
               setJapaneseUTF8Conf(jconf.release());
            } else {
               WARN_MSG("Cannot read Japanese table or config or macron file : " << TABLE_UTF_8 << ", " << CONF_UTF_8);
            }
         }

         // shift_jis
         {
            std::unique_ptr<JConf> jconf(new JConf(SHIFT_JIS_STRS));
            std::ifstream table(TABLE_SHIFT_JIS.c_str());
            std::ifstream conf(CONF_SHIFT_JIS.c_str());
            std::ifstream macron(MACRON_TABLE.c_str());
            if (jconf->read(table, conf, macron)) {
               setJapaneseShiftJISConf(jconf.release());
            } else {
               WARN_MSG("Cannot read Japanese table or config or macron file :" << TABLE_SHIFT_JIS << ", " << CONF_SHIFT_JIS);
            }
         }

         // euc-jp
         {
            std::unique_ptr<JConf> jconf(new JConf(EUC_JP_STRS));
            std::ifstream table(TABLE_EUC_JP.c_str());
            std::ifstream conf(CONF_EUC_JP.c_str());
            std::ifstream macron(MACRON_TABLE.c_str());
            if (jconf->read(table, conf, macron)) {
               setJapaneseEUCJPConf(jconf.release());
            } else {
               WARN_MSG("Cannot read Japanese table or config or macron file : " << TABLE_EUC_JP << ", " << CONF_EUC_JP);
            }
         }
         break;
      }
      default :
         ERR_MSG("Unknown language flag : " << lang);
         return false;
      }
   }
   return true;
}

/*!
 set default confs
 */
void ConfManager::setDefaultConfs(ConfGroup& confs) const
{
   confs.clear();
   const ConfList::const_iterator itrEnd(confList.end());
   for (ConfList::const_iterator itr(confList.begin()); itrEnd != itr ; ++itr) {
      confs.add(*itr);
   }
   // unknown conf
   confs.add(&uConf);

   return;
}
