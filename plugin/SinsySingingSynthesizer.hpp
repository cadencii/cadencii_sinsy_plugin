#include <vector>
#include <cadencii-nt/singing/cadencii_singing_plugin_sdk.h>
#include "../lib/label/LabelMaker.h"
#include "./SinsySession.hpp"

namespace cadencii {
namespace plugin {

class SinsySingingSynthesizer : public cadencii::singing::ISingingSynthesizer
{
public:
    SinsySingingSynthesizer(int sample_rate);

    virtual ~SinsySingingSynthesizer();

    void operator () (double * left, double * right, size_t length) override;

    bool setConfig(std::string const& key, std::string const& value) override;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;

    static double const TEMPO_;
    static int const TICK_ORDER_;
    static double const WAVEFORM_MAX_AMPLITUDE;
};

}
}
