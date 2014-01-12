#ifdef _MSC_VER
    static inline std::string methodName(std::string const& name)
    {
        auto collon = name.rfind("::");
        if (collon == std::string::npos) {
            return name;
        } else {
            auto begin = collon + 2;
            auto end = name.rfind("(");
            return name.substr(begin, end);
        }
    }
    #define __METHOD_NAME__ methodName(__FUNCTION__)
#else
    #define __METHOD_NAME__ std::string(__FUNCTION__)
#endif


class ScoreWritableStub : public sinsy::IScoreWritable
{
public:
   	void setEncoding(std::string const& encoding) override
   	{
        pushResult(__METHOD_NAME__, encoding);
   	}
 
    void changeTempo(double tempo) override
    {
        pushResult(__METHOD_NAME__, tempo);
    }

    void changeBeat(sinsy::Beat const& beat) override
    {
        pushResult(__METHOD_NAME__, beat);
    }

    void changeDynamics(sinsy::Dynamics const& dynamics) override
    {
        pushResult(__METHOD_NAME__, dynamics);
    }

    void changeKey(sinsy::Key const& key) override
    {
        pushResult(__METHOD_NAME__, key);
    }

    void startCrescendo() override
    {
        pushResult(__METHOD_NAME__);
    }

    void startDiminuendo() override
    {
        pushResult(__METHOD_NAME__);
    }

    void stopCrescendo() override
    {
        pushResult(__METHOD_NAME__);
    }

    void stopDiminuendo() override
    {
        pushResult(__METHOD_NAME__);
    }

    void addNote(sinsy::Note const& note) override
    {
        pushResult(__METHOD_NAME__, note);
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

#undef __METHOD_NAME__
