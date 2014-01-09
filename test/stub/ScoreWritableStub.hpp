class ScoreWritableStub : public sinsy::IScoreWritable
{
public:
   	void setEncoding(std::string const& encoding) override
   	{
   		pushResult(__FUNCTION__, encoding);
   	}
 
	void changeTempo(double tempo) override
	{
		pushResult(__FUNCTION__, tempo);
	}

	void changeBeat(sinsy::Beat const& beat) override
	{
		pushResult(__FUNCTION__, beat);
	}

	void changeDynamics(sinsy::Dynamics const& dynamics) override
	{
		pushResult(__FUNCTION__, dynamics);
	}

	void changeKey(sinsy::Key const& key) override
	{
		pushResult(__FUNCTION__, key);
	}

	void startCrescendo() override
	{
		pushResult(__FUNCTION__);
	}

	void startDiminuendo() override
	{
		pushResult(__FUNCTION__);
	}

	void stopCrescendo() override
	{
		pushResult(__FUNCTION__);
	}

	void stopDiminuendo() override
	{
		pushResult(__FUNCTION__);
	}

	void addNote(sinsy::Note const& note) override
	{
        pushResult(__FUNCTION__, note);
	}

	std::vector<std::string> getResult() const
	{
		return result_;
	}

private:
	template<class T>
	void pushResult(std::string const& func_name, T const& v)
	{
		std::ostringstream s;
		s << func_name << ":" << v;
		result_.push_back(s.str());
	}

	void pushResult(std::string const& func_name)
	{
		result_.push_back(func_name);
	}

	std::vector<std::string> result_;
};
