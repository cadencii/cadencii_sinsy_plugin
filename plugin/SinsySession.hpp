#include <memory>
#include <map>
#include <vector>
#include <cadencii-nt/singing/cadencii_singing_plugin_sdk.h>

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

    void takeSynthesizeResult(std::vector<double> & buffer, size_t offset, size_t length);

protected:
    SinsySession(cadencii::singing::IScoreProvider * provider,
                 int sample_rate,
                 std::string const& language,
                 std::string const& dictionary_path,
                 std::vector<std::string> const& voices);

    virtual void writeScore(sinsy::IScoreWritable &) const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

}
}
