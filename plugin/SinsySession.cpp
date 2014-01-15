#include <cassert>
#include <algorithm>
#include <cadencii-nt/vsq/NoteNumberUtil.hpp>
#include "../src/lib/label/LabelMaker.h"
#include "../src/lib/label/LabelStrings.h"
#include "../src/lib/score/util_score.h"
#include "../src/lib/japanese/JConf.h"
#include "SinsySession.hpp"

extern "C" char const* binary_japanese_euc_jp_conf_start;
extern "C" char const* binary_japanese_euc_jp_conf_end;
extern "C" char const* binary_japanese_euc_jp_table_start;
extern "C" char const* binary_japanese_euc_jp_table_end;
extern "C" char const* binary_japanese_macron_start;
extern "C" char const* binary_japanese_macron_end;
extern "C" char const* binary_japanese_shift_jis_conf_start;
extern "C" char const* binary_japanese_shift_jis_conf_end;
extern "C" char const* binary_japanese_shift_jis_table_start;
extern "C" char const* binary_japanese_shift_jis_table_end;
extern "C" char const* binary_japanese_utf_8_conf_start;
extern "C" char const* binary_japanese_utf_8_conf_end;
extern "C" char const* binary_japanese_utf_8_table_start;
extern "C" char const* binary_japanese_utf_8_table_end;
extern "C" char const* binary_japanese_macron_start;
extern "C" char const* binary_japanese_macron_end;

static void init_stream(std::stringstream & stream, char const* from, char const* to)
{
    std::stringstream temp;
    size_t const count = (to - from) / sizeof(char);
    temp.write(from, count);
    temp.flush();
    temp.seekg(0);
    stream.swap(temp);
}

namespace cadencii {
namespace plugin {

double const SinsySession::MIN_SECONDS_BETWEEN_SESSIONS_ = 1.0;

struct SinsySession::Impl
{
    typedef cadencii::singing::tick_t tick_t;
    typedef std::shared_ptr<cadencii::singing::IScoreEvent> event_t;
    typedef std::multimap<tick_t, event_t> event_map_t;

public:
    //!
    //! \brief Impl     Create first session.
    //! \param provider
    //! \param sample_rate
    //! \param default_tempo
    //! \param voices
    //! \param language
    //! \param dictionary_path
    //!
    Impl(cadencii::singing::IScoreProvider * provider,
         int sample_rate,
         double default_tempo,
         std::vector<std::string> const& voices,
         std::string const& language,
         std::string const& dictionary_path)
        : language_(language)
        , dictionary_path_(dictionary_path)
        , voices_(voices)
        , sample_rate_(sample_rate)
        , provider_(provider)
        , session_samples_(0)
        , total_taken_samples_(0)
    {
        using namespace cadencii::singing;

        engine_.load(voices_);

        event_map_t empty;
        event_map_t score_source = takeEvents(provider, empty, default_tempo);
        initWithEvents(score_source);
    }

    //!
    //! \brief Impl     Initialize not-first session.
    //! \param provider
    //! \param sample_rate
    //! \param language
    //! \param dictionary_path
    //! \param voices
    //!
    Impl(cadencii::singing::IScoreProvider * provider,
         int sample_rate,
         std::string const& language,
         std::string const& dictionary_path,
         std::vector<std::string> const& voices)
        : language_(language)
        , dictionary_path_(dictionary_path)
        , voices_(voices)
        , sample_rate_(sample_rate)
        , provider_(provider)
        , session_samples_(0)
        , total_taken_samples_(0)
    {
        engine_.load(voices_);
    }

    virtual ~Impl()
    {}

    std::shared_ptr<SinsySession> doCreateNextSession()
    {
        std::shared_ptr<SinsySession> session;
        session.reset(new SinsySession(provider_, sample_rate_, language_, dictionary_path_, voices_));

        event_map_t score_source = takeEvents(provider_,
                                              remaining_,
                                              last_tempo_);
        session->impl_->initWithEvents(score_source);

        return std::move(session);
    }

