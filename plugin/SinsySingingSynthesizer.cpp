#include "SinsySingingSynthesizer.hpp"

namespace cadencii {
namespace plugin {

double const SinsySingingSynthesizer::TEMPO_ = 120.0;
int const SinsySingingSynthesizer::TICK_ORDER_ = 2;
double const SinsySingingSynthesizer::WAVEFORM_MAX_AMPLITUDE = 32768.0;

SinsySingingSynthesizer::SinsySingingSynthesizer(int sample_rate)
    : cadencii::singing::ISingingSynthesizer(sample_rate)
{}


SinsySingingSynthesizer::~SinsySingingSynthesizer()
{}


void SinsySingingSynthesizer::operator () (double * left, double * right, size_t length)
{
    if (!provider_) {
        return;
    }

}


void SinsySingingSynthesizer::setConfig(std::string const& config)
{
    //TODO:
    voices_.clear();
    voices_.push_back("/Users/kbinani/Downloads/hts_voice_nitech_jp_song070_f001-0.90/nitech_jp_song070_f001.htsvoice");
    std::string language = "j";
    std::string dictionary = "/Users/kbinani/Documents/github/kbinani/sinsy/src/dic";
    converter_.setLanguages(language, dictionary);
}

#if 0
void setTrack(VSQ_NS::Track const& track, VSQ_NS::TempoList const& tempo_list) override
{
    using namespace VSQ_NS;
    score_.clear();
    score_.changeTempo(TEMPO);
    Event::List const* const events = track.events();
    int size = events->size();
    tick_t last_clock = 0;
    for (int i = 0; i < size; ++i) {
        Event const* const event = events->get(i);
        if (event->type != EventType::NOTE) {
            continue;
        }
        if (last_clock != event->clock) {
            sinsy::Note rest;
            rest.setRest(true);
            rest.setDuration((event->clock - last_clock) * TICK_ORDER_);
            score_.addNote(rest);
        }
        sinsy::Note note;
        note.setRest(false);
        note.setDuration(event->getLength() * TICK_ORDER_);
        std::string lyric("あ");
        if (event->lyricHandle.getLyricCount() > 0) {
            lyric = event->lyricHandle.getLyricAt(0).phrase;
        }
        note.setLyric(lyric);
        int const octave = NoteNumberUtil::getNoteOctave(event->note) + 1;
        int const step = (event->note % STEP_NUM + STEP_NUM) % STEP_NUM;
        sinsy::Pitch pitch(step, octave);
        note.setPitch(pitch);
        score_.addNote(note);
        last_clock = event->clock + event->getLength();
    }
}

void setReceiver(std::shared_ptr<cadencii::audio::AudioReceiver> const& receiver) override
{
    receiver_ = receiver;
}

void start(uint64_t length) override
{
    sinsy::HtsEngine engine;
    std::vector<std::string> voices {"/Users/kbinani/Downloads/hts_voice_nitech_jp_song070_f001-0.90/nitech_jp_song070_f001.htsvoice"};
    if (!engine.load(voices)) {
        std::cout << "start; failed to load voices" << std::endl;
    }

    sinsy::Converter converter;
    std::string language = "j";
    std::string dictionary = "/Users/kbinani/Documents/github/kbinani/sinsy/src/dic";
    if (!converter.setLanguages(language, dictionary)) {
        std::cout << "start; fail in converter.setLanguages" << std::endl;
    }
    sinsy::LabelMaker labelMaker(converter);
    labelMaker << score_;
    labelMaker.fix();
    sinsy::LabelStrings label;
    labelMaker.outputLabel(label, false, 1, 2);

    HTS_Engine_set_sampling_frequency(&engine.engine, sample_rate_);

    if (HTS_Engine_get_nvoices(&engine.engine) == 0 || label.size() == 0) {
       return;
    }

    HTS_Engine_set_audio_buff_size(&engine.engine, 0);

    bool result = true;
    if (HTS_Engine_synthesize_from_strings(&engine.engine, (char **)label.getData(), label.size()) != TRUE) {
        result = false;
    }

    size_t const buffer_length = 1024;
    std::vector<double> waveform_buffer ;
    waveform_buffer.resize(buffer_length);
    std::fill(std::begin(waveform_buffer), std::end(waveform_buffer), 0.0);

    size_t const num_samples = (std::min)(HTS_Engine_get_nsamples(&engine.engine), static_cast<size_t>(length));
    size_t current_sample = 0;
    size_t remain_samples = num_samples;
    while (remain_samples > 0) {
        size_t const amount = (std::min)(remain_samples, buffer_length);
        for (size_t i = 0; i < amount; ++i, ++current_sample, --remain_samples) {
            double x = engine.engine.gss.gspeech[current_sample];
            waveform_buffer[i] = x / WAVEFORM_MAX_AMPLITUDE;
        }
        receiver_->push(waveform_buffer.data(), waveform_buffer.data(), amount, 0);
    }

    size_t const x = HTS_Engine_get_audio_buff_size(&engine.engine);
    HTS_Engine_set_audio_buff_size(&engine.engine, x);

    HTS_Engine_refresh(&engine.engine);
}
#endif

}
}
