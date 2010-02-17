#include "FripPlugin.h"
#include "FripPluginCreator.h"

RecorderPlugin *FripPluginCreator::createRecorderPluginInstance()
{
	return new FripPlugin();
}

Q_EXPORT_PLUGIN2( FripPluginCreator, FripPluginCreator )
