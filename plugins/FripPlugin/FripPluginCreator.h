#ifndef TANO_FRIPPLUGINCREATOR_H_
#define TANO_FRIPPLUGINCREATOR_H_

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include <plugins/Plugins.h>

class FripPluginCreator : public QObject, public RecorderPluginCreator
{
	Q_OBJECT;
	Q_INTERFACES( RecorderPluginCreator );
	Q_CLASSINFO("PLUGINNAME", "FripPlugin");

 public:
	RecorderPlugin *createRecorderPluginInstance();
};


#endif // TANO_FRIPPLUGIN_H_
