#include <gtest/gtest.h>

#include "../plugin/SinsySingingSynthesizer.hpp"
#include "stub/ScoreProviderStub.hpp"

TEST(SinsySingingSynthesizer, synthesize)
{
    using namespace cadencii::singing;
    using namespace cadencii::plugin;

    auto provider = std::make_shared<ScoreProviderStub>();
    provider->addToPool(std::make_shared<NoteEvent>("あ", 60, 1920), 1920);

    int const sample_rate = 44100;
    SinsySingingSynthesizer synth(sample_rate);
    synth.bind(provider);
    synth.setConfig("com.github.cadencii.sinsy_plugin.language",
                    "j");
    synth.setConfig("com.github.cadencii.sinsy_plugin.dictionary",
                    "../src/dic");
    synth.setConfig("com.github.cadencii.sinsy_plugin.htvoice",
                    "fixture/nitech_jp_song070_f001.htsvoice");
    int const length = sample_rate * 6;
    std::shared_ptr<double> left;
    left.reset(new double[length]);
    std::shared_ptr<double> right;
    right.reset(new double[length]);
    std::fill(left.get(), left.get() + length, 0.0);
    std::fill(right.get(), right.get() + length, 0.0);

    EXPECT_NO_THROW(synth.render(left.get(), right.get(), length));
}
