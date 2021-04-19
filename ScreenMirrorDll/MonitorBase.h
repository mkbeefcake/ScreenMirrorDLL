#pragma once

class MonitorBase
{
public:
	MonitorBase() { curMonitorID = -1; }

public:

	virtual bool isInitialized() = 0;
	virtual bool isSupported() = 0;

	virtual int GetMonitors() = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual BOOL GetScreenSize(int& width, int& height) = 0;
	virtual BOOL GetScreenData(void* buffer, unsigned int bufferSize) = 0;

	virtual BOOL SetCurrentMonitor(int monitorId) 
	{
		this->curMonitorID = monitorId;
		return TRUE;
	}

protected:
	int curMonitorID;

};