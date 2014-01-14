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
#include <limits>
#include <deque>
#include <vector>
#include <fstream>
#include "util_log.h"
#include "util_string.h"
#include "util_converter.h"
#include "StringTokenizer.h"
#include "JConf.h"
#include "Deleter.h"

using namespace sinsy;

namespace
{
const std::string SIL_STR = "sil";
const std::string SEPARATOR = ",";
const std::string LANGUAGE_INFO = "JPN";
const std::string MACRON = "MACRON";
const std::string VOWEL_REDUCTION = "VOWEL_REDUCTION";
const std::string PHONEME_CL = "PHONEME_CL";
const std::string VOWELS = "VOWELS";
const std::string MULTIBYTE_CHAR_RANGE = "MULTIBYTE_CHAR_RANGE";
const size_t INVALID_IDX = std::numeric_limits<size_t>::max();
const std::string DEFAULT_VOWELS = "a,i,u,e,o,N";
const std::string PHONEME_SEPARATOR = ",";

class PhonemeJudge
{
public:
   //! constructor
   PhonemeJudge(const std::string& v, const std::string& b) {
      {
         StringTokenizer st(v, PHONEME_SEPARATOR);
         size_t sz(st.size());
         for (size_t i(0); i < sz; ++i) {
            std::string phoneme(st.at(i));
            cutBlanks(phoneme);
            if (!phoneme.empty()) {
               this->vowels.insert(phoneme);
            }
         }
      }
      {
         StringTokenizer st(b, PHONEME_SEPARATOR);
         size_t sz(st.size());
         for (size_t i(0); i < sz; ++i) {
            std::string phoneme(st.at(i));
            cutBlanks(phoneme);
            if (!phoneme.empty()) {
               this->breaks.insert(phoneme);
            }
         }
      }
   }

   //! destructor
   virtual ~PhonemeJudge() {}

   //! return whether vowel or not
   const std::string& getType(const std::string& phoneme) const {
      if (vowels.end() != vowels.find(phoneme)) {
         return PhonemeInfo::TYPE_VOWEL;
      }
      if (breaks.end() != breaks.find(phoneme)) {
         return PhonemeInfo::TYPE_BREAK;
      }
      return PhonemeInfo::TYPE_CONSONANT;
   }

private:
   //! copy constructor (donot use)
   PhonemeJudge(const PhonemeJudge&);

   //! assignment operator (donot use)
   PhonemeJudge& operator=(const PhonemeJudge&);

   //! vowels
   std::set<std::string> vowels;

   //! breaks such as /cl/
   std::set<std::string> breaks;
};

class InfoAdder
{
public:
   //! constructor
   InfoAdder(sinsy::IConvertable& c, const std::string& cl, const PhonemeJudge& pj) :
      convertable(c), clPhoneme(cl), phonemeJudge(pj), waiting(false), vowelReductionIdx(INVALID_IDX), enableFlag(true), falsettoFlag(false), macronFlag(false) {
   }

   //! destructor
   virtual ~InfoAdder() {
      reflect();
   }

   //! set enable flag
   void setEnableFlag(bool e) {
      enableFlag = e;
   }

   //! set falsetto flag
   void setFalsettoFlag(bool f) {
      falsettoFlag = f;
   }

   //! set macron flag
   void setMacronFlag(bool f) {
      macronFlag = f;
   }

   //! add syllable
   void addSyllable(const PhonemeTable::PhonemeList& p, bool vowelReductionFlag) {
      if (p.empty()) { // fail safe
         WARN_MSG("Cannot add Japanese syllable : no phonemes");
         return;
      }
      bool clFlag = ((1 == p.size()) && (clPhoneme == p[0])) ? true : false;

      if (clFlag) { // cl
         if (pList.empty()) { // first time
            pList.push_back(new PhonemeTable::PhonemeList(p));
            waiting = true;
         } else if (pList.back()->back() != clPhoneme) { // over second time, and not following cl
            pList.back()->push_back(clPhoneme);
         }
      } else { // not cl
         if (waiting) { // previous syllable has vowel reduction
            PhonemeTable::PhonemeList* prevPhonemes(pList.back());
            if (INVALID_IDX != vowelReductionIdx) {
               prevPhonemes->erase(prevPhonemes->begin() + vowelReductionIdx);
            }
            std::copy(p.begin(), p.end(), std::back_inserter(*prevPhonemes));
            waiting = false;
            vowelReductionIdx = INVALID_IDX;
         } else {
            pList.push_back(new PhonemeTable::PhonemeList(p));
         }
      }

      if (vowelReductionFlag) {
         if (1 == p.size()) { // vowels, N or cl
            WARN_MSG("Vowel reduction symbol was ignored : only one  phoneme \"" << p[0] << "\"");
         } else {
            waiting = true;
            vowelReductionIdx = pList.back()->size() - 1; // last phoneme ( = vowel)
         }
      }
   }

