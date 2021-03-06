#include "Window.h"
#include "Utilities.h"

Window::Window()
{ 
	Setup("Window", sf::Vector2u(640,480));
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{ 
	Setup(title,size);
}

Window::~Window()
{
	m_window.close();
}

void Window::Setup(const std::string& title, const sf::Vector2u& size)
{
	m_windowTitle = title;
	m_windowSize = size;
	m_isFullscreen = true;
	m_isDone = false;
	m_isFocused = true;
	m_isResizable = false;
	m_isVSyncOn = true;
	m_frameRateLimit = 60;

	m_eventManager.AddCallback(StateType(0),"Fullscreen_toggle",&Window::ToggleFullscreen,this);
	m_eventManager.AddCallback(StateType(0),"Window_close", &Window::Close, this);

	Create();
}

void Window::Create()
{
	sf::Uint32 style;

	if		(m_isFullscreen)	style = sf::Style::Fullscreen;
	else if (!m_isResizable)	style = sf::Style::Close;

	m_window.create(sf::VideoMode(m_windowSize.x,m_windowSize.y,32),m_windowTitle,style);
	m_window.setVerticalSyncEnabled(m_isVSyncOn);
	m_window.setFramerateLimit(m_frameRateLimit);

	sf::Image icon;
	if (icon.loadFromFile(Utils::loadAsset("icon/carrie.png")))
		m_window.setIcon(70, 70, icon.getPixelsPtr());
}

void Window::ApplyLetterBoxView(const uint16_t p_iWindowWidth, const uint16_t p_iWindowHeight)
{
	sf::View view = m_window.getView();

	const float windowRatio = p_iWindowWidth / static_cast<float>(p_iWindowHeight);
	const float viewRatio = view.getSize().x / view.getSize().y;
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float posX = 0.0f;
	float posY = 0.0f;
	bool horizontalSpacing = true;

	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.0f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.0f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	this->m_window.setView(view);
}

void Window::BeginDraw() { m_window.clear(sf::Color(0, 0, 0)); }
void Window::EndDraw() { m_window.display(); }

bool Window::IsDone() const { return m_isDone; }
bool Window::IsFullscreen() const { return m_isFullscreen; }
bool Window::IsFocused() const { return m_isFocused; }
bool Window::IsResizable() const { return m_isResizable; }

sf::RenderWindow* Window::GetRenderWindow() { return &m_window; }
EventManager* Window::GetEventManager() { return &m_eventManager; }
sf::Vector2u Window::GetWindowSize() const { return m_windowSize; }

sf::Vector2u Window::GetWindowCenter() const
{
	return sf::Vector2u(m_windowSize.x / 2, m_windowSize.y / 2);
}

sf::FloatRect Window::GetViewSpace() const
{
	const sf::Vector2f viewCenter = m_window.getView().getCenter();
	const sf::Vector2f viewSize = m_window.getView().getSize();
	const sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	const sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::ToggleFullscreen(EventDetails* l_details)
{
	m_isFullscreen = !m_isFullscreen;
	m_window.close();
	Create();
}

void Window::Close(EventDetails* l_details) { m_isDone = true; }

void Window::Update()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}

		m_eventManager.HandleEvent(event);
	}

	m_eventManager.Update();
}
