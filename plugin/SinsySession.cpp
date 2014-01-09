#include <cassert>
#include <algorithm>
#include <cadencii-nt/vsq/NoteNumberUtil.hpp>
#include "../src/lib/label/LabelMaker.h"
#include "../src/lib/label/LabelStrings.h"
#include "../src/lib/score/util_score.h"
#include "SinsySession.hpp"

namespace cadencii {
namespace plugin {

struct SinsySession::Impl
{
    typedef std::shared_ptr<cadencii::singing::IScoreEvent> event_t;
    typedef std::multimap<cadencii::singing::tick_t, event_t> event_map_t;

public:
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
    {
        using namespace cadencii::singing;

        event_map_t empty;
        event_map_t score_source = takeEvents(provider, empty, default_tempo);
        initWithEvents(score_source);
    }

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
        sinsy::Converter converter;
        converter.setLanguages(language_, dictionary_path_);

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

    void doTakeSynthesizeResult(std::vector<double> & buffer, size_t offset, size_t length)
    {
        size_t const to = offset + length;
        buffer.resize(length);
        size_t index = offset;
        for (size_t i = 0; i < length; ++i, ++index) {
            double x = (index < synthesized_samples_)
                ? engine_.engine.gss.gspeech[index]
                : 0.0;
            double const WAVEFORM_MAX_AMPLITUDE = 32768.0;
            buffer[i] = x / WAVEFORM_MAX_AMPLITUDE;
        }
    }

    void doWriteScore(sinsy::IScoreWritable & w) const
    {
        score_.write(w);
    }

private:
    void initWithEvents(event_map_t const& events)
    {
        using namespace cadencii::singing;

        tick_t last_tick = 0;

        struct NextNote
        {
            sinsy::Note note_;
            tick_t start_tick_;
        };

        std::shared_ptr<NextNote> next_note;
        next_note.reset(new NextNote());
        next_note->note_.setRest(true);
        next_note->start_tick_ = 0;

        for (auto it = events.begin(); it != events.end(); ++it) {
            tick_t tick = it->first;
            event_t event = it->second;
            NoteEvent * note_event = dynamic_cast<NoteEvent *>(event.get());
            TempoEvent * tempo_event = dynamic_cast<TempoEvent *>(event.get());
            if (note_event) {
                if (tick != last_tick) {
                    sinsy::Note note(next_note->note_);
                    size_t const duration = tick - next_note->start_tick_;
                    note.setDuration(duration * TICK_SCALE_);
                    score_.addNote(note);
                }

                int const note_number = note_event->noteNumber();

                next_note.reset(new NextNote());
                next_note->note_.setRest(false);
                next_note->note_.setLyric(note_event->lyric());
                int const octave = VSQ_NS::NoteNumberUtil::getNoteOctave(note_number) + 1;
                int const step = (note_number % STEP_NUM_ + STEP_NUM_) % STEP_NUM_;
                sinsy::Pitch pitch(step, octave);
                next_note->note_.setPitch(pitch);
                next_note->note_.setDuration(note_event->duration() * TICK_SCALE_);
                next_note->start_tick_ = tick;

                last_tick = tick;
            } else if (tempo_event) {
                if (tick != last_tick) {
                    sinsy::Note note(next_note->note_);
                    size_t const duration = tick - next_note->start_tick_;
                    note.setDuration(duration * TICK_SCALE_);
                    note.setTieStart(true);
                    score_.addNote(note);
                }
                score_.changeTempo(tempo_event->tempo());
                last_tick = tick;
            }
        }

        if (next_note && !next_note->note_.isRest()) {
            score_.addNote(next_note->note_);
        }

        double tempo;
        bool const find_result = getLastTempo(events, tempo);
        assert(find_result);
        last_tempo_ = tempo;
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
        tick_t delta;
        while (provider->next(event, delta)) {
            current += delta;
            tick_t const tick = current;
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
    : impl_(std::make_shared<Impl>(provider, sample_rate, default_tempo, voices, language, dictionary_path))
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


void SinsySession::takeSynthesizeResult(std::vector<double> & buffer, size_t offset, size_t length)
{
    impl_->doTakeSynthesizeResult(buffer, offset, length);
}


void SinsySession::writeScore(sinsy::IScoreWritable &w) const
{
    impl_->doWriteScore(w);
}

}
}
