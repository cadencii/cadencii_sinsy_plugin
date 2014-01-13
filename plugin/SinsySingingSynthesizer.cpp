#include "SinsySingingSynthesizer.hpp"

namespace cadencii {
namespace plugin {

double const SinsySingingSynthesizer::TEMPO_ = 120.0;
int const SinsySingingSynthesizer::TICK_ORDER_ = 2;
double const SinsySingingSynthesizer::WAVEFORM_MAX_AMPLITUDE = 32768.0;

struct SinsySingingSynthesizer::Impl
{
    explicit Impl(SinsySingingSynthesizer * self)
        : self_(self)
        , language_initialized_(false)
        , dictionary_initialized_(false)
    {}

    ~Impl()
    {}

    void render(double * left, double * right, size_t length)
    {
        using namespace cadencii::singing;

        size_t remaining = length;
        size_t processed = 0;
        std::vector<double> buffer;
        size_t const LEN = 1024;
        while (remaining > 0) {
            if (!current_session_) {
                std::shared_ptr<IScoreProvider> provider = self_->getProvider();
                current_session_.reset(new SinsySession(provider.get(),
                                                        self_->sampleRate(),
                                                        SinsySingingSynthesizer::TEMPO_,
                                                        voices_,
                                                        language_,
                                                        dictionary_));
                current_session_->synthesize();
            }
            size_t const amount = (std::min)({remaining, LEN, current_session_->getRemainingSessionLength()});
            current_session_->takeSynthesizeResult(buffer, amount);
            for (size_t i = 0; i < amount; ++i, ++processed) {
                double const v = buffer[i];
                left[processed] = v;
                right[processed] = v;
            }
            remaining -= amount;
            if (current_session_->getRemainingSessionLength() == 0) {
                current_session_ = current_session_->createNextSession();
                current_session_->synthesize();
            }
        }
    }

    bool setConfig(std::string const& key, std::string const& value)
    {
        if (key == CONFIG_KEY_DICTIONARY_PATH) {
            dictionary_ = value;
            dictionary_initialized_ = true;
            return initializeConverter();
        } else if (key == CONFIG_KEY_HTVOICE_PATH) {
            voices_.clear();
            voices_ = split(value, CONFIG_KEY_HTVOICE_PATH_DELIMITER);
            return true;
        } else if (key == CONFIG_KEY_LANGUAGE) {
            language_ = value;
            language_initialized_ = true;
            return initializeConverter();
        } else {
            return false;
        }
    }

private:
    bool initializeConverter()
    {
        if (dictionary_initialized_ && language_initialized_) {
            converter_.setLanguages(language_, dictionary_);
            return true;
        } else {
            return false;
        }
    }

    std::vector<std::string>
    split(std::string const& str, std::string const& delim)
    {
        std::vector<std::string> res;
        size_t current = 0, found, delimlen = delim.size();
        while((found = str.find(delim, current)) != std::string::npos) {
            res.push_back(std::string(str, current, found - current));
            current = found + delimlen;
        }
        res.push_back(std::string(str, current, str.size() - current));
        return res;
    }

    SinsySingingSynthesizer * const self_;
    std::vector<std::string> voices_;

    std::string language_;
    bool language_initialized_;

    std::string dictionary_;
    bool dictionary_initialized_;

    sinsy::Converter converter_;

    std::shared_ptr<SinsySession> current_session_;

    static std::string const CONFIG_KEY_HTVOICE_PATH;
    static std::string const CONFIG_KEY_DICTIONARY_PATH;
    static std::string const CONFIG_KEY_LANGUAGE;
    static std::string const CONFIG_KEY_HTVOICE_PATH_DELIMITER;
};


static std::string const CONFIG_KEY_DOMAIN = "com.github.cadencii.sinsy_plugin.";
std::string const SinsySingingSynthesizer::Impl::CONFIG_KEY_HTVOICE_PATH = CONFIG_KEY_DOMAIN + std::string("htvoice");
std::string const SinsySingingSynthesizer::Impl::CONFIG_KEY_DICTIONARY_PATH = CONFIG_KEY_DOMAIN + std::string("dictionary");
std::string const SinsySingingSynthesizer::Impl::CONFIG_KEY_LANGUAGE = CONFIG_KEY_DOMAIN + std::string("language");
std::string const SinsySingingSynthesizer::Impl::CONFIG_KEY_HTVOICE_PATH_DELIMITER = "\n";


SinsySingingSynthesizer::SinsySingingSynthesizer(int sample_rate)
    : cadencii::singing::ISingingSynthesizer(sample_rate)
    , impl_(std::make_shared<Impl>(this))
{}


SinsySingingSynthesizer::~SinsySingingSynthesizer()
{}


void SinsySingingSynthesizer::operator () (double * left, double * right, size_t length)
{
    impl_->render(left, right, length);
}


bool SinsySingingSynthesizer::setConfig(std::string const& key, std::string const& value)
{
    return impl_->setConfig(key, value);
}

std::shared_ptr<cadencii::singing::IScoreProvider>
SinsySingingSynthesizer::getProvider()
{
    return provider_;
}

}
}