   //! get phonemes of last syllable
   const PhonemeTable::PhonemeList* getLastPhonemes() const {
      if (pList.empty()) {
         return NULL;
      }
      return pList.back();
   }

   //! get phonemes of last syllable
   PhonemeTable::PhonemeList* getLastPhonemes() {
      if (pList.empty()) {
         return NULL;
      }
      return pList.back();
   }

private:
   //! copy constructor (donot use)
   InfoAdder(const InfoAdder&);

   //! assignment operator (donot use)
   InfoAdder& operator=(const InfoAdder&);

   //! reflect to convertable
   void reflect() {
      if (pList.empty()) return;

      // last syllable has silent vowel
      if (waiting) {
         if (pList.size() <= 1) {
            WARN_MSG("Syllable that has vowel reductions needs previous or next syllable");
         } else {
            PhonemeTable::PhonemeList* lastPhonemes(pList.back());
            pList.pop_back();
            if (INVALID_IDX != vowelReductionIdx) {
               lastPhonemes->erase(lastPhonemes->begin() + vowelReductionIdx);
            }
            std::copy(lastPhonemes->begin(), lastPhonemes->end(), std::back_inserter(*(pList.back())));
            delete lastPhonemes;
         }
         waiting = false;
         vowelReductionIdx = INVALID_IDX;
      }

      // add
      {
         std::string info = (macronFlag) ? "1" : "0";
         std::string lastPhoneme;
         std::vector<PhonemeTable::PhonemeList*>::iterator itr(pList.begin());
         const std::vector<PhonemeTable::PhonemeList*>::iterator itrEnd(pList.end());
         for (; itrEnd != itr; ++itr) {
            const PhonemeTable::PhonemeList& phonemes(**itr);
            if (phonemes.empty()) { // fail safe
               continue;
            }

            // same vowel
            if ((1 == phonemes.size()) && (phonemes[0] == lastPhoneme)) {
               continue;
            }

            std::vector<PhonemeInfo> phonemeInfos;
            phonemeInfos.reserve(phonemes.size());
            const std::vector<std::string>::const_iterator itrEnd(phonemes.end());
            for (std::vector<std::string>::const_iterator itr(phonemes.begin()); itrEnd != itr; ++itr) {
               const std::string& type(phonemeJudge.getType(*itr));
               phonemeInfos.push_back(PhonemeInfo(type, *itr, enableFlag, falsettoFlag));
            }
            convertable.addInfo(phonemeInfos, LANGUAGE_INFO, info);
            lastPhoneme = phonemes.back();
         }
      }

      // clear
      std::for_each(pList.begin(), pList.end(), Deleter<PhonemeTable::PhonemeList>());
      pList.clear();
   }

   //! target
   sinsy::IConvertable& convertable;

   //! phoneme of cl
   const std::string clPhoneme;

   //! phoneme type judge
   const PhonemeJudge& phonemeJudge;

   //! waiting flag
   bool waiting;

   //! index of vowel reduction
   size_t vowelReductionIdx;

   //! enable flag
   bool enableFlag;

   //! falsetto flag
   bool falsettoFlag;

   //! macron flag
   bool macronFlag;

