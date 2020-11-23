/*********************************************************************
* File Name: http_net_print.c 
*
* Provides callback headers and resolution for user's custom
* HTTP NET Application.
*
* This file is automatically generated by the MPFS Utility
* ALL MODIFICATIONS WILL BE OVERWRITTEN BY THE MPFS GENERATOR
*
*********************************************************************/

/*****************************************************************************
 Copyright (C) 2012-2018 Microchip Technology Inc. and its subsidiaries.

 Microchip Technology Inc. and its subsidiaries.

 Subject to your compliance with these terms, you may use Microchip software
 and any derivatives exclusively with Microchip products. It is your
 responsibility to comply with third party license terms applicable to your
 use of third party software (including open source software) that may
 accompany Microchip software.

 THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
 PURPOSE.
 
 IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *****************************************************************************/

#include "configuration.h"
#include "definitions.h" 
#include "tcpip/tcpip.h"
#include "http_net_print.h"
/****************************************************************************
  Section:
    Application Dynamic Variables processing functions.
    For easy processing and facilitating of the transfer from the old style of 
    processing the dynamic variables with pre-defined functions,
    the new dynamic variables functions maintain the old name and
    a table showing the correspondence between the dynamic variable name
    and the function that processes that variable i smaintained in this
    coming HTTP_APP_DynVarTbl[].
    Note that this is just an example.
    The application could opt for any type of proccessing it needs to do at run time.

    See http_net.h for details regarding each of these functions.
****************************************************************************/

// table with the processed dynamic variables in this demo
static HTTP_APP_DYNVAR_ENTRY HTTP_APP_DynVarTbl[] = 
{
 // varName                      varFnc
{"hellomsg",					TCPIP_HTTP_Print_hellomsg},
{"cookiename",					TCPIP_HTTP_Print_cookiename},
{"cookiefav",					TCPIP_HTTP_Print_cookiefav},
{"builddate",					TCPIP_HTTP_Print_builddate},
{"led",					TCPIP_HTTP_Print_led},
{"ledSelected",					TCPIP_HTTP_Print_ledSelected},
{"version",					TCPIP_HTTP_Print_version},
{"btn",					TCPIP_HTTP_Print_btn},
{"pot",					TCPIP_HTTP_Print_pot},
{"uploadedmd5",					TCPIP_HTTP_Print_uploadedmd5},
{"snmp_en",					TCPIP_HTTP_Print_snmp_en},
{"read_comm",					TCPIP_HTTP_Print_read_comm},
{"write_comm",					TCPIP_HTTP_Print_write_comm},
{"status_fail",					TCPIP_HTTP_Print_status_fail},
{"config_mac",					TCPIP_HTTP_Print_config_mac},
{"config_hostname",					TCPIP_HTTP_Print_config_hostname},
{"config_dhcpchecked",					TCPIP_HTTP_Print_config_dhcpchecked},
{"config_ip",					TCPIP_HTTP_Print_config_ip},
{"config_gw",					TCPIP_HTTP_Print_config_gw},
{"config_subnet",					TCPIP_HTTP_Print_config_subnet},
{"config_dns1",					TCPIP_HTTP_Print_config_dns1},
{"config_dns2",					TCPIP_HTTP_Print_config_dns2},
{"reboot",					TCPIP_HTTP_Print_reboot},
{"rebootaddr",					TCPIP_HTTP_Print_rebootaddr},
{"status_ok",					TCPIP_HTTP_Print_status_ok},
{"ddns_status",					TCPIP_HTTP_Print_ddns_status},
{"ddns_status_msg",					TCPIP_HTTP_Print_ddns_status_msg},
{"ddns_service",					TCPIP_HTTP_Print_ddns_service},
{"ddns_user",					TCPIP_HTTP_Print_ddns_user},
{"ddns_pass",					TCPIP_HTTP_Print_ddns_pass},
{"ddns_host",					TCPIP_HTTP_Print_ddns_host},
};

