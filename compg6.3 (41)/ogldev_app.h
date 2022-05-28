#ifndef OGLDEV_APP_H
#define OGLDEV_APP_H

class OgldevApp
{
protected:
	OgldevApp();
	void CalcFPS();
	void RenderFPS();
	float GetRunningTime();
private:
	long long m_frameTime;
	long long m_startTime;
	int m_frameCount;
        int m_fps;
};

#endif