   //! phoneme list
   std::vector<PhonemeTable::PhonemeList*> pList;
};

/*!
 convert string of char code to char
 */
bool str2char(const std::string& s, unsigned char& c)
{
   int tmp(-1);
   if ((2 < s.size()) && ('0' == s[0]) && ('x' == s[1])) {
      std::string sub(s.substr(2));
      std::istringstream iss(sub);
      iss >> std::hex >> tmp;
   } else {
      std::istringstream iss(s);
      iss >> tmp;
   }
   if ((tmp < 0) || (std::numeric_limits<unsigned char>::max() < tmp)) {
      ERR_MSG("Config of multibyte char range is wrong format: " << s);
      return false;
   }
   c = static_cast<unsigned char>(tmp);
   return true;
}

/*!
 set multibyte char range from str to mRange
 */
bool setMultibyteCharRange(MultibyteCharRange& mRange, const std::string& str)
{
   bool ret(true);
   StringTokenizer st1(str, ";");
   size_t size1(st1.size());
   for (size_t i(0); i < size1; ++i) {
      StringTokenizer st2(st1.at(i), ",");
      size_t size2(st2.size());
      if (3 != size2) {
         ERR_MSG("Config of multibyte char range is wrong format: " << str);
         return false;
      }

      size_t size(0);
      unsigned char begin(0), end(0);
      {
         std::istringstream iss(st2.at(0));
         iss >> size;
      }
      if (!str2char(st2.at(1), begin)) {
         ret = false;
         continue;
         // don't return false here to set other ranges
      }
      if (!str2char(st2.at(2), end)) {
         ret = false;
         continue;
         // don't return false here to set other ranges
      }

      if (false == mRange.addRange(size, begin, end)) {
         ret = false;
         continue;
         // don't return false here to set other ranges
      }
   }
   return ret;
}

/*!
 expand prevInfoAdder to infoAdder
 */
bool expand(InfoAdder& prevInfoAdder, InfoAdder& infoAdder, const MacronTable& macronTable, const std::string& clSymbol)
{
   if (NULL != infoAdder.getLastPhonemes()) { // fail safe
      ERR_MSG("Dst InfoAdder is not empty (Source code is wrong)");
      return false;
   }

   PhonemeTable::PhonemeList* prevPhonemes(prevInfoAdder.getLastPhonemes());
   if (!prevPhonemes) {
      return false;
   }
   PhonemeTable::PhonemeList dst1;
   PhonemeTable::PhonemeList dst2;
   if (macronTable.divide(*prevPhonemes, dst1, dst2)) {
      *prevPhonemes = dst1;
      infoAdder.addSyllable(dst2, false);
   } else {
      if (clSymbol == prevPhonemes->back()) { // "cl"
         WARN_MSG("Macron cannot follow double consonant(cl)");

         // retry after erasing "cl" from phoneme list of previous InfoAdder
         prevPhonemes->pop_back();
         if (!expand(prevInfoAdder, infoAdder, macronTable, clSymbol)) {
            return false;
         }
         // add "cl"
         infoAdder.getLastPhonemes()->push_back(clSymbol);
      } else { // not "cl"
         dst2.push_back(prevPhonemes->back());
         infoAdder.addSyllable(dst2, false);
      }
   }
   return true;
}

};

/*!
 constructor

 @param enc encoding strings (e.g. "utf_8, utf8, utf-8")
 */
JConf::JConf(const std::string& enc)
{
   StringTokenizer tokeizer(enc, SEPARATOR);
   size_t sz(tokeizer.size());

   for (size_t i(0); i < sz; ++i) {
      std::string e(tokeizer.at(i));
      cutBlanks(e);
      toLower(e);
      encodings.insert(e);
   }
}

/*!
 destructor
*/
JConf::~JConf()
{
}

/*!
 read phoneme table and config from files

 @param table phoneme table file path
 @param conf  config file path
 @return true if success
 */
bool JConf::read(const std::string& table, const std::string& conf, const std::string& macron)
{
   std::ifstream table_stream(table.c_str());
   if (!phonemeTable.read(table_stream)) {
      ERR_MSG("Cannot read phoneme table file : " << table);
      return false;
   }

   std::ifstream conf_stream(conf.c_str());
   if (!config.read(conf_stream)) {
      ERR_MSG("Cannot read config file : " << conf);
      phonemeTable.clear();
      return false;
   }

   std::ifstream macron_stream(macron.c_str());
   if (!macronTable.read(macron_stream)) {
      ERR_MSG("Cannot read macron table file : " << macron);
      macronTable.clear();
      return false;
   }

   // set multibyte char ranges
   std::string strCharRange(config.get(MULTIBYTE_CHAR_RANGE));
   if (!setMultibyteCharRange(multibyteCharRange, strCharRange)) {
      ERR_MSG("Failed to set multibyte char ranges");
      return false;
   }

   return true;
}

