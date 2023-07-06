#include "platform_sys.h"

#include <cstring>
#include "cache.h"
#include "core.h"

using namespace std;

srt::CInfoBlock& srt::CInfoBlock::copyFrom(const CInfoBlock& obj)
{
   std::copy(obj.m_piIP, obj.m_piIP + 4, m_piIP);
   m_iIPversion       = obj.m_iIPversion;
   m_ullTimeStamp     = obj.m_ullTimeStamp;
   m_iSRTT            = obj.m_iSRTT;
   m_iBandwidth       = obj.m_iBandwidth;
   m_iLossRate        = obj.m_iLossRate;
   m_iReorderDistance = obj.m_iReorderDistance;
   m_dInterval        = obj.m_dInterval;
   m_dCWnd            = obj.m_dCWnd;

   return *this;
}

bool srt::CInfoBlock::operator==(const CInfoBlock& obj) const
{
   if (m_iIPversion != obj.m_iIPversion)
      return false;

   else if (m_iIPversion == AF_INET)
      return (m_piIP[0] == obj.m_piIP[0]);

   for (int i = 0; i < 4; ++ i)
   {
      if (m_piIP[i] != obj.m_piIP[i])
         return false;
   }

   return true;
}

srt::CInfoBlock* srt::CInfoBlock::clone()
{
   CInfoBlock* obj = new CInfoBlock;

   std::copy(m_piIP, m_piIP + 4, obj->m_piIP);
   obj->m_iIPversion       = m_iIPversion;
   obj->m_ullTimeStamp     = m_ullTimeStamp;
   obj->m_iSRTT            = m_iSRTT;
   obj->m_iBandwidth       = m_iBandwidth;
   obj->m_iLossRate        = m_iLossRate;
   obj->m_iReorderDistance = m_iReorderDistance;
   obj->m_dInterval        = m_dInterval;
   obj->m_dCWnd            = m_dCWnd;

   return obj;
}

int srt::CInfoBlock::getKey()
{
   if (m_iIPversion == AF_INET)
      return m_piIP[0];

   return m_piIP[0] + m_piIP[1] + m_piIP[2] + m_piIP[3];
}

void srt::CInfoBlock::convert(const sockaddr_any& addr, uint32_t aw_ip[4])
{
   if (addr.family() == AF_INET)
   {
      aw_ip[0] = addr.sin.sin_addr.s_addr;
      aw_ip[1] = aw_ip[2] = aw_ip[3] = 0;
   }
   else
   {
      memcpy((aw_ip), addr.sin6.sin6_addr.s6_addr, sizeof addr.sin6.sin6_addr.s6_addr);
   }
}
