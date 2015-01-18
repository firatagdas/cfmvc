#include "cfapimodel.h"

CFMVC_MODEL_BEGIN(CFAPIModel)

CFAPIModel::CFAPIModel(CFMvc *mvc, const QVariant &config)
    : CFModel(mvc, config)
{
}

CFAPIModel::~CFAPIModel()
{

}

CFMVC_MODEL_END(CFAPIModel)
