

class TunDevice;

class Application
{
public:
	Application(HINSTANCE inst);

	~Application();

	void connect();

	void disconnect();

	HINSTANCE hInst() const;

	bool connected() const;

	const char* uniqueWindowTitle() const;

	const char* connectedTitle() const;

	const char* disconnectedTitle() const;

	const char* quitTitle() const;

private:
	std::auto_ptr<TunDevice> m_tunDevice;

	bool m_connected;

	HINSTANCE m_inst;
};