    void doSynthesize()
    {
        using namespace sinsy;

        synthesized_samples_ = 0;

        Converter converter;
        {
            // setup Japanese UTF-8 conf
            std::unique_ptr<JConf> jconf(new JConf(ConfManager::UTF_8_STRS));
            std::stringstream table;
            init_stream(table, binary_japanese_utf_8_table_start, binary_japanese_utf_8_table_end);
            std::stringstream conf;
            init_stream(conf, binary_japanese_utf_8_conf_start, binary_japanese_utf_8_conf_end);
            std::stringstream macron;
            init_stream(macron, binary_japanese_macron_start, binary_japanese_macron_end);
            jconf->read(table, conf, macron);
            converter.setJapaneseUTF8Conf(jconf.release());
        }
        {
            // setup Japanese Shift_JIS conf
            std::unique_ptr<JConf> jconf(new JConf(ConfManager::SHIFT_JIS_STRS));
            std::stringstream table;
            init_stream(table, binary_japanese_shift_jis_table_start, binary_japanese_shift_jis_table_end);
            std::stringstream conf;
            init_stream(conf, binary_japanese_shift_jis_conf_start, binary_japanese_shift_jis_conf_end);
            std::stringstream macron;
            init_stream(macron, binary_japanese_macron_start, binary_japanese_macron_end);
            jconf->read(table, conf, macron);
            converter.setJapaneseShiftJISConf(jconf.release());
        }
        {
            // setup Japanese EUC-JP conf
            std::unique_ptr<JConf> jconf(new JConf(ConfManager::EUC_JP_STRS));
            std::stringstream table;
            init_stream(table, binary_japanese_euc_jp_table_start, binary_japanese_euc_jp_table_end);
            std::stringstream conf;
            init_stream(conf, binary_japanese_euc_jp_conf_start, binary_japanese_euc_jp_conf_end);
            std::stringstream macron;
            init_stream(macron, binary_japanese_macron_start, binary_japanese_macron_end);
            jconf->read(table, conf, macron);
            converter.setJapaneseEUCJPConf(jconf.release());
        }

        sinsy::LabelMaker labelMaker(converter);
        labelMaker << score_;
        labelMaker.fix();
        sinsy::LabelStrings label;
        labelMaker.outputLabel(label, false, 1, 2);

        HTS_Engine_set_sampling_frequency(&engine_.engine, sample_rate_);

        if (HTS_Engine_get_nvoices(&engine_.engine) == 0 || label.size() == 0) {
            return;
        }

        HTS_Engine_set_audio_buff_size(&engine_.engine, 0);

        HTS_Engine_synthesize_from_strings(&engine_.engine, (char **)label.getData(), label.size());

        synthesized_samples_ = HTS_Engine_get_nsamples(&engine_.engine);
    }

    void doTakeSynthesizeResult(std::vector<double> & buffer, size_t length)
    {
        buffer.resize(length);
        size_t index = total_taken_samples_;
        for (size_t i = 0; i < length; ++i, ++index) {
            double x = (index < synthesized_samples_)
                ? engine_.engine.gss.gspeech[index]
                : 0.0;
            double const WAVEFORM_MAX_AMPLITUDE = 32768.0;
            buffer[i] = x / WAVEFORM_MAX_AMPLITUDE;
        }
        total_taken_samples_ += length;
    }

    void doWriteScore(sinsy::IScoreWritable & w) const
    {
        score_.write(w);
    }

    size_t getSessionLength() const
    {
        return session_samples_;
    }

    size_t getRemainingSessionLength() const
    {
        assert(session_samples_ >= total_taken_samples_);
        return session_samples_ - total_taken_samples_;
    }

private:
    struct NextNote
    {
        NextNote(sinsy::Note const& note, tick_t tick)
            : note_(note)
            , start_tick_(tick)
        {}

        sinsy::Note note_;
        tick_t start_tick_;
    };

    std::shared_ptr<NextNote>
    addEvent(event_t const& event,
             tick_t tick,
             std::shared_ptr<NextNote> const& previous_next_note)
    {
        assert(previous_next_note.get());

        if (previous_next_note->note_.isRest()) {
            return addEventAfterRest(event, tick, previous_next_note);
        } else {
            return addEventAfterNote(event, tick, previous_next_note);
        }
    }

    std::shared_ptr<NextNote>
    createNextRest(tick_t start_tick)
    {
        sinsy::Note rest;
        rest.setRest(true);
        auto result = std::make_shared<NextNote>(rest, start_tick);
        return result;
    }

    std::shared_ptr<NextNote>
    createNextNote(tick_t start_tick, sinsy::Note const& note)
    {
        return std::make_shared<NextNote>(note, start_tick);
    }

    void createNote(sinsy::Note & note, cadencii::singing::NoteEvent const* source)
    {
        int const note_number = source->noteNumber();
        note.setRest(false);
        note.setLyric(source->lyric());
        int const octave = VSQ_NS::NoteNumberUtil::getNoteOctave(note_number) + 1;
        int const step = (note_number % STEP_NUM_ + STEP_NUM_) % STEP_NUM_;
        sinsy::Pitch pitch(step, octave);
        note.setPitch(pitch);
        note.setDuration(source->duration() * TICK_SCALE_);
    }

