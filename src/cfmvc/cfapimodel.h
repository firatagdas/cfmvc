#ifndef CFAPIMODEL_H
#define CFAPIMODEL_H

#include <QObject>
#include "cfmodel.h"

class CFAPIModel : public CFModel
{
    Q_OBJECT
    CFMVC_MODEL_OBJECT(CFAPIModel)

public:
    explicit CFAPIModel(CFMvc *mvc, const QVariant &config = QVariant());
    ~CFAPIModel();

Q_SIGNALS:

public Q_SLOTS:
};

#endif // CFAPIMODEL_H
