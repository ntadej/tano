#ifndef TANO_PLUGINS_H_
#define TANO_PLUGINS_H_

#include <QtCore/QtPlugin>
#include <QtCore/QString>

class RecorderPlugin
{
public:
	virtual ~RecorderPlugin() {};

	virtual void init() = 0;
	virtual QString name() const = 0;
};

class RecorderPluginCreator
{
public:
	virtual ~RecorderPluginCreator() {};
	virtual RecorderPlugin* createRecorderPluginInstance() = 0;
};

Q_DECLARE_INTERFACE(RecorderPluginCreator, "RecorderPluginCreator/0.1")

#endif // TANO_PLUGINS_H_

