#include <gtest/gtest.h>
#include <algorithm>

#include "../plugin/SinsySession.hpp"
#include "stub/ScoreProviderStub.hpp"
#include "stub/ScoreWritableStub.hpp"

class SinsySessionStub : public cadencii::plugin::SinsySession
{
public:
    SinsySessionStub(cadencii::singing::IScoreProvider * provider,
                     int sample_rate,
                     double default_tempo,
                     std::vector<std::string> const& voices,
                     std::string const& language,
                     std::string const& dictionary_path)
        : SinsySession(provider, sample_rate, default_tempo, voices, language, dictionary_path)
    {}

    SinsySessionStub(cadencii::singing::IScoreProvider * provider,
                     int sample_rate,
                     std::string const& language,
                     std::string const& dictionary_path,
                     std::vector<std::string> const& voices)
        : SinsySession(provider, sample_rate, language, dictionary_path, voices)
    {}

    void writeScore(sinsy::IScoreWritable & w) const override
    {
        SinsySession::writeScore(w);
    }

    static std::shared_ptr<SinsySessionStub> 
    createSinsySessionStub(cadencii::singing::IScoreProvider * provider)
    {
        int const sample_rate = 44100;
        double const default_tempo = 120.0;
        std::string language = "j";
        std::string dictionary_path = "";
        std::vector<std::string> voices;
        voices.push_back("");

        std::shared_ptr<SinsySessionStub> result;
        result.reset(new SinsySessionStub(provider,
                                          sample_rate,
                                          default_tempo,
                                          voices,
                                          language,
                                          dictionary_path));
        return result;
    }
};


TEST(SinsySession, createFirstSessionTest)
{
    using namespace cadencii::singing;

    auto provider = std::make_shared<ScoreProviderStub>();
    provider->addToPool(std::make_shared<TempoEvent>(120.0), 0);
    provider->addToPool(std::make_shared<NoteEvent>("a", 60, 480), 1920);

    auto stub = SinsySessionStub::createSinsySessionStub(provider.get());

    auto writable = std::make_shared<ScoreWritableStub>();
    stub->writeScore(*writable);
    std::vector<std::string> result = writable->getResult();

    EXPECT_EQ(3, result.size());
    EXPECT_EQ("changeTempo:120", result[0]);
    EXPECT_EQ("addNote:[rest(3840)  (tie:) (slur:)]", result[1]);
    EXPECT_EQ("addNote:[pitch(960) C4 a (tie:) (slur:)]", result[2]);
}
