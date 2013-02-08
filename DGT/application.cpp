#include "stdafx.h"
#include "application.h"
#include "anytun/tunDevice.h"

Application::Application(HINSTANCE inst) 
	: m_inst(inst)
	, m_connected(false)
{
	//m_tunDevice.reset(new TunDevice("", "", "", 0));
}

Application::~Application()
{

}

HINSTANCE Application::hInst() const
{
	return m_inst;
}

const char* Application::uniqueWindowTitle() const
{
	return TEXT("DGT_not_visible_main_window");
}

const char* Application::connectedTitle() const
{
	return TEXT("Connect");
}

const char* Application::disconnectedTitle() const
{
	return TEXT("Disconnect");
}

const char* Application::quitTitle() const
{
	return TEXT("Quit");
}

bool Application::connected() const
{
	return m_connected;
}

void Application::connect()
{
	m_connected = true;
}

void Application::disconnect()
{
	m_connected = false;
}
