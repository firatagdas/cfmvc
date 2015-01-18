#include "cfmodel.h"

CFModel::CFModel(CFMvc *mvc, const QVariant &config)
    : QObject(0)
    , m_mvc(mvc)
    , m_config(config)
{
}

CFModel::~CFModel()
{
}