/*!
 convert lyrics to phonemes
*/
bool JConf::convert(const std::string& enc, ConvertableList::iterator begin, ConvertableList::iterator end) const
{
   // check encoding
   if (!checkEncoding(enc)) {
      return true; // no relation
   }

   const std::string macronSymbol(config.get(MACRON));
   const std::string clSymbol(config.get(PHONEME_CL));
   const std::string vowelReductionSymbol(config.get(VOWEL_REDUCTION));
   std::string vowels(config.get(VOWELS));

   if (vowels.empty()) {
      vowels = DEFAULT_VOWELS;
   }

   PhonemeJudge phonemeJudge(vowels, clSymbol);

   std::vector<InfoAdder*> infoAdderList;

   for (ConvertableList::iterator itr(begin); itr != end; ++itr) {
      IConvertable& convertable(**itr);

      InfoAdder* infoAdder = new InfoAdder(**itr, clSymbol, phonemeJudge);

      std::string lyric(convertable.getLyric());

      size_t pos = std::string::npos;

      while (std::string::npos != (pos = lyric.find(DISABLE_CHAR))) {
         infoAdder->setEnableFlag(false);
         lyric.erase(pos, 1);
      }
      while (std::string::npos != (pos = lyric.find(FALSETTO_CHAR))) {
         infoAdder->setFalsettoFlag(true);
         lyric.erase(pos, 1);
      }
      while (!lyric.empty()) {
         if (!vowelReductionSymbol.empty() && (0 == lyric.compare(0, vowelReductionSymbol.size(), vowelReductionSymbol))) { // vowel reduction
            WARN_MSG("Vowel reduction symbol appeared at the invalid place");
            lyric.erase(0, vowelReductionSymbol.size());
         } else if (0 == lyric.compare(0, macronSymbol.size(), macronSymbol)) { // macron
            if (NULL != infoAdder->getLastPhonemes()) {
               // do nothing
            } else if (infoAdderList.empty()) {
               WARN_MSG("Macron have to follow another lyric");
            } else {
               expand(*(infoAdderList.back()), *infoAdder, macronTable, clSymbol);
            }
            infoAdder->setMacronFlag(true);
            lyric.erase(0, macronSymbol.size());
         } else { // others
            PhonemeTable::Result result(phonemeTable.find(lyric));
            if (!result.isValid()) {
               break;
            }
            lyric.erase(0, result.getMatchedLength());
            const PhonemeTable::PhonemeList* phonemes(result.getPhonemeList());

            //  vowel reduction symbol
            bool vl = false;
            if (!vowelReductionSymbol.empty() && (0 == lyric.compare(0, vowelReductionSymbol.size(), vowelReductionSymbol))) { // vowel reduction
               vl = true;
               lyric.erase(0, vowelReductionSymbol.size());
            }

            // cl
            if (!clSymbol.empty() && (1 == phonemes->size()) && (clSymbol == (*phonemes)[0])) {
               if (NULL == infoAdder->getLastPhonemes()) { // first phoneme in this note
                  std::string l(lyric);
                  while (std::string::npos != (pos = l.find(vowelReductionSymbol))) { // erase vowel reduction symbols
                     l.erase(pos, vowelReductionSymbol.size());
                  }
                  while (std::string::npos != (pos = l.find(macronSymbol))) { // erase macrons
                     l.erase(pos, macronSymbol.size());
                  }
                  if (l.empty()) { // only cl
                     if (infoAdderList.empty()) {
                        WARN_MSG("If there is only a phoneme \"cl\" in a note, \"cl\" have to follow vowel");
                     } else {
                        expand(*(infoAdderList.back()), *infoAdder, macronTable, clSymbol);
                     }
                  }
               }
            }
            infoAdder->addSyllable(*phonemes, vl);
         }
      }
      infoAdderList.push_back(infoAdder);
   }

   // clear list of InfoAdder
   std::for_each(infoAdderList.begin(), infoAdderList.end(), Deleter<InfoAdder>());
   infoAdderList.clear();

   return true;
}

/*!
 get sil string

 return sil str
 */
std::string JConf::getSilStr() const
{
   return SIL_STR;
}

/*!
 check encoding
 */
bool JConf::checkEncoding(const std::string& enc) const
{
   std::string encoding(enc);
   toLower(encoding);
   Encodings::const_iterator itr(encodings.find(encoding));
   return encodings.end() != itr;
}

/*!
 get multibyte char range
 */
const MultibyteCharRange& JConf::getMultibyteCharRange() const
{
   return multibyteCharRange;
}
