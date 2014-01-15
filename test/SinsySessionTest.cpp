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
                     std::vector<std::string> const& voices)
        : SinsySession(provider, sample_rate, default_tempo, voices)
    {}

    SinsySessionStub(cadencii::singing::IScoreProvider * provider,
                     int sample_rate,
                     std::vector<std::string> const& voices)
        : SinsySession(provider, sample_rate, voices)
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
        std::vector<std::string> voices;
        voices.push_back("fixture/nitech_jp_song070_f001.htsvoice");

        std::shared_ptr<SinsySessionStub> result;
        result.reset(new SinsySessionStub(provider,
                                          sample_rate,
                                          default_tempo,
                                          voices));
        return result;
    }

    static double getMinIntervalBetweenSessions()
    {
        return MIN_SECONDS_BETWEEN_SESSIONS_;
    }
};


TEST(SinsySession, createFirstSessionWithDefaultTempoTest)
{
    using namespace cadencii::singing;

    auto provider = std::make_shared<ScoreProviderStub>();

    //        |----.----.----.----|----.----.----.----|-- ....
    // TEMPO: ^-120
    //  NOTE: [rest              ][a@C4]

    provider->addToPool(std::make_shared<NoteEvent>("a", 60, 480), 1920);

    auto stub = SinsySessionStub::createSinsySessionStub(provider.get());

    auto writable = std::make_shared<ScoreWritableStub>();
    stub->writeScore(*writable);
    std::vector<std::string> result = writable->getResult();

    EXPECT_EQ(3, result.size());
    EXPECT_EQ("changeTempo:120", result[0]);
    EXPECT_EQ("addNote:[rest(3840)  (tie:) (slur:)]", result[1]);
    EXPECT_EQ("addNote:[pitch(960) C4 a (tie:) (slur:)]", result[2]);

    // session continues to "last note end time" + getMinIntervalBetweenSessions() sec.
    size_t const expected = (size_t)((2.5 + SinsySessionStub::getMinIntervalBetweenSessions()) * 44100);
    size_t const actual = stub->getSessionLength();
    EXPECT_EQ(expected, actual);
}


TEST(SinsySession, createFirstSession)
{
    using namespace cadencii::singing;

    auto provider = std::make_shared<ScoreProviderStub>();

    //        |----.----.----.----|----.----.----.----|-- ....
    // TEMPO: ^-121               ^-60      ^-30      ^-15
    //  NOTE:           [Hello@C3     ]          [a@C4]

    provider->addToPool(std::make_shared<TempoEvent>(121), 0);
    provider->addToPool(std::make_shared<NoteEvent>("Hello", 48, 1440), 960);
    provider->addToPool(std::make_shared<TempoEvent>(60), 1920);
    provider->addToPool(std::make_shared<TempoEvent>(30), 2880);
    provider->addToPool(std::make_shared<NoteEvent>("a", 60, 480), 3360);
    provider->addToPool(std::make_shared<TempoEvent>(15), 3840);

    auto stub = SinsySessionStub::createSinsySessionStub(provider.get());

    auto writable = std::make_shared<ScoreWritableStub>();
    stub->writeScore(*writable);
    std::vector<std::string> result = writable->getResult();

    {
        std::vector<std::string> expected;
        expected.push_back("changeTempo:121");
        expected.push_back("addNote:[rest(1920)  (tie:) (slur:)]");
        expected.push_back("addNote:[pitch(1920) C3 Hello (tie:Start) (slur:)]");
        expected.push_back("changeTempo:60");
        expected.push_back("addNote:[pitch(960) C3 Hello (tie:Stop) (slur:)]");
        expected.push_back("addNote:[rest(960)  (tie:) (slur:)]");
        expected.push_back("changeTempo:30");
        expected.push_back("addNote:[rest(960)  (tie:) (slur:)]");
        expected.push_back("addNote:[pitch(960) C4 a (tie:) (slur:)]");
        expected.push_back("changeTempo:15");

        EXPECT_EQ(expected.size(), result.size());
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(expected[i], result[i]);
        }
    }

    {
        size_t const expected = (size_t)((1920 / (8 * 121.0) + 960 / (8 * 60.0) + 960 / (8 * 30.0) + SinsySessionStub::getMinIntervalBetweenSessions()) * 44100);
        size_t const actual = stub->getSessionLength();
        EXPECT_EQ(expected, actual);
    }
}
