#include <memory>
#include <map>
#include <vector>
#include "cadencii_synth_api.h"

#include "lib/temporary/ScoreDoctor.h"
#include "lib/hts_engine_API/HtsEngine.h"

namespace cadencii {
namespace plugin {

class SinsySession
{
public:
    SinsySession(cadencii::singing::IScoreProvider * provider,
                 int sample_rate,
                 double default_tempo,
                 std::vector<std::string> const& voices,
                 std::string const& language,
                 std::string const& dictionary_path);

    virtual ~SinsySession();

    std::shared_ptr<SinsySession> createNextSession();

    void synthesize();

    void takeSynthesizeResult(std::vector<double> & buffer, size_t length);

    //!
    //! \brief getSessionLength     Get the sample length of this session.
    //! \return
    size_t getSessionLength() const;

    //!
    //! \brief getRemainingSessionLength    Get remaining synthesized result samples.
    //!                                     Equals to "getSessionLength() - total takeSynthesizeResult'ed samples".
    //! \return
    size_t getRemainingSessionLength() const;

protected:
    SinsySession(cadencii::singing::IScoreProvider * provider,
                 int sample_rate,
                 std::string const& language,
                 std::string const& dictionary_path,
                 std::vector<std::string> const& voices);

    virtual void writeScore(sinsy::IScoreWritable &) const;

protected:
    static double const MIN_SECONDS_BETWEEN_SESSIONS_;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

}
}
