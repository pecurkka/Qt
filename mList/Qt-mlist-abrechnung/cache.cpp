#include "cache.h"
#include <hbolog.h>

using namespace hbo;

Cache::Cache()
{
    initKonsumMap();
    initKonsumPriceMap();
}

Cache::~Cache()
{
}

Cache * Cache::cache()
{
    return (instance());
}

Cache * Cache::instance()
{
    static Cache INSTANCE;

    return & INSTANCE;
}

//INIT
void Cache::initKonsumMap()
{
    m_konsumTypMap[100]="Getränk";    
    //IT
    m_konsumTypMap[101]="Getränk (Besprechung)";
    m_konsumTypMap[102]="Getränk (Defekt)";    
    //TECHNIK
    m_konsumTypMap[103]="Getränk (Besprechung)";
    m_konsumTypMap[104]="Getränk (Defekt)";

    m_konsumTypMap[200]="Nespresso";
    m_konsumTypMap[201]="Nespresso (Besprechung)";
    m_konsumTypMap[202]="Nespresso (Defekt)";

    m_konsumTypMap[300]="Kaffee";
    //IT
    m_konsumTypMap[301]="Kaffee (Besprechung)";
    m_konsumTypMap[302]="Kaffee (Defekt)";
    //TECHNIK
    m_konsumTypMap[303]="Kaffee (Besprechung)";
    m_konsumTypMap[304]="Kaffee (Defekt)";

    m_konsumTypMap[900]="Chefteller (Fleisch)";
    m_konsumTypMap[901]="Chefteller (Vegetarisch)";
    m_konsumTypMap[902]="Chefteller (Suppe)";

    m_konsumTypMap[950]="ILIKE Fleisch/Fish (warm)";
    m_konsumTypMap[951]="ILIKE Veggi (warm)";
    m_konsumTypMap[952]="ILIKE (kalt)";
}

void Cache::initKonsumPriceMap()
{
    m_konsumPriceMap[100]=0.5;
    m_konsumPriceMap[101]=0.0;
    m_konsumPriceMap[102]=0.0;
    m_konsumPriceMap[103]=0.0;
    m_konsumPriceMap[104]=0.0;

    m_konsumPriceMap[200]=0.3;
    m_konsumPriceMap[201]=0.0;
    m_konsumPriceMap[202]=0.0;

    m_konsumPriceMap[300]=0.15;
    m_konsumPriceMap[301]=0.0;
    m_konsumPriceMap[302]=0.0;
    m_konsumPriceMap[303]=0.0;
    m_konsumPriceMap[304]=0.0;

    m_konsumPriceMap[900]=3.5;
    m_konsumPriceMap[901]=3.0;
    m_konsumPriceMap[902]=3.0;

    m_konsumPriceMap[950]=3.0;
    m_konsumPriceMap[951]=2.5;
    m_konsumPriceMap[952]=2.0;
}




//GET
QString Cache::getKonsumName(int konsumTyp)
{
    if(!m_konsumTypMap.contains(konsumTyp))
    {
        critical()<<"KONSUM TYP <" << konsumTyp << "> NOT FOUND IN KONSUM TYP MAP";
        return (QString::number(konsumTyp));
    }

    return (m_konsumTypMap[konsumTyp]);
}

double Cache::getKonsumPrice(int konsumTyp)
{
    if(!m_konsumPriceMap.contains(konsumTyp))
    {
        critical()<<"KONSUM TYP <" << konsumTyp << "> NOT FOUND IN KONSUM PRICE MAP";
        return (konsumTyp);
    }

    return (m_konsumPriceMap[konsumTyp]);
}