    std::shared_ptr<NextNote>
    addEventAfterRest(event_t const& event, tick_t tick, std::shared_ptr<NextNote> const& previous_next_note)
    {
        using namespace cadencii::singing;

        if (tick > previous_next_note->start_tick_) {
            size_t const duration = tick - previous_next_note->start_tick_;

            sinsy::Note note(previous_next_note->note_);
            note.setDuration(duration);
            score_.addNote(note);
        }

        NoteEvent * note_event = dynamic_cast<NoteEvent *>(event.get());
        TempoEvent * tempo_event = dynamic_cast<TempoEvent *>(event.get());

        if (note_event) {
            sinsy::Note note;
            createNote(note, note_event);
            auto r = createNextNote(tick, note);
            return r;
        } else if (tempo_event) {
            score_.changeTempo(tempo_event->tempo());
            return createNextRest(tick);
        } else {
            return previous_next_note;
        }
    }

    std::shared_ptr<NextNote>
    addEventAfterNote(event_t const& event, tick_t tick, std::shared_ptr<NextNote> const& previous_next_note)
    {
        using namespace cadencii::singing;

        tick_t const end_tick = previous_next_note->start_tick_ + previous_next_note->note_.getDuration();

        NoteEvent * note_event = dynamic_cast<NoteEvent *>(event.get());
        TempoEvent * tempo_event = dynamic_cast<TempoEvent *>(event.get());
        assert(note_event || tempo_event);

        if (tick < end_tick) {
            if (tick > previous_next_note->start_tick_) {
                sinsy::Note note(previous_next_note->note_);
                size_t const duration = tick - previous_next_note->start_tick_;
                note.setDuration(duration);
                note.setTieStart(true);
                score_.addNote(note);
            }
            if (note_event) {
                sinsy::Note next_note;
                createNote(next_note, note_event);
                return createNextNote(tick, next_note);
            } else if (tempo_event) {
                score_.changeTempo(tempo_event->tempo());
                sinsy::Note next_note(previous_next_note->note_);
                next_note.setDuration(previous_next_note->note_.getDuration() - (tick - previous_next_note->start_tick_));
                return createNextNote(tick, next_note);
            }
        } else if (end_tick <= tick) {
            sinsy::Note note(previous_next_note->note_);
            score_.addNote(note);
            if (end_tick < tick) {
                size_t const duration = tick - end_tick;
                sinsy::Note rest;
                rest.setRest(true);
                rest.setDuration(duration);
                score_.addNote(rest);
            }
            if (note_event) {
                sinsy::Note next_note;
                createNote(next_note, note_event);
                return createNextNote(tick, next_note);
            } else if (tempo_event) {
                score_.changeTempo(tempo_event->tempo());
                return createNextRest(tick);
            }
        }

        return previous_next_note;
    }

    void initWithEvents(event_map_t const& events)
    {
        using namespace cadencii::singing;

        tick_t last_tick = 0;

        std::shared_ptr<NextNote> next_note = createNextRest(0);

        for (auto it = events.begin(); it != events.end(); ++it) {
            tick_t tick = it->first * TICK_SCALE_;
            event_t event = it->second;
            next_note = addEvent(event, tick, next_note);
            last_tick = tick;
        }

        if (next_note && !next_note->note_.isRest()) {
            score_.addNote(next_note->note_);
        }

        double tempo;
        bool const find_result = getLastTempo(events, tempo);
        assert(find_result);
        last_tempo_ = tempo;

        session_samples_ = static_cast<size_t>(sample_rate_ * calculateSessionLength(events));
    }

    static event_map_t takeEvents(cadencii::singing::IScoreProvider * provider,
                                  event_map_t const& remaining,
                                  double first_tempo)
    {
        using namespace cadencii::singing;

        tick_t const first_tick = remaining.empty() ? 0 : remaining.begin()->first;
        event_map_t result = shift(remaining, first_tick);

        tick_t current = 0;
        event_t event;
        tick_t delta = 0;
        while (provider->next(event, delta)) {
            current += delta;
            result.insert(std::make_pair(current, event));
        }

        double dummy;
        if (!getTempoAt0(result, dummy)) {
            result.insert(std::make_pair(0, std::make_shared<TempoEvent>(first_tempo)));
        }

        return result;
    }

    template<class InputConstIterator, class Predicate>
    static bool findTempo(
            InputConstIterator const& begin,
            InputConstIterator const& end,
            Predicate const& predicate,
            double & result)
    {
        auto it = std::find_if(begin, end, predicate);
        if (it != end) {
            using namespace cadencii::singing;
            TempoEvent * tempo = dynamic_cast<TempoEvent *>(it->second.get());
            assert(tempo != nullptr);
            result = tempo->tempo();
            return true;
        } else {
            return false;
        }
    }

    static bool getTempoAt0(event_map_t const& events, double & result)
    {
        auto predicate = [](event_map_t::value_type const& value) {
            if (value.first == 0) {
                using namespace cadencii::singing;
                TempoEvent * tempo = dynamic_cast<TempoEvent *>(value.second.get());
                return tempo != nullptr;
            } else {
                return false;
            }
        };
        return findTempo(events.begin(), events.end(), predicate, result);
    }