// Function that processes the dynamic variables
// It uses the HTTP_APP_DynVarTbl[] for detecting which variable is currently processed
// and what function should be launched.
//
// Note: Again, this is just an example.
// Processing a large number of dynamic variables using this approach is highly inneficient
// and should be avoided.
// Better methods should be emplyed rather than linearly polling each variable name until the required name was found.
//
TCPIP_HTTP_DYN_PRINT_RES TCPIP_HTTP_NET_DynPrint(TCPIP_HTTP_NET_CONN_HANDLE connHandle, const TCPIP_HTTP_DYN_VAR_DCPT *vDcpt, const TCPIP_HTTP_NET_USER_CALLBACK *pCBack)
{
    int ix;
    HTTP_APP_DYNVAR_ENTRY *pEntry;

    // search for a dynamic variable name
    pEntry = HTTP_APP_DynVarTbl;
    for(ix = 0; ix < sizeof(HTTP_APP_DynVarTbl)/sizeof(*HTTP_APP_DynVarTbl); ++ix, ++pEntry)
    {
        if(strcmp(vDcpt->dynName, pEntry->varName) == 0)
        {   // found it
            return (*pEntry->varFnc)(connHandle, vDcpt);
        }
    }

    // not found; do nothing
    return TCPIP_HTTP_DYN_PRINT_RES_DONE;
}

static HTTP_APP_DYNVAR_BUFFER httpDynVarBuffers[HTTP_APP_DYNVAR_BUFFERS_NO];

// helper to get one of the application's dynamic buffer that are used in the
// dynamic variables processing
HTTP_APP_DYNVAR_BUFFER *HTTP_APP_GetDynamicBuffer(void)
{
    int ix;
    HTTP_APP_DYNVAR_BUFFER *pDynBuffer;

    pDynBuffer = httpDynVarBuffers;
    for(ix = 0; ix < sizeof(httpDynVarBuffers)/sizeof(*httpDynVarBuffers); ++ix, pDynBuffer++)
    {
        if(pDynBuffer->busy == 0)
        {
           pDynBuffer->busy = 1;
            return pDynBuffer;
        }
    }
    return 0;
}

/****************************************************************************
  Section:
    Application initialization and HTTP registration.
    Here the application registers with the HTTP module the functions
    that will process the HTTP events (dynamic variables, SSI events, Post, Get, etc.).
    Note that without registering the process functions with HTTP, there won't be any web page processing.
    There is no default processing for a web page!

    See http_net_print.h for details regarding each of these functions.
****************************************************************************/
void HTTP_APP_Initialize(void)
{
    int ix;


    for(ix = 0; ix < sizeof(httpDynVarBuffers)/sizeof(*httpDynVarBuffers); ++ix)
    {
        httpDynVarBuffers[ix].busy = 0;
        httpDynVarBuffers[ix].bufferSize = HTTP_APP_DYNVAR_BUFFER_SIZE;
    }

    TCPIP_HTTP_NET_USER_CALLBACK appHttpCBack =
    {
        .getExecute = TCPIP_HTTP_NET_ConnectionGetExecute,              // Process the "GET" command
        .postExecute = TCPIP_HTTP_NET_ConnectionPostExecute,            // Process the "POST" command
        .fileAuthenticate = TCPIP_HTTP_NET_ConnectionFileAuthenticate,  // Process the file authentication
        .userAuthenticate = TCPIP_HTTP_NET_ConnectionUserAuthenticate,  // Process the user authentication

        .dynamicPrint = TCPIP_HTTP_NET_DynPrint,                        // Process the dynamic variable callback
        .dynamicAck = TCPIP_HTTP_NET_DynAcknowledge,                    // Acknowledgment function for when the dynamic variable processing is completed
        .eventReport = TCPIP_HTTP_NET_EventReport,                      // HTTP Event notification callback

        .ssiNotify = TCPIP_HTTP_NET_SSINotification,                    // SSI command calback
    };

    TCPIP_HTTP_NET_USER_HANDLE httpH = TCPIP_HTTP_NET_UserHandlerRegister(&appHttpCBack);
    if(httpH == 0)
    {
        SYS_CONSOLE_MESSAGE("APP: Failed to register the HTTP callback! \r\n");
    }
}

