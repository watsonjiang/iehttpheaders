#include "ITransactionProcessor.h"


class CCookieDumper : 
  public ITransactionProcessor
{
public:
  CCookieDumper();
  ~CCookieDumper();

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount);
  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount);
};
