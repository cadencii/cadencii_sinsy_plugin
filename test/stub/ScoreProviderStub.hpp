#include <cadencii-nt/singing/cadencii_singing_plugin_sdk.h>

class ScoreProviderStub : public cadencii::singing::IScoreProvider
{
public:
    bool next(std::shared_ptr<cadencii::singing::IScoreEvent> & out_event,
    	 	  cadencii::singing::tick_t & out_delta) override
    {
    	if (pool_.empty()) {
    		return false;
    	} else {
            auto front = pool_.begin();
    		out_event = front->second;
    		out_delta = front->first - last_provided_;
    		last_provided_ = front->first;
    		pool_.erase(front);
    		return true;
    	}
    }

    void addToPool(std::shared_ptr<cadencii::singing::IScoreEvent> const& event, cadencii::singing::tick_t tick)
    {
    	pool_.insert(std::make_pair(tick, event));
    }

    void clear()
    {
    	pool_.clear();
    	last_provided_ = 0;
    }

private:
    std::multimap<cadencii::singing::tick_t, std::shared_ptr<cadencii::singing::IScoreEvent>> pool_;
    cadencii::singing::tick_t last_provided_;
};
