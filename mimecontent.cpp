#include "mimecontent.h"
#include <iostream>


MIMEContent::MIMEContent(int nMIMEType)
{
    m_nMIMETypeIHandle = nMIMEType;

}


MIMEContent::~MIMEContent()
{

}

bool MIMEContent::QueryType(int nContentType)
{
    return nContentType == m_nMIMETypeIHandle ? true : false;
}





