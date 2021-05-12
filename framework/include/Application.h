#ifndef __APPLICATION_H_
#define __APPLICATION_H_

struct GLFWwindow;

class Application
{
public:

	Application() : m_running(false) {}
	virtual ~Application() {}

	bool	create(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

	void	run(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

	void	quit()	{ m_running = false; }

	void	showFrameData(bool a_showFrameData);

protected:

	virtual bool	onCreate() = 0;
	virtual void	Update(float a_deltaTime) = 0;
	virtual void	Draw() = 0;
	virtual void	Destroy() = 0;
	
	GLFWwindow*		m_window = nullptr;
	unsigned int	m_windowWidth = 0;
	unsigned int	m_windowHeight = 0;
	bool			m_running = false;
	bool			m_showFrameData = false;
};

#endif // __APPLICATION_H_