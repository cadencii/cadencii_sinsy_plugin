#include "SinsySingingSynthesizer.hpp"

#ifdef _MSC_VER
#define SINSY_PLUGIN_SYMBOL_EXPORT extern "C" __declspec(dllexport)
#else
#define SINSY_PLUGIN_SYMBOL_EXPORT extern "C" __attribute__((visibility("default")))
#endif

SINSY_PLUGIN_SYMBOL_EXPORT
cadencii::singing::ISingingSynthesizer * cadencii_create_synthesizer(int sample_rate)
{
    using namespace cadencii::plugin;
    try {
        return new SinsySingingSynthesizer(sample_rate);
    } catch (...) {
        return nullptr;
    }
}

SINSY_PLUGIN_SYMBOL_EXPORT
bool cadencii_release_synthesizer(cadencii::singing::ISingingSynthesizer * synth)
{
    using namespace cadencii::plugin;
    try {
        SinsySingingSynthesizer * s = dynamic_cast<SinsySingingSynthesizer *>(synth);
        if (s) {
            delete s;
            return true;
        } else {
            return false;
        }
    } catch (...) {
        return false;
    }
}