    static bool getLastTempo(event_map_t const& events, double & result)
    {
        auto predicate = [](event_map_t::value_type const& value) {
            using namespace cadencii::singing;
            TempoEvent * tempo = dynamic_cast<TempoEvent *>(value.second.get());
            return tempo != nullptr;
        };
        return findTempo(events.rbegin(), events.rend(), predicate, result);
    }

    static event_map_t shift(event_map_t const& events, cadencii::singing::tick_t shift_amount)
    {
        event_map_t result;
        std::for_each(std::begin(events), std::end(events), [shift_amount, &result](event_map_t::value_type const& value) {
            result.insert(std::make_pair(value.first + shift_amount, value.second));
        });
        return result;
    }

    static std::map<tick_t, double> extractTempoTable(event_map_t const& events)
    {
        using namespace cadencii::singing;

        std::map<tick_t, double> result;
        std::for_each(std::begin(events), std::end(events), [&result](event_map_t::value_type const& v) {
            tick_t const tick = v.first;
            event_t const event = v.second;
            TempoEvent * tempo = dynamic_cast<TempoEvent *>(event.get());
            if (tempo) {
                result[tick] = tempo->tempo();
            }
        });
        return result;
    }

    static double getSecondFromTick(std::map<tick_t, double> const& tempo_table, tick_t tick)
    {
        double total_second = 0.0;
        tick_t last_tick = 0;
        double last_tempo = tempo_table.at(0);
        for (auto it = tempo_table.begin(); it->first < tick; ++it) {
            tick_t const t = it->first;
            double const tempo = it->second;
            total_second += (t - last_tick) / (8 * last_tempo);
            last_tick = t;
            last_tempo = tempo;
        }
        total_second += (tick - last_tick) / (8 * last_tempo);
        return total_second;
    }

    //!
    //! \brief calculateSessionLength   Get the length of the session in seconds.
    //! \param events
    //! \return
    static double calculateSessionLength(event_map_t const& events)
    {
        using namespace cadencii::singing;

        std::map<tick_t, double> tempo_table = extractTempoTable(events);
        auto last_note = std::find_if(events.rbegin(), events.rend(), [](event_map_t::value_type const& v) {
            NoteEvent * note = dynamic_cast<NoteEvent *>(v.second.get());
            return note != nullptr;
        });
        double last_note_end_sec = 0.0;
        if (last_note != events.rend()) {
            NoteEvent * last_note_event = dynamic_cast<NoteEvent *>(last_note->second.get());
            tick_t const last_note_end = last_note->first + last_note_event->duration();
            last_note_end_sec = getSecondFromTick(tempo_table, last_note_end);
        }
        return last_note_end_sec + SinsySession::MIN_SECONDS_BETWEEN_SESSIONS_;
    }

private:
    sinsy::ScoreDoctor score_;
    sinsy::HtsEngine engine_;
    std::string const language_;
    std::string const dictionary_path_;
    std::vector<std::string> const voices_;
    int const sample_rate_;
    event_map_t remaining_;
    cadencii::singing::IScoreProvider * const provider_;
    double last_tempo_;
    size_t synthesized_samples_;
    size_t session_samples_;
    size_t total_taken_samples_;

    static int const TICK_SCALE_ = 2;
    static int const STEP_NUM_ = 12;
};


SinsySession::SinsySession(
        cadencii::singing::IScoreProvider * provider,
        int sample_rate,
        double default_tempo,
        std::vector<std::string> const& voices,
        std::string const& language,
        std::string const& dictionary_path)
    : impl_(new Impl(provider, sample_rate, default_tempo, voices, language, dictionary_path))
{}


SinsySession::SinsySession(
        cadencii::singing::IScoreProvider * provider,
        int sample_rate,
        std::string const& language,
        std::string const& dictionary_path,
        std::vector<std::string> const& voices)
    : impl_(std::make_shared<Impl>(provider, sample_rate, language, dictionary_path, voices))
{}


SinsySession::~SinsySession()
{}


std::shared_ptr<SinsySession>
SinsySession::createNextSession()
{
    return impl_->doCreateNextSession();
}


void SinsySession::synthesize()
{
    impl_->doSynthesize();
}


void SinsySession::takeSynthesizeResult(std::vector<double> & buffer, size_t length)
{
    impl_->doTakeSynthesizeResult(buffer, length);
}


void SinsySession::writeScore(sinsy::IScoreWritable &w) const
{
    impl_->doWriteScore(w);
}


size_t SinsySession::getSessionLength() const
{
    return impl_->getSessionLength();
}

size_t SinsySession::getRemainingSessionLength() const
{
    return impl_->getRemainingSessionLength();
}

}
}
