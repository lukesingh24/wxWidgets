/* -------------------------------------------------------------------------
 * Project: GSocket (Generic Socket) for WX
 * Name:    gsockosx.c
 * Purpose: GSocket: Mac OS X mach-o part
 * CVSID:   $Id$
 * Mac code by Brian Victor, February 2002.  Email comments to bhv1@psu.edu
 * ------------------------------------------------------------------------- */

#include "wx/setup.h"

#if wxUSE_SOCKETS

#include <stdlib.h>
#include "wx/gsocket.h"
#include "wx/unix/gsockunx.h"

#include <CoreFoundation/CoreFoundation.h>

#define ALL_CALLBACK_TYPES (kCFSocketReadCallBack | kCFSocketWriteCallBack | kCFSocketConnectCallBack)

struct MacGSocketData
{
  CFSocketRef socket;
  CFRunLoopSourceRef source;
};

void Mac_Socket_Callback(CFSocketRef s, CFSocketCallBackType callbackType,
                         CFDataRef address, const void* data, void* info)
{
  GSocket* socket = (GSocket*)info;
  struct MacGSocketData* macdata;
  macdata = (struct MacGSocketData*)socket->m_gui_dependent;
  if (!macdata) return;
  switch (callbackType)
  {
    case kCFSocketConnectCallBack:
      assert(!socket->m_server);
      socket->m_functions->Detected_Write(socket);
      break;
    case kCFSocketReadCallBack:
      socket->m_functions->Detected_Read(socket);
      break;
    case kCFSocketWriteCallBack:
      socket->m_functions->Detected_Write(socket);
      break;
    default:
      break;  /* We shouldn't get here. */
  }
}

struct MacGSocketData* _GSocket_Get_Mac_Socket(GSocket *socket)
{
  /* If socket is already created, returns a pointer to the data */
  /* Otherwise, creates socket and returns the pointer */
  CFSocketContext cont;
  struct MacGSocketData* data = (struct MacGSocketData*)socket->m_gui_dependent;

  if (data && data->source) return data;

  /* CFSocket has not been created, create it: */
  if (socket->m_fd < 0 || !data) return NULL;
  cont.version = 0; cont.retain = NULL;
  cont.release = NULL; cont.copyDescription = NULL;
  cont.info = socket;

  CFSocketRef cf = CFSocketCreateWithNative(NULL, socket->m_fd,
  			ALL_CALLBACK_TYPES, Mac_Socket_Callback, &cont);
  CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(NULL, cf, 0);
  assert(source);
  /* Turn off kCFSocketCloseOnInvalidate  (NOTE: > 10.2 only!) */
  /* Another default flag that we don't turn on here is for DataCallBack and
     also AcceptCallback (which overlap in bits) which we don't use anyway */
  /* FIXME: For < 10.2 compatibility fix GSocket to call a platform-dependent
     function to close the socket so that we can just call invalidate and
     avoid having to set any special flags at all. */
  CFSocketSetSocketFlags(cf, kCFSocketAutomaticallyReenableReadCallBack | kCFSocketAutomaticallyReenableWriteCallBack);
  socket->m_gui_dependent = (char*)data;

  /* Keep the source and the socket around. */
  data->source = source;
  data->socket = cf;

  return data;
}

int _GSocket_GUI_Init(void)
{
    return 1;
}

void _GSocket_GUI_Cleanup(void)
{
}

int _GSocket_GUI_Init_Socket(GSocket *socket)
{
    struct MacGSocketData *data = malloc(sizeof(struct MacGSocketData));
    if (data)
    {
        socket->m_gui_dependent = (char*)data;
        data->socket = NULL;
        data->source = NULL;
        return 1;
    }
    return 0;
}

void _GSocket_GUI_Destroy_Socket(GSocket *socket)
{
    struct MacGSocketData *data = (struct MacGSocketData*)(socket->m_gui_dependent);
    if (data)
    {
        CFSocketInvalidate(data->socket);
        CFRelease(data->socket);
        free(data);
    }
}

void _GSocket_Install_Callback(GSocket *socket, GSocketEvent event)
{
    int c;
    struct MacGSocketData* data = _GSocket_Get_Mac_Socket(socket);
    if (!data) return;
    switch (event)
    {
     case GSOCK_CONNECTION:
         if(socket->m_server)
            c = kCFSocketReadCallBack;
         else
            c = kCFSocketConnectCallBack;
         break;
     case GSOCK_LOST:
     case GSOCK_INPUT:
         c = kCFSocketReadCallBack;
         break;
     case GSOCK_OUTPUT:
         c = kCFSocketWriteCallBack;
         break;
     default:
         c = 0;
    }
    CFSocketEnableCallBacks(data->socket, c);
}

void _GSocket_Uninstall_Callback(GSocket *socket, GSocketEvent event)
{
    int c;
    struct MacGSocketData* data = _GSocket_Get_Mac_Socket(socket);
    if (!data) return;
    switch (event)
    {
     case GSOCK_CONNECTION:
         if(socket->m_server)
            c = kCFSocketReadCallBack;
         else
            c = kCFSocketConnectCallBack;
         break;
     case GSOCK_LOST:
     case GSOCK_INPUT:
         c = kCFSocketReadCallBack;
         break;
     case GSOCK_OUTPUT:
         c = kCFSocketWriteCallBack;
         break;
     default:
         c = 0;
    }
    CFSocketDisableCallBacks(data->socket, c);
}

void _GSocket_Enable_Events(GSocket *socket)
{
    struct MacGSocketData* data = _GSocket_Get_Mac_Socket(socket);
    if (!data) return;

    CFRunLoopAddSource(CFRunLoopGetCurrent(), data->source, kCFRunLoopDefaultMode);
}

void _GSocket_Disable_Events(GSocket *socket)
{
    struct MacGSocketData* data = _GSocket_Get_Mac_Socket(socket);
    if (!data) return;

    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), data->source, kCFRunLoopCommonModes);
}

#endif // wxUSE_SOCKETS
