#ifndef TANO_FRIPPLUGIN_H_
#define TANO_FRIPPLUGIN_H_

#include <QtCore/QString>

#include "plugins/Plugins.h"

class FripPlugin : public RecorderPlugin
{
public:
	FripPlugin();
	~FripPlugin();

	QString name() const;
	void init();
};

#endif // TANO_FRIPPLUGIN_H_
