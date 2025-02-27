#include "teraterm.h"
#include "tttypes.h"
#include "ttplugin.h"
#include "tt_res.h"
#include <stdlib.h>
#include <stdio.h>

#define ORDER 2501

static HANDLE hInst; /* Instance handle of TTX*.DLL */

typedef struct {
  PParseParam origParseParam;
  PTTSet ts;
  PComVar cv;
} TInstVar;

static TInstVar *pvar;
static TInstVar InstVar;

static void PASCAL TTXParseParam(wchar_t *Param, PTTSet ts, PCHAR DDETopic) {
  static const TTMessageBoxInfoW mbinfo = {
    NULL,
    NULL, L"TTXShowCommandLine",
    NULL, L"%s",
    MB_OK | MB_ICONEXCLAMATION };
  TTMessageBoxW(pvar->cv->HWin, &mbinfo, pvar->ts->UILanguageFileW, Param);

  pvar->origParseParam(Param, ts, DDETopic);
}

static void PASCAL TTXGetSetupHooks(TTXSetupHooks *hooks) {
  pvar->origParseParam = *hooks->ParseParam;
  *hooks->ParseParam = TTXParseParam;
}

static void PASCAL TTXInit(PTTSet ts, PComVar cv)
{
  pvar->ts = ts;
  pvar->cv = cv;
}

static TTXExports Exports = {
  sizeof(TTXExports),
  ORDER,

  TTXInit,
  NULL, // TTXGetUIHooks,
  TTXGetSetupHooks,
  NULL, // TTXOpenTCP,
  NULL, // TTXCloseTCP,
  NULL, // TTXSetWinSize,
  NULL, // TTXModifyMenu,
  NULL, // TTXModifyPopupMenu,
  NULL, // TTXProcessCommand,
  NULL, // TTXEnd
};

BOOL __declspec(dllexport) PASCAL TTXBind(WORD Version, TTXExports *exports) {
  int size = sizeof(Exports) - sizeof(exports->size);

  if (size > exports->size) {
    size = exports->size;
  }
  memcpy((char *)exports + sizeof(exports->size),
    (char *)&Exports + sizeof(exports->size),
    size);
  return TRUE;
}

BOOL WINAPI DllMain(HANDLE hInstance,
		    ULONG ul_reason_for_call,
		    LPVOID lpReserved)
{
  switch( ul_reason_for_call ) {
    case DLL_THREAD_ATTACH:
      /* do thread initialization */
      break;
    case DLL_THREAD_DETACH:
      /* do thread cleanup */
      break;
    case DLL_PROCESS_ATTACH:
      /* do process initialization */
      hInst = hInstance;
      pvar = &InstVar;
      break;
    case DLL_PROCESS_DETACH:
      /* do process cleanup */
      break;
  }
  return TRUE;
}
