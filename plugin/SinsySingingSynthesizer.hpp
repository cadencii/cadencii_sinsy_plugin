#include <vector>
#include "cadencii_synth_api.h"
#include "lib/label/LabelMaker.h"
#include "./SinsySession.hpp"

namespace cadencii {
namespace plugin {

class SinsySingingSynthesizer : public cadencii::singing::ISingingSynthesizer
{
public:
    SinsySingingSynthesizer(int sample_rate);

    virtual ~SinsySingingSynthesizer();

    bool setConfig(std::string const& key, std::string const& value) override;

protected:
    void operator () (double * left, double * right, size_t length) override;

    std::shared_ptr<cadencii::singing::IScoreProvider>
    getProvider();

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;

    static double const TEMPO_;
    static int const TICK_ORDER_;
    static double const WAVEFORM_MAX_AMPLITUDE;
};

}
}
