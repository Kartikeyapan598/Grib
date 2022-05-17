#include "data.h"


Data* Data::m_dataInstance;

Data::Data()
{
    if(!m_dataInstance) {
        m_dataInstance = this;
    }
    else {
        return;
    }
}

Data::~Data() {

